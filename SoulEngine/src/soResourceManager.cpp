/********************************************************************/
/**
* @LC     10/10/2016
* @file   soResourceManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/14/2015
* @brief  Implementacion de la clase soResourceManager
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soResourceManager.h"

/************************************************************************/
/* Definicion de la clase soResourceManager                             */
/************************************************************************/
namespace SoulSDK
{
	soResourceManager::soResourceManager()
	{
		Connect(std::bind(&soResourceManager::OnStartUp, this));
		m_ResourceID = 0;
		m_ResourceMap.clear();
	}

	soResourceManager::~soResourceManager()
	{
	}

	RESULT soResourceManager::OnStartUp()
	{
		for (uint8 i = 0; i < RT_TOTAL; i++)
		{
			m_ResourceMap[static_cast<eResourceType>(i)] = SOE_NEW std::vector<soResource*>;
		}

		return EC_OK;
	}

	void soResourceManager::OnShutDown()
	{
		SUPER::OnShutDown();
		int8 i = RT_TOTAL;
		i--;

		for (; i >= 0; i--)
		{
			while (!m_ResourceMap.at(i)->empty())
			{
				m_ResourceMap.at(i)->back()->ShutDown();
				SAFE_DELETE(m_ResourceMap.at(i)->back());
				m_ResourceMap[i]->pop_back();
			}

			SAFE_DELETE(m_ResourceMap.at(i));
		}

		m_ResourceID = 0;
	}

	void soResourceManager::CheckValidResourceName(ResourceParameters& _ResourceParameters)
	{
		if (_ResourceParameters.ResourceName.empty())
		{
			_ResourceParameters.ResourceName = soString(TEXT("Default"));
			_ResourceParameters.ResourceName += uint2ws(m_ResourceID);
		}
		else
		{
			for (uint32 i = 0; i < static_cast<uint32>(m_ResourceMap[_ResourceParameters.ResourceType]->size()); i++)
			{
				if (m_ResourceMap[_ResourceParameters.ResourceType]->at(i)->GetszResourceName() == _ResourceParameters.ResourceName)
				{
					_ResourceParameters.ResourceName += uint2ws(m_ResourceID);
					break;
				}
			}
		}
	}

	soResource* soResourceManager::CheckByFilePath(ResourceParameters& _ResourceParameters)
	{
		for (uint32 i = 0; i < static_cast<uint32>(m_ResourceMap[_ResourceParameters.ResourceType]->size()); i++)
		{
			if (m_ResourceMap[_ResourceParameters.ResourceType]->at(i)->GetszFilePath() == _ResourceParameters.FilePath)
			{
				int32 References = m_ResourceMap[_ResourceParameters.ResourceType]->at(i)->GetReferences();
				References++;
				m_ResourceMap[_ResourceParameters.ResourceType]->at(i)->SetReferences(References);
				return m_ResourceMap[_ResourceParameters.ResourceType]->at(i);
			}
		}
		return NULL;
	}

	soResource* soResourceManager::Load(ResourceParameters& _ResourceParameters)
	{
		soResource* LoadedResource = CheckByFilePath(_ResourceParameters);
		if (LoadedResource != NULL)
		{
			return LoadedResource;
		}

		CheckValidResourceName(_ResourceParameters);

		soResource* NewResource = m_ResourceFactory.CreateResourceByType(_ResourceParameters.ResourceType);

		if (FAIL(NewResource->Load(_ResourceParameters)))
		{
			SAFE_DELETE(NewResource);
			return NULL;
		}

		int32 StartReferences = 1;
		NewResource->SetnID(m_ResourceID);
		NewResource->SetReferences(StartReferences);
		m_ResourceMap.at(_ResourceParameters.ResourceType)->push_back(NewResource);
		GrowID();

		return NewResource;
	}

	soResource* soResourceManager::Create(ResourceParameters& _ResourceParameters)
	{
		soResource* LoadedResource = CheckByFilePath(_ResourceParameters);
		if (LoadedResource != NULL)
		{
			return LoadedResource;
		}

		CheckValidResourceName(_ResourceParameters);

		soResource* NewResource = m_ResourceFactory.CreateResourceByType(_ResourceParameters.ResourceType);

		if (FAIL(NewResource->Create(_ResourceParameters)))
		{
			SAFE_DELETE(NewResource);
			return NULL;
		}

		int32 StartReferences = 1;
		NewResource->SetnID(m_ResourceID);
		NewResource->SetReferences(StartReferences);
		m_ResourceMap.at(_ResourceParameters.ResourceType)->push_back(NewResource);
		GrowID();

		return NewResource;
	}

	void soResourceManager::Destroy(soResource* _Resource)
	{
		if (_Resource == NULL)
		{
			return;
		}

		int32 References = _Resource->GetReferences();
		References--; 
		_Resource->SetReferences(References);

		if (_Resource->GetReferences() == 0)
		{
			for (uint32 i = 0; i < m_ResourceMap.at(_Resource->GetResourceType())->size(); i++)
			{
				if (m_ResourceMap.at(_Resource->GetResourceType())->at(i)->GetnID() == _Resource->GetnID())
				{
					m_ResourceMap.at(_Resource->GetResourceType())->erase(m_ResourceMap.at(_Resource->GetResourceType())->begin() + i);
					SAFE_DELETE(_Resource);
					return;
				}
			}
		}
		else if(References<0)
		SAFE_DELETE(_Resource);
	}
}