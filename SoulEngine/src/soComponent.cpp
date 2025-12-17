/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soActor.h
* @Author Lehi Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Definition of base class soComponent
* @bug	  No know Bugs
*/
/************************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soComponent.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soComponent								    */
	/************************************************************************/
	soComponent::soComponent()
	{
		Connect(std::bind(&soComponent::OnStartUp, this, std::placeholders::_1));

		m_Update = true;
		m_Visible = true;
		m_ComponentType = 0;
	}

	soComponent::~soComponent()
	{
		ShutDown();
	}

	void soComponent::Update(float _DeltaTime)
	{
		(void)_DeltaTime;
		m_ComponentTransform.Update();
	}

	void soComponent::Render()
	{
	}

	RESULT soComponent::OnStartUp(const ComponentData& _ComponentData)
	{		
		m_ComponentType = _ComponentData._ComponentType;
		return EC_OK;
	}

	void soComponent::OnShutDown()
	{
	}
}
