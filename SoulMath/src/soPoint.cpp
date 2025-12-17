/********************************************************************/
/**
* @LC	  30/05/2016
* @file   soPoint.cpp
* @Author Aldo Muñoz(lehi.munoz@gmail.com)
* @date   30/05/2016
* @brief  Implementacion de la clase soPoint
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"		/*!< Funciones matematicas basicas optimisadas para Windows */
#endif
#include "soPoint.h"

/************************************************************************/
/* Definicion de la clase soPoint                                       */
/************************************************************************/
namespace SoulSDK
{
	soPoint::soPoint(const soPoint & _Point)
	{
		X = _Point.X;
		Y = _Point.Y;
	}

	soPoint::~soPoint()
	{
	}

	bool soPoint::operator==(const soPoint & _Point)
	{
		if (X == _Point.X && Y == _Point.Y)
		{
			return true;
		}
		return false;
	}
	bool soPoint::operator!=(const soPoint & _Point)
	{
		if (X != _Point.X || Y != _Point.Y)
		{
			return true;
		}
		return false;
	}
}
