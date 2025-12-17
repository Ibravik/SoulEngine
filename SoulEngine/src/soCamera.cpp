/********************************************************************/
/**
* LC	  11/30/2016
* @file   soCamera.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soCamera
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soCamera.h"

/************************************************************************/
/* Definicion de la clase soCamera									    */
/************************************************************************/
namespace SoulSDK
{
	soCamera::soCamera()
	{		
		Connect(std::bind(&soCamera::OnStartUp, this, std::placeholders::_1));
		m_Orthographic = false;
		m_Rotation = soMath::IdentityMatrix3x3();
	}

	soCamera::~soCamera()
	{
		ShutDown();
	}

	void soCamera::IsDirty()
	{
		m_IsDirty = true;
	}

	void soCamera::SetCameraAxes()
	{
		m_RightDir.X = m_ViewMatrix.m00;
		m_RightDir.Y = m_ViewMatrix.m10;
		m_RightDir.Z = m_ViewMatrix.m20;
		if (!m_LockUp)
		{
			m_UpDir.X = m_ViewMatrix.m01;
			m_UpDir.Y = m_ViewMatrix.m11;
			m_UpDir.Z = m_ViewMatrix.m21;
		}
		m_CurrentUp.X = m_ViewMatrix.m01;
		m_CurrentUp.Y = m_ViewMatrix.m11;
		m_CurrentUp.Z = m_ViewMatrix.m21;
		m_FrontDir.X = m_ViewMatrix.m02;
		m_FrontDir.Y = m_ViewMatrix.m12;
		m_FrontDir.Z = m_ViewMatrix.m22;
	}

	void soCamera::ReBuildProyection()
	{
		if (m_Orthographic)
		{
			// Setup orthographic projection matrix
			{
				float L = 0.0f;
				float R = m_CameraViewPort.m_ViewPort.Width;
				float B = m_CameraViewPort.m_ViewPort.Height;
				float T = 0.0f;
				float F = m_Far;
				float N = m_Near;

				m_ProjectionMatrix = soMatrix4x4(
					2.0f / (R - L),	0.0f,				0.0f,			0.0f,
					0.0f,			2.0f / (B - T),		0.0f,			0.0f,
					0.0f,			0.0f,				1.0f / (F - N),	0.0f,
					0.0f,			0.0f,				-N / (F - N),	1.0f
				);
			}
		}
		else
		{
			m_ProjectionMatrix = soMath::MatrixPerspectiveForLH(m_Fov, m_AspectRatio, m_Near, m_Far);
		}

		IsDirty();
	}

	void soCamera::Resize()
	{
		m_AspectRatio = m_CameraViewPort.m_ViewPort.Width / m_CameraViewPort.m_ViewPort.Height;

		IsDirty();
	}
		
	RESULT soCamera::OnStartUp(const CameraData& _CameraInfo)
	{
		m_MaxPitch = _CameraInfo.MaxPitch;
		m_MaxVelocity = _CameraInfo.MaxVel;
		m_InvertY = _CameraInfo.InvertY;
		m_EnableUpMovement = _CameraInfo.EnableUpMovement;
		m_Position = _CameraInfo.Position;
		m_Fov = _CameraInfo.Fov;
		m_AspectRatio = _CameraInfo.AspectRatio;
		m_Near = _CameraInfo.Near;
		m_Far = _CameraInfo.Far;
		m_UpDir = _CameraInfo.Up;
		m_CurrentUp = m_UpDir;
		m_FrontDir = _CameraInfo.Front;
		m_Velocity = soVector3D(0, 0, 0);
		m_LockUp = _CameraInfo.LockUp;

		m_Name = _CameraInfo.CameraName;

		m_CameraViewPort.StartUp(_CameraInfo.ViewportData);
				
		//Inicializacion de la matriz de proyeccion
		ReBuildProyection();

		//Indica que la camara debe actualizarse
		IsDirty();

		return EC_OK;
	}

	void soCamera::OnShutDown()
	{

	}

	void soCamera::Update(float _DeltaTime)
	{
		if (m_IsDirty)
		{
			//Ahora Actualizamos la posicion de la camara
			m_Position += m_Velocity * _DeltaTime;

			//Actualizamos la rotacion
			if (m_Rotation != soMath::IdentityMatrix3x3())
			{
				m_FrontDir = m_FrontDir * (m_Rotation * _DeltaTime);
				soMath::NormalizeVector(m_FrontDir);
			}

			// TO DO: Añadir la desaceleracion de la camara 
			m_Velocity = soVector3D(0.0f, 0.0f, 0.0f);
			m_Rotation = soMath::IdentityMatrix3x3();

			m_LookAt = m_Position + m_FrontDir;

			//Caculamos la nieva matriz view con el up modificable
			m_ViewMatrix = soMath::MatrixLookAtLH(m_Position, m_LookAt, m_UpDir);

			//Caculamos la nieva matriz proj
			ReBuildProyection();
		
			//Seteamos los ejes a partir de la matriz de vista
			SetCameraAxes();

			//Recalculamos el frustum
			m_Frustum.ConstructFrustum(m_ProjectionMatrix, m_ViewMatrix);

			//Calculamos los angulos yaw y pitch
			float lookLengthOnXZ = soMath::Sqrt(m_FrontDir.Z * m_FrontDir.Z + m_FrontDir.X * m_FrontDir.X);
			m_Pitch = soRadians(soMath::ArcTan(m_FrontDir.Y / lookLengthOnXZ)).GetDegrees();
			m_Yaw = soRadians(soMath::ArcTan(m_FrontDir.X / m_FrontDir.Z)).GetDegrees();

			m_IsDirty = false;
		}		
	}

	void soCamera::SetOrthographic(bool _Orthographic)
	{
		m_Orthographic = _Orthographic;

		//Indica que la camara debe actualizarse
		IsDirty();
	}

	void soCamera::MoveForward(float _Scale)
	{
		IsDirty();
		if (m_EnableUpMovement)
		{
			m_Velocity += m_FrontDir * (m_MaxVelocity * _Scale);
		}
		else
		{
			soVector3D MoveVector(m_FrontDir.X, 0.0f, m_FrontDir.Z);
			soMath::NormalizeVector(MoveVector);
			MoveVector = MoveVector * m_MaxVelocity;
			m_Velocity += MoveVector;
		}
	}

	void soCamera::MoveBackward(float _Scale)
	{
		IsDirty();
		if (m_EnableUpMovement)
		{
			m_Velocity += -m_FrontDir * (m_MaxVelocity * _Scale);
		}
		else
		{
			soVector3D MoveVector(-m_FrontDir.X, 0.0f, -m_FrontDir.Z);
			soMath::NormalizeVector(MoveVector);
			MoveVector = MoveVector * m_MaxVelocity;
			m_Velocity += MoveVector;
		}
	}

	void soCamera::MoveLeft(float _Scale)
	{
		IsDirty();
		m_Velocity += -m_RightDir * (m_MaxVelocity * _Scale);
	}

	void soCamera::MoveRight(float _Scale)
	{
		IsDirty();
		m_Velocity += m_RightDir * (m_MaxVelocity * _Scale);
	}

	void soCamera::MoveUp(float _Scale)
	{
		IsDirty();
		if (m_EnableUpMovement)
		{
			m_Velocity += m_UpDir * (m_MaxVelocity * _Scale);
		}
	}

	void soCamera::MoveDown(float _Scale)
	{
		IsDirty();
		if (m_EnableUpMovement)
		{
			m_Velocity += -m_UpDir * (m_MaxVelocity * _Scale);
		}
	}
		
	void soCamera::Yaw(bool _Positive)
	{
		IsDirty();

		soDegrees _Angle((_Positive ? m_MaxRotationVel.Y : -m_MaxRotationVel.Y));
		_Angle.Unwind();

		soQuaternion RotationQuaternion(m_UpDir, _Angle);
		soMatrix3x3 Rotation;
		soMath::QuaternionToMatrixRotation3x3(RotationQuaternion, Rotation);
		m_Rotation *= Rotation;
	}

	void soCamera::Pitch(bool _Positive)
	{
		IsDirty();
		soDegrees _Angle((_Positive ? m_MaxRotationVel.X : -m_MaxRotationVel.X));
		_Angle.Unwind();

		soRadians Radians;
		Radians = (m_InvertY) ? -_Angle.GetRadians() : _Angle.GetRadians();
		m_Pitch -= Radians.GetDegrees();

		if (m_Pitch.GetRadians() > m_MaxPitch.GetRadians())
		{
			Radians += m_Pitch.GetRadians() - m_MaxPitch.GetRadians();
		}
		else if (m_Pitch.GetRadians() < -m_MaxPitch.GetRadians())
		{
			Radians += m_Pitch.GetRadians() + m_MaxPitch.GetRadians();
		}

		soQuaternion RotationQuaternion(m_RightDir, Radians);
		soMatrix3x3 Rotation;
		soMath::QuaternionToMatrixRotation3x3(RotationQuaternion, Rotation);
		m_Rotation *= Rotation;
	}

	void soCamera::Roll(bool _Positive)
	{
		IsDirty();
		soDegrees _Angle((_Positive ? m_MaxRotationVel.Z : -m_MaxRotationVel.Z));
		_Angle.Unwind();
		
		soQuaternion RotationQuaternion(m_FrontDir, _Angle);
		soMatrix3x3 Rotation;
		soMath::QuaternionToMatrixRotation3x3(RotationQuaternion, Rotation);
		m_Rotation *= Rotation;
	}
	
	soRay soCamera::CreateRay(soPoint _MousePos)
	{
		soRay ray;
		//Primero normalizamos las coordenadas del click
		float px = (((2.0f * (float)_MousePos.X) / m_CameraViewPort.m_ViewPort.Width) - 1.0f) / m_ProjectionMatrix.Line[0][0];
		float py = (1.0f - ((2.0f * (float)_MousePos.Y) / m_CameraViewPort.m_ViewPort.Height)) / m_ProjectionMatrix.Line[1][1];
				
		ray.m_VecOrigin = soVector3D(0.0f, 0.0f, 0.0f);
		ray.m_VecDirection = soVector3D(px, py, 1.0f);

		soMatrix4x4 invertViewMatrix = soMath::MatrixInverse4x4(m_ViewMatrix);
		// transform the ray's origin, w = 1.
		float norm;

		norm = invertViewMatrix.Line[0][3] * ray.m_VecDirection.X + invertViewMatrix.Line[1][3] * ray.m_VecDirection.Y + invertViewMatrix.Line[2][3] * ray.m_VecDirection.Z + invertViewMatrix.Line[3][3];
		soVector3D Origen;
		if (norm)
		{
			Origen.X = (invertViewMatrix.Line[0][0] * ray.m_VecDirection.X + invertViewMatrix.Line[1][0] * ray.m_VecDirection.Y + invertViewMatrix.Line[2][0] * ray.m_VecDirection.Z + invertViewMatrix.Line[3][0]) / norm;
			Origen.Y = (invertViewMatrix.Line[0][1] * ray.m_VecDirection.X + invertViewMatrix.Line[1][1] * ray.m_VecDirection.Y + invertViewMatrix.Line[2][1] * ray.m_VecDirection.Z + invertViewMatrix.Line[3][1]) / norm;
			Origen.Z = (invertViewMatrix.Line[0][2] * ray.m_VecDirection.X + invertViewMatrix.Line[1][2] * ray.m_VecDirection.Y + invertViewMatrix.Line[2][2] * ray.m_VecDirection.Z + invertViewMatrix.Line[3][2]) / norm;
		}
		else
		{
			Origen.X = 0.0f;
			Origen.Y = 0.0f;
			Origen.Z = 0.0f;
		}
		ray.m_VecOrigin = Origen;

		// transform the ray's direction, w = 0.
		soVector3D Direction;
		Direction.X = invertViewMatrix.Line[0][0] * ray.m_VecDirection.X + invertViewMatrix.Line[1][0] * ray.m_VecDirection.Y + invertViewMatrix.Line[2][0] * ray.m_VecDirection.Z;
		Direction.Y = invertViewMatrix.Line[0][1] * ray.m_VecDirection.X + invertViewMatrix.Line[1][1] * ray.m_VecDirection.Y + invertViewMatrix.Line[2][1] * ray.m_VecDirection.Z;
		Direction.Z = invertViewMatrix.Line[0][2] * ray.m_VecDirection.X + invertViewMatrix.Line[1][2] * ray.m_VecDirection.Y + invertViewMatrix.Line[2][2] * ray.m_VecDirection.Z;

		ray.m_VecDirection = Direction;

		// normalize the direction
		soMath::NormalizeVector(ray.m_VecDirection);

		return ray;
	}
	

	bool soCamera::SphereIncideFrustum(const soSphere & _Sphere)
	{
		return m_Frustum.IsSphereIn(_Sphere);
	}

	bool soCamera::PointIncideFrustum(const soVector3D & _Point)
	{
		return m_Frustum.IsPointIn(_Point);
	}

	bool soCamera::AABBIncideFrustum(const soAABB& _AABB)
	{
		return m_Frustum.IsAABBIn(_AABB);
	}

}