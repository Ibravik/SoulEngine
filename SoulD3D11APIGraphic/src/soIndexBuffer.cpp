/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soIndexBuffer.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soIndexBuffer
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soBuffer.h"
#include "soIndexBuffer.h"

/************************************************************************/
/* Definicion de la clase soIndexBuffer					                */
/************************************************************************/
namespace SoulSDK
{
	soIndexBuffer::soIndexBuffer()
	{
		Connect(std::bind(&soIndexBuffer::OnStartUp, this, std::placeholders::_1));
		m_IndexArray = NULL;
		m_ptrIndexBuffer = NULL;
	}
	
	soIndexBuffer::~soIndexBuffer()
	{
		ShutDown();
	}

	RESULT soIndexBuffer::CreateMeshIndexInfo(const std::vector<aiMesh*>& MaterialMesh)
	{
		//Primero tomamos el numero total de indices
		for (uint32 j = 0; j < static_cast<uint32>(MaterialMesh.size()); j++)
		{
			for (uint32 i = 0; i < MaterialMesh.at(j)->mNumFaces; i++)
			{
				m_IndexArraySize += MaterialMesh.at(j)->mFaces[i].mNumIndices;
			}
		}

		if (m_IndexArraySize == 0)
		{
			SOE_OutputLOG("soIndexBuffer", "CreateMeshIndexInfo()", "FAIL to create Index info.");
			return EC_FALSE;
		}

		CreateIndexArray(m_IndexArraySize);

		//Ahora llenamos la informacion por indice
		uint32 k = 0;
		uint32 PrevTotal = 0;
		for (uint32 j = 0; j < static_cast<uint32>(MaterialMesh.size()); j++)
		{			
			for (uint32 i = 0; i < MaterialMesh.at(j)->mNumFaces; i++)
			{				
				for (uint32 a = 0; a < MaterialMesh.at(j)->mFaces[i].mNumIndices; a++)
				{
					m_IndexArray[k] = (MaterialMesh.at(j)->mFaces[i].mIndices[a] + PrevTotal);
					k++;
				}				
			}
			PrevTotal = k;
		}
		return EC_OK;
	}

	RESULT soIndexBuffer::OnStartUp(const std::vector<aiMesh*>& _Mesh)
	{		
		if (FAIL(CreateMeshIndexInfo(_Mesh)))
		{
			SOE_OutputLOG("soIndexBuffer", "OnStartUp()", "FAIL to create index info");
			return EC_FALSE;
		}

		return EC_OK;
	}

	void soIndexBuffer::OnShutDown()
	{
		DeleteIndexArray();
		SAFE_DELETE(m_ptrIndexBuffer);
	}

	RESULT soIndexBuffer::CreateIndexBuffer()
	{
		BufferData bd;
		bd.D3D11_USAGE = API_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(uint32) * GetIndexArraySize();
		bd.D3D11_BIND_FLAG = API_BIND_INDEX_BUFFER;
		bd.CPUAccesFlag = 0;
		bd.D3D11_SUBRESOURCE_DATA.pSysMem = *&m_IndexArray;

		m_ptrIndexBuffer = new soBuffer;
		if (FAIL(m_ptrIndexBuffer->StartUp(bd)))
		{
			SOE_OutputLOG("soConstantBuffer", "OnStartUp()", "FAIL to create constant buffer.");
			return EC_FALSE;
		}
		return EC_OK;
	}

}
