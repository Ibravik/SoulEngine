/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soVertexShader.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Declaration of class soVertexShader
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soShader.h"
#include "soInputLayout.h"

namespace SoulSDK
{	
	/************************************************************************/
	/* Declaración de la clase base soVertexShader                          */
	/************************************************************************/
	class soVertexShader : public soShader
	{
		/************************************************************************/
		/* Constructores y destructor					                        */
		/************************************************************************/
	public:
		soVertexShader();								/*!< Constructor Standar */
		~soVertexShader();								/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:		
		soInputLayout m_InputLayout;					/*!< Objeto que contiene la variable ImputLayout de directX */
		ID3D11VertexShader* m_ptrVertexShader;			/*!< Puntero al recurso de directX VertexShader */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const ShaderData& _ShaderData);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown() override;				/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}


