/************************************************************************/
/**
* @LC	  6/8/2016
* @file   soResourceFactory.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/8/2016
* @brief  Declaration of class soResourceFactory
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para compilar                      */
/************************************************************************/
#include "soResource.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soResourceFactory                            */
	/************************************************************************/
	class soResourceFactory
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soResourceFactory();							  				/*!< Constructor Standard */
		~soResourceFactory();							  				/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	public:
		soResource* CreateResourceByType(const uint8& _ResourceType);	//Funcion para crear un nuevo edificio
	};
}