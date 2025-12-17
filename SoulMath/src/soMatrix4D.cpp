/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soMatrix4D.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/17/2015
* @brief  Implementacion de soMatrix4D
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
#include "soVector4D.h"
#include "soMatrix4D.h"
#include <soStandardTools.h>

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones de la clase soMatrix3D                       */
	/************************************************************************/
	soMatrix4x4::soMatrix4x4(const float m0[4], const float m1[4], const float m2[4], const float m3[4])
	{
		m00 = m0[0]; m01 = m0[1]; m02 = m0[2], m03 = m0[3];
		m10 = m1[0]; m11 = m1[1]; m12 = m1[2], m13 = m1[3];
		m20 = m2[0]; m21 = m2[1]; m22 = m2[2], m23 = m2[3];
		m30 = m3[0]; m31 = m3[1]; m32 = m3[2], m33 = m3[3];
	}
		
	soMatrix4x4::soMatrix4x4(const soMatrix4x4& _CopyObj)
	{
		memcpy(&fMatrix, _CopyObj.fMatrix, sizeof(float) * 16);
	}

	soMatrix4x4::~soMatrix4x4()
	{
	}

	void soMatrix4x4::ResetMatrix()
	{
		memset(this, 0, sizeof(soMatrix4x4));
	}

	//bool soMatrix4x4::Equal(soMatrix4x4 & _Matrix4x4, float _Precision)
	//{
	//	if (Math::Abs(fMatrix[0] - _Matrix4x4.fMatrix[0]) <= _Precision &&
	//		Math::Abs(fMatrix[1] - _Matrix4x4.fMatrix[1]) <= _Precision &&
	//		Math::Abs(fMatrix[2] - _Matrix4x4.fMatrix[2]) <= _Precision &&
	//		Math::Abs(fMatrix[3] - _Matrix4x4.fMatrix[3]) <= _Precision &&
	//		Math::Abs(fMatrix[4] - _Matrix4x4.fMatrix[4]) <= _Precision &&
	//		Math::Abs(fMatrix[5] - _Matrix4x4.fMatrix[5]) <= _Precision &&
	//		Math::Abs(fMatrix[6] - _Matrix4x4.fMatrix[6]) <= _Precision &&
	//		Math::Abs(fMatrix[7] - _Matrix4x4.fMatrix[7]) <= _Precision &&
	//		Math::Abs(fMatrix[8] - _Matrix4x4.fMatrix[8]) <= _Precision &&
	//		Math::Abs(fMatrix[9] - _Matrix4x4.fMatrix[9]) <= _Precision &&
	//		Math::Abs(fMatrix[10] - _Matrix4x4.fMatrix[10]) <= _Precision &&
	//		Math::Abs(fMatrix[11] - _Matrix4x4.fMatrix[11]) <= _Precision &&
	//		Math::Abs(fMatrix[12] - _Matrix4x4.fMatrix[12]) <= _Precision &&
	//		Math::Abs(fMatrix[13] - _Matrix4x4.fMatrix[13]) <= _Precision &&
	//		Math::Abs(fMatrix[14] - _Matrix4x4.fMatrix[14]) <= _Precision &&
	//		Math::Abs(fMatrix[15] - _Matrix4x4.fMatrix[15]) <= _Precision)
	//	{
	//		return true;
	//	}
	//	return false;
	//}

	bool soMatrix4x4::Equal(const soMatrix4x4 & _Matrix4x4, float _Precision)
	{
		if (soMath::Abs(fMatrix[0] - _Matrix4x4.fMatrix[0]) <= _Precision &&
			soMath::Abs(fMatrix[1] - _Matrix4x4.fMatrix[1]) <= _Precision &&
			soMath::Abs(fMatrix[2] - _Matrix4x4.fMatrix[2]) <= _Precision &&
			soMath::Abs(fMatrix[3] - _Matrix4x4.fMatrix[3]) <= _Precision &&
			soMath::Abs(fMatrix[4] - _Matrix4x4.fMatrix[4]) <= _Precision &&
			soMath::Abs(fMatrix[5] - _Matrix4x4.fMatrix[5]) <= _Precision &&
			soMath::Abs(fMatrix[6] - _Matrix4x4.fMatrix[6]) <= _Precision &&
			soMath::Abs(fMatrix[7] - _Matrix4x4.fMatrix[7]) <= _Precision &&
			soMath::Abs(fMatrix[8] - _Matrix4x4.fMatrix[8]) <= _Precision &&
			soMath::Abs(fMatrix[9] - _Matrix4x4.fMatrix[9]) <= _Precision &&
			soMath::Abs(fMatrix[10] - _Matrix4x4.fMatrix[10]) <= _Precision &&
			soMath::Abs(fMatrix[11] - _Matrix4x4.fMatrix[11]) <= _Precision &&
			soMath::Abs(fMatrix[12] - _Matrix4x4.fMatrix[12]) <= _Precision &&
			soMath::Abs(fMatrix[13] - _Matrix4x4.fMatrix[13]) <= _Precision &&
			soMath::Abs(fMatrix[14] - _Matrix4x4.fMatrix[14]) <= _Precision &&
			soMath::Abs(fMatrix[15] - _Matrix4x4.fMatrix[15]) <= _Precision)
		{
			return true;
		}
		return false;
	}

	soVector4D soMatrix4x4::GetLine(unsigned int _value)const
	{
		soVector4D ReturnVector;
		ReturnVector.X = Line[_value][0];
		ReturnVector.Y = Line[_value][1];
		ReturnVector.Z = Line[_value][2];
		ReturnVector.W = Line[_value][3];
		return ReturnVector;
	}

	soVector4D soMatrix4x4::GetColum(unsigned int _value)const
	{
		soVector4D returnVector;
		returnVector.X = Line[0][_value];
		returnVector.Y = Line[1][_value];
		returnVector.Z = Line[2][_value];
		returnVector.W = Line[3][_value];
		return returnVector;
	}

	void soMatrix4x4::operator=(const soMatrix4x4& _Matrix4x4)
	{
		memcpy(&fMatrix, _Matrix4x4.fMatrix, sizeof(float) * 16);
	}

	soMatrix4x4 soMatrix4x4::operator + (const soMatrix4x4& _Matrix4x4)
	{
		soMatrix4x4 AditionMatrix;

#ifdef SO_INTRIN_MATH
		soVector4D TempVec;
		soVector4D Vector1 = GetLine(0);
		soVector4D Vector2 = _Matrix4x4.GetLine(0);
		__m128 Temp = _mm_add_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		AditionMatrix.Line[0][0] = TempVec.X;
		AditionMatrix.Line[0][1] = TempVec.Y;
		AditionMatrix.Line[0][2] = TempVec.Z;
		AditionMatrix.Line[0][3] = TempVec.W;

		Vector1 = GetLine(1);
		Vector2 = _Matrix4x4.GetLine(1);
		Temp = _mm_add_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		AditionMatrix.Line[1][0] = TempVec.X;
		AditionMatrix.Line[1][1] = TempVec.Y;
		AditionMatrix.Line[1][2] = TempVec.Z;
		AditionMatrix.Line[1][3] = TempVec.W;


		Vector1 = GetLine(2);
		Vector2 = _Matrix4x4.GetLine(2);
		Temp = _mm_add_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		AditionMatrix.Line[2][0] = TempVec.X;
		AditionMatrix.Line[2][1] = TempVec.Y;
		AditionMatrix.Line[2][2] = TempVec.Z;
		AditionMatrix.Line[2][3] = TempVec.W;
		
		Vector1 = GetLine(3);
		Vector2 = _Matrix4x4.GetLine(3);
		Temp = _mm_add_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		AditionMatrix.Line[3][0] = TempVec.X;
		AditionMatrix.Line[3][1] = TempVec.Y;
		AditionMatrix.Line[3][2] = TempVec.Z;
		AditionMatrix.Line[3][3] = TempVec.W;
#else
		AditionMatrix.m00 = m00 + _Matrix4x4.m00;
		AditionMatrix.m01 = m01 + _Matrix4x4.m01;
		AditionMatrix.m02 = m02 + _Matrix4x4.m02;
		AditionMatrix.m03 = m03 + _Matrix4x4.m03;
		AditionMatrix.m10 = m10 + _Matrix4x4.m10;
		AditionMatrix.m11 = m11 + _Matrix4x4.m11;
		AditionMatrix.m12 = m12 + _Matrix4x4.m12;
		AditionMatrix.m13 = m13 + _Matrix4x4.m13;
		AditionMatrix.m20 = m20 + _Matrix4x4.m20;
		AditionMatrix.m21 = m21 + _Matrix4x4.m21;
		AditionMatrix.m22 = m22 + _Matrix4x4.m22;
		AditionMatrix.m23 = m23 + _Matrix4x4.m23;
		AditionMatrix.m30 = m30 + _Matrix4x4.m30;
		AditionMatrix.m31 = m31 + _Matrix4x4.m31;
		AditionMatrix.m32 = m32 + _Matrix4x4.m32;
		AditionMatrix.m33 = m33 + _Matrix4x4.m33;
#endif
		return AditionMatrix;
	}

	soMatrix4x4 soMatrix4x4::operator - (const soMatrix4x4& _Matrix4x4)
	{
		soMatrix4x4 SubtractionMatrix;
#ifdef SO_INTRIN_MATH
		soVector4D TempVec;
		soVector4D Vector1 = GetLine(0);
		soVector4D Vector2 = _Matrix4x4.GetLine(0);
		__m128 Temp = _mm_sub_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		SubtractionMatrix.Line[0][0] = TempVec.X;
		SubtractionMatrix.Line[0][1] = TempVec.Y;
		SubtractionMatrix.Line[0][2] = TempVec.Z;
		SubtractionMatrix.Line[0][3] = TempVec.W;

		Vector1 = GetLine(1);
		Vector2 = _Matrix4x4.GetLine(1);
		Temp = _mm_sub_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		SubtractionMatrix.Line[1][0] = TempVec.X;
		SubtractionMatrix.Line[1][1] = TempVec.Y;
		SubtractionMatrix.Line[1][2] = TempVec.Z;
		SubtractionMatrix.Line[1][3] = TempVec.W;

		Vector1 = GetLine(2);
		Vector2 = _Matrix4x4.GetLine(2);
		Temp = _mm_sub_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		SubtractionMatrix.Line[2][0] = TempVec.X;
		SubtractionMatrix.Line[2][1] = TempVec.Y;
		SubtractionMatrix.Line[2][2] = TempVec.Z;
		SubtractionMatrix.Line[2][3] = TempVec.W;

		Vector1 = GetLine(3);
		Vector2 = _Matrix4x4.GetLine(3);
		Temp = _mm_sub_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		SubtractionMatrix.Line[3][0] = TempVec.X;
		SubtractionMatrix.Line[3][1] = TempVec.Y;
		SubtractionMatrix.Line[3][2] = TempVec.Z;
		SubtractionMatrix.Line[3][3] = TempVec.W;
#else
		SubtractionMatrix.m00 = m00 - _Matrix4x4.m00;
		SubtractionMatrix.m01 = m01 - _Matrix4x4.m01;
		SubtractionMatrix.m02 = m02 - _Matrix4x4.m02;
		SubtractionMatrix.m03 = m03 - _Matrix4x4.m03;
		SubtractionMatrix.m10 = m10 - _Matrix4x4.m10;
		SubtractionMatrix.m11 = m11 - _Matrix4x4.m11;
		SubtractionMatrix.m12 = m12 - _Matrix4x4.m12;
		SubtractionMatrix.m13 = m13 - _Matrix4x4.m13;
		SubtractionMatrix.m20 = m20 - _Matrix4x4.m20;
		SubtractionMatrix.m21 = m21 - _Matrix4x4.m21;
		SubtractionMatrix.m22 = m22 - _Matrix4x4.m22;
		SubtractionMatrix.m23 = m23 - _Matrix4x4.m23;
		SubtractionMatrix.m30 = m30 - _Matrix4x4.m30;
		SubtractionMatrix.m31 = m31 - _Matrix4x4.m31;
		SubtractionMatrix.m32 = m32 - _Matrix4x4.m32;
		SubtractionMatrix.m33 = m33 - _Matrix4x4.m33;
#endif
		return SubtractionMatrix;
	}


	soMatrix4x4 soMatrix4x4::operator *	(const soMatrix4x4& _Matrix4x4)
	{
		soMatrix4x4 MultiplicationMatrix;
#ifdef SO_INTRIN_MATH
		// Use vW to hold the original row
		soVector4D LineVector = GetLine(0);
		__m128 vW = _mm_load_ps((float*)&LineVector);
		// Splat the component X,Y,Z then W
		__m128 vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		// Perform the opertion on the first row
		soVector4D ColVector0 = _Matrix4x4.GetLine(0); 
		soVector4D ColVector1 = _Matrix4x4.GetLine(1);
		soVector4D ColVector2 = _Matrix4x4.GetLine(2);
		soVector4D ColVector3 = _Matrix4x4.GetLine(3);
		vX = _mm_mul_ps(vX, _mm_load_ps((float*)&ColVector0));
		vY = _mm_mul_ps(vY, _mm_load_ps((float*)&ColVector1));
		vZ = _mm_mul_ps(vZ, _mm_load_ps((float*)&ColVector2));
		vW = _mm_mul_ps(vW, _mm_load_ps((float*)&ColVector3));
		// Perform a binary add to reduce cumulative errors
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		soVector4D tempvector;
		memcpy(&tempvector, &vX, sizeof(float) * 4);
		MultiplicationMatrix.Line[0][0] = tempvector.X;
		MultiplicationMatrix.Line[0][1] = tempvector.Y;
		MultiplicationMatrix.Line[0][2] = tempvector.Z;
		MultiplicationMatrix.Line[0][3] = tempvector.W;
		// Repeat for the other 3 rows
		LineVector = GetLine(1);
		vW = _mm_load_ps((float*)&LineVector);
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, _mm_load_ps((float*)&ColVector0));
		vY = _mm_mul_ps(vY, _mm_load_ps((float*)&ColVector1));
		vZ = _mm_mul_ps(vZ, _mm_load_ps((float*)&ColVector2));
		vW = _mm_mul_ps(vW, _mm_load_ps((float*)&ColVector3));
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		memcpy(&tempvector, &vX, sizeof(float) * 4);
		MultiplicationMatrix.Line[1][0] = tempvector.X;
		MultiplicationMatrix.Line[1][1] = tempvector.Y;
		MultiplicationMatrix.Line[1][2] = tempvector.Z;
		MultiplicationMatrix.Line[1][3] = tempvector.W;
		LineVector = GetLine(2);
		vW = _mm_load_ps((float*)&LineVector);
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, _mm_load_ps((float*)&ColVector0));
		vY = _mm_mul_ps(vY, _mm_load_ps((float*)&ColVector1));
		vZ = _mm_mul_ps(vZ, _mm_load_ps((float*)&ColVector2));
		vW = _mm_mul_ps(vW, _mm_load_ps((float*)&ColVector3));
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		memcpy(&tempvector, &vX, sizeof(float) * 4);
		MultiplicationMatrix.Line[2][0] = tempvector.X;
		MultiplicationMatrix.Line[2][1] = tempvector.Y;
		MultiplicationMatrix.Line[2][2] = tempvector.Z;
		MultiplicationMatrix.Line[2][3] = tempvector.W;
		LineVector = GetLine(3);
		vW = _mm_load_ps((float*)&LineVector);
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, _mm_load_ps((float*)&ColVector0));
		vY = _mm_mul_ps(vY, _mm_load_ps((float*)&ColVector1));
		vZ = _mm_mul_ps(vZ, _mm_load_ps((float*)&ColVector2));
		vW = _mm_mul_ps(vW, _mm_load_ps((float*)&ColVector3));
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		memcpy(&tempvector, &vX, sizeof(float) * 4);
		MultiplicationMatrix.Line[3][0] = tempvector.X;
		MultiplicationMatrix.Line[3][1] = tempvector.Y;
		MultiplicationMatrix.Line[3][2] = tempvector.Z;
		MultiplicationMatrix.Line[3][3] = tempvector.W;
#else
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				for (unsigned int k = 0; k < 4; k++)
				{
					MultiplicationMatrix.Line[i][j] += (Line[i][k] * _Matrix4x4.Line[k][j]);
				}
			}
		}
#endif
		return MultiplicationMatrix;
	}

	soVector4D soMatrix4x4::operator * (const soVector4D& _Vector4D)
	{
		soVector4D A = _Vector4D;
		soVector4D MultiplicatedVector;

		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				MultiplicatedVector[i] += (Line[i][j] * A[j]);
			}
		}

		return MultiplicatedVector;
	}

	soMatrix4x4 soMatrix4x4::operator *	(const float& _Value)
	{
		soMatrix4x4 MultiplicatedMatrix;
#ifdef SO_INTRIN_MATH
		soVector4D TempVect;
		__m128 A = _mm_load_ps1(&_Value);
		soVector4D Vector1 = GetLine(0);
		__m128 Temp = _mm_mul_ps(_mm_load_ps((float*)&Vector1), A);
		memcpy(&TempVect, &Temp, sizeof(float) * 4);
		MultiplicatedMatrix.Line[0][0] = TempVect.X;
		MultiplicatedMatrix.Line[0][1] = TempVect.Y;
		MultiplicatedMatrix.Line[0][2] = TempVect.Z;
		MultiplicatedMatrix.Line[0][3] = TempVect.W;

		Vector1 = GetLine(1);
		Temp = _mm_mul_ps(_mm_load_ps((float*)&Vector1), A);
		memcpy(&TempVect, &Temp, sizeof(float) * 4);
		MultiplicatedMatrix.Line[1][0] = TempVect.X;
		MultiplicatedMatrix.Line[1][1] = TempVect.Y;
		MultiplicatedMatrix.Line[1][2] = TempVect.Z;
		MultiplicatedMatrix.Line[1][3] = TempVect.W;

		Vector1 = GetLine(2);
		Temp = _mm_mul_ps(_mm_load_ps((float*)&Vector1), A);
		memcpy(&TempVect, &Temp, sizeof(float) * 4);
		MultiplicatedMatrix.Line[2][0] = TempVect.X;
		MultiplicatedMatrix.Line[2][1] = TempVect.Y;
		MultiplicatedMatrix.Line[2][2] = TempVect.Z;
		MultiplicatedMatrix.Line[2][3] = TempVect.W;

		Vector1 = GetLine(3);
		Temp = _mm_mul_ps(_mm_load_ps((float*)&Vector1), A);
		memcpy(&TempVect, &Temp, sizeof(float) * 4);
		MultiplicatedMatrix.Line[3][0] = TempVect.X;
		MultiplicatedMatrix.Line[3][1] = TempVect.Y;
		MultiplicatedMatrix.Line[3][2] = TempVect.Z;
		MultiplicatedMatrix.Line[3][3] = TempVect.W;
#else
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				MultiplicatedMatrix.Line[j][i] = Line[j][i] * _Value;
			}
		}
#endif
		return MultiplicatedMatrix;
	}

	void soMatrix4x4::operator += (const soMatrix4x4& _Matrix4x4)
	{
#ifdef SO_INTRIN_MATH
		soVector4D TempVec;
		soVector4D Vector1 = GetLine(0);
		soVector4D Vector2 = _Matrix4x4.GetLine(0);
		__m128 Temp = _mm_add_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		Line[0][0] = TempVec.X;
		Line[0][1] = TempVec.Y;
		Line[0][2] = TempVec.Z;
		Line[0][3] = TempVec.W;

		Vector1 = GetLine(1);
		Vector2 = _Matrix4x4.GetLine(1);
		Temp = _mm_add_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		Line[1][0] = TempVec.X;
		Line[1][1] = TempVec.Y;
		Line[1][2] = TempVec.Z;
		Line[1][3] = TempVec.W;

		Vector1 = GetLine(2);
		Vector2 = _Matrix4x4.GetLine(2);
		Temp = _mm_add_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		Line[2][0] = TempVec.X;
		Line[2][1] = TempVec.Y;
		Line[2][2] = TempVec.Z;
		Line[2][3] = TempVec.W;

		Vector1 = GetLine(3);
		Vector2 = _Matrix4x4.GetLine(3);
		Temp = _mm_add_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		Line[3][0] = TempVec.X;
		Line[3][1] = TempVec.Y;
		Line[3][2] = TempVec.Z;
		Line[3][3] = TempVec.W;
#else
		m00 += _Matrix4x4.m00;
		m01 += _Matrix4x4.m01;
		m02 += _Matrix4x4.m02;
		m03 += _Matrix4x4.m03;
		m10 += _Matrix4x4.m10;
		m11 += _Matrix4x4.m11;
		m12 += _Matrix4x4.m12;
		m13 += _Matrix4x4.m13;
		m20 += _Matrix4x4.m20;
		m21 += _Matrix4x4.m21;
		m22 += _Matrix4x4.m22;
		m23 += _Matrix4x4.m23;
		m30 += _Matrix4x4.m30;
		m31 += _Matrix4x4.m31;
		m32 += _Matrix4x4.m32;
		m33 += _Matrix4x4.m33;
#endif 

	}

	void soMatrix4x4::operator -= (const soMatrix4x4& _Matrix4x4)
	{
#ifdef SO_INTRIN_MATH
		soVector4D TempVec;
		soVector4D Vector1 = GetLine(0);
		soVector4D Vector2 = _Matrix4x4.GetLine(0);
		__m128 Temp = _mm_sub_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		Line[0][0] = TempVec.X;
		Line[0][1] = TempVec.Y;
		Line[0][2] = TempVec.Z;
		Line[0][3] = TempVec.W;

		Vector1 = GetLine(1);
		Vector2 = _Matrix4x4.GetLine(1);
		Temp = _mm_sub_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		Line[1][0] = TempVec.X;
		Line[1][1] = TempVec.Y;
		Line[1][2] = TempVec.Z;
		Line[1][3] = TempVec.W;

		Vector1 = GetLine(2);
		Vector2 = _Matrix4x4.GetLine(2);
		Temp = _mm_sub_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		Line[2][0] = TempVec.X;
		Line[2][1] = TempVec.Y;
		Line[2][2] = TempVec.Z;
		Line[2][3] = TempVec.W;

		Vector1 = GetLine(3);
		Vector2 = _Matrix4x4.GetLine(3);
		Temp = _mm_sub_ps(_mm_load_ps((float*)&Vector1), _mm_load_ps((float*)&Vector2));
		memcpy(&TempVec, &Temp, sizeof(float) * 4);
		Line[3][0] = TempVec.X;
		Line[3][1] = TempVec.Y;
		Line[3][2] = TempVec.Z;
		Line[3][3] = TempVec.W;
#else
		m00 -= _Matrix4x4.m00;
		m01 -= _Matrix4x4.m01;
		m02 -= _Matrix4x4.m02;
		m03 -= _Matrix4x4.m03;
		m10 -= _Matrix4x4.m10;
		m11 -= _Matrix4x4.m11;
		m12 -= _Matrix4x4.m12;
		m13 -= _Matrix4x4.m13;
		m20 -= _Matrix4x4.m20;
		m21 -= _Matrix4x4.m21;
		m22 -= _Matrix4x4.m22;
		m23 -= _Matrix4x4.m23;
		m30 -= _Matrix4x4.m30;
		m31 -= _Matrix4x4.m31;
		m32 -= _Matrix4x4.m32;
		m33 -= _Matrix4x4.m33;
#endif 
	}

	void soMatrix4x4::operator*=(const soMatrix4x4 & _Matrix4x4)
	{
		soMatrix4x4 MultiplicationMatrix;
#ifdef SO_INTRIN_MATH
		// Use vW to hold the original row
		soVector4D LineVector = GetLine(0);
		__m128 vW = _mm_load_ps((float*)&LineVector);
		// Splat the component X,Y,Z then W
		__m128 vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		// Perform the opertion on the first row
		soVector4D ColVector0 = _Matrix4x4.GetLine(0);
		soVector4D ColVector1 = _Matrix4x4.GetLine(1);
		soVector4D ColVector2 = _Matrix4x4.GetLine(2);
		soVector4D ColVector3 = _Matrix4x4.GetLine(3);
		vX = _mm_mul_ps(vX, _mm_load_ps((float*)&ColVector0));
		vY = _mm_mul_ps(vY, _mm_load_ps((float*)&ColVector1));
		vZ = _mm_mul_ps(vZ, _mm_load_ps((float*)&ColVector2));
		vW = _mm_mul_ps(vW, _mm_load_ps((float*)&ColVector3));
		// Perform a binary add to reduce cumulative errors
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		soVector4D tempvector;
		memcpy(&tempvector, &vX, sizeof(float) * 4);
		MultiplicationMatrix.Line[0][0] = tempvector.X;
		MultiplicationMatrix.Line[0][1] = tempvector.Y;
		MultiplicationMatrix.Line[0][2] = tempvector.Z;
		MultiplicationMatrix.Line[0][3] = tempvector.W;
		// Repeat for the other 3 rows
		LineVector = GetLine(1);
		vW = _mm_load_ps((float*)&LineVector);
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, _mm_load_ps((float*)&ColVector0));
		vY = _mm_mul_ps(vY, _mm_load_ps((float*)&ColVector1));
		vZ = _mm_mul_ps(vZ, _mm_load_ps((float*)&ColVector2));
		vW = _mm_mul_ps(vW, _mm_load_ps((float*)&ColVector3));
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		memcpy(&tempvector, &vX, sizeof(float) * 4);
		MultiplicationMatrix.Line[1][0] = tempvector.X;
		MultiplicationMatrix.Line[1][1] = tempvector.Y;
		MultiplicationMatrix.Line[1][2] = tempvector.Z;
		MultiplicationMatrix.Line[1][3] = tempvector.W;
		LineVector = GetLine(2);
		vW = _mm_load_ps((float*)&LineVector);
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, _mm_load_ps((float*)&ColVector0));
		vY = _mm_mul_ps(vY, _mm_load_ps((float*)&ColVector1));
		vZ = _mm_mul_ps(vZ, _mm_load_ps((float*)&ColVector2));
		vW = _mm_mul_ps(vW, _mm_load_ps((float*)&ColVector3));
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		memcpy(&tempvector, &vX, sizeof(float) * 4);
		MultiplicationMatrix.Line[2][0] = tempvector.X;
		MultiplicationMatrix.Line[2][1] = tempvector.Y;
		MultiplicationMatrix.Line[2][2] = tempvector.Z;
		MultiplicationMatrix.Line[2][3] = tempvector.W;
		LineVector = GetLine(3);
		vW = _mm_load_ps((float*)&LineVector);
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, _mm_load_ps((float*)&ColVector0));
		vY = _mm_mul_ps(vY, _mm_load_ps((float*)&ColVector1));
		vZ = _mm_mul_ps(vZ, _mm_load_ps((float*)&ColVector2));
		vW = _mm_mul_ps(vW, _mm_load_ps((float*)&ColVector3));
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		memcpy(&tempvector, &vX, sizeof(float) * 4);
		MultiplicationMatrix.Line[3][0] = tempvector.X;
		MultiplicationMatrix.Line[3][1] = tempvector.Y;
		MultiplicationMatrix.Line[3][2] = tempvector.Z;
		MultiplicationMatrix.Line[3][3] = tempvector.W;
#else
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				for (unsigned int k = 0; k < 4; k++)
				{
					MultiplicationMatrix.Line[i][j] += (Line[i][k] * _Matrix4x4.Line[k][j]);
				}
			}
		}
#endif
		*this = MultiplicationMatrix;
	}

	void soMatrix4x4::operator*=(const float& _Value)
	{
#ifdef SO_INTRIN_MATH
		soVector4D TempVect;
		__m128 A = _mm_load_ps1(&_Value);
		soVector4D Vector1 = GetLine(0);
		__m128 B = _mm_load_ps((float*)&Vector1);
		__m128 Temp = _mm_mul_ps(B, A);
		memcpy(&TempVect, &Temp, sizeof(float) * 4);
		Line[0][0] = TempVect.X;
		Line[0][1] = TempVect.Y;
		Line[0][2] = TempVect.Z;
		Line[0][3] = TempVect.W;
		Vector1 = GetLine(1);
		B = _mm_load_ps((float*)&Vector1);
		Temp = _mm_mul_ps(B, A);
		memcpy(&TempVect, &Temp, sizeof(float) * 4);
		Line[1][0] = TempVect.X;
		Line[1][1] = TempVect.Y;
		Line[1][2] = TempVect.Z;
		Line[1][3] = TempVect.W;
		Vector1 = GetLine(2);
		B = _mm_load_ps((float*)&Vector1);
		Temp = _mm_mul_ps(B, A);
		memcpy(&TempVect, &Temp, sizeof(float) * 4);
		Line[2][0] = TempVect.X;
		Line[2][1] = TempVect.Y;
		Line[2][2] = TempVect.Z;
		Line[2][3] = TempVect.W;
		Vector1 = GetLine(3);
		B = _mm_load_ps((float*)&Vector1);
		Temp = _mm_mul_ps(B, A);
		memcpy(&TempVect, &Temp, sizeof(float) * 4);
		Line[3][0] = TempVect.X;
		Line[3][1] = TempVect.Y;
		Line[3][2] = TempVect.Z;
		Line[3][3] = TempVect.W;
#else
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				Line[j][i] = Line[j][i] * _Value;
			}
		}
#endif
	}

	bool soMatrix4x4::operator == (const soMatrix4x4& _Matrix4x4)
	{
		return (fMatrix[0] == _Matrix4x4.fMatrix[0] &&
				fMatrix[1] == _Matrix4x4.fMatrix[1] &&
				fMatrix[2] == _Matrix4x4.fMatrix[2] &&
				fMatrix[3] == _Matrix4x4.fMatrix[3] &&
				fMatrix[4] == _Matrix4x4.fMatrix[4] &&
				fMatrix[5] == _Matrix4x4.fMatrix[5] &&
				fMatrix[6] == _Matrix4x4.fMatrix[6] &&
				fMatrix[7] == _Matrix4x4.fMatrix[7] &&
				fMatrix[8] == _Matrix4x4.fMatrix[8] &&
				fMatrix[9] == _Matrix4x4.fMatrix[9] &&
				fMatrix[10] == _Matrix4x4.fMatrix[10] &&
				fMatrix[11] == _Matrix4x4.fMatrix[11] &&
				fMatrix[12] == _Matrix4x4.fMatrix[12] &&
				fMatrix[13] == _Matrix4x4.fMatrix[13] &&
				fMatrix[14] == _Matrix4x4.fMatrix[14] &&
				fMatrix[15] == _Matrix4x4.fMatrix[15] ? true : false);
	}

	bool soMatrix4x4::operator != (const soMatrix4x4& _Matrix4x4)
	{
		return (fMatrix[0] != _Matrix4x4.fMatrix[0] ||
				fMatrix[1] != _Matrix4x4.fMatrix[1] ||
				fMatrix[2] != _Matrix4x4.fMatrix[2] ||
				fMatrix[3] != _Matrix4x4.fMatrix[3] ||
				fMatrix[4] != _Matrix4x4.fMatrix[4] ||
				fMatrix[5] != _Matrix4x4.fMatrix[5] ||
				fMatrix[6] != _Matrix4x4.fMatrix[6] ||
				fMatrix[7] != _Matrix4x4.fMatrix[7] ||
				fMatrix[8] != _Matrix4x4.fMatrix[8] ||
				fMatrix[9] != _Matrix4x4.fMatrix[9] ||
				fMatrix[10] != _Matrix4x4.fMatrix[10] ||
				fMatrix[11] != _Matrix4x4.fMatrix[11] ||
				fMatrix[12] != _Matrix4x4.fMatrix[12] ||
				fMatrix[13] != _Matrix4x4.fMatrix[13] ||
				fMatrix[14] != _Matrix4x4.fMatrix[14] ||
				fMatrix[15] != _Matrix4x4.fMatrix[15] ? true : false);
	}
}