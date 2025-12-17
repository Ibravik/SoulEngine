/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soModel.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   1/23/2016
* @brief  Implementacion de la clase soModel
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soModel.h"
#include "soMaterialManager.h"
#include "soGraphicDebugManager.h"

/************************************************************************/
/* Definicion de la clase soModel						                */
/************************************************************************/
namespace SoulSDK
{
	soModel::soModel()
	{
		m_IsRougthnes = (uint32)1;
	}

	soModel::~soModel()
	{
		ShutDown();
	}

	void soModel::ShutDown()
	{
		SAFE_DELETE_STDVECTOR(m_MeshList);
	}

	RESULT soModel::Load(const ResourceParameters& _ResourceParameters)
	{
		SUPER::Load(_ResourceParameters);

		//Primero leemos la informacion del archivo fuente
		if (FAIL(ReadFileInfo(_ResourceParameters.FilePath)))
		{
			return EC_FALSE;
		}

		//Ahora generamos una lista con los meshes que compartan el mismo material
		for (uint32 i = 0; i < m_Scene->mNumMaterials; i++)
		{
			std::vector<aiMesh*> MaterialMesh;

			for (uint32 j = 0; j < m_Scene->mNumMeshes; j++)
			{
				
				if (m_Scene->mMeshes[j]->mMaterialIndex == i)
				{
					MaterialMesh.push_back(m_Scene->mMeshes[j]);
				}
			}

			soMesh* NewMesh = SOE_NEW soMesh;
			MeshData _MeshData;
			_MeshData.MaterialMesh = &MaterialMesh;
			_MeshData.FilePath = GetFilePath(_ResourceParameters.FilePath);

			if (FAIL(NewMesh->StartUp(_MeshData)))
			{//Si el mesh se crea correctamente
				SAFE_DELETE(NewMesh);
			}
			else
			{
				uint32 ID = (uint32)m_MeshList.size();
				NewMesh->m_MeshName = m_szFileName + soString(TEXT("_Mesh_")) + uint2ws(ID);

				MaterialData MD;
				MD.FilePath = _MeshData.FilePath;
				MD.Material = m_Scene->mMaterials[i];
				MD.Name = m_szFileName + soString(TEXT("_Material_")) + uint2ws(ID);

				m_DefaultMaterialID.push_back(soMaterialManager::Instance().CreateNewMaterial(MD));

				m_MeshList.push_back(NewMesh);
			}
		}

		CreateBoundingSphere();

		CreateTriangleList();

		return EC_OK;
	}

	void soModel::Render(uint32 __MatProp, std::vector<uint32> _MaterialIDs)
	{
		//Seteamos los settings propios de cada mesh para su rendereo
		for (uint32 j = 0; j < m_MeshList.size(); j++)
		{
			m_MeshList.at(j)->Render(__MatProp, _MaterialIDs[j]);
		}
	}

	RESULT soModel::ReadFileInfo(soString _FilePath)
	{

		// Create an instance of the Importer class
		// And have it read the given file with some example postprocessing
		// Usually - if speed is not the most important aspect for you - you'll 
		// propably to request more postprocessing than we do in this example.
		m_Scene = m_Importer.ReadFile(ws2s(_FilePath), aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_Triangulate);

		// If the import failed, report it
		if (!m_Scene || m_Scene->mNumMeshes == 0)
		{
			SOE_OutputLOG("soStaticModel", "ReadFileInfo()", "Failed to read 3D model file.");
			return EC_FALSE;
		} 

		return EC_OK;
	}

	void soModel::CreateTriangleList()
	{
		for (uint32 i = 0; i < static_cast<uint32>(m_MeshList.size()); i++)
		{
			//Generamos cada triangulo a partir de la lista de vertices e indices
			for (uint32 j = 0; j < m_MeshList.at(i)->m_IndexBuffer.GetIndexArraySize(); )
			{
				Triangle NewTriangle;
				for (uint8 k = 0; k < 3; k++)
				{
					NewTriangle.TriangeVertex[k] = &m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(m_MeshList.at(i)->m_IndexBuffer.GetIndexArrayElement(j));
					j++;
				}

				NewTriangle.SetTriangleCenter();
				m_ModelTriangleList.push_back(NewTriangle);
			}
		}
	}

	void soModel::CreateBoundingSphere()
	{
		if (!m_MeshList.empty())
		{		
			uint32 TotalModelVertex = 0;
			uint32 Index = 0;

			//Generamos un nuevo buffer temporal con todos los vertices
			for (uint32 i = 0; i < m_MeshList.size(); i++)
			{				
				TotalModelVertex += m_MeshList.at(i)->m_VertexBuffer.GetsVertexArraySize();
			}
			soVertexBuffer TempBuffer;
			TempBuffer.CreatesVertexArray(TotalModelVertex);

			//Llenamos la informacion por vertice
			for (uint32 i = 0; i < m_MeshList.size(); i++)
			{				
				for (uint32 j = 0; j < m_MeshList.at(i)->m_VertexBuffer.GetsVertexArraySize(); j++)
				{
					TempBuffer.GetsVertexArrayElement(Index) = m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j);
					Index++;
				}
			}
			
			TempBuffer.ComputeBoundingSphere();
			m_BoundingSphere = TempBuffer.GetBoundingSphere();
		}
	}
}