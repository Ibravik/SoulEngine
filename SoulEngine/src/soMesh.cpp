/********************************************************************/
/**
* @LC	  10/9/2016
* @file   soMesh.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soStaticMesh
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soWorldManager.h"
#include "soMaterialManager.h"
#include "soMesh.h"

/************************************************************************/
/* Definicion de la clase soStaticMesh								    */
/************************************************************************/
namespace SoulSDK
{
	soMesh::soMesh()
	{
		Connect(std::bind(&soMesh::OnStartUp, this, std::placeholders::_1));
	}

	soMesh::~soMesh()
	{
		ShutDown();
	}

	RESULT soMesh::OnStartUp(const MeshData& _MeshData)
	{
		//Creamos el index y vertex buffer
		if (FAIL(m_VertexBuffer.StartUp(*_MeshData.MaterialMesh)))
		{
			return EC_FALSE;
		}
		m_VertexBuffer.CreateVertexBuffer();

		if (FAIL(m_IndexBuffer.StartUp(*_MeshData.MaterialMesh)))
		{
			return EC_FALSE;
		}
		m_IndexBuffer.CreateIndexBuffer();

		m_BoundingSphere = m_VertexBuffer.GetBoundingSphere();
	
		m_APT = _MeshData.APT;

		return EC_OK;
	}

	void soMesh::OnShutDown()
	{

	}
	
	void soMesh::Render(uint32 _MatProp, uint32 _MatID)
	{
		soWorldManager::Instance().m_SceneGraph.m_RenderedMeshNum ++;

		//Seteamos los parametros del material
		soMaterialManager::Instance().GetMaterialByID(_MatID)->Render(_MatProp);
				
		uint32 StartSlot = 0;
		uint32 NumBuffers = 1;
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		APIGI_FORMAT AF = APIGI_FORMAT_R32_UINT;
		uint32 StartIndexLocation = 0; 
		int32 StartVertexLocation = 0;

		//Seteamos el VertexBuffer del mesh
		soDXGIManager::Instance().SetVertexBuffer(StartSlot, NumBuffers, m_VertexBuffer, stride, offset);
		
		//Seteamos la topologia primitiva
		soDXGIManager::Instance().SetPrimitiveTopology(m_APT);

		if (m_APT == API_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		{
			//Seteamos el index buffer del mesh
			soDXGIManager::Instance().SetIndexBuffer(m_IndexBuffer, AF, offset);

			//Imprimimos el mesh
			soDXGIManager::Instance().DrawIndexed(m_IndexBuffer, StartIndexLocation, StartVertexLocation);
		}
		else if (m_APT == API_PRIMITIVE_TOPOLOGY_LINELIST)
		{
			//Imprimimos la lista de lineas 
			uint32 VertexArraySize = m_VertexBuffer.GetsVertexArraySize();
			soDXGIManager::Instance().Draw(VertexArraySize, StartVertexLocation);
		}

		soMaterialManager::Instance().GetMaterialByID(_MatID)->EndRender();
	}
}
