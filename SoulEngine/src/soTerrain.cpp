/********************************************************************/
/**
* @LC     11/8/2016
* @file   soTerrain.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/8/2016
* @brief  Implementacion de la clase soTerrain
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soTerrain.h"
#include "soResourceManager.h"

/************************************************************************/
/* Definicion de la clase soTerrain									    */
/************************************************************************/
namespace SoulSDK
{
	soTerrain::soTerrain()
	{
		Connect(std::bind(&soTerrain::OnStartUp, this));
	}

	soTerrain::~soTerrain()
	{
	}

	RESULT soTerrain::OnStartUp()
	{
		//TODO: Generar el grid a partir de la textura, por ahora es generico 

		TerrainData _TerrainData;
		_TerrainData.Width = 256.0f;
		_TerrainData.Height = 256.0f;

		//Determine the number of vertices and indices in the 256x256 grid
		uint32 VertexCount = (uint32)((_TerrainData.Width - 1) * (_TerrainData.Height - 1) * 8);

		// Set the index count to the same as the vertex count.
		uint32 IndexCount = VertexCount;

		// Create the vertex array.
		m_TerrainData.m_VertexBuffer.CreatesVertexArray(VertexCount);

		// Create the index array.
		m_TerrainData.m_IndexBuffer.CreateIndexArray(IndexCount);

		// Initialize the index into the vertex and index arrays.
		uint32 index = 0;

		//Uvs data
		/*float UvsJumpX = 1.0f / _TerrainData.Width;
		float UvsJumpY = 1.0f / _TerrainData.Height;*/

		// Load the vertex array and index array with data.
		for (float j = -(_TerrainData.Height / 2.0f); j < (_TerrainData.Height / 2.0f) - 1.0f; j++)
		{
			for (float i = -(_TerrainData.Width / 2.0f); i < (_TerrainData.Width / 2.0f) - 1.0f; i++)
			{
				// Line 1 - Upper left.
				m_TerrainData.m_VertexBuffer.GetsVertexArrayElement(index).Position = soVector4D((i* UNIT_X_MT), 0.0f, (j* UNIT_X_MT) + UNIT_X_MT, 1.0f);
				m_TerrainData.m_IndexBuffer.GetIndexArrayElement(index) = index;
				index++;

				// Line 1 - Upper right.
				m_TerrainData.m_VertexBuffer.GetsVertexArrayElement(index).Position = soVector4D((i* UNIT_X_MT) + UNIT_X_MT, 0.0f, (j* UNIT_X_MT) + UNIT_X_MT, 1.0f);
				m_TerrainData.m_IndexBuffer.GetIndexArrayElement(index) = index;
				index++;

				// Line 2 - Upper right
				m_TerrainData.m_VertexBuffer.GetsVertexArrayElement(index).Position = soVector4D((i* UNIT_X_MT) + UNIT_X_MT, 0.0f, (j* UNIT_X_MT) + UNIT_X_MT, 1.0f);
				m_TerrainData.m_IndexBuffer.GetIndexArrayElement(index) = index;
				index++;

				// Line 2 - Bottom right.
				m_TerrainData.m_VertexBuffer.GetsVertexArrayElement(index).Position = soVector4D((i* UNIT_X_MT) + UNIT_X_MT, 0.0f, (j* UNIT_X_MT), 1.0f);
				m_TerrainData.m_IndexBuffer.GetIndexArrayElement(index) = index;
				index++;

				// Line 3 - Bottom right.
				m_TerrainData.m_VertexBuffer.GetsVertexArrayElement(index).Position = soVector4D((i* UNIT_X_MT) + UNIT_X_MT, 0.0f, (j* UNIT_X_MT), 1.0f);
				m_TerrainData.m_IndexBuffer.GetIndexArrayElement(index) = index;
				index++;

				// Line 3 - Bottom left.
				m_TerrainData.m_VertexBuffer.GetsVertexArrayElement(index).Position = soVector4D((i* UNIT_X_MT), 0.0f, (j* UNIT_X_MT), 1.0f);
				m_TerrainData.m_IndexBuffer.GetIndexArrayElement(index) = index;
				index++;

				// Line 4 - Bottom left.
				m_TerrainData.m_VertexBuffer.GetsVertexArrayElement(index).Position = soVector4D((i* UNIT_X_MT), 0.0f, (j* UNIT_X_MT), 1.0f);
				m_TerrainData.m_IndexBuffer.GetIndexArrayElement(index) = index;
				index++;

				// Line 4 - Upper left.
				m_TerrainData.m_VertexBuffer.GetsVertexArrayElement(index).Position = soVector4D((i* UNIT_X_MT), 0.0f, (j* UNIT_X_MT) + UNIT_X_MT, 1.0f);
				m_TerrainData.m_IndexBuffer.GetIndexArrayElement(index) = index;
				index++;
			}			
		}
		
		if (FAIL(m_TerrainData.m_VertexBuffer.CreateVertexBuffer()))
		{
			return EC_FALSE;
		}

		if (FAIL(m_TerrainData.m_IndexBuffer.CreateIndexBuffer()))
		{
			return EC_FALSE;
		}

		m_TerrainData.m_APT = API_PRIMITIVE_TOPOLOGY_LINELIST;
		
		return EC_OK;
	}

	void soTerrain::OnShutDown()
	{

	}

	void soTerrain::Render()
	{
		m_TerrainData.Render(0, 0);
	}

	void soTerrain::SetHeightMap(soString _FilePath)
	{
	}
}