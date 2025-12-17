/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soWorldManager.h
* @Author Lehi Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Definition of base class soWorldManager
* @bug	  No know Bugs
*/
/************************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soAssetManager.h"
#include "soWorldManager.h"
#include "SoulEngineDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soWorldManager                                */
	/************************************************************************/
	soWorldManager::soWorldManager()
	{
		Connect(std::bind(&soWorldManager::OnStartUp, this));
		m_CurrentID = 1;
	}

	soWorldManager::~soWorldManager()
	{
		
	}

	RESULT soWorldManager::CreateNewActor(const soString& _ActorName)
	{
		soActor* FinalActor = SOE_NEW soActor();

		FinalActor->SetActorID(m_CurrentID);	
		FinalActor->m_ActorName = _ActorName;
		CheckValidActorName(*FinalActor);
		FinalActor->StartUp();

		m_ActorList.push_back(FinalActor);

		m_SceneGraph.CreateNode(FinalActor);
		
		m_CurrentID++;
		return EC_OK;
	}

	RESULT soWorldManager::DeleteActor(uint32 _ActorID)
	{
		for (uint32 i = 0; i < (uint32)m_ActorList.size(); i++)
		{
			if (m_ActorList.at(i)->GetActorID() == _ActorID)
			{
				m_DeletedList.push_back(m_ActorList.at(i));
			}
		}

		return EC_OK;
	}

	RESULT soWorldManager::LoadActor(const soString& _AssetFileName)
	{
		soActor* FinalActor = static_cast<soActor*>(soAssetManager::Instance().LoadAssetFromFile(_AssetFileName));

		if (!FinalActor)
		{
			SOE_OutputLOG("soWorldManager", "CreateNewActor", "Failed to Load Actor Asset");
			return EC_FALSE;
		}

		FinalActor->SetActorID(m_CurrentID);
		CheckValidActorName(*FinalActor);

		m_ActorList.push_back(FinalActor);

		m_SceneGraph.CreateNode(m_ActorList.back());
		m_CurrentID++;
		return EC_OK;
	}

	soActor* soWorldManager::GetActorByName(const soString& _ActorName)
	{
		for (uint32 i = 0; i < static_cast<uint32>(m_ActorList.size()); i++)
		{
			if (m_ActorList.at(i)->m_ActorName == _ActorName)
			{
				return m_ActorList.at(i);
			}
		}

		return NULL;
	}

	soActor* soWorldManager::GetActorByID(uint64 _ActorID)
	{
		for (uint32 i = 0; i < static_cast<uint32>(m_ActorList.size()); i++)
		{
			if (m_ActorList.at(i)->GetActorID() == _ActorID)
			{
				return m_ActorList.at(i);
			}
		}

		return NULL;
	}

	void soWorldManager::Update(float _DeltaTime)
	{	
		while (!m_DeletedList.empty())
		{
			for (std::vector<soActor*>::iterator it = m_ActorList.begin(); it != m_ActorList.end(); it++)
			{
				if ((*it)->GetActorID() == m_DeletedList.back()->GetActorID()) //razon para eliminarlos en orden (for y find)
				{					
					m_SceneGraph.RemoveNodeByActorID((*it)->GetActorID());
					m_ActorList.erase(it);
					m_DeletedList.pop_back();
					it = m_ActorList.begin();
					break;
				}
			}
		}

		for (uint32 i = 0; i < static_cast<uint32>(m_ActorList.size()); i++)
		{
			m_ActorList.at(i)->Update(_DeltaTime);
		}
	}

	void soWorldManager::Render()
	{
		for (uint32 i = 0; i < static_cast<uint32>(m_ActorList.size()); i++)
		{
			m_ActorList.at(i)->Render();
		}
	}

	RESULT soWorldManager::OnStartUp()
	{
		return m_SceneGraph.StartUp();
	}

	void soWorldManager::OnShutDown()
	{
	}

	void soWorldManager::CheckValidActorName(soActor& _Actor)
	{
		if (_Actor.m_ActorName.empty())
		{
			_Actor.m_ActorName = soString(TEXT("Default"));
#ifdef UNICODE
			_Actor.m_ActorName += uint2ws(_Actor.GetActorID());
#else
			_Actor.m_ActorName += uint2s(_Actor.GetActorID());
#endif // UNICODE

			
			return;
		}
		else
		{
			for (uint32 i = 0; i < static_cast<uint32>(m_ActorList.size()); i++)
			{
				if (m_ActorList.at(i)->m_ActorName == _Actor.m_ActorName && m_ActorList.at(i)->GetActorID() != _Actor.GetActorID())
				{
#ifdef UNICODE
					_Actor.m_ActorName += uint2ws(_Actor.GetActorID());
#else
					_Actor.m_ActorName += uint2s(_Actor.GetActorID());
#endif
					return;
				}
			}
		}
	}

}
