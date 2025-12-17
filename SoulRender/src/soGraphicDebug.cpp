/********************************************************************/
/**
* @LC	  3/6/2016
* @file   soGraphicDebug.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   1/28/2016
* @brief  Implementacion de la clase soGraphicDebug
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include <assert.h>
#include "soDirectXGraphics.h"
#include "soVertexBuffer.h"
#include "soVertexShader.h"
#include "soFragmentShader.h"
#include "soSkeletalMesh.h"
#include "soGraphicDebug.h"

/************************************************************************/
/* Definicion de la clase soGraphicDebug				                */
/************************************************************************/
namespace SoulSDK
{
	soGraphicDebug::soGraphicDebug()
	{
		m_ID3D11Device = NULL;
		m_VertexShader = NULL;
		m_PixelShader = NULL;
		m_DynamicBufferBeigin = NULL;
		m_StaticBufferBeigin = NULL;
		m_StaticBufferVertexCount = 0;
		m_DynamicBufferVertexCount = 0;
		m_bDynamicBufferUpdated = false;
		m_bStaticBufferUpdated = false;
	}

	soGraphicDebug::~soGraphicDebug()
	{
		DestroyBuffers();
	}

	void soGraphicDebug::AddToStaticBuffer(soVector3D& _From, soVector3D& _To)
	{
		//Copiamos los nuevos vertices
		SimpleVertex* NewArray = SOE_NEW SimpleVertex[m_StaticBuffer.GetpVertexInfoArraySize() + 2];

		if (!m_StaticBuffer.pVertexInfoArrayIsEmpty())
		{
			for (unsigned int i = 0; i < m_StaticBuffer.GetpVertexInfoArraySize(); i++)
			{
				NewArray[i].Position = m_StaticBuffer.GetpVertexInfoArrayElement(i).Position;
			}
		}

		//Aumentamos el numero de vertices en 2
		unsigned int Size = m_StaticBuffer.GetpVertexInfoArraySize() + 2;
		NewArray[Size - 2].Position = _From;
		NewArray[Size - 1].Position = _To;

		//Una vez copiado todo, liberamos el buffer actual
		m_StaticBuffer.DeletepVertexInfoArray();

		//Ahora creamos nuevamente el buffer
		m_StaticBuffer.SetpVertexInfoArray(*NewArray, Size);

		m_bStaticBufferUpdated = true;

		return;
	}

	void soGraphicDebug::AddToDynamicBuffer(soVector3D& _From, soVector3D& _To)
	{
		//Copiamos los nuevos vertices
		SimpleVertex* NewArray = SOE_NEW SimpleVertex[m_DynamicBuffer.GetpVertexInfoArraySize() + 2];

		if (!m_DynamicBuffer.pVertexInfoArrayIsEmpty())
		{
			for (unsigned int i = 0; i < m_DynamicBuffer.GetpVertexInfoArraySize(); i++)
			{
				NewArray[i].Position = m_DynamicBuffer.GetpVertexInfoArrayElement(i).Position;
			}
		}

		//Aumentamos el numero de vertices en 2
		unsigned int Size = m_DynamicBuffer.GetpVertexInfoArraySize() + 2;
		NewArray[Size - 2].Position = _From;
		NewArray[Size - 1].Position = _To;

		//Una vez copiado todo, liberamos el buffer actual
		m_DynamicBuffer.DeletepVertexInfoArray();

		//Ahora creamos nuevamente el buffer
		m_DynamicBuffer.SetpVertexInfoArray(*NewArray, Size);

		m_bDynamicBufferUpdated = true;

		return;
	}

	void soGraphicDebug::DestroyDynamicBuffer()
	{
		//Al terminar el frame, liberamos el buffer
		SAFE_RELEASE(m_DynamicBuffer.GetpVertexBuffer());
		m_DynamicBuffer.DeletepVertexInfoArray();
	}

	void soGraphicDebug::DestroyBuffers()
	{
		//Al terminar el frame, liberamos el buffer
		SAFE_RELEASE(m_DynamicBuffer.GetpVertexBuffer());
		m_DynamicBuffer.DeletepVertexInfoArray();

		//Al terminar el frame, liberamos el buffer
		SAFE_RELEASE(m_StaticBuffer.GetpVertexBuffer());
		m_StaticBuffer.DeletepVertexInfoArray();
	}

	void soGraphicDebug::UpdateDynamicBuffer()
	{
		if (m_bDynamicBufferUpdated)
		{
			SAFE_RELEASE(m_DynamicBuffer.GetpVertexBuffer());

			BufferInfo VertexBuffer;
			VertexBuffer.D3D11Usage = D3D11_USAGE_DEFAULT;
			VertexBuffer.ByteWidth = sizeof(SimpleVertex) * m_DynamicBuffer.GetpVertexInfoArraySize();
			VertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			VertexBuffer.CPUAccesFlag = 0;

			m_DynamicBuffer.CreateVertexBuffer(m_ID3D11Device, VertexBuffer);

			m_bDynamicBufferUpdated = false;
		}
	}

	void soGraphicDebug::UpdateStaticBuffer()
	{
		if (m_bStaticBufferUpdated)
		{
			SAFE_RELEASE(m_StaticBuffer.GetpVertexBuffer());

			BufferInfo VertexBuffer;
			VertexBuffer.D3D11Usage = D3D11_USAGE_DEFAULT;
			VertexBuffer.ByteWidth = sizeof(SimpleVertex) * m_StaticBuffer.GetpVertexInfoArraySize();
			VertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			VertexBuffer.CPUAccesFlag = 0;

			m_StaticBuffer.CreateVertexBuffer(m_ID3D11Device, VertexBuffer);

			m_bStaticBufferUpdated = false;
		}
	}


	void soGraphicDebug::AddDebugPoint(float _X, float _Y, float _Z, bool _IsStatic)
	{
		if (_IsStatic)
		{
			AddToStaticBuffer(soVector3D(_X, _Y, _Z) , soVector3D(_X, _Y, _Z));
		}
		else
		{
			AddToDynamicBuffer(soVector3D(_X, _Y, _Z), soVector3D(_X, _Y, _Z));
		}
	}

	void soGraphicDebug::AddDebugPoint(const soVector3D& _Point, bool _IsStatic)
	{
		AddDebugPoint(_Point.X, _Point.Y, _Point.Z, _IsStatic);
	}

	void soGraphicDebug::AddDebugLine(float _X1, float _Y1, float _Z1, float _X2, float _Y2, float _Z2, bool _IsStatic)
	{
		if (_IsStatic)
		{
			AddToStaticBuffer(soVector3D(_X1, _Y1, _Z1), soVector3D(_X2, _Y2, _Z2));
		}
		else
		{
			AddToDynamicBuffer(soVector3D(_X1, _Y1, _Z1), soVector3D(_X2, _Y2, _Z2));
		}
	}

	void soGraphicDebug::AddDebugLine(const soVector3D& _From, const soVector3D& _To, bool _IsStatic)
	{
		AddDebugLine(_From.X, _From.Y, _From.Z, _To.X, _To.Y, _To.Z, _IsStatic);
	}

	void soGraphicDebug::AddDebugAABB(const soVector3D& _AABBMin, const soVector3D _AABBMax, bool _IsStatic)
	{
		AddDebugLine(_AABBMin, soVector3D(_AABBMin.X, _AABBMin.Y, _AABBMax.Z), _IsStatic);
		AddDebugLine(soVector3D(_AABBMin.X, _AABBMin.Y, _AABBMax.Z), soVector3D(_AABBMax.X, _AABBMin.Y, _AABBMax.Z), _IsStatic);
		AddDebugLine(soVector3D(_AABBMax.X, _AABBMin.Y, _AABBMax.Z), soVector3D(_AABBMax.X, _AABBMin.Y, _AABBMin.Z), _IsStatic);
		AddDebugLine(soVector3D(_AABBMax.X, _AABBMin.Y, _AABBMin.Z), _AABBMin, _IsStatic);

		AddDebugLine(_AABBMax, soVector3D(_AABBMin.X, _AABBMax.Y, _AABBMax.Z), _IsStatic);
		AddDebugLine(_AABBMax, soVector3D(_AABBMax.X, _AABBMax.Y, _AABBMin.Z), _IsStatic);
		AddDebugLine(soVector3D(_AABBMin.X, _AABBMax.Y, _AABBMax.Z), soVector3D(_AABBMin.X, _AABBMax.Y, _AABBMin.Z), _IsStatic);
		AddDebugLine(soVector3D(_AABBMax.X, _AABBMax.Y, _AABBMin.Z), soVector3D(_AABBMin.X, _AABBMax.Y, _AABBMin.Z), _IsStatic);

		AddDebugLine(soVector3D(_AABBMin.X, _AABBMax.Y, _AABBMax.Z), soVector3D(_AABBMin.X, _AABBMin.Y, _AABBMax.Z), _IsStatic);
		AddDebugLine(_AABBMax, soVector3D(_AABBMax.X, _AABBMin.Y, _AABBMax.Z), _IsStatic);
		AddDebugLine(_AABBMin, soVector3D(_AABBMin.X, _AABBMax.Y, _AABBMin.Z), _IsStatic);
		AddDebugLine(soVector3D(_AABBMax.X, _AABBMax.Y, _AABBMin.Z), soVector3D(_AABBMax.X, _AABBMin.Y, _AABBMin.Z), _IsStatic);
	}

	void soGraphicDebug::AddDebugAABB(const soAABB& _AABB, bool _IsStatic)
	{
		AddDebugAABB(_AABB.m_AABBMin, _AABB.m_AABBMax, _IsStatic);
	}

	void soGraphicDebug::AddDebugFrustum(soFrustrum& _Frustum, bool _IsStatic)
	{
		soVector3D n1, n2, n3, n4;
		soVector3D f1, f2, f3, f4;
		soVector3D n = (_Frustum.m_ForwardVector * _Frustum.m_NearZ);
		soVector3D f = (_Frustum.m_ForwardVector * _Frustum.m_FarZ);

		n1 = n + _Frustum.m_RightVector * _Frustum.m_NearZ * _Frustum.m_rFactor + _Frustum.m_UpVector * _Frustum.m_NearZ * _Frustum.m_uFactor;
		n2 = n - _Frustum.m_RightVector * _Frustum.m_NearZ * _Frustum.m_rFactor + _Frustum.m_UpVector * _Frustum.m_NearZ * _Frustum.m_uFactor;
		n3 = n - _Frustum.m_RightVector * _Frustum.m_NearZ * _Frustum.m_rFactor - _Frustum.m_UpVector * _Frustum.m_NearZ * _Frustum.m_uFactor;
		n4 = n + _Frustum.m_RightVector * _Frustum.m_NearZ * _Frustum.m_rFactor - _Frustum.m_UpVector * _Frustum.m_NearZ * _Frustum.m_uFactor;

		f1 = f + _Frustum.m_RightVector * _Frustum.m_FarZ * _Frustum.m_rFactor + _Frustum.m_UpVector * _Frustum.m_FarZ * _Frustum.m_uFactor;
		f2 = f - _Frustum.m_RightVector * _Frustum.m_FarZ * _Frustum.m_rFactor + _Frustum.m_UpVector * _Frustum.m_FarZ * _Frustum.m_uFactor;
		f3 = f - _Frustum.m_RightVector * _Frustum.m_FarZ * _Frustum.m_rFactor - _Frustum.m_UpVector * _Frustum.m_FarZ * _Frustum.m_uFactor;
		f4 = f + _Frustum.m_RightVector * _Frustum.m_FarZ * _Frustum.m_rFactor - _Frustum.m_UpVector * _Frustum.m_FarZ * _Frustum.m_uFactor;

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

		AddDebugLine(n4, f4, _IsStatic);
	}

	void soGraphicDebug::AddDebugSkeletalMesh(soSkeletalMesh& _Mesh, bool _IsStatic, float _ScalingAABB /* = 1.0f */)
	{
		//Queue de chequeo de nodos
		float S = 1 * _ScalingAABB;
		soAABB CheckBox(soVector3D(0, 0, 0), soVector3D(S, S, S), soVector3D(-S, -S, -S));

		//Queue de chequeo de nodos
		std::queue<soNodeGraph*> GraphList;
		soNodeGraph* GraphNode;
		bool JoinOn = false;

		//Nodo inicial del grafo
		GraphList.push(&_Mesh.GetRootNode());
		
		while (!GraphList.empty())
		{
			//Tomamos el primer nodo de la lista
			GraphNode = GraphList.front();

			//Sacamos al nodo visitado
			GraphList.pop();

			soVector4D Position1(0, 0, 0, 1);

			//Buscamos si el nodo del grafo pertenese al esqueleto
			if (GraphNode->mJoin != NULL)
			{
				Position1 = Position1 * GraphNode->mJoin->GetGlobalMatrix();
				JoinOn = true;
			}
			else
			{
				Position1 = Position1 * GraphNode->mGlobalTransform;
			}

			Position1 = Position1 * _Mesh.GetSkinnedModel().GetTransform();
			/*CheckBox.m_AABBCenter = Position1;
			CheckBox.m_AABBMax = CheckBox.m_AABBCenter + soVector3D(S, S, S);
			CheckBox.m_AABBMin = CheckBox.m_AABBCenter + soVector3D(-S, -S, -S);
			AddDebugAABB(CheckBox, _IsStatic);*/

			//Agregamos los nodos hijos a la lista
			for (unsigned i = 0; i < GraphNode->GetChildrensArraySize(); i++)
			{
				GraphList.push(&GraphNode->GetChildrensArrayElement(i));
				
				if (JoinOn)
				{//Si el nodo si pertenecio a la lista de huesos...
					JoinOn = false;
					//Verificamos si el hijo pertesese a la lista de huesos
					soVector4D Position2(0, 0, 0, 1);
					if (GraphNode->GetChildrensArrayElement(i).mJoin != NULL)
					{
						Position2 = Position2 * GraphNode->GetChildrensArrayElement(i).mJoin->GetGlobalMatrix();
						JoinOn = true;
					}
					else
					{
						Position2 = Position2 * GraphNode->GetChildrensArrayElement(i).mGlobalTransform;
					}
					
					if (JoinOn)
					{	
						Position2 = Position2 *_Mesh.GetSkinnedModel().GetTransform();
						AddDebugLine(soVector3D(Position1.X, Position1.Y, Position1.Z), soVector3D(Position2.X, Position2.Y, Position2.Z), _IsStatic);
					}
				}
			}
		}
	}

	void soGraphicDebug::InitGraphicDebug(ID3D11Device * _ID3D11Device, soVertexShader* _VS, soFragmentShader* _FS)
	{
		assert(_ID3D11Device != NULL);
		assert(_VS != NULL);
		assert(_FS != NULL);

		m_ID3D11Device = _ID3D11Device;
		m_VertexShader = _VS;
		m_PixelShader = _FS;

		//Alojamos la memoria para el pool de vertices de ambos buffers
		//m_StaticBufferBeigin = soV

	}

	void soGraphicDebug::UpdateGraphicDebug(float _DeltaTime)
	{
		UpdateDynamicBuffer();
		UpdateStaticBuffer();
	}

	void soGraphicDebug::RenderGrapicDebug(ID3D11DeviceContext* _pImmediateContext)
	{
		assert(_pImmediateContext != NULL);

		//Rendereamos los GraphicDebuguer
		_pImmediateContext->UpdateSubresource(m_VertexShader->GetBuffer(CB_Object).GetConstantBuffer(), 0, NULL, &m_CBObject, 0, 0);
		_pImmediateContext->VSSetConstantBuffers(1, 1, &m_VertexShader->GetBuffer(CB_Object).GetConstantBuffer());

		_pImmediateContext->VSSetShader(m_VertexShader->GetpVertexShader(), NULL, 0);
		_pImmediateContext->IASetInputLayout(m_VertexShader->GetInputLayout().GetpInputLayOut());

		_pImmediateContext->PSSetShader(m_PixelShader->GetpPixelShader(), NULL, 0);

		//Primero rendereamos los Estaticos
		if (m_StaticBuffer.GetpVertexInfoArraySize() != 0)
		{
			//Seteamos el VertexBuffer del mesh
			UINT stride = sizeof(SimpleVertex);
			UINT offset = 0;
			_pImmediateContext->IASetVertexBuffers(0, 1, &m_StaticBuffer.GetpVertexBuffer(), &stride, &offset);

			//Seteamos la topologia primitiva
			_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

			//Imprimimos el mesh
			_pImmediateContext->Draw(m_StaticBuffer.GetpVertexInfoArraySize(), 0);
		}

		//Ahora los dinamicos
		if (m_DynamicBuffer.GetpVertexInfoArraySize() != 0)
		{
			//Seteamos el VertexBuffer del mesh
			UINT stride = sizeof(SimpleVertex);
			UINT offset = 0;
			_pImmediateContext->IASetVertexBuffers(0, 1, &m_DynamicBuffer.GetpVertexBuffer(), &stride, &offset);

			//Seteamos la topologia primitiva
			_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

			//Imprimimos el mesh
			_pImmediateContext->Draw(m_DynamicBuffer.GetpVertexInfoArraySize(), 0);
		}
	}

	void soGraphicDebug::EndRender()
	{
		//Por ultimo, liberamos el buffer dinamico
		DestroyDynamicBuffer();
	}
}