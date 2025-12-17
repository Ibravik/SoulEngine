/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soSampler.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/29/2016
* @brief  Declaration of soSampler. El sampler es indispensable para la
		  toma de muestra de la textura dentro del shader. Toda textura 
		  debe de tener un sampler asignado, ya sea especifico o default
* @bug	  No know Bugs Segun dice el inge
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras para compilacion                              */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"

namespace SoulSDK
{	
	/************************************************************************/
	/* Declaracion de la clase soSampler							        */
	/************************************************************************/
	class soSampler : public soStandardClass<const SamplerData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soSampler();										/*!< Constructor Default */
		~soSampler();										/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		ID3D11SamplerState* m_ptrSampler;					/*!< Interfaz de d3d11 para manejo de sampler */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private: 
		RESULT OnStartUp(const SamplerData& _SamplerData);	/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;					/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}

