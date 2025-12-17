/************************************************************************/
/**
* @LC	  11/21/2016
* @file   soSkeletalModelComponent.h
* @Author Lehi Munoz (lehi.munoz@gmail.com)
* @date   9/29/2016
* @brief  Definicion of base class soSkeletalModelComponent.
* @bug	  No know Bugs
*/
/************************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soResourceManager.h"
#include "soSkeletalModelComponent.h"


namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soSkeletalModelComponent					    */
	/************************************************************************/
	soSkeletalModelComponent::soSkeletalModelComponent()
	{
		Connect(std::bind(&soSkeletalModelComponent::OnStartUp, this, std::placeholders::_1));
		m_Model = NULL;
		m_InFrustum = false;
	}

	soSkeletalModelComponent::~soSkeletalModelComponent()
	{
		ShutDown();
	}

	void soSkeletalModelComponent::Update(float _DeltaTime)
	{
		SUPER::Update(_DeltaTime);

		m_AnimationData.Update(_DeltaTime);
		
		if (m_Model && m_InFrustum)
		{
			m_Model->Update(m_AnimationData);
			m_SkeletalCB = m_Model->m_SkeletalCB;

			if(m_AnimationData.OutAnimation._Animation != NULL)
				m_SkeletalCB.IndexAnimationOut = m_AnimationData.OutAnimation._Animation->m_ID;

			if (m_AnimationData.InAnimation._Animation != NULL)
				m_SkeletalCB.IndexAnimationIn = m_AnimationData.InAnimation._Animation->m_ID;
		}	

		SetBlendAnimationSettings();
	}

	void soSkeletalModelComponent::ChangeModel(soString ResourcePath)
	{
		ResourceParameters RP;
		RP.FilePath = ResourcePath;
		RP.ResourceType = RT_3DSkeletalModel;

		soSkeletalModel* NewModel = dynamic_cast<soSkeletalModel*>(soResourceManager::InstancePtr()->Load(RP));

		if (!NewModel)
		{
			SOE_OutputLOG("soStaticModelComponent", "OnStartUp", "Cannot Load File");
			return;
		}

		m_MaterialID = NewModel->m_DefaultMaterialID;

		soResourceManager::Instance().Destroy(m_Model);

		ZeroMemory(&m_AnimationData, sizeof(AnimationData));

		m_Model = NewModel;
	}

	void soSkeletalModelComponent::Render(uint32 _MatProp)
	{
		SUPER::Render();

		if (m_Model)
		{
			m_Model->Render(_MatProp, m_MaterialID);
		}
	}
		
	void soSkeletalModelComponent::PlayAnimation(soString _AnimationName)
	{
		if (m_Model)
		{	
			if (m_AnimationData.InAnimation._Animation != NULL)
			{
				if (m_AnimationData.InAnimation._Animation->m_Name != _AnimationName)
				{
					m_NextAnimation._Animation = m_Model->PlayAnimation(_AnimationName);
					m_NextAnimation.AnimationTime = 0.0f;
					m_AnimationData.Blend = true;
				}
			}
			else
			{
				m_AnimationData.InAnimation._Animation = m_Model->PlayAnimation(_AnimationName);
				m_AnimationData.InAnimation.AnimationTime = 0.0f;
				m_AnimationData.Blend = false;
			}
		}
	}

	void soSkeletalModelComponent::SetBlendAnimationSettings()
	{
		if (m_AnimationData.Blend && m_AnimationData.BlendFactor == 1.0f)
		{			
			m_AnimationData.OutAnimation = m_AnimationData.InAnimation;
			m_AnimationData.InAnimation = m_NextAnimation;

			m_AnimationData.Blend = false;
			SetFadeTime();
		}
	}

	void soSkeletalModelComponent::AddAnimation(soString& _FilePath)
	{
		if (m_Model)
		{
			m_Model->AddAnimation(_FilePath);
		}
	}

	soString soSkeletalModelComponent::GetAnimationName(uint32 _Index)
	{
		if (m_Model)
		{
			std::map<soString, soAnimation>::iterator It = m_Model->m_AnimationMap.begin();
			uint32 i = 0;

			while (It != m_Model->m_AnimationMap.end() && i != _Index)
			{
				It++;
				i++;
			}

			if (It != m_Model->m_AnimationMap.end())
			{
				return (*It).first;
			}
			return soString();
		}
		return soString();
	}

	RESULT soSkeletalModelComponent::OnStartUp(const ComponentData& _ComponentData)
	{
		SUPER::OnStartUp(_ComponentData);

		ResourceParameters RP;
		RP.FilePath = _ComponentData.ResourcePath;
		RP.ResourceType = RT_3DSkeletalModel;

		soSkeletalModel* NewModel = dynamic_cast<soSkeletalModel*>(soResourceManager::InstancePtr()->Load(RP));

		if (!NewModel)
		{
			SOE_OutputLOG("soStaticModelComponent", "OnStartUp", "Cannot Load File");
			return EC_FALSE;
		}

		m_MaterialID = NewModel->m_DefaultMaterialID;

		m_Model = NewModel;
		return EC_OK;
	}

	void soSkeletalModelComponent::OnShutDown()
	{
		if(soResourceManager::IsStarted())
			soResourceManager::Instance().Destroy(m_Model);
	}

	void soSkeletalModelComponent::SetFadeTime()
	{
		float EndTime = 0;
		if (m_AnimationData.OutAnimation._Animation != NULL && m_AnimationData.InAnimation._Animation != NULL)
		{
			EndTime = soMath::Min(m_AnimationData.OutAnimation._Animation->m_Duration - m_AnimationData.OutAnimation.AnimationTime, m_AnimationData.InAnimation._Animation->m_Duration - m_AnimationData.InAnimation.AnimationTime);
		}

		if (EndTime < 2.0f)
		{
			m_AnimationData.FadeTime = EndTime;
		}
		else
		{
			m_AnimationData.FadeTime = 2.0f;
		}

		m_AnimationData.BlendTime = 0.0f;
		m_AnimationData.BlendFactor = 0.0f;
	}
}
