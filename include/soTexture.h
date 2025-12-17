/********************************************************************/
/**
* @LC     10/12/2016
* @file   soTexture.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/25/2015
* @brief  Implementacion de la textura como recurso.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "Soul_Graphics.h"
#include "soResource.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soTexture									*/
	/************************************************************************/
	class soTexture : public soResource
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soTexture();																/*!< Constructor Standard */
		~soTexture();																/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		soAPITexture m_Texture;														/*!< Textura de la appi grafica */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	public:
		RESULT StartUp(const TextureData& _TextureData);							/*!< Inicializacion del recurso */
		virtual RESULT Load(const ResourceParameters& _ResourceParameters)override;	/*!< Cargar desde Archivo, la ruta esta especificada en la estructura Resource identification */
	};

}