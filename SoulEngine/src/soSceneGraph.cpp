/********************************************************************/
/**
* LC	  10/9/2016
* @file   soSceneGraph.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/13/2015
* @brief  Implementacion de la clase soSceneGraph
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soActor.h"
#include "soSceneGraph.h"

/************************************************************************/
/* Definicion de la clase soSceneGraph								    */
/************************************************************************/
namespace SoulSDK
{
	soSceneGraph::soSceneGraph()
	{
		Connect(std::bind(&soSceneGraph::OnStartUp, this));
		m_RenderedModelNum = 0;
		m_RenderedMeshNum = 0;
	}

	soSceneGraph::~soSceneGraph()
	{
		ShutDown();
	}

	RESULT soSceneGraph::OnStartUp()
	{
		m_ScenneRoot.m_InFrustum = true;
		soActor* Temp = SOE_NEW soActor;

		uint32 ID = 0;
		Temp->SetActorID(ID);
		Temp->m_ActorName = soString(TEXT("Scenne Root"));

		m_ScenneRoot.SetActor(Temp);
		return m_ScenneRoot.StartUp();		
	}

	void soSceneGraph::OnShutDown()
	{
		SAFE_DELETE_STDVECTOR(m_NodeList);
	}

	void soSceneGraph::CullingByFrustrum()
	{
		m_CullingNodeList.clear();

		for (uint32 i = 0; i < static_cast<uint32>(m_NodeList.size()); i++)
		{
			if (m_NodeList.at(i)->m_InFrustum)
			{
				m_CullingNodeList.push_back(m_NodeList.at(i));
			}

			m_NodeList.at(i)->m_InFrustum = false;
		}
	}

	void soSceneGraph::Update()
	{
		m_RenderedMeshNum = 0;
		m_RenderedModelNum = 0;

		soAIManager::Instance().Search(&m_ScenneRoot, NULL, PFT_BFS);		

		CullingByFrustrum();
	}

	void soSceneGraph::GenerateCustomNodeList(uint8 _MatProp)
	{
		m_CustomNodeList.clear();
		for (uint32 i = 0; i < static_cast<uint32>(m_CullingNodeList.size()); i++)
		{
			if (m_CullingNodeList.at(i)->CheckMaterialProperty(_MatProp))
			{
				m_CustomNodeList.push_back(m_CullingNodeList.at(i));
			}
		}
	}

	void soSceneGraph::CreateNode(soActor* _ActorNode)
	{
		soSceneNode* NewNode = SOE_NEW soSceneNode;
		NewNode->SetActor(_ActorNode);
		NewNode->StartUp();
		m_ScenneRoot.AddChild(NewNode);
		m_NodeList.push_back(NewNode);
	}		

	soSceneNode* soSceneGraph::FindNodeByActorID(uint32 _ID)
	{
		for (uint32 i = 0; i < static_cast<uint32>(m_NodeList.size()); i++)
		{
			if (m_NodeList.at(i)->m_ptrActor->GetActorID() == _ID)
			{
				return m_NodeList.at(i);
			}
		}
		return NULL;
	}

	RESULT soSceneGraph::RemoveNodeByActorID(uint32 _ID)
	{
		soSceneNode* Node = FindNodeByActorID(_ID);

		if (Node)
		{
			if (Node->m_Parent != NULL)
			{
				//Si somos destruidos, ligamos nuestros hijos al parent
				for (uint32 i = 0; i < static_cast<uint32>(Node->m_conexions.size()); i++)
				{
					Node->m_Parent->AddChild((soSceneNode*)Node->m_conexions.at(i));
				}
			}
			Node->SetParent(NULL);

			for (uint32 i = 0; i < static_cast<uint32>(m_NodeList.size()); i++)
			{
				if (m_NodeList.at(i)->m_ID == Node->m_ID)
				{
					m_NodeList.erase(m_NodeList.begin() + i);
				}
			}

			SAFE_DELETE(Node);

			return EC_OK;
		}
		return EC_FALSE;
	}

}