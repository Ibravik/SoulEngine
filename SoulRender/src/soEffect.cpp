/********************************************************************/
/**
* @LC	  10/10/2016
* @file   soEffect.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/29/2015
* @brief  Implementacion de la clase soEffect
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soTechnique.h"
#include "soEffect.h"

/************************************************************************/
/* Definicion de la clase soEffect										*/
/************************************************************************/
namespace SoulSDK
{
	soEffect::soEffect()
	{
		Connect(std::bind(&soEffect::OnStartUp, this, std::placeholders::_1));
		m_TechniqueList.clear();		/*!< Lista de tecnicas disponibles */
		m_TechniqueID = 0;
	}
	
	soEffect::~soEffect()
	{
		ShutDown();
	}

	RESULT soEffect::OnStartUp(const EffectData& _EffectData)
	{
		m_Name = _EffectData._Name;

		return EC_OK;
	}

	void soEffect::OnShutDown()
	{
		SAFE_DELETE_STDVECTOR(m_TechniqueList);
	}

	RESULT soEffect::AddTechnique(const TechniqueData& _TechniqueData)
	{
		soTechnique* NewTechnique = SOE_NEW soTechnique;
		NewTechnique->StartUp(_TechniqueData);
		NewTechnique->m_ID = m_TechniqueID;
		m_TechniqueNameMap[NewTechnique->m_Name] = m_TechniqueID;
		m_TechniqueID++;		
		m_TechniqueList.push_back(NewTechnique);
		return EC_OK;
	}

	soTechnique* soEffect::GetTechnique(soString _TechniqueName)
	{
		if (m_TechniqueNameMap.find(soString(_TechniqueName)) != m_TechniqueNameMap.end())
		{
			return m_TechniqueList.at(m_TechniqueNameMap.at(soString(_TechniqueName)));
		}

		return NULL;
	}

	soTechnique * soEffect::GetTechnique(uint8 _TechniqueID)
	{
		if (_TechniqueID < m_TechniqueID)
		{
			return m_TechniqueList.at(_TechniqueID);
		}

		return NULL;
	}

	void soEffect::Resize()
	{
		for (uint8 i = 0; i < static_cast<uint8>(m_TechniqueList.size()); i++)
		{
			m_TechniqueList.at(i)->Resize();
		}
	}

	void soEffect::Render()
	{
		for (uint8 i = 0; i < static_cast<uint8>(m_TechniqueList.size()); i++)
		{
			m_TechniqueList.at(i)->Render();
		}
	}
}