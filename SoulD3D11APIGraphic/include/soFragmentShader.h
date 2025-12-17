
/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soFragmentShader.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Declaration of class soFragmentShader.
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
	/* Declaración de la clase base soFragmentShader                        */
	/************************************************************************/
	class soFragmentShader : public soShader
	{
		/************************************************************************/
		/* Constructores y destructor                                           */
		/************************************************************************/
	public:
		soFragmentShader();								/*!< Constructor Standard */
		~soFragmentShader();							/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/	
	public:		
		ID3D11PixelShader* m_ptrPixelShader;			/*!< Almacena el recurso creado por directx del shader */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:		
		RESULT OnStartUp(const ShaderData& _ShaderData);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown() override;				/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}

