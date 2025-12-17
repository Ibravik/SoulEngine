/********************************************************************/
/**
* @LC	  10/10/2016
* @file   soTechnique.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/29/2015
* @brief  Implementacion de la clase soTechnique
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soRenderPass.h"
#include "soTechnique.h"

/************************************************************************/
/* Definicion de la clase soTechnique									*/
/************************************************************************/
namespace SoulSDK
{
	soTechnique::soTechnique()
	{
		Connect(std::bind(&soTechnique::OnStartUp, this, std::placeholders::_1));
		m_RenderPassID = 0;
		m_RenderPassList.clear();
		m_RenderPassNameMap.clear();
	}
	
	soTechnique::~soTechnique()
	{
		ShutDown();
	}

	RESULT soTechnique::OnStartUp(const TechniqueData& _TechniqueData)
	{
		m_Name = _TechniqueData._Name;

		return EC_OK;
	}

	void soTechnique::OnShutDown()
	{
		SAFE_DELETE_STDVECTOR(m_RenderPassList);
	}

	RESULT soTechnique::AddPass(const RenderPassData& _RenderPassData)
	{
		soRenderPass* NewPass = SOE_NEW soRenderPass;
		NewPass->StartUp(_RenderPassData);
		NewPass->m_ID = m_RenderPassID;
		m_RenderPassNameMap[_RenderPassData._Name] = NewPass->m_ID;
		m_RenderPassID++;
		m_RenderPassList.push_back(NewPass);
		return EC_OK;
	}

	soRenderPass* soTechnique::GetPass(soString _PassName)
	{
		if (m_RenderPassNameMap.find(soString(_PassName)) != m_RenderPassNameMap.end())
		{
			return m_RenderPassList.at(m_RenderPassNameMap.at(soString(_PassName)));
		}

		return NULL;
	}

	soRenderPass * soTechnique::GetPass(uint8 _PassID)
	{
		if (_PassID < m_RenderPassID)
		{
			return m_RenderPassList.at(_PassID);
		}

		return NULL;
	}

	void soTechnique::Resize()
	{
		for (uint8 i = 0; i < static_cast<uint8>(m_RenderPassList.size()); i++)
		{
			m_RenderPassList.at(i)->Resize();
		}
	}

	void soTechnique::Render()
	{
		for (uint8 i = 0; i < static_cast<uint8>(m_RenderPassList.size()); i++)
		{
			m_RenderPassList.at(i)->Render();
		}
	}

}