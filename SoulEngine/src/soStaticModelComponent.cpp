/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soStaticModelComponent.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Definition of base class soStaticModelComponent
* @bug	  No know Bugs
*/
/************************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soResourceManager.h"
#include "soStaticModelComponent.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soStaticModelComponent					    */
	/************************************************************************/
	soStaticModelComponent::soStaticModelComponent()
	{
		Connect(std::bind(&soStaticModelComponent::OnStartUp, this, std::placeholders::_1));
		m_Model = NULL;
	}

	soStaticModelComponent::~soStaticModelComponent()
	{
		ShutDown();
	}

	void soStaticModelComponent::ChangeModel(soString ResourcePath)
	{
		ResourceParameters RP;
		RP.FilePath = ResourcePath;
		RP.ResourceType = RT_3DStaticModel;

		soStaticModel* NewModel = dynamic_cast<soStaticModel*>(soResourceManager::InstancePtr()->Load(RP));

		if (!NewModel)
		{
			SOE_OutputLOG("soStaticModelComponent", "OnStartUp", "Cannot Load File");
			return;
		}

		m_MaterialID = NewModel->m_DefaultMaterialID;

		soResourceManager::Instance().Destroy(m_Model);

		m_Model = NewModel;
	}

	void soStaticModelComponent::Update(float _DeltaTime)
	{
		SUPER::Update(_DeltaTime);

		m_Model->Update(_DeltaTime);
	}

	void soStaticModelComponent::Render(uint32 _MatProp)
	{
		m_Model->Render(_MatProp, m_MaterialID);
	}
		
	RESULT soStaticModelComponent::OnStartUp(const ComponentData& _ComponentData)
	{	
		SUPER::OnStartUp(_ComponentData);

		ResourceParameters RP;
		RP.FilePath = _ComponentData.ResourcePath;
		RP.ResourceType = RT_3DStaticModel;

		soStaticModel* NewModel = dynamic_cast<soStaticModel*>(soResourceManager::InstancePtr()->Load(RP));

		if (!NewModel)
		{
			SOE_OutputLOG("soStaticModelComponent", "OnStartUp", "Cannot Load File");
			return EC_FALSE;
		}

		m_MaterialID = NewModel->m_DefaultMaterialID;
		m_Model = NewModel;

		return EC_OK;
	}

	void soStaticModelComponent::OnShutDown()
	{
		soResourceManager::Instance().Destroy(m_Model);
	}
}
