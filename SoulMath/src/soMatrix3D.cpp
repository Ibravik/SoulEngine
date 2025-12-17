/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soMatrix3D.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/17/2015
* @brief  Implementacion de soMatrix3D
* @bug	   No known bugs.
*/
/********************************************************************/

/***********************************************************************************/
/* Header files inclusion														   */
/***********************************************************************************/
#include <memory>
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"		/*!< Funciones matematicas basicas optimisadas para Windows */
#endif
#include "soPlatformMath.h"
#include "soVector3D.h"
#include "soMatrix3D.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones de la clase soMatrix3D                       */
	/************************************************************************/
	soMatrix3x3::soMatrix3x3(const float m0[3], const float m1[3], const float m2[3])
	{
		m00 = m0[0]; m01 = m0[1]; m02 = m0[2];
		m10 = m1[0]; m11 = m1[1]; m12 = m1[2];
		m20 = m2[0]; m21 = m2[1]; m22 = m2[2];
	}

	soMatrix3x3::soMatrix3x3(const soMatrix3x3& _CopyObj)
	{
		memcpy(&fMatrix, _CopyObj.fMatrix, sizeof(float) * 9);
	}

	soMatrix3x3::~soMatrix3x3()
	{
	}

	void soMatrix3x3::ResetMatrix()
	{
		memset(this, 0, sizeof(soMatrix3x3));
	}

	bool soMatrix3x3::Equal(const soMatrix3x3 & _Matrix3x3, float _Precision)
	{
		if (soMath::Abs(fMatrix[0] - _Matrix3x3.fMatrix[0]) <= _Precision &&
			soMath::Abs(fMatrix[1] - _Matrix3x3.fMatrix[1]) <= _Precision &&
			soMath::Abs(fMatrix[2] - _Matrix3x3.fMatrix[2]) <= _Precision &&
			soMath::Abs(fMatrix[3] - _Matrix3x3.fMatrix[3]) <= _Precision &&
			soMath::Abs(fMatrix[4] - _Matrix3x3.fMatrix[4]) <= _Precision &&
			soMath::Abs(fMatrix[5] - _Matrix3x3.fMatrix[5]) <= _Precision &&
			soMath::Abs(fMatrix[6] - _Matrix3x3.fMatrix[6]) <= _Precision &&
			soMath::Abs(fMatrix[7] - _Matrix3x3.fMatrix[7]) <= _Precision &&
			soMath::Abs(fMatrix[8] - _Matrix3x3.fMatrix[8]) <= _Precision)
		{
			return true;
		} 
		return false;
	}

	bool soMatrix3x3::Equal(soMatrix3x3 & _Matrix3x3, float _Precision)
	{
		if (soMath::Abs(fMatrix[0] - _Matrix3x3.fMatrix[0]) <= _Precision &&
			soMath::Abs(fMatrix[1] - _Matrix3x3.fMatrix[1]) <= _Precision &&
			soMath::Abs(fMatrix[2] - _Matrix3x3.fMatrix[2]) <= _Precision &&
			soMath::Abs(fMatrix[3] - _Matrix3x3.fMatrix[3]) <= _Precision &&
			soMath::Abs(fMatrix[4] - _Matrix3x3.fMatrix[4]) <= _Precision &&
			soMath::Abs(fMatrix[5] - _Matrix3x3.fMatrix[5]) <= _Precision &&
			soMath::Abs(fMatrix[6] - _Matrix3x3.fMatrix[6]) <= _Precision &&
			soMath::Abs(fMatrix[7] - _Matrix3x3.fMatrix[7]) <= _Precision &&
			soMath::Abs(fMatrix[8] - _Matrix3x3.fMatrix[8]) <= _Precision)
		{
			return true;
		}
		return false;
	}

	void soMatrix3x3::operator=(soMatrix3x3 _Matrix3x3)
	{
		memcpy(&fMatrix, &_Matrix3x3.fMatrix, sizeof(float) * 9);
	}

	soMatrix3x3 soMatrix3x3::operator + (const soMatrix3x3& _Matrix3x3)
	{
		soMatrix3x3 AditionMatrix;

		AditionMatrix.m00 = m00 + _Matrix3x3.m00;
		AditionMatrix.m01 = m01 + _Matrix3x3.m01;
		AditionMatrix.m02 = m02 + _Matrix3x3.m02;
		AditionMatrix.m10 = m10 + _Matrix3x3.m10;
		AditionMatrix.m11 = m11 + _Matrix3x3.m11;
		AditionMatrix.m12 = m12 + _Matrix3x3.m12;
		AditionMatrix.m20 = m20 + _Matrix3x3.m20;
		AditionMatrix.m21 = m21 + _Matrix3x3.m21;
		AditionMatrix.m22 = m22 + _Matrix3x3.m22;

		return AditionMatrix;
	}

	soMatrix3x3 soMatrix3x3::operator - (const soMatrix3x3& _Matrix3x3)
	{
		soMatrix3x3 SubtractionMatrix;

		SubtractionMatrix.m00 = m00 - _Matrix3x3.m00;
		SubtractionMatrix.m01 = m01 - _Matrix3x3.m01;
		SubtractionMatrix.m02 = m02 - _Matrix3x3.m02;
		SubtractionMatrix.m10 = m10 - _Matrix3x3.m10;
		SubtractionMatrix.m11 = m11 - _Matrix3x3.m11;
		SubtractionMatrix.m12 = m12 - _Matrix3x3.m12;
		SubtractionMatrix.m20 = m20 - _Matrix3x3.m20;
		SubtractionMatrix.m21 = m21 - _Matrix3x3.m21;
		SubtractionMatrix.m22 = m22 - _Matrix3x3.m22;

		return SubtractionMatrix;
	}

	soMatrix3x3 soMatrix3x3::operator *	(const soMatrix3x3& _Matrix3x3)
	{
		soMatrix3x3 MultiplicationMatrix;
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				for (unsigned int k = 0; k < 3; k++)
				{
					MultiplicationMatrix.Line[i][j] += (Line[i][k] * _Matrix3x3.Line[k][j]);
				}
			}
		}
		return MultiplicationMatrix;
	}

	soVector3D soMatrix3x3::operator * (const soVector3D& _Vector3D)
	{
		soVector3D A = _Vector3D;
		soVector3D MultiplicatedVector;

		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MultiplicatedVector[i] += (Line[i][j] * A[j]);
			}
		}

		return MultiplicatedVector;
	}

	soMatrix3x3 soMatrix3x3::operator *	(const float& _Value)
	{
		soMatrix3x3 MultiplicatedMatrix;
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MultiplicatedMatrix.Line[j][i] = Line[j][i] * _Value;
			}
		}
		return MultiplicatedMatrix;
	}

	void soMatrix3x3::operator += (const soMatrix3x3& _Matrix3x3)
	{
		m00 += _Matrix3x3.m00;
		m01 += _Matrix3x3.m01;
		m02 += _Matrix3x3.m02;
		m10 += _Matrix3x3.m10;
		m11 += _Matrix3x3.m11;
		m12 += _Matrix3x3.m12;
		m20 += _Matrix3x3.m20;
		m21 += _Matrix3x3.m21;
		m22 += _Matrix3x3.m22;
	}

	void soMatrix3x3::operator -= (const soMatrix3x3& _Matrix3x3)
	{
		m00 -= _Matrix3x3.m00;
		m01 -= _Matrix3x3.m01;
		m02 -= _Matrix3x3.m02;
		m10 -= _Matrix3x3.m10;
		m11 -= _Matrix3x3.m11;
		m12 -= _Matrix3x3.m12;
		m20 -= _Matrix3x3.m20;
		m21 -= _Matrix3x3.m21;
		m22 -= _Matrix3x3.m22;
	}

	void soMatrix3x3::operator*=(const soMatrix3x3 & _Matrix3x3)
	{
		soMatrix3x3 MultiplicationMatrix;
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				for (unsigned int k = 0; k < 3; k++)
				{
					MultiplicationMatrix.Line[i][j] += (Line[i][k] * _Matrix3x3.Line[k][j]);
				}
			}
		}
		*this = MultiplicationMatrix;
	}

	void soMatrix3x3::operator*=(const soVector3D & _Vector3D)
	{
		soVector3D A = _Vector3D;
		soVector3D MultiplicatedVector;

		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MultiplicatedVector[i] += (Line[i][j] * A[j]);
			}
		}
	}

	void soMatrix3x3::operator*=(const float& _Value)
	{
		soMatrix3x3 MultiplicatedMatrix;
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				MultiplicatedMatrix.Line[j][i] = Line[j][i] * _Value;
			}
		}
		*this = MultiplicatedMatrix;
	}

	bool soMatrix3x3::operator == (const soMatrix3x3& _Matrix3x3)
	{
		return (fMatrix[0] == _Matrix3x3.fMatrix[0] &&
				fMatrix[1] == _Matrix3x3.fMatrix[1] &&
				fMatrix[2] == _Matrix3x3.fMatrix[2] &&
				fMatrix[3] == _Matrix3x3.fMatrix[3] &&
				fMatrix[4] == _Matrix3x3.fMatrix[4] &&
				fMatrix[5] == _Matrix3x3.fMatrix[5] &&
				fMatrix[6] == _Matrix3x3.fMatrix[6] &&
				fMatrix[7] == _Matrix3x3.fMatrix[7] &&
				fMatrix[8] == _Matrix3x3.fMatrix[8] ? true : false);
	}

	bool soMatrix3x3::operator != (const soMatrix3x3& _Matrix3x3)
	{
		return (fMatrix[0] != _Matrix3x3.fMatrix[0] ||
				fMatrix[1] != _Matrix3x3.fMatrix[1] ||
				fMatrix[2] != _Matrix3x3.fMatrix[2] ||
				fMatrix[3] != _Matrix3x3.fMatrix[3] ||
				fMatrix[4] != _Matrix3x3.fMatrix[4] ||
				fMatrix[5] != _Matrix3x3.fMatrix[5] ||
				fMatrix[6] != _Matrix3x3.fMatrix[6] ||
				fMatrix[7] != _Matrix3x3.fMatrix[7] ||
				fMatrix[8] != _Matrix3x3.fMatrix[8] ? true : false);
	}
}