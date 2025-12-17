/********************************************************************/
/**
* LC	  11/30/2016
* @file   soEditorCamera.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/30/2016
* @brief  Implementacion de la clase soEditorCamera
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soEditorCamera.h"

/************************************************************************/
/* Definicion de la clase soEditorCamera							    */
/************************************************************************/
namespace SoulSDK
{
	soEditorCamera::soEditorCamera()
	{
		Connect(std::bind(&soEditorCamera::OnStartUp, this, std::placeholders::_1));
		m_Active = false;
		m_ActiveOrthographic = 0;		
	}

	soEditorCamera::~soEditorCamera()
	{
	}

	RESULT soEditorCamera::OnStartUp(const CameraData&_CameraData)
	{
		SUPER::OnStartUp(_CameraData);

		TextureData T2D;
		RenderTargetData RTD;
		T2D.T2DData.TextureSize = soDXGIManager::Instance().m_DefaultBackBuffer.m_ptrRenderTargetTexture->m_TextureSize;
		T2D.T2DData.DXGI_FORMAT = APIGI_FORMAT_R8G8B8A8_UNORM;
		T2D.T2DData.D3D11_BIND_FLAG = API_BIND_RENDER_TARGET | API_BIND_SHADER_RESOURCE;
		RTD._TextureData = T2D;

		if (FAIL(m_CameraRenderTarget.StartUp(RTD)))
		{
			return EC_FALSE;
		}

		m_OrthographicPos[OP_Top] = soVector3D(0, 300, 0);
		m_OrthographicPos[OP_Bottom] = soVector3D(0, -300, 0);
		m_OrthographicPos[OP_Left] = soVector3D(-300, 0, 0);
		m_OrthographicPos[OP_Right] = soVector3D(300, 0, 0);
		m_OrthographicPos[OP_Front] = soVector3D(0, 0, 300);
		m_OrthographicPos[OP_Back] = soVector3D(0, 0, -300);

		m_OrthographicUp[OP_Top] = soVector3D(0, 0, -1);
		m_OrthographicUp[OP_Bottom] = soVector3D(0, 0, 1);
		m_OrthographicUp[OP_Left] = soVector3D(0, 1, 0);
		m_OrthographicUp[OP_Right] = soVector3D(0, 1, 0);
		m_OrthographicUp[OP_Front] = soVector3D(0, 1, 0);
		m_OrthographicUp[OP_Back] = soVector3D(0, 1, 0);

		m_OrthographicFront[OP_Top] = soVector3D(0, -1, 0);
		m_OrthographicFront[OP_Bottom] = soVector3D(0, 1, 0);
		m_OrthographicFront[OP_Left] = soVector3D(1, 0, 0);
		m_OrthographicFront[OP_Right] = soVector3D(-1, 0, 0);
		m_OrthographicFront[OP_Front] = soVector3D(0, 0, -1);
		m_OrthographicFront[OP_Back] = soVector3D(0, 0, 1);

		return EC_OK;
	}

	void soEditorCamera::Update(float _DeltaTime)
	{
		if (m_IsDirty)
		{
			soVector3D* Pos, *Up, *Front;

			if (m_Orthographic)
			{
				Pos = &m_OrthographicPos[m_ActiveOrthographic];
				Up = &m_OrthographicUp[m_ActiveOrthographic];
				Front = &m_OrthographicFront[m_ActiveOrthographic];
			}
			else
			{
				Pos = &m_Position;
				Up = &m_UpDir;
				Front = &m_FrontDir;
			}

			//Ahora Actualizamos la posicion de la camara			
			*Pos += m_Velocity * _DeltaTime;

			//Actualizamos la rotacion
			if (m_Rotation != soMath::IdentityMatrix3x3())
			{
				*Front = *Front * (m_Rotation * _DeltaTime);
				soMath::NormalizeVector(*Front);
			}

			// TO DO: Añadir la desaceleracion de la camara 
			m_Velocity = soVector3D(0.0f, 0.0f, 0.0f);
			m_Rotation = soMath::IdentityMatrix3x3();

			m_LookAt = *Pos + *Front;

			//Caculamos la nieva matriz view con el up modificable
			m_ViewMatrix = soMath::MatrixLookAtLH(*Pos, m_LookAt, *Up);

			//Caculamos la nieva matriz proj
			ReBuildProyection();

			//Seteamos los ejes a partir de la matriz de vista
			SetCameraAxes();

			//Recalculamos el frustum
			m_Frustum.ConstructFrustum(m_ProjectionMatrix, m_ViewMatrix);

			//Calculamos los angulos yaw y pitch
			float lookLengthOnXZ = soMath::Sqrt(Front->Z * Front->Z + Front->X * Front->X);
			m_Pitch = soRadians(soMath::ArcTan(Front->Y / lookLengthOnXZ)).GetDegrees();
			m_Yaw = soRadians(soMath::ArcTan(Front->X / Front->Z)).GetDegrees();

			m_IsDirty = false;
		}
	}

	void soEditorCamera::Resize()
	{
		SUPER::Resize();

		//Render target camera resize
		RenderTargetData RT;
		RT._TextureData = m_CameraRenderTarget.m_RenderTargetTextureData;
		RT._TextureData.T2DData.TextureSize = soDXGIManager::Instance().m_DefaultBackBuffer.m_ptrRenderTargetTexture->m_TextureSize;
		m_CameraRenderTarget.ShutDown();
		m_CameraRenderTarget.StartUp(RT);
	}

}