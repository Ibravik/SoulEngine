/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soVector3D.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/17/2015
* @brief  Implementacion de soVector3D
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
#include "soVector2D.h"
#include "soVector4D.h"
#include "soMatrix3D.h"
#include "soVector3D.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones de la clase soVector3D                       */
	/************************************************************************/
	soVector3D::soVector3D(const soVector2D & _CopyObj)
	{
		memset(&X, 0, sizeof(float) * 3);
		memcpy(&X, &_CopyObj.X, sizeof(float) * 2);
	}

	soVector3D::soVector3D(const soVector3D& _CopyObj)
	{
		memset(&X, 0, sizeof(float) * 3);
		memcpy(&X, &_CopyObj.X, sizeof(float) * 3);
	}

	soVector3D::soVector3D(const soVector4D & _CopyObj)
	{
		memset(&X, 0, sizeof(float) * 3);
		memcpy(&X, &_CopyObj.X, sizeof(float) * 3);
	}

	soVector3D::~soVector3D()
	{

	}
	
	bool soVector3D::Equal(soVector3D _Vector3D, float _Precision) const
	{
		if (soMath::Abs(X - _Vector3D.X) <= _Precision && soMath::Abs(Y - _Vector3D.Y) <= _Precision && soMath::Abs(Z - _Vector3D.Z) <= _Precision)
		{
			return true;
		}
		return false;
	}

	void soVector3D::operator=(const soVector3D& _Vector3D)
	{
		memcpy(&X, &_Vector3D.X, sizeof(float) * 3);
	}

	void soVector3D::operator	= (const soVector2D& _Vector2D)
	{
		memcpy(&X, &_Vector2D.X, sizeof(float) * 2);
	}

	void soVector3D::operator	= (const soVector4D& _Vector4D)
	{
		memcpy(&X, &_Vector4D.X, sizeof(float) * 3);
	}

	soVector3D soVector3D::operator - (const soVector3D& _Vector3D)
	{
		return soVector3D(X - _Vector3D.X, Y - _Vector3D.Y, Z - _Vector3D.Z);
	}

	soVector3D soVector3D::operator-()
	{
		return soVector3D(-X, -Y, -Z);
	}

	soVector3D soVector3D::operator + (const soVector3D& _Vector3D)
	{
		return soVector3D(X + _Vector3D.X, Y + _Vector3D.Y, Z + _Vector3D.Z);
	}

	soVector3D soVector3D::operator/(const float& _Value)
	{
		return soVector3D(X/_Value, Y/_Value, Z/_Value);
	}

	soVector3D soVector3D::operator*(const float& _Value)
	{
		return soVector3D(X*_Value, Y*_Value, Z*_Value);
	}

	soVector3D soVector3D::operator*(const soMatrix3x3 & _Matrix3x3)
	{
		return soVector3D(	_Matrix3x3.m00 * X + _Matrix3x3.m10 * Y + _Matrix3x3.m20 * Z,
							_Matrix3x3.m01 * X + _Matrix3x3.m11 * Y + _Matrix3x3.m21 * Z,
							_Matrix3x3.m02 * X + _Matrix3x3.m12 * Y + _Matrix3x3.m22 * Z);
	}

	SoulSDK::soVector3D soVector3D::operator*(const soVector3D& _Vector3D)
	{
		return soVector3D(X*_Vector3D.X, Y*_Vector3D.Y, Z*_Vector3D.Z);
	}

	float& soVector3D::operator[](const unsigned int& _Index)
	{
		return (&X)[_Index];
	}

	void soVector3D::operator += (const soVector3D& _Vector3D)
	{
		X += _Vector3D.X;
		Y += _Vector3D.Y;
		Z += _Vector3D.Z;
	}

	void soVector3D::operator -= (const soVector3D& _Vector3D)
	{
		X -= _Vector3D.X;
		Y -= _Vector3D.Y;
		Z -= _Vector3D.Z;
	}

	void soVector3D::operator/=(const float& _Value)
	{
		X /= _Value;
		Y /= _Value;
		Z /= _Value;
	}

	void soVector3D::operator*=(const float& _Value)
	{
		X *= _Value;
		Y *= _Value;
		Z *= _Value;
	}

	void soVector3D::operator*=(const soMatrix3x3 & _Matrix3x3)
	{
		soVector3D Result(	_Matrix3x3.m00 * X + _Matrix3x3.m10 * Y + _Matrix3x3.m20 * Z,
							_Matrix3x3.m01 * X + _Matrix3x3.m11 * Y + _Matrix3x3.m21 * Z,
							_Matrix3x3.m02 * X + _Matrix3x3.m12 * Y + _Matrix3x3.m22 * Z);

		*this = Result;
	}

	bool soVector3D::operator>(const soVector3D & _Vector3D)
	{
		return (X > _Vector3D.X && Y > _Vector3D.Y && Z > _Vector3D.Z ? true : false);
	}

	bool soVector3D::operator<(const soVector3D & _Vector3D)
	{
		return (X < _Vector3D.X && Y < _Vector3D.Y && Z < _Vector3D.Z ? true : false);
	}

	bool soVector3D::operator == (const soVector3D& _Vector3D)
	{
		return (X == _Vector3D.X && Y == _Vector3D.Y && Z == _Vector3D.Z ? true : false);
	}

	bool soVector3D::operator!= (const soVector3D& _Vector3D)
	{
		return (X != _Vector3D.X || Y != _Vector3D.Y || Z != _Vector3D.Z ? true : false);
	}
}