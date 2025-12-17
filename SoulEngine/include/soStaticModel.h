/********************************************************************/
/**
* @LC	  10/12/2016
* @file   soStaticModel.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   1/22/2016
* @brief  Declaracion de la clase soStaticModel. Un modelo estatico es 
		  aquel que no tiene animacion.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soModel.h"
#include "soMesh.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soStaticModel							    */
	/************************************************************************/
	class soStaticModel : public soModel
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:	
		soStaticModel();																				/*!< Constructor Standard */
		~soStaticModel();																				/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables de la clase                                 */
		/************************************************************************/
	public:					

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	public:
	};
}

