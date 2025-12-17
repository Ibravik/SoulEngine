/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soAnimation.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/22/2016
* @brief  Implementacion de la clase soAnimation. Abstrae la informacion
		  de toda una animacion.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soKeyFrame.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soAnimation								    */
	/************************************************************************/
	class soAnimation : public soStandardClass<const aiAnimation&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soAnimation();										/*!< Constructor Standard */
		virtual ~soAnimation();								/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables de la clase                                 */
		/************************************************************************/
	public:
		soString m_Name;								/*!< Nombre de la animacion */
		uint32 m_ID;									/*!< ID unico en el mapa de animaciones donde este */
		float m_Duration;								/*!< Duracion de la animacion */
		float m_TicksPerSecond;							/*!< Ticks por segundo */
		std::map<soString, soKeyFrame> m_KeyFrameInfo;	/*!< Mapa de canales de animacion */
		std::map<uint32, soKeyFrame> m_KeyFrameMap;		/*!< Mapa de canales de animacion */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const aiAnimation& _aiAnimation);	/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;					/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}