/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soRasterizerState.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/25/2015
* @brief  Implementacion de la clase soRasterizerState. El rasterizador 
		  determina el manejo de los triangulos en el shader.
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
	/* Declaracion de la clase soRasterizerState						    */
	/************************************************************************/
	class soRasterizerState : public soStandardClass<const RasterizerStateData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soRasterizerState();											/*!< Constructor Standard */
		~soRasterizerState();											/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		ID3D11RasterizerState* m_ptrRasterizerState;					/*!< Rasterizador DXGI */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const RasterizerStateData& _RasterizerStateData);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;								/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}
