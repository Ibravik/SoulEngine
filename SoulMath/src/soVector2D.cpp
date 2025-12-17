/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soVector2D.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/17/2015
* @brief  Implementacion de soVector2D
* @bug	   No known bugs.
*/
/********************************************************************/

/***********************************************************************************/
/* Header files inclusion														   */
/***********************************************************************************/
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"		/*!< Funciones matematicas basicas optimisadas para Windows */
#endif
#include "soPlatformMath.h"
#include "soRadians.h"
#include "soDegrees.h"
#include "soVector3D.h"
#include "soVector4D.h"
#include "soVector2D.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones de la clase soVector2D                      */
	/************************************************************************/

	soVector2D::soVector2D(const soVector2D& _CopyObj)
	{
		memcpy(&X, &_CopyObj.X, sizeof(float) * 2);
	}

	soVector2D::soVector2D(const soVector3D & _CopyObj)
	{
		memset(&X, 0, sizeof(float) * 2);
		memcpy(&X, &_CopyObj.X, sizeof(float) * 2);
	}

	soVector2D::soVector2D(const soVector4D & _CopyObj)
	{
		memset(&X, 0, sizeof(float) * 2);
		memcpy(&X, &_CopyObj.X, sizeof(float) * 2);
	}

	soVector2D::~soVector2D()
	{
	}

	bool soVector2D::Equal(soVector2D& _Vector2D, float _Precision) const
	{
		if (soMath::Abs(X - _Vector2D.X) <= _Precision && soMath::Abs(Y - _Vector2D.Y) <= _Precision)
		{
			return true;
		}
		return false;
	}

	void soVector2D::operator=(const soVector2D& _Vector2D)
	{
		memcpy(&X, &_Vector2D.X, sizeof(float) * 2);
	}

	void soVector2D::operator	= (const soVector3D& _Vector3D)
	{
		memcpy(&X, &_Vector3D.X, sizeof(float) * 2);
	}

	void soVector2D::operator	= (const soVector4D& _Vector4D)
	{
		memcpy(&X, &_Vector4D.X, sizeof(float) * 2);
	}

	soVector2D soVector2D::operator + (const soVector2D& _Vector2D)
	{
		return soVector2D(X + _Vector2D.X, Y + _Vector2D.Y);
	}

	soVector2D soVector2D::operator - (const soVector2D& _Vector2D)
	{
		return soVector2D(X - _Vector2D.X, Y - _Vector2D.Y);
	}

	SoulSDK::soVector2D soVector2D::operator-()
	{
		return soVector2D(-X,- Y);
	}

	soVector2D soVector2D::operator*(const float& _Value)
	{
		return soVector2D(X * _Value, Y * _Value);
	}

	soVector2D soVector2D::operator/(const float& _Value)
	{
		return soVector2D(X / _Value, Y / _Value);
	}

	void soVector2D::operator += (const soVector2D& _Vector2D)
	{
		X += _Vector2D.X;
		Y += _Vector2D.Y;
	}

	void soVector2D::operator -= (const soVector2D& _Vector2D)
	{
		X -= _Vector2D.X;
		Y -= _Vector2D.Y;
	}

	void soVector2D::operator*=(const float& _Value)
	{
		X *= _Value;
		Y *= _Value;
	}

	void soVector2D::operator/=(const float& _Value)
	{
		X /= _Value;
		Y /= _Value;
	}

	bool soVector2D::operator == (const soVector2D& _Vector2D)
	{
		return (X == _Vector2D.X && Y == _Vector2D.Y ? true : false);
	}

	bool soVector2D::operator != (const soVector2D& _Vector2D)
	{
		return (X != _Vector2D.X || Y != _Vector2D.Y ? true : false);
	}

	float& soVector2D::operator [] (const unsigned int& _Index)
	{
		return (&X)[_Index];
	}
}