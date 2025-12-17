/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soViewPort.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/29/2016
* @brief  Declaration of soViewPort.El viewport es el area con tamaño en 
		  pixeles donde se pone el render final
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
	/* Declaracion de la clase soViewPort							        */
	/************************************************************************/
	class soViewPort : public soStandardClass<const ViewPortData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soViewPort();										/*!< Constructor Default */
		~soViewPort();										/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		D3D11_VIEWPORT m_ViewPort;							/*!< Viewport */
			
		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const ViewPortData& _ViewPortData);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;					/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}
