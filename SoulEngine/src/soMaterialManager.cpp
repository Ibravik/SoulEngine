/********************************************************************/
/**
* @LC     12/6/2016
* @file   soMaterialManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   12/6/2016
* @brief  Implementacion de la clase soMaterialManager
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soMaterialManager.h"

/************************************************************************/
/* Definicion de la clase soResourceManager                             */
/************************************************************************/
namespace SoulSDK
{
	soMaterialManager::soMaterialManager()
	{
		Connect(std::bind(&soMaterialManager::OnStartUp, this));
		m_CurrentID = 0;
	}

	soMaterialManager::~soMaterialManager()
	{
	}

	RESULT soMaterialManager::OnStartUp()
	{
		//Creacion de material default
		MaterialData MD;
		MD.ID = m_CurrentID;
		MD.Name = TEXT("Default Material");

		CreateNewMaterial(MD);
		return EC_OK;
	}

	void soMaterialManager::OnShutDown()
	{
		std::map<uint32, soMaterial*>::iterator it = m_MaterialMap.begin();

		while (it != m_MaterialMap.end())
		{
			SAFE_DELETE((*it).second);
			m_MaterialMap.erase(it);
			it = m_MaterialMap.begin();
		}
	}

	soMaterial* soMaterialManager::CheckByName(soString& _MaterialName)
	{
		std::map<uint32, soMaterial*>::iterator it = m_MaterialMap.begin();

		while (it != m_MaterialMap.end())
		{
			if ((*it).second->m_MaterialName == _MaterialName)
			{
				return (*it).second;
			}
			it++;
		}

		return NULL;
	}

	uint32 soMaterialManager::CreateNewMaterial(MaterialData& _MaterialData)
	{
		soMaterial* NewMaterial = NULL;
		if (!_MaterialData.FilePath.empty())
		{
			NewMaterial = CheckByName(_MaterialData.Name);
		}

		if (NewMaterial != NULL)
		{
			return NewMaterial->m_ID;
		}

		_MaterialData.ID = m_CurrentID;
		NewMaterial = SOE_NEW soMaterial;
		
		if (FAIL(NewMaterial->StartUp(_MaterialData)))
		{
			SAFE_DELETE(NewMaterial);
			return NULL;
		}

		NewMaterial->m_References = 1;
		m_MaterialMap[m_CurrentID] = NewMaterial;
		GrowID();

		return NewMaterial->m_ID;
	}

	soMaterial* soMaterialManager::GetMaterialByID(uint32 _ID)
	{
		if (m_MaterialMap.find(_ID) != m_MaterialMap.end())
		{
			return m_MaterialMap.at(_ID);
		}
		return m_MaterialMap.at(0);
	}

	void soMaterialManager::DestroyMaterialByID(uint32 _ID)
	{
		soMaterial* _Material = GetMaterialByID(_ID);
		if (_Material->m_ID == 0)
		{
			return;
		}

		_Material->m_References--;

		if (_Material->m_References == 0)
		{
			std::map<uint32, soMaterial*>::iterator it = m_MaterialMap.begin();

			while (it != m_MaterialMap.end())
			{
				if ((*it).second->m_ID == _Material->m_ID)
				{
					SAFE_DELETE((*it).second);
					m_MaterialMap.erase(it);
					return;
				}
				it++;
			}
		}

		else if (_Material->m_References < 0)
			SAFE_DELETE(_Material);
	}

}
