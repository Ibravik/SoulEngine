/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soQuaternions.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/8/2016
* @brief  Implementacion de soQuaternions
* @bug	   No known bugs.
*/
/********************************************************************/

/***********************************************************************************/
/* Header files inclusion														   */
/***********************************************************************************/
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"
#endif
#include "soPlatformMath.h"
#include "soVector3D.h"
#include "soMatrix3D.h"
#include "soMatrix4D.h"
#include "soQuaternion.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones de la clase soQuaternions                     */
	/************************************************************************/

	soQuaternion::soQuaternion(const soMatrix3x3 & _RotationMatrix)
	{
		FromMatrixRotation(_RotationMatrix);
		soMath::NormalizeQuaternion(*this);
	}

	soQuaternion::soQuaternion(const soMatrix4x4 & _RotationMatrix)
	{
		FromMatrixRotation(_RotationMatrix);
		soMath::NormalizeQuaternion(*this);
	}

	soQuaternion::soQuaternion(const soVector3D& _Axis, const soDegrees& _EulerAngle)
	{
		FromAxisAngle(_Axis, _EulerAngle);
		soMath::NormalizeQuaternion(*this);
	}

	soQuaternion::soQuaternion(const soVector3D& _Axis, const soRadians& _RadianAngle)
	{
		FromAxisAngle(_Axis, soDegrees(_RadianAngle.GetDegrees()));
		soMath::NormalizeQuaternion(*this);
	}

	soQuaternion::soQuaternion(const soDegrees& _Yaw, const soDegrees& _Pich, const soDegrees& _Roll)
	{
		FromEulerAngle(_Yaw, _Pich, _Roll);
		soMath::NormalizeQuaternion(*this);
	}

	soQuaternion::soQuaternion(const soRadians& _Yaw, const soRadians& _Pich, const soRadians& _Roll)
	{
		FromEulerAngle(soDegrees(_Yaw.GetDegrees()), soDegrees(_Pich.GetDegrees()), soDegrees(_Roll.GetDegrees()));
		soMath::NormalizeQuaternion(*this);
	}

	soQuaternion::~soQuaternion()
	{
	}

	void soQuaternion::FromMatrixRotation(const soMatrix3x3 & _RotationMatrix)
	{
		soQuaternion Result;
		float Tr = _RotationMatrix.Line[0][0] + _RotationMatrix.Line[1][1] + _RotationMatrix.Line[2][2] + 1.0f, fourD;
		unsigned int i, j, k;

		/*
		w >= 0.5 ?
		*/
		if (Tr >= 1.0f)
		{
			fourD = 2.0f * soMath::Sqrt(Tr);
			W = fourD / 4.0f;
			X = (_RotationMatrix.Line[1][2] - _RotationMatrix.Line[2][1]) / fourD;
			Y = (_RotationMatrix.Line[2][0] - _RotationMatrix.Line[0][2]) / fourD;
			Z = (_RotationMatrix.Line[0][1] - _RotationMatrix.Line[1][0]) / fourD;
		}
		else
		{
			/*
			Find the largest component.
			*/
			if (_RotationMatrix.Line[0][0] > _RotationMatrix.Line[1][1])
			{
				i = 0;
			}
			else
			{
				i = 1;
			}
			if (_RotationMatrix.Line[2][2] > _RotationMatrix.Line[i][i])
			{
				i = 2;
			}

			/*
			Set j and k to point to the next two co_RotationMatrix.Lineponents
			*/
			j = (i + 1) % 3;
			k = (j + 1) % 3;

			/*
			fourD = 4 * largest co_RotationMatrix.Lineponent
			*/
			fourD = 2.0f * sqrt(_RotationMatrix.Line[i][i] - _RotationMatrix.Line[j][j] - _RotationMatrix.Line[k][k] + 1.0f);

			/*
			Set the largest co_RotationMatrix.Lineponent
			*/
			Result[i] = fourD / 4.0f;

			/*
			Calculate re_RotationMatrix.Lineaining co_RotationMatrix.Lineponents
			*/
			Result[j] = (_RotationMatrix.Line[j][i] + _RotationMatrix.Line[i][j]) / fourD;
			Result[k] = (_RotationMatrix.Line[k][i] + _RotationMatrix.Line[i][k]) / fourD;
			Result[3] = (_RotationMatrix.Line[k][j] - _RotationMatrix.Line[j][k]) / fourD;
		
			*this = Result;
		}
	}

	void soQuaternion::FromMatrixRotation(const soMatrix4x4 & _RotationMatrix)
	{
		const float diag = _RotationMatrix.fMatrix[0] + _RotationMatrix.fMatrix[5] + _RotationMatrix.fMatrix[10] + 1;
		
		if (diag > 0.0f)
		{
			const float scale = soMath::Sqrt(diag) * 2.0f; // get scale from diagonal

			// TODO: speed this up
			X = (_RotationMatrix.fMatrix[6] - _RotationMatrix.fMatrix[9]) / scale;
			Y = (_RotationMatrix.fMatrix[8] - _RotationMatrix.fMatrix[2]) / scale;
			Z = (_RotationMatrix.fMatrix[1] - _RotationMatrix.fMatrix[4]) / scale;
			W = 0.25f * scale;
		}
		else
		{
			if (_RotationMatrix.fMatrix[0] > _RotationMatrix.fMatrix[5] && _RotationMatrix.fMatrix[0] > _RotationMatrix.fMatrix[10])
			{
				// 1st element of diag is greatest value
				// find scale according to 1st element, and double it
				const float scale = soMath::Sqrt(1.0f + _RotationMatrix.fMatrix[0] - _RotationMatrix.fMatrix[5] - _RotationMatrix.fMatrix[10]) * 2.0f;

				// TODO: speed this up
				X = 0.25f * scale;
				Y = (_RotationMatrix.fMatrix[4] + _RotationMatrix.fMatrix[1]) / scale;
				Z = (_RotationMatrix.fMatrix[2] + _RotationMatrix.fMatrix[8]) / scale;
				W = (_RotationMatrix.fMatrix[6] - _RotationMatrix.fMatrix[9]) / scale;
			}
			else if (_RotationMatrix.fMatrix[5] > _RotationMatrix.fMatrix[10])
			{
				// 2nd element of diag is greatest value
				// find scale according to 2nd element, and double it
				const float scale = soMath::Sqrt(1.0f + _RotationMatrix.fMatrix[5] - _RotationMatrix.fMatrix[0] - _RotationMatrix.fMatrix[10]) * 2.0f;

				// TODO: speed this up
				X = (_RotationMatrix.fMatrix[4] + _RotationMatrix.fMatrix[1]) / scale;
				Y = 0.25f * scale;
				Z = (_RotationMatrix.fMatrix[9] + _RotationMatrix.fMatrix[6]) / scale;
				W = (_RotationMatrix.fMatrix[8] - _RotationMatrix.fMatrix[2]) / scale;
			}
			else
			{
				// 3rd element of diag is greatest value
				// find scale according to 3rd element, and double it
				const float scale = soMath::Sqrt(1.0f + _RotationMatrix.fMatrix[10] - _RotationMatrix.fMatrix[0] - _RotationMatrix.fMatrix[5]) * 2.0f;

				// TODO: speed this up
				X = (_RotationMatrix.fMatrix[8] + _RotationMatrix.fMatrix[2]) / scale;
				Y = (_RotationMatrix.fMatrix[9] + _RotationMatrix.fMatrix[6]) / scale;
				Z = 0.25f * scale;
				W = (_RotationMatrix.fMatrix[1] - _RotationMatrix.fMatrix[4]) / scale;
			}
		}
	}

	void soQuaternion::FromAxisAngle(const soVector3D& _Axis, const soDegrees& _EulerAngle)
	{
		float angleRad = _EulerAngle.GetRadians() * 0.5f;

		float sa(soMath::Sin(angleRad));

		// Build a quaternion!
		W = soMath::Cos(angleRad);
		X = _Axis.X * sa;
		Y = _Axis.Y * sa;
		Z = _Axis.Z * sa;
	}

	void soQuaternion::FromEulerAngle(const soDegrees& _Yaw, const soDegrees& _Pich, const soDegrees& _Roll)
	{
		float cosY = soMath::Cos(_Yaw.GetRadians()  * 0.5f);
		float sinY = soMath::Sin(_Yaw.GetRadians()  * 0.5f);
		float cosP = soMath::Cos(_Pich.GetRadians() * 0.5f);
		float sinP = soMath::Sin(_Pich.GetRadians() * 0.5f);
		float cosR = soMath::Cos(_Roll.GetRadians() * 0.5f);
		float sinR = soMath::Sin(_Roll.GetRadians() * 0.5f);

		X =	cosR * sinP * cosY + sinR * cosP * sinY;
		Y =	cosR * cosP * sinY - sinR * sinP * cosY;
		Z =	sinR * cosP * cosY - cosR * sinP * sinY;
		W =	cosR * cosP * cosY + sinR * sinP * sinY;
	}

	bool soQuaternion::Equal(const soQuaternion & _Quaternion, float _Precision) const
	{
		if (soMath::Abs(X - _Quaternion.X) < _Precision &&
			soMath::Abs(Y - _Quaternion.Y) < _Precision &&
			soMath::Abs(Z - _Quaternion.Z) < _Precision &&
			soMath::Abs(W - _Quaternion.W) < _Precision)
		{
			return true;
		}
		return false;
	}

	bool soQuaternion::Equal(soQuaternion & _Quaternion, float _Precision) const
	{
		if (soMath::Abs(X - _Quaternion.X) < _Precision &&
			soMath::Abs(Y - _Quaternion.Y) < _Precision &&
			soMath::Abs(Z - _Quaternion.Z) < _Precision &&
			soMath::Abs(W - _Quaternion.W) < _Precision)
		{
			return true;
		}
		return false;
	}

	void soQuaternion::operator=(soQuaternion _Quaternion)
	{
		memcpy(&X, &_Quaternion.X, sizeof(float) * 4);
	}

	soQuaternion soQuaternion::operator+(const soQuaternion & b)
	{
		return soQuaternion(
			X + b.X,
			Y + b.Y,
			Z + b.Z,
			W + b.W);
	}

	soQuaternion soQuaternion::operator-(const soQuaternion & b)
	{
		return soQuaternion(
			X - b.X,
			Y - b.Y,
			Z - b.Z,
			W - b.W);
	}

	soQuaternion soQuaternion::operator*(const  soQuaternion & b)
	{
		return soQuaternion(
			W * b.W - X * b.X - Y * b.Y - Z * b.Z,
			W * b.X + X * b.W + Y * b.Z - Z * b.Y,
			W * b.Y - X * b.Z + Y * b.W + Z * b.X,
			W * b.Z + X * b.Y - Y * b.X + Z * b.W);
	}

	soQuaternion soQuaternion::operator*(const float& s)
	{
		return soQuaternion(
			s * X,
			s * Y,
			s * Z,
			s * W);
	}

	soQuaternion operator*(const float& s, soQuaternion & a)
	{
		return soQuaternion(
			s * a[0],
			s * a[1],
			s * a[2],
			s * a[3]);
	}

	float & soQuaternion::operator[](const unsigned int& _Index)
	{
		return (&X)[_Index];
	}

	void soQuaternion::operator+=(const soQuaternion & q)
	{
		soQuaternion Result = *this + q;

		*this = Result;
	}

	void soQuaternion::operator-=(const soQuaternion & q)
	{
		soQuaternion Result = *this - q;

		*this = Result;
	}

	void soQuaternion::operator*=(const soQuaternion & q)
	{
		soQuaternion Result = *this * q;

		*this = Result;
	}

	void soQuaternion::operator*=(float s)
	{
		soQuaternion Result = *this * s;

		*this = Result;
	}

	bool soQuaternion::operator == (const soQuaternion& q)
	{
		return (X == q.X && Y == q.Y && Z == q.Z && W == q.W ? true : false);
	}

	bool soQuaternion::operator!= (const soQuaternion& q)
	{
		return (X != q.X || Y != q.Y || Z != q.Z || W != q.W ? true : false);
	}
}