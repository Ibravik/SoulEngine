/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soStaticVertexBuffer.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soStaticVertexBuffer
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soBuffer.h"
#include "soVertexBuffer.h"
#include <d3dx9.h>

/************************************************************************/
/* Definicion de la clase soStaticVertexBuffer			                */
/************************************************************************/
namespace SoulSDK
{
	soVertexBuffer::soVertexBuffer()
	{
		Connect(std::bind(&soVertexBuffer::OnStartUp, this, std::placeholders::_1));
		m_ptrVertexBuffer = NULL;
	}

	soVertexBuffer::~soVertexBuffer()
	{
		ShutDown();
	}

	RESULT soVertexBuffer::CreateMeshVertexInfo(const std::vector<aiMesh*>& MaterialMesh)
	{
		//Primero creamos el arreglo deacuerdo al numero de vertices que contiene el mesh
		uint32 VertexNumber = 0;
		for (uint32 i = 0; i < static_cast<uint32>(MaterialMesh.size()); i++)
		{
			VertexNumber += MaterialMesh.at(i)->mNumVertices;
		}
		CreatesVertexArray(VertexNumber);

		uint32 VertIndex = 0;
		uint32 InitIndex = 0;
		for (uint32 i = 0; i < static_cast<uint32>(MaterialMesh.size()); i++)
		{
			//Ahora llenamos la informacion por vertice
			InitIndex = VertIndex;
			for (uint32 j = 0; j < MaterialMesh.at(i)->mNumVertices; j++)
			{
				m_sVertexArray[VertIndex].Position.X = MaterialMesh.at(i)->mVertices[j].x;
				m_sVertexArray[VertIndex].Position.Y = MaterialMesh.at(i)->mVertices[j].y;
				m_sVertexArray[VertIndex].Position.Z = MaterialMesh.at(i)->mVertices[j].z;
				m_sVertexArray[VertIndex].Position.W = 1.0f;

				if (MaterialMesh.at(i)->HasTextureCoords(0))
				{
					m_sVertexArray[VertIndex].Texture.X = MaterialMesh.at(i)->mTextureCoords[0][j].x;
					m_sVertexArray[VertIndex].Texture.Y = MaterialMesh.at(i)->mTextureCoords[0][j].y;
				}
				else
				{
					m_sVertexArray[VertIndex].Texture.X = 0.0f;
					m_sVertexArray[VertIndex].Texture.Y = 0.0f;
				}

				if (MaterialMesh.at(i)->HasNormals())
				{
					m_sVertexArray[VertIndex].Normals.X = MaterialMesh.at(i)->mNormals[j].x;
					m_sVertexArray[VertIndex].Normals.Y = MaterialMesh.at(i)->mNormals[j].y;
					m_sVertexArray[VertIndex].Normals.Z = MaterialMesh.at(i)->mNormals[j].z;
				}
				else
				{
					m_sVertexArray[VertIndex].Normals.X = 0.0f;
					m_sVertexArray[VertIndex].Normals.Y = 0.0f;
					m_sVertexArray[VertIndex].Normals.Z = 0.0f;
				}

				if (MaterialMesh.at(i)->HasTangentsAndBitangents())
				{
					m_sVertexArray[VertIndex].Tangent.X = MaterialMesh.at(i)->mTangents[j].x;
					m_sVertexArray[VertIndex].Tangent.Y = MaterialMesh.at(i)->mTangents[j].y;
					m_sVertexArray[VertIndex].Tangent.Z = MaterialMesh.at(i)->mTangents[j].z;
			
					m_sVertexArray[VertIndex].BiNormals.X = MaterialMesh.at(i)->mBitangents[j].x;
					m_sVertexArray[VertIndex].BiNormals.Y = MaterialMesh.at(i)->mBitangents[j].y;
					m_sVertexArray[VertIndex].BiNormals.Z = MaterialMesh.at(i)->mBitangents[j].z;
				}
				else
				{
					m_sVertexArray[VertIndex].Tangent.X = 0.0f;
					m_sVertexArray[VertIndex].Tangent.Y = 0.0f;
					m_sVertexArray[VertIndex].Tangent.Z = 0.0f;

					m_sVertexArray[VertIndex].BiNormals.X = 0.0f;
					m_sVertexArray[VertIndex].BiNormals.Y = 0.0f;
					m_sVertexArray[VertIndex].BiNormals.Z = 0.0f;
				}
				VertIndex++;
			}

			if (MaterialMesh.at(i)->HasBones())
			{
				//Por ultimo seteamos la informacion de a que vertice afectamos y con cuanta intencidad
				for (uint32 a = 0; a < MaterialMesh.at(i)->mNumBones; a++)
				{
					for (uint32 j = 0; j < MaterialMesh.at(i)->mBones[a]->mNumWeights; j++)
					{
						//Tomamos la informacion del id del vertice y el peso que le corresponde
						uint32 VertexID = MaterialMesh.at(i)->mBones[a]->mWeights[j].mVertexId;
						float Weight = MaterialMesh.at(i)->mBones[a]->mWeights[j].mWeight;

						VertexID += InitIndex;
						for (uint32 k = 0; k < 4; k++)
						{							
							if (m_sVertexArray[VertexID].BoneName[k] == NULL)
							{
								soString* NewName = SOE_NEW soString;
#ifdef UNICODE
								*NewName = soString(s2ws(std::string(MaterialMesh.at(i)->mBones[a]->mName.C_Str())));
#else
								*NewName = std::string(MaterialMesh.at(i)->mBones[a]->mName.C_Str());
#endif
								m_sVertexArray[VertexID].BoneName[k] = NewName;
								m_sVertexArray[VertexID].BoneWeight[k] = Weight;
								break;
							}
						}
					}
				}
			}
		}
		return EC_OK;
	}
		
	RESULT soVertexBuffer::OnStartUp(const std::vector<aiMesh*>& _Mesh)
	{			
		if (FAIL(CreateMeshVertexInfo(_Mesh)))
		{
			SOE_OutputLOG("soVertexBuffer", "OnStartUp()", "FAIL to create Vertex array info.");
			return EC_FALSE;
		}	

		ComputeBoundingSphere();

		return EC_OK;
	}

	void soVertexBuffer::OnShutDown()
	{
		DeletesVertexArray();
		SAFE_DELETE(m_ptrVertexBuffer);
	}

	RESULT soVertexBuffer::CreateVertexBuffer()
	{
		if (m_sVertexArraySize != 0)
		{
			BufferData bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.D3D11_USAGE = API_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Vertex) * GetsVertexArraySize();
			bd.D3D11_BIND_FLAG = API_BIND_VERTEX_BUFFER;
			bd.CPUAccesFlag = 0;
			bd.D3D11_SUBRESOURCE_DATA.pSysMem = *&m_sVertexArray;

			m_ptrVertexBuffer = SOE_NEW soBuffer;
			if (FAIL(m_ptrVertexBuffer->StartUp(bd)))
			{
				SOE_OutputLOG("soVertexBuffer", "OnStartUp()", "FAIL to create constant buffer.");
				SAFE_DELETE(m_ptrVertexBuffer);
				return EC_FALSE;
			}
			return EC_OK;
		}
		return EC_FALSE;
	}

	void soVertexBuffer::ComputeBoundingSphere()
	{
		D3DXVECTOR3 center;

		DWORD numVertices = GetsVertexArraySize();
		DWORD fvfSize = sizeof(Vertex);

		D3DXComputeBoundingSphere((D3DXVECTOR3*)*&m_sVertexArray, numVertices, fvfSize, &center, &m_BoundingSphere.m_fRadius);
		
		m_BoundingSphere.m_SphereCenter = soVector3D(center.x, center.y, center.z);
	}

}
