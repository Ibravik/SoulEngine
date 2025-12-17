/********************************************************************/
/**
* @LC	  3/31/2016
* @file   soColor.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/17/2015
* @brief  Implementacion de soColorRGBA
* @bug	   No known bugs.
*/
/********************************************************************/

/***********************************************************************************/
/* Header files inclusion														   */
/***********************************************************************************/
#include "soColor.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones de la clase soColorRGBA                      */
	/************************************************************************/
	soColorRGBA::soColorRGBA(const unsigned char _RGBA[4])
	{
		r = _RGBA[0];
		g = _RGBA[1];
		b = _RGBA[2];
		a = _RGBA[3];
	}

	soColorRGBA::soColorRGBA(const short& _r, const short& _g, const short& _b, const short& _a)
	{
		r = (unsigned char)_r;
		g = (unsigned char)_g;
		b = (unsigned char)_b;
		a = (unsigned char)_a;
	}

	soColorRGBA::soColorRGBA(const soColorRGBA& _CopyObj)
	{
		*this = _CopyObj;
	}

	soColorRGBA::~soColorRGBA()
	{

	}

	void soColorRGBA::operator=(const unsigned int& _Value)
	{
		l = _Value;
	}

	unsigned char& soColorRGBA::operator[](const unsigned int& _Index)
	{
		return (&r)[_Index];
	}

	soColorRGBA soColorRGBA::operator+(const soColorRGBA & _Color)
	{
		return soColorRGBA(r + _Color.r, g + _Color.g, b + _Color.b, a + _Color.a);
	}

	soColorRGBA soColorRGBA::operator-(const soColorRGBA & _Color)
	{
		return soColorRGBA(r - _Color.r, g - _Color.g, b - _Color.b, a - _Color.a);
	}

	void soColorRGBA::operator+=(const soColorRGBA & _Color)
	{
		r += _Color.r;
		g += _Color.g;
		b += _Color.b;
		a += _Color.a;
	}

	void soColorRGBA::operator-=(const soColorRGBA & _Color)
	{
		r -= _Color.r;
		g -= _Color.g;
		b -= _Color.b;
		a -= _Color.a;
	}

	bool soColorRGBA::operator==(const soColorRGBA & _Color)
	{
		return (l == _Color.l ? true : false);
	}

	bool soColorRGBA::operator!=(const soColorRGBA & _Color)
	{
		return (l != _Color.l ? false : true);
	}
}