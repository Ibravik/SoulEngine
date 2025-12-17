/********************************************************************/
/**
* LC	  11/21/2016
* @file   soScenneNode.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/13/2015
* @brief  Implementacion de la clase soScenneNode
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soActor.h"
#include "soMaterial.h"
#include "soStaticModel.h"
#include "soStaticModelComponent.h"
#include "soSkeletalModel.h"
#include "soSkeletalModelComponent.h"
#include "soCameraManager.h"
#include "soCamera.h"
#include "soSceneNode.h"
#include "soWorldManager.h"
#include "soGraphicDebugManager.h"
#include "soMaterialManager.h"

/************************************************************************/
/* Definicion de la clase soScenneNode								    */
/************************************************************************/
namespace SoulSDK
{
	soSceneNode::soSceneNode()
	{
		Connect(std::bind(&soSceneNode::OnStartUp, this));
		m_ptrActor = NULL;
		m_Parent = NULL;
	}

	soSceneNode::~soSceneNode()
	{
		ShutDown();
	}
	
	RESULT soSceneNode::OnStartUp()
	{
		m_ID = m_ptrActor->GetActorID();
		return EC_OK;
	}

	void soSceneNode::OnShutDown()
	{		
		SAFE_DELETE(m_ptrActor);
	}
	
	void soSceneNode::SetActor(soActor* _ptrActor)
	{
		m_ptrActor = _ptrActor;
	}

	soMatrix4x4 soSceneNode::GetParentTransform()
	{
		if (m_Parent == NULL)
		{
			return soMath::IdentityMatrix4x4();
		}

		return m_Parent->m_ptrActor->m_ActorTransform.m_FinalTransform;
	}

	soMatrix4x4 soSceneNode::GetTransform()
	{
		return m_ptrActor->m_ActorTransform.m_Transform;
	}

	soMatrix4x4 soSceneNode::GetFinalNodeTransform()
	{
		return m_ptrActor->m_ActorTransform.m_FinalTransform;
	}

	void soSceneNode::Culling()
	{
		if (m_Parent != NULL &&m_Parent->m_InFrustum == true)
		{
			std::vector<soComponent*> MeshList = m_ptrActor->GetComponentbyType(CT_Mesh_Static | CT_Mesh_Dynamic);

			if (MeshList.empty())
			{
				m_InFrustum = true;
			}

			for (uint32 i = 0; i < static_cast<uint32>(MeshList.size()); i++)
			{
				soStaticModelComponent* StaticModel = dynamic_cast<soStaticModelComponent*>(MeshList.at(i));
				if (StaticModel != NULL)
				{
					soSphere CullingSphere = StaticModel->GetModel()->GetBoundingSphere();
					CullingSphere.m_fRadius *= 2.0f * (soMath::Max3(StaticModel->m_ComponentTransform.m_FinalTransform.Line[0][0], StaticModel->m_ComponentTransform.m_FinalTransform.Line[1][1], StaticModel->m_ComponentTransform.m_FinalTransform.Line[2][2]));
					soVector4D Center = CullingSphere.m_SphereCenter;
					Center.W = 1.0f;
					CullingSphere.m_SphereCenter = Center * StaticModel->m_ComponentTransform.m_FinalTransform;

					if (soCameraManager::Instance().m_ActiveCamera->SphereIncideFrustum(CullingSphere))
					{
						m_InFrustum = true;
						soWorldManager::Instance().m_SceneGraph.m_RenderedModelNum++;
					}
					else
					{
						m_InFrustum = false;
					}
				}

				soSkeletalModelComponent* SkeletalModel = dynamic_cast<soSkeletalModelComponent*>(MeshList.at(i));
				if (SkeletalModel != NULL)
				{
					soSphere CullingSphere = SkeletalModel->GetModel()->GetBoundingSphere();
					CullingSphere.m_fRadius *= 2.0f * soMath::Max3(SkeletalModel->m_ComponentTransform.m_FinalTransform.Line[0][0], SkeletalModel->m_ComponentTransform.m_FinalTransform.Line[1][1], SkeletalModel->m_ComponentTransform.m_FinalTransform.Line[2][2]);
					soVector4D Center = CullingSphere.m_SphereCenter;
					Center.W = 1.0f;
					CullingSphere.m_SphereCenter = Center * SkeletalModel->m_ComponentTransform.m_FinalTransform;

					if (soCameraManager::Instance().m_ActiveCamera->SphereIncideFrustum(CullingSphere))
					{
						m_InFrustum = true;
						SkeletalModel->m_InFrustum = true;
						soWorldManager::Instance().m_SceneGraph.m_RenderedModelNum++;
					}
					else
					{
						m_InFrustum = false;
						SkeletalModel->m_InFrustum = false;
					}
				}
			}
		}
	}

	bool soSceneNode::CheckMaterialProperty(uint8 _MaterialProp)
	{
		std::vector<soComponent*> MeshList = m_ptrActor->GetComponentbyType(CT_Mesh_Static | CT_Mesh_Dynamic);

		if (MeshList.empty())
		{
			return false;
		}

		for (uint32 i = 0; i < static_cast<uint32>(MeshList.size()); i++)
		{
			soStaticModelComponent* StaticModel = dynamic_cast<soStaticModelComponent*>(MeshList.at(i));
			if (StaticModel != NULL)
			{
				for (uint32 j = 0; j < static_cast<uint32>(StaticModel->GetModel()->m_MeshList.size()); j++)
				{
					return soMaterialManager::Instance().GetMaterialByID(StaticModel->m_MaterialID[j])->CheckMaterialProperty(_MaterialProp);
				}
			}

			soSkeletalModelComponent* SkeletalModel = dynamic_cast<soSkeletalModelComponent*>(MeshList.at(i));
			if (SkeletalModel != NULL)
			{
				for (uint32 j = 0; j < static_cast<uint32>(SkeletalModel->GetModel()->m_MeshList.size()); j++)
				{
					return soMaterialManager::Instance().GetMaterialByID(SkeletalModel->m_MaterialID[j])->CheckMaterialProperty(_MaterialProp);
				}
			}
		}
		return false;
	}
	
	void soSceneNode::SetParent(soSceneNode* _Parent)
	{		
		//Primero me desligo de mi pade actual, si tengo
		if (m_Parent != NULL)
		{
			m_Parent->RemoveChild(this);
		}

		m_Parent = _Parent;
	}

	void soSceneNode::RemoveParent()
	{
		m_Parent = NULL;
	}

	void soSceneNode::AddChild(soSceneNode* _Child)
	{
		AddConexions(_Child);

		_Child->SetParent(this);
	}

	void soSceneNode::RemoveChild(soSceneNode* _Child)
	{
		if (FAIL(RemoveConexion(*_Child)))
		{
			return;
		}
	}

	void soSceneNode::Visit(soNode* _prev)
	{
		//Seteamos el transform padre al nodo
		SUPER::Visit(_prev);
		if (m_ptrActor != NULL)
		{
			if (m_Parent != NULL)
			{
				m_ptrActor->m_ActorTransform.m_ParentTransform = m_Parent->m_ptrActor->m_ActorTransform.m_FinalTransform;
				m_ptrActor->m_ActorTransform.Update();
				Culling();
			}			
		}
		else
		{
			ShutDown();
		}
	}
}