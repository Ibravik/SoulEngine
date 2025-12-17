/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soComputeShader.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/15/2016
* @brief  Declaration of class soComputeShader
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soShader.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaración de la clase base soComputeShader                          */
	/************************************************************************/
	class soComputeShader : public soShader
	{
		/************************************************************************************************************************/
		/* Constructores y destructor																							*/
		/************************************************************************************************************************/
	public:
		soComputeShader();								/*!< Constructor Standar */
		~soComputeShader();								/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		ID3D11ComputeShader* m_ptrComputeShader;		/*!< Puntero al recurso de directX VertexShader */
		
		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const ShaderData& _ShaderData);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown() override;				/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};

}