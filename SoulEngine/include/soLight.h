/********************************************************************/
/**
* @LC	  10/25/2016
* @file   soLight.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/25/2016
* @brief  Implementacion de la clase soLight. Encapsula la informacion
		  de una luz necesaria para el shader.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulEngineDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de diferenciador de tipos de luz                         */
	/************************************************************************/
	enum LightType
	{
		LT_Directional = 0,
		LT_Spoot,
		LT_Point,
		LT_Area,
		LT_TotalLight
	};

	/************************************************************************/
	/* Declaracion de la clase soLight									    */
	/************************************************************************/
	class soLight : public soStandardClass<const LightData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soLight();										/*!< Constructor Standard */
		~soLight();										/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		soVector3D m_PositionView;
		float m_AttenuationBegin;
		soVector3D m_Color;
		float m_AttenuationEnd;

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const LightData& _LightData);	/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;				/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}