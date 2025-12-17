/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soVector4D.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/17/2015
* @brief  Implementacion de soVector4D
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
#include "soDegrees.h"
#include "soRadians.h"
#include "soVector2D.h"
#include "soVector3D.h"
#include "soMatrix4D.h"
#include "soVector4D.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones de la clase soVector4D                       */
	/************************************************************************/
	soVector4D::soVector4D(const soVector2D & _CopyObj)
	{
		memset(&X, 0, sizeof(float) * 4);
		memcpy(&X, &_CopyObj.X, sizeof(float) * 2);
	}

	soVector4D::soVector4D(const soVector3D & _CopyObj)
	{
		memset(&X, 0, sizeof(float) * 4);
		memcpy(&X, &_CopyObj.X, sizeof(float) * 3);
	}

	soVector4D::soVector4D(const soVector4D& _CopyObj)
	{
		memset(&X, 0, sizeof(float) * 4);
		memcpy(&X, &_CopyObj.X, sizeof(float) * 4);
	}

	soVector4D::~soVector4D()
	{

	}

	bool soVector4D::Equal(soVector4D _Vector4D, float _Precision) const
	{
		if (soMath::Abs(X - _Vector4D.X) <= _Precision && soMath::Abs(Y - _Vector4D.Y) <= _Precision && soMath::Abs(Z - _Vector4D.Z) <= _Precision && soMath::Abs(W - _Vector4D.W) <= _Precision)
		{
			return true;
		}
		return false;
	}

	void soVector4D::operator=(const soVector4D& _Vector4D)
	{
		memcpy(&X, &_Vector4D.X, sizeof(float) * 4);
	}

	void soVector4D::operator	= (const soVector2D& _Vector2D)
	{
		memcpy(&X, &_Vector2D.X, sizeof(float) * 2);
	}

	void soVector4D::operator	= (const soVector3D& _Vector3D)
	{
		memcpy(&X, &_Vector3D.X, sizeof(float) * 3);
	}

	soVector4D soVector4D::operator - (const soVector4D& _Vector4D)
	{
		return soVector4D(X - _Vector4D.X, Y - _Vector4D.Y, Z - _Vector4D.Z, W - _Vector4D.W);
	}

	soVector4D soVector4D::operator * (const soVector4D& _Vector4D)
	{
		return soVector4D(X * _Vector4D.X, Y * _Vector4D.Y, Z * _Vector4D.Z, W * _Vector4D.W);
	}

	SoulSDK::soVector4D soVector4D::operator-()
	{
		return soVector4D(-X, -Y, -Z, -W);
	}

	soVector4D soVector4D::operator*(const soMatrix4x4 & _Matrix4x4)
	{
		return soVector4D(	_Matrix4x4.m00 * X + _Matrix4x4.m01 * Y + _Matrix4x4.m02 * Z + _Matrix4x4.m03 * W,
							_Matrix4x4.m10 * X + _Matrix4x4.m11 * Y + _Matrix4x4.m12 * Z + _Matrix4x4.m13 * W,
							_Matrix4x4.m20 * X + _Matrix4x4.m21 * Y + _Matrix4x4.m22 * Z + _Matrix4x4.m23 * W,
							_Matrix4x4.m30 * X + _Matrix4x4.m31 * Y + _Matrix4x4.m32 * Z + _Matrix4x4.m33 * W);
	}

	soVector4D soVector4D::operator + (const soVector4D& _Vector4D)
	{
		return soVector4D(X + _Vector4D.X, Y + _Vector4D.Y, Z + _Vector4D.Z, W + _Vector4D.W);
	}

	soVector4D soVector4D::operator*(const float& _Value)
	{
		return soVector4D(X * _Value, Y * _Value, Z * _Value, W * _Value);
	}

	soVector4D soVector4D::operator/(const float& _Value)
	{
		return soVector4D(X / _Value, Y / _Value, Z / _Value, W / _Value);
	}

	float& soVector4D::operator[](const unsigned int& _Index)
	{
		return (&X)[_Index];
	}

	void soVector4D::operator += (const soVector4D& _Vector4D)
	{
		X += _Vector4D.X;
		Y += _Vector4D.Y;
		Z += _Vector4D.Z;
		W += _Vector4D.W;
	}

	void soVector4D::operator -= (const soVector4D& _Vector4D)
	{
		X -= _Vector4D.X;
		Y -= _Vector4D.Y;
		Z -= _Vector4D.Z;
		W -= _Vector4D.W;
	}

	void soVector4D::operator*=(const soVector4D & _Vector4D)
	{
		soVector4D Result(X * _Vector4D.X, Y * _Vector4D.Y, Z * _Vector4D.Z, W * _Vector4D.W);

		*this = Result;
	}

	void soVector4D::operator*=(const soMatrix4x4 & _Matrix4x4)
	{
		soVector4D Result(	_Matrix4x4.m00 * X + _Matrix4x4.m01 * Y + _Matrix4x4.m02 * Z + _Matrix4x4.m03 * W,
							_Matrix4x4.m10 * X + _Matrix4x4.m11 * Y + _Matrix4x4.m12 * Z + _Matrix4x4.m13 * W,
							_Matrix4x4.m20 * X + _Matrix4x4.m21 * Y + _Matrix4x4.m22 * Z + _Matrix4x4.m23 * W,
							_Matrix4x4.m30 * X + _Matrix4x4.m31 * Y + _Matrix4x4.m32 * Z + _Matrix4x4.m33 * W);

		*this = Result;
	}

	void soVector4D::operator*=(const float& _Value)
	{
		X *= _Value;
		Y *= _Value;
		Z *= _Value;
		W *= _Value;
	}

	void soVector4D::operator/=(const float& _Value)
	{
		X /= _Value;
		Y /= _Value;
		Z /= _Value;
		W /= _Value;
	}

	bool soVector4D::operator == (const soVector4D& _Vector4D)
	{
		return (X == _Vector4D.X && Y == _Vector4D.Y && Z == _Vector4D.Z && W == _Vector4D.W ? true : false);
	}

	bool soVector4D::operator!= (const soVector4D& _Vector4D)
	{
		return (X != _Vector4D.X || Y != _Vector4D.Y || Z != _Vector4D.Z || W != _Vector4D.W ? true : false);
	}
}