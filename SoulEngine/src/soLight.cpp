/********************************************************************/
/**
* LC	  10/25/2016
* @file   soLight.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/25/2016
* @brief  Implementacion de la clase soLight
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soLight.h"

/************************************************************************/
/* Definicion de la clase soLight									    */
/************************************************************************/
namespace SoulSDK
{
	soLight::soLight()
	{
		Connect(std::bind(&soLight::OnStartUp, this, std::placeholders::_1));
	}

	soLight::~soLight()
	{
		ShutDown();
	}

	RESULT soLight::OnStartUp(const LightData& _LightData)
	{		
		m_Color = _LightData.color;
		m_PositionView = _LightData.positionView;
		m_AttenuationBegin = _LightData.attenuationBegin; 
		m_AttenuationEnd = _LightData.attenuationEnd;

		return EC_OK;
	}	

	void soLight::OnShutDown()
	{

	}

}
