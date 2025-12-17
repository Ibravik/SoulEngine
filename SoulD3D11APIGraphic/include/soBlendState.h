/********************************************************************/
/**
* @LC	  10/22/2016
* @file   soBlendState.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/22/2016
* @brief  Implementacion de la clase soBlendState. Un estado de blend 
		  define el comportamiento del afla entre el render target y 
		  la textura recurso 
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soBlendState								    */
	/************************************************************************/
	class soBlendState : public soStandardClass<const BlendData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soBlendState();									/*!< Constructor Standard */
		virtual ~soBlendState();						/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		ID3D11BlendState* m_ptrBlendState;				/*!< Puntero al estado de blend aplicable */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const BlendData& _BlendData);	/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown() override;				/*!< Libera los recursos solicitados por la clase para su iniciacion */

	};
}

