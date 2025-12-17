/********************************************************************/
/**
* LC	  10/9/2016
* @file   soEditorControl.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/21/2016
* @brief  Implementacion de la clase soEditorControl
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "Soul_Graphics.h"
#include "soModel.h"
#include "soActor.h"
#include "soStaticModelComponent.h"
#include "soSkeletalModelComponent.h"
#include "soStaticModel.h"
#include "soSkeletalModel.h"
#include "soCameraManager.h"
#include "soCamera.h"
#include "soGraphicDebugManager.h"
#include "soWorldManager.h"
#include "soEditorControl.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soEditorControl							    */
	/************************************************************************/

	soEditorControl::soEditorControl()
	{
		Connect(std::bind(&soEditorControl::OnStartUp, this));
		m_SelectedNode = NULL;
		m_EditMaterial = false;
	}

	soEditorControl::~soEditorControl()
	{
		ShutDown();
	}

	RESULT soEditorControl::OnStartUp()
	{
		return EC_OK;
	}

	void soEditorControl::OnShutDown()
	{

	}

	void soEditorControl::HitSelectorSphere(soRay& _Ray)
	{
		m_SphereTouced.clear();
		/* Creamos una lista de objetos impactados por el rayo                  */

		for (uint32 i = 0; i < static_cast<uint32>(soWorldManager::Instance().m_SceneGraph.m_CullingNodeList.size()); i++)
		{
			soActor* ActualActor = soWorldManager::Instance().m_SceneGraph.m_CullingNodeList.at(i)->m_ptrActor;

			std::vector<soComponent*> MeshList = ActualActor->GetComponentbyType(CT_Mesh_Static | CT_Mesh_Dynamic);

			for (uint32 j = 0; j < static_cast<uint32>(MeshList.size()); j++)
			{
				soStaticModelComponent* StaticModel = dynamic_cast<soStaticModelComponent*>(MeshList.at(j));

				if (StaticModel != NULL)
				{
					//Obtenemos la esfera transformada a la matriz del componente
					soSphere CullingSphere = StaticModel->GetModel()->GetBoundingSphere();					
					CullingSphere.m_fRadius *= soMath::Max3(StaticModel->m_ComponentTransform.m_FinalTransform.Line[0][0], StaticModel->m_ComponentTransform.m_FinalTransform.Line[1][1], StaticModel->m_ComponentTransform.m_FinalTransform.Line[2][2]);
					soVector4D Center = CullingSphere.m_SphereCenter;
					Center.W = 1.0f; 
					CullingSphere.m_SphereCenter = Center * StaticModel->m_ComponentTransform.m_FinalTransform;

					if (CullingSphere.IsCollition(&_Ray))
					{
						m_SphereTouced.push_back(soWorldManager::Instance().m_SceneGraph.m_CullingNodeList.at(i));
					}
				}

				soSkeletalModelComponent* SkeletalModel = dynamic_cast<soSkeletalModelComponent*>(MeshList.at(j));

				if (SkeletalModel != NULL)
				{
					//Obtenemos la esfera transformada a la matriz del componente
					soSphere CullingSphere = SkeletalModel->GetModel()->GetBoundingSphere();
					CullingSphere.m_fRadius *= soMath::Max3(SkeletalModel->m_ComponentTransform.m_FinalTransform.Line[0][0], SkeletalModel->m_ComponentTransform.m_FinalTransform.Line[1][1], SkeletalModel->m_ComponentTransform.m_FinalTransform.Line[2][2]);
					soVector4D Center = CullingSphere.m_SphereCenter;
					Center.W = 1.0f;
					CullingSphere.m_SphereCenter = Center * SkeletalModel->m_ComponentTransform.m_FinalTransform;

					if (CullingSphere.IsCollition(&_Ray))
					{
						m_SphereTouced.push_back(soWorldManager::Instance().m_SceneGraph.m_CullingNodeList.at(i));
					}
				}
			}
			ActualActor = NULL;
		}
	}

	soSceneNode* soEditorControl::HitMesh(soRay& _Ray)
	{
		if (m_SphereTouced.empty())
		{
			return NULL;
		}

		soVector3D CloseTriangleCenter(soCameraManager::Instance().m_ActiveCamera->m_Far, soCameraManager::Instance().m_ActiveCamera->m_Far, soCameraManager::Instance().m_ActiveCamera->m_Far);
		soSceneNode* ClosedNode = NULL;

		/* Transformamos el rayo a espacio local del modelo	                    */
		while (!m_SphereTouced.empty())
		{
			std::vector<soComponent*> MeshList = m_SphereTouced.back()->m_ptrActor->GetComponentbyType(CT_Mesh_Static | CT_Mesh_Dynamic);

			//Tomamos el modelo mas cercano de la lista
			for (uint32 j = 0; j < static_cast<uint32>(MeshList.size()); j++)
			{
				Triangle ActualTriangle;
				Vertex ActualVertex[3];
				for (uint8 i = 0; i < 3; i++)
				{
					ActualTriangle.TriangeVertex[i] = &ActualVertex[i];
				}

				soStaticModelComponent* StaticModel = dynamic_cast<soStaticModelComponent*>(MeshList.at(j));

				if (StaticModel != NULL)
				{
					for (uint32 k = 0; k < static_cast<uint32>(StaticModel->GetModel()->m_ModelTriangleList.size()); k++)
					{
						for (uint8 i = 0; i < 3; i++)
						{
							*ActualTriangle.TriangeVertex[i] = *StaticModel->GetModel()->m_ModelTriangleList.at(k).TriangeVertex[i];

							ActualTriangle.TriangeVertex[i]->Position *= StaticModel->m_ComponentTransform.m_FinalTransform;

							ZeroMemory(ActualTriangle.TriangeVertex[i]->BoneName, sizeof(void*) * 4);
						}

						if (ActualTriangle.Triangle_intersection(_Ray))
						{
							ActualTriangle.SetTriangleCenter();

							if (soMath::Vector3DLength(ActualTriangle.TriangleCenter - soCameraManager::Instance().m_ActiveCamera->m_Position) < soMath::Vector3DLength(CloseTriangleCenter - soCameraManager::Instance().m_ActiveCamera->m_Position))
							{
								CloseTriangleCenter = ActualTriangle.TriangleCenter;
								ClosedNode = m_SphereTouced.back();
							}
						}
					}
				}

				soSkeletalModelComponent* SkeletalModel = dynamic_cast<soSkeletalModelComponent*>(MeshList.at(j));

				if (SkeletalModel != NULL)
				{
					for (uint32 k = 0; k < static_cast<uint32>(SkeletalModel->GetModel()->m_ModelTriangleList.size()); k++)
					{
						for (uint8 i = 0; i < 3; i++)
						{
							*ActualTriangle.TriangeVertex[i] = *SkeletalModel->GetModel()->m_ModelTriangleList.at(k).TriangeVertex[i];

							soMatrix4x4 invBone = SkeletalModel->m_AnimationData.BoneMatrix[ActualTriangle.TriangeVertex[i]->BoneIndex[0]] * ActualTriangle.TriangeVertex[i]->BoneWeight[0];
							invBone += SkeletalModel->m_AnimationData.BoneMatrix[ActualTriangle.TriangeVertex[i]->BoneIndex[1]] * ActualTriangle.TriangeVertex[i]->BoneWeight[1];
							invBone += SkeletalModel->m_AnimationData.BoneMatrix[ActualTriangle.TriangeVertex[i]->BoneIndex[2]] * ActualTriangle.TriangeVertex[i]->BoneWeight[2];
							invBone += SkeletalModel->m_AnimationData.BoneMatrix[ActualTriangle.TriangeVertex[i]->BoneIndex[3]] * ActualTriangle.TriangeVertex[i]->BoneWeight[3];

							ActualTriangle.TriangeVertex[i]->Position *= invBone;
							ActualTriangle.TriangeVertex[i]->Position *= SkeletalModel->m_ComponentTransform.m_FinalTransform;

							ZeroMemory(ActualTriangle.TriangeVertex[i]->BoneName, sizeof(void*) * 4);
						}

						if (ActualTriangle.Triangle_intersection(_Ray))
						{
							ActualTriangle.SetTriangleCenter();

							if (soMath::Vector3DLength(ActualTriangle.TriangleCenter - soCameraManager::Instance().m_ActiveCamera->m_Position) < soMath::Vector3DLength(CloseTriangleCenter - soCameraManager::Instance().m_ActiveCamera->m_Position))
							{
								CloseTriangleCenter = ActualTriangle.TriangleCenter;
								ClosedNode = m_SphereTouced.back();
							}
						}
					}
				}
			}
			m_SphereTouced.pop_back();
		}
		m_SphereTouced.clear();
		m_SelectedNode = ClosedNode;
		return ClosedNode;
	}
	
	void soEditorControl::ResetEditorData()
	{
		m_ActualSelectedMesh = 0;
		m_ActualAnimation = 0;
		m_EditMaterial = false;
		m_ActualSelectedComponent = 0;
		m_StaticModel = NULL;
		m_SkeletalModel = NULL;
	}

	void soEditorControl::SelectNodeModel(soRay _Ray)
	{
		HitSelectorSphere(_Ray);
		SetSelectedNode(HitMesh(_Ray));
	}

	void soEditorControl::SetSelectedNode(soSceneNode* _SelectedNode)
	{
		m_SelectedNode = _SelectedNode;

		if (m_SelectedNode)
		{
			ResetEditorData();
		}
	}

	void soEditorControl::AddComponentToSelectedActor(ComponentData & _Data)
	{
		_Data.userData = m_SelectedNode->m_ptrActor;
		soComponent* NewComponent = m_ComponentFactory.CreateComponent(_Data);

		if (NewComponent)
		{
			m_SelectedNode->m_ptrActor->AddNewComponent(NewComponent);
		}		
	}

	void soEditorControl::RemoveComponentToSelectedActor()
	{
		m_SelectedNode->m_ptrActor->RemoveComponentByID(m_ActualSelectedComponent);
	}
}