	/********************************************************************/
/**
* @LC	  9/19/2016
* @file   soGraphicDebugManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   1/28/2016
* @brief  Implementacion de la clase soGraphicDebugManager
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soGraphicDebugManager.h"

#define SphereFaces 16
#define PointFaces 4

/************************************************************************/
/* Definicion de la clase soGraphicDebug				                */
/************************************************************************/
namespace SoulSDK
{
	soGraphicDebugManager::soGraphicDebugManager()
	{
		Connect(std::bind(&soGraphicDebugManager::OnStartUp, this));
	}

	soGraphicDebugManager::~soGraphicDebugManager()
	{
	}

	void soGraphicDebugManager::AddToStaticBuffer(soVector3D& _From, soVector3D& _To, soColorRGBA _Color)
	{
		//Copiamos los nuevos vertices
		DebugVertex VertexFrom;
		DebugVertex VertexTo;

		VertexFrom.Position = _From;
		VertexFrom.Position.W = 1.0f;
		VertexFrom.Color = _Color;
		VertexTo.Position = _To;
		VertexTo.Position.W = 1.0f;
		VertexTo.Color = _Color;

		m_StaticPool.InsertElement(VertexFrom);
		m_StaticPool.InsertElement(VertexTo);
	}

	void soGraphicDebugManager::AddToDynamicBuffer(soVector3D& _From, soVector3D& _To, soColorRGBA _Color)
	{
		//Copiamos los nuevos vertices
		DebugVertex VertexFrom;
		DebugVertex VertexTo;

		VertexFrom.Position = _From;
		VertexFrom.Position.W = 1.0f;
		VertexFrom.Color = _Color;
		VertexTo.Position = _To;
		VertexTo.Position.W = 1.0f;
		VertexTo.Color = _Color;

		m_DynamicPool.InsertElement(VertexFrom);
		m_DynamicPool.InsertElement(VertexTo);
	}

	void soGraphicDebugManager::AddDebugSphere(const soSphere& _Sphere, bool _IsStatic, soColorRGBA _Color)
	{
		float JumpAngle = soMath::DOUBLE_PI / SphereFaces;

		for (uint32 i = 0; i < SphereFaces; i++)
		{
			float CurrentRadio = soMath::Cos(JumpAngle * i) * _Sphere.m_fRadius;
			float NextRadio = soMath::Cos(JumpAngle * (i + 1)) * _Sphere.m_fRadius;

			DebugVertex V1, V2, V3;
			V1.Color = V2.Color = V3.Color = _Color;

			V1.Position.Y = (soMath::Sin(JumpAngle * i) * _Sphere.m_fRadius) + _Sphere.m_SphereCenter.Y;
			V2.Position.Y = V1.Position.Y;

			if (i != SphereFaces - 1)
			{
				V3.Position.Y = (soMath::Sin(JumpAngle * (i + 1)) * _Sphere.m_fRadius) + _Sphere.m_SphereCenter.Y;
			}
			else
			{
				V3.Position.Y = (soMath::Sin(JumpAngle * 0) * _Sphere.m_fRadius) + _Sphere.m_SphereCenter.Y;
			}

			for (uint32 j = 0; j < SphereFaces; j++)
			{
				V1.Position.X = (soMath::Cos(JumpAngle * j) * CurrentRadio) + _Sphere.m_SphereCenter.X;
				V1.Position.Z = (soMath::Sin(JumpAngle * j) * CurrentRadio) + _Sphere.m_SphereCenter.Z;
				V1.Position.W = 1.0f;

				V3.Position.X = (soMath::Cos(JumpAngle * j) * NextRadio) + _Sphere.m_SphereCenter.X;
				V3.Position.Z = (soMath::Sin(JumpAngle * j) * NextRadio) + _Sphere.m_SphereCenter.Z;
				V3.Position.W = 1.0f;

				if (j != SphereFaces - 1)
				{
					V2.Position.X = (soMath::Cos(JumpAngle * (j + 1)) * CurrentRadio) + _Sphere.m_SphereCenter.X;
					V2.Position.Z = (soMath::Sin(JumpAngle * (j + 1)) * CurrentRadio) + _Sphere.m_SphereCenter.Z;
					V2.Position.W = 1.0f;
				}
				else
				{
					V2.Position.X = (soMath::Cos(JumpAngle * 0) * CurrentRadio) + _Sphere.m_SphereCenter.X;
					V2.Position.Z = (soMath::Sin(JumpAngle * 0) * CurrentRadio) + _Sphere.m_SphereCenter.Z;
					V2.Position.W = 1.0f;
				}

				if (_IsStatic)
				{
					m_StaticPool.InsertElement(V1);
					m_StaticPool.InsertElement(V2);
					m_StaticPool.InsertElement(V1);
					m_StaticPool.InsertElement(V3);
				}
				else
				{
					m_DynamicPool.InsertElement(V1);
					m_DynamicPool.InsertElement(V2);
					m_DynamicPool.InsertElement(V1);
					m_DynamicPool.InsertElement(V3);
				}
			}
		}
	}
	
	void soGraphicDebugManager::AddDebugPositionControl(const soVector3D& _Center, bool _IsStatic, float _Scale, soColorRGBA _Color1, soColorRGBA _Color2, soColorRGBA _Color3)
	{
		soVector3D X(_Center.X + (1.0f * _Scale),	_Center.Y,			_Center.Z);
		soVector3D Y(_Center.X,			_Center.Y + (1.0f * _Scale),	_Center.Z);
		soVector3D Z(_Center.X,			_Center.Y,			_Center.Z + (1.0f * _Scale));

		AddDebugLine(soVector3D(	_Center.X,			_Center.Y,			_Center.Z), X, _IsStatic, _Color1);
		AddDebugLine(soVector3D(	_Center.X,			_Center.Y,			_Center.Z), Y, _IsStatic, _Color2);
		AddDebugLine(soVector3D(	_Center.X,			_Center.Y,			_Center.Z), Z, _IsStatic, _Color3);
	}

	bool soGraphicDebugManager::IsEmpty()
	{
		if (m_StaticPool.GetPoolObjectsNumber() != 0)
		{
			return false;
		}

		if (m_DynamicPool.GetPoolObjectsNumber() != 0)
		{
			return false;
		}

		return true;
	}

	RESULT soGraphicDebugManager::OnStartUp()
	{
		//Primero inicializamos las pools
		m_StaticPool.StartUp(MAX_Vertex);
		m_DynamicPool.StartUp(MAX_Vertex);

		//Ahora generamos los constant buffers 
		ConstantBufferData bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.ByteWidth = m_StaticPool.GetPoolSizeOff();
		bd.D3D11_BIND_FLAG = API_BIND_VERTEX_BUFFER;
		bd.D3D11_SUBRESOURCE_DATA.pSysMem = &*m_StaticPool.m_Begin;

		if (FAIL(m_StaticBuffer.StartUp(bd)))
		{
			SOE_OutputLOG("soGraphicDebugManager", "OnStartUp()", "FAIL to create constant buffer.");
			return EC_FALSE;
		}	

		bd.D3D11_SUBRESOURCE_DATA.pSysMem = &*m_DynamicPool.m_Begin;
		if (FAIL(m_DynamicBuffer.StartUp(bd)))
		{
			SOE_OutputLOG("soGraphicDebugManager", "OnStartUp()", "FAIL to create constant buffer.");
			return EC_FALSE;
		}		

		return EC_OK;
	}

	void soGraphicDebugManager::OnShutDown()
	{
	}

	void soGraphicDebugManager::AddDebugPoint(const soVector3D& _Point, bool _IsStatic, float _Scale, soColorRGBA _Color)
	{
		float JumpAngle = soMath::DOUBLE_PI / PointFaces;

		for (uint32 i = 0; i < PointFaces; i++)
		{
			float CurrentRadio = soMath::Cos(JumpAngle * i) * 0.5f * _Scale;
			float NextRadio = soMath::Cos(JumpAngle * (i + 1))* 0.5f * _Scale;

			DebugVertex V1, V2, V3;
			V1.Color = V2.Color = V3.Color = _Color;

			V1.Position.Y = (soMath::Sin(JumpAngle * i)* 0.5f * _Scale) + _Point.Y;
			V2.Position.Y = V1.Position.Y;

			if (i != PointFaces - 1)
			{
				V3.Position.Y = (soMath::Sin(JumpAngle * (i + 1))* 0.5f * _Scale) + _Point.Y;
			}
			else
			{
				V3.Position.Y = (soMath::Sin(JumpAngle * 0)* 0.5f * _Scale) + _Point.Y;
			}

			for (uint32 j = 0; j < PointFaces; j++)
			{
				V1.Position.X = (soMath::Cos(JumpAngle * j) * CurrentRadio) + _Point.X;
				V1.Position.Z = (soMath::Sin(JumpAngle * j) * CurrentRadio) + _Point.Z;
				V1.Position.W = 1.0f;

				V3.Position.X = (soMath::Cos(JumpAngle * j) * NextRadio) + _Point.X;
				V3.Position.Z = (soMath::Sin(JumpAngle * j) * NextRadio) + _Point.Z;
				V3.Position.W = 1.0f;

				if (j != PointFaces - 1)
				{
					V2.Position.X = (soMath::Cos(JumpAngle * (j + 1)) * CurrentRadio) + _Point.X;
					V2.Position.Z = (soMath::Sin(JumpAngle * (j + 1)) * CurrentRadio) + _Point.Z;
					V2.Position.W = 1.0f;
				}
				else
				{
					V2.Position.X = (soMath::Cos(JumpAngle * 0) * CurrentRadio) + _Point.X;
					V2.Position.Z = (soMath::Sin(JumpAngle * 0) * CurrentRadio) + _Point.Z;
					V2.Position.W = 1.0f;
				}

				if (_IsStatic)
				{
					m_StaticPool.InsertElement(V1);
					m_StaticPool.InsertElement(V2);
					m_StaticPool.InsertElement(V1);
					m_StaticPool.InsertElement(V3);
				}
				else
				{
					m_DynamicPool.InsertElement(V1);
					m_DynamicPool.InsertElement(V2);
					m_DynamicPool.InsertElement(V1);
					m_DynamicPool.InsertElement(V3);
				}
			}
		}
	}

	void soGraphicDebugManager::AddDebugLine(const soVector3D& _From, const soVector3D& _To, bool _IsStatic, soColorRGBA _Color)
	{
		soVector3D From = _From;
		soVector3D To = _To;

		if (_IsStatic)
		{
			AddToStaticBuffer(From, To, _Color);
		}
		else
		{
			AddToDynamicBuffer(From, To, _Color);
		}
	}

	void soGraphicDebugManager::AddDebugAABB(const soAABB& _AABB, bool _IsStatic, soColorRGBA _Color)
	{
		AddDebugLine(_AABB.m_AABBMin, soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMax.Z), _IsStatic, _Color);
		AddDebugLine(soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMax.Z), soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMax.Z), _IsStatic, _Color);
		AddDebugLine(soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMax.Z), soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMin.Z), _IsStatic, _Color);
		AddDebugLine(soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMin.Z), _AABB.m_AABBMin, _IsStatic, _Color);

		AddDebugLine(_AABB.m_AABBMax, soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMax.Z), _IsStatic, _Color);
		AddDebugLine(_AABB.m_AABBMax, soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMin.Z), _IsStatic, _Color);
		AddDebugLine(soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMax.Z), soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMin.Z), _IsStatic, _Color);
		AddDebugLine(soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMin.Z), soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMin.Z), _IsStatic, _Color);

		AddDebugLine(soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMax.Z), soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMax.Z), _IsStatic, _Color);
		AddDebugLine(_AABB.m_AABBMax, soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMax.Z), _IsStatic, _Color);
		AddDebugLine(_AABB.m_AABBMin, soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMin.Z), _IsStatic, _Color);
		AddDebugLine(soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMin.Z), soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMin.Z), _IsStatic, _Color);
	}

	void soGraphicDebugManager::AddDebugCameraFrustum(soCamera& _Camera, bool _IsStatic)
	{
		/*soVector3D n1, n2, n3, n4;
		soVector3D f1, f2, f3, f4;
		soVector3D n = _Camera.m_Frustum.Near.m_Normal * _Camera.m_Near;
		soVector3D f = _Camera.m_Frustum.Near.m_Normal * _Camera.m_Far;
		float r = _Camera.m_Far / (_Camera.m_Far - _Camera.m_Near);

		n1 = n + _Camera.m_RightDir * _Camera.m_Near * r + _Camera.m_UpDir * _Camera.m_Near * _Frustum.m_uFactor;
		n2 = n - _Camera.m_RightDir * _Camera.m_Near * r + _Camera.m_UpDir * _Camera.m_Near * _Frustum.m_uFactor;
		n3 = n - _Camera.m_RightDir * _Camera.m_Near * r - _Camera.m_UpDir * _Camera.m_Near * _Frustum.m_uFactor;
		n4 = n + _Camera.m_RightDir * _Camera.m_Near * r - _Camera.m_UpDir * _Camera.m_Near * _Frustum.m_uFactor;

		f1 = f + _Camera.m_RightDir * _Camera.m_Far * r + _Camera.m_UpDir * _Camera.m_Far * _Frustum.m_uFactor;
		f2 = f - _Camera.m_RightDir * _Camera.m_Far * r + _Camera.m_UpDir * _Camera.m_Far * _Frustum.m_uFactor;
		f3 = f - _Camera.m_RightDir * _Camera.m_Far * r - _Camera.m_UpDir * _Camera.m_Far * _Frustum.m_uFactor;
		f4 = f + _Camera.m_RightDir * _Camera.m_Far * r - _Camera.m_UpDir * _Camera.m_Far * _Frustum.m_uFactor;

		n1 += _Frustum.m_EyePosition;
		n2 += _Frustum.m_EyePosition;
		n3 += _Frustum.m_EyePosition;
		n4 += _Frustum.m_EyePosition;
		f1 += _Frustum.m_EyePosition;
		f2 += _Frustum.m_EyePosition;
		f3 += _Frustum.m_EyePosition;
		f4 += _Frustum.m_EyePosition;

		AddDebugLine(n1, n2, _IsStatic);
		AddDebugLine(n2, n3, _IsStatic);
		AddDebugLine(n3, n4, _IsStatic);
		AddDebugLine(n4, n1, _IsStatic);

		AddDebugLine(f1, f2, _IsStatic);
		AddDebugLine(f2, f3, _IsStatic);
		AddDebugLine(f3, f4, _IsStatic);
		AddDebugLine(f4, f1, _IsStatic);

		AddDebugLine(n1, f1, _IsStatic);

		AddDebugLine(n2, f2, _IsStatic);

		AddDebugLine(n3, f3, _IsStatic);

		AddDebugLine(n4, f4, _IsStatic);*/
	}

	void soGraphicDebugManager::Render()
	{
		uint32 stride = sizeof(DebugVertex);
		uint32 offset = 0;
		uint32 StartSlot = 0;
		uint32 NumBuffers = 1;
		int32 StartVertexLocation = 0;
		soVertexBuffer VertexBuffer;

		//Seteamos topologia 
		API_PRIMITIVE_TOPOLOGY Topology = API_PRIMITIVE_TOPOLOGY_LINELIST;
		soDXGIManager::Instance().SetPrimitiveTopology(Topology);

		if (m_StaticPool.GetPoolObjectsNumber() != 0)
		{
			//Seteamos el VertexBuffer estatico		
			m_StaticBuffer.UpdateBuffer();
			VertexBuffer.m_ptrVertexBuffer = m_StaticBuffer.m_ptrConstantBuffer;
			soDXGIManager::Instance().SetVertexBuffer(StartSlot, NumBuffers, VertexBuffer, stride, offset);

			//Imprimimos estatico		
			uint32 VertexNumber = m_StaticPool.GetPoolObjectsNumber();
			soDXGIManager::Instance().Draw(VertexNumber, StartVertexLocation);
		}
		
		if (m_DynamicPool.GetPoolObjectsNumber() != 0)
		{
			//Seteamos el VertexBuffer dinamico
			m_DynamicBuffer.UpdateBuffer();
			VertexBuffer.m_ptrVertexBuffer = m_DynamicBuffer.m_ptrConstantBuffer;
			soDXGIManager::Instance().SetVertexBuffer(StartSlot, NumBuffers, VertexBuffer, stride, offset);

			//Imprimimos dinamico		
			uint32 VertexNumber = m_DynamicPool.GetPoolObjectsNumber();
			soDXGIManager::Instance().Draw(VertexNumber, StartVertexLocation);

			//Reseteamos dinamico
			m_DynamicPool.Reset();
		}

		VertexBuffer.m_ptrVertexBuffer = NULL;
	}
}