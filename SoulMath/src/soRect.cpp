/********************************************************************/
/**
* @LC	  30/05/2016
* @file   soRect.cpp
* @Author Aldo Muñoz(lehi.munoz@gmail.com)
* @date   30/05/2016
* @brief  Implementacion de la clase soRect
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"		/*!< Funciones matematicas basicas optimisadas para Windows */
#endif
#include "soRect.h"

/************************************************************************/
/* Definicion de la clase soRect                                        */
/************************************************************************/
namespace SoulSDK
{
	SoulSDK::soRect::soRect(const soRect & _Rect)
	{
		X = _Rect.X;
		Y = _Rect.Y;
		m_Width = _Rect.m_Width;
		m_Height = _Rect.m_Height;
	}

	soRect::~soRect()
	{
	}

	bool SoulSDK::soRect::operator==(const soRect & _Rect)
	{
		if (X == _Rect.X &&
			Y == _Rect.Y &&
			m_Width == _Rect.m_Width &&
			m_Height == _Rect.m_Height)
		{
			return true;
		}
		return false;
	}

	bool SoulSDK::soRect::operator!=(const soRect & _Rect)
	{
		if (X != _Rect.X ||
			Y != _Rect.Y ||
			m_Width != _Rect.m_Width &&
			m_Height != _Rect.m_Height)
		{
			return true;
		}
		return false;
	}
}