/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soBuffer.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/25/2015
* @brief  Implementacion de la clase soBuffer. Buffer exclusivo
		  para DXGI. Esta es una clase base de generacion de buffers, 
		  sin una utilidad definida.
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
	/* Declaracion de la clase soBuffer									    */
	/************************************************************************/
	class soBuffer : public soStandardClass<const BufferData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soBuffer();										/*!< Constructor Standard */
		~soBuffer();									/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/								
	public:
		ID3D11Buffer* m_ptrBuffer;						/*!< Puntero al Buffer creado en el dispositivo por directx */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const BufferData& _BufferData);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown() override;				/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}