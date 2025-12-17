/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soActor.h
* @Author Lehi Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Definition of base class soActor
* @bug	  No know Bugs
*/
/************************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soActor.h"
#include "soPhysicComponent.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soActor									    */
	/************************************************************************/
	soActor::soActor()
	{
		Connect(std::bind(&soActor::OnStartUp, this));
		m_ComponentFlag = 0;
		m_TotalComponentNumber = 0;
	}
	
	soActor::~soActor()
	{
		ShutDown();
	}

	RESULT soActor::OnStartUp()
	{		
		return EC_OK;
	}

	void soActor::OnShutDown()
	{
		SAFE_DELETE_STDVECTOR(m_Components);
	}

	void soActor::Update(float _DeltaTime)
	{
		m_ActorTransform.Update();

		for (uint32 i = 0; i < static_cast<uint32>(m_Components.size()); i++)
		{
			if (m_Components.at(i)->m_Update)
			{				
				m_Components.at(i)->m_ComponentTransform.m_ParentTransform = m_ActorTransform.m_FinalTransform;
				m_Components.at(i)->Update(_DeltaTime);
			}
		}
	}

	void soActor::Render()
	{
		for (uint32 i = 0; i < static_cast<uint32>(m_Components.size()); i++)
		{
			if (m_Components.at(i)->m_Visible)
			{
				m_Components.at(i)->Render();
			}
		}
	}

	void soActor::AddNewComponent(soComponent* _Component)
	{
		if (_Component != NULL)
		{
			m_TotalComponentNumber++;
			m_Components.push_back(_Component);
		}
	}

	void soActor::RemoveComponentByID(uint32 _PosInList)
	{
		if (_PosInList < m_Components.size())
		{
			SAFE_DELETE(m_Components.at(_PosInList));
			m_Components.erase(m_Components.begin() + _PosInList);
			m_TotalComponentNumber--;
		}
	}

	std::vector<soComponent*> soActor::GetComponentbyType(uint64 _ComponentFlag)
	{
		std::vector<soComponent*> CompByType;

		for (uint32 i = 0; i < static_cast<uint32>(m_Components.size()); i++)
		{
			if (m_Components.at(i)->m_ComponentType & _ComponentFlag)
			{
				CompByType.push_back(m_Components.at(i));
			}
		}

		return CompByType;
	}
}