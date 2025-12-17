/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soPhysicObject.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/8/2016
* @brief  Declaration of class soPhysicObject. Se pretende unicamente
		  simplificar el uso de objetos fisicos a partir de polimorfismo
* @bug	  No know Bugs
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"		/*!< Funciones matematicas basicas optimisadas para Windows */
#endif
#include "soPlatformMath.h"
#include "soPhysicObject.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soPhysicObject                                */
	/************************************************************************/
	soPhysicObject::soPhysicObject()
	{
	}
	
	soPhysicObject::~soPhysicObject()
	{
	}
}