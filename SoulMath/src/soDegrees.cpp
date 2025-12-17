/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soDegrees.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/30/2015
* @brief  Implementacion de onDegrees
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************************************************************/
/* Header files inclusion																								*/
/************************************************************************************************************************/
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"		/*!< Funciones matematicas basicas optimisadas para Windows */
#endif
#include "soPlatformMath.h"
#include "soRadians.h"
#include "soDegrees.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones de la clase soDegrees                        */
	/************************************************************************/
	soDegrees::soDegrees(const soDegrees& _CopyObj)
	{
		m_fDegrees = _CopyObj.GetDegrees();
	}

	soDegrees::soDegrees(const soRadians & _CopyObj)
	{
		m_fDegrees = _CopyObj.GetDegrees();
	}

	soDegrees::~soDegrees()
	{

	}

	float soDegrees::GetRadians() const
	{
		return m_fDegrees * soMath::DEG_TO_RAD;
	}

	float soDegrees::GetDegrees() const
	{
		return m_fDegrees;
	}

	void soDegrees::Unwind() 
	{
		while (m_fDegrees >= 360.0f)
		{
			m_fDegrees -= 360.0f;
		}
	}

	bool soDegrees::Equal(const float& _Value, float _Precision) const
	{
		if (soMath::Abs(m_fDegrees - _Value) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soDegrees::Equal(float& _Value, float _Precision) const
	{
		if (soMath::Abs(m_fDegrees - _Value) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soDegrees::Equal(const soDegrees& _Euler, float _Precision) const
	{
		if (soMath::Abs(m_fDegrees - _Euler.GetDegrees()) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soDegrees::Equal(soDegrees& _Euler, float _Precision) const
	{
		if (soMath::Abs(m_fDegrees - _Euler.GetDegrees()) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soDegrees::Equal(const soRadians& _Radian, float _Precision) const
	{
		if (soMath::Abs(m_fDegrees - _Radian.GetDegrees()) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soDegrees::Equal(soRadians& _Radian, float _Precision) const
	{
		if (soMath::Abs(m_fDegrees - _Radian.GetDegrees()) <= _Precision)
		{
			return true;
		}
		return false;
	}

	void soDegrees::operator= (const soRadians& _Radians)
	{
		float Degres = _Radians.GetDegrees();
		memcpy(&m_fDegrees, &Degres, sizeof(float));
	}

	void soDegrees::operator= (const float& _Value)
	{
		memcpy(&m_fDegrees, &_Value, sizeof(float));
	}

	void soDegrees::operator=(const soDegrees& _Degrees)
	{
		float Degres = _Degrees.GetDegrees();
		memcpy(&m_fDegrees, &Degres, sizeof(float));
	}

	soDegrees operator+(const soDegrees & _Degrees1, const soDegrees & _Degrees2)
	{
		return soDegrees(_Degrees1.GetDegrees() + _Degrees2.GetDegrees());
	}

	soDegrees operator+(soDegrees & _Degrees1, const soDegrees & _Degrees2)
	{
		return soDegrees(_Degrees1.GetDegrees() + _Degrees2.GetDegrees());
	}

	soDegrees operator+(const soDegrees & _Degrees1, soDegrees & _Degrees2)
	{
		return soDegrees(_Degrees1.GetDegrees() + _Degrees2.GetDegrees());
	}

	soDegrees operator+(soDegrees & _Degrees1, soDegrees & _Degrees2)
	{
		return soDegrees(_Degrees1.GetDegrees() + _Degrees2.GetDegrees());
	}

	soDegrees soDegrees::operator+(const soRadians & _Radians)
	{
		return soDegrees(m_fDegrees +_Radians.GetDegrees());
	}

	soDegrees soDegrees::operator+(soRadians & _Radians)
	{
		return soDegrees(m_fDegrees + _Radians.GetDegrees());
	}

	soDegrees operator+(const soDegrees & _Degrees, const float & _Value)
	{
		return soDegrees(_Degrees.GetDegrees() + _Value);
	}

	soDegrees operator+(const float & _Value, const soDegrees & _Degrees)
	{
		return soDegrees(_Degrees.GetDegrees() + _Value);
	}

	soDegrees operator-(const soDegrees & _Degrees1, const soDegrees & _Degrees2)
	{
		return soDegrees(_Degrees1.GetDegrees() - _Degrees2.GetDegrees());
	}

	soDegrees operator-(soDegrees & _Degrees1, const soDegrees & _Degrees2)
	{
		return soDegrees(_Degrees1.GetDegrees() - _Degrees2.GetDegrees());
	}

	soDegrees operator-(const soDegrees & _Degrees1, soDegrees & _Degrees2)
	{
		return soDegrees(_Degrees1.GetDegrees() - _Degrees2.GetDegrees());
	}

	soDegrees operator-(soDegrees & _Degrees1, soDegrees & _Degrees2)
	{
		return soDegrees(_Degrees1.GetDegrees() - _Degrees2.GetDegrees());
	}

	soDegrees soDegrees::operator-(const soRadians & _Radians)
	{
		return soDegrees(m_fDegrees - _Radians.GetDegrees());
	}

	soDegrees soDegrees::operator-(soRadians & _Radians)
	{
		return soDegrees(m_fDegrees - _Radians.GetDegrees());
	}

	soDegrees operator-(const soDegrees & _Degrees, const float & _Value)
	{
		return soDegrees(_Degrees.GetDegrees() - _Value);
	}

	soDegrees operator-(const float & _Value, const soDegrees & _Degrees)
	{
		return soDegrees(_Degrees.GetDegrees() - _Value);
	}

	void soDegrees::operator+=(const soDegrees & _Degrees)
	{
		m_fDegrees += _Degrees.GetDegrees();
	}

	void soDegrees::operator+=(const soRadians & _Radians)
	{
		m_fDegrees += _Radians.GetDegrees();
	}

	void soDegrees::operator+=(const float & _Value)
	{
		m_fDegrees += _Value;
	}

	void operator+=(float & _Value, soDegrees & _Degrees)
	{
		_Degrees += _Value;
	}

	void soDegrees::operator-=(const soDegrees & _Degrees)
	{
		m_fDegrees -= _Degrees.GetDegrees();
	}

	void soDegrees::operator-=(const soRadians & _Radians)
	{
		m_fDegrees -= _Radians.GetDegrees();
	}

	void soDegrees::operator-=(const float & _Value)
	{
		m_fDegrees -= _Value;
	}

	void operator-=(float & _Value, soDegrees & _Degrees)
	{
		_Degrees -= _Value;
	}

	bool soDegrees::operator==(const soDegrees & _Degrees)
	{
		return (m_fDegrees == _Degrees.GetDegrees() ? true : false);
	}

	bool soDegrees::operator==(const soRadians & _Radians)
	{
		return (m_fDegrees == _Radians.GetDegrees() ? true : false);
	}

	bool soDegrees::operator!=(const soDegrees & _Degrees)
	{
		return (m_fDegrees != _Degrees.GetDegrees() ? true : false);
	}

	bool soDegrees::operator!=(const soRadians & _Radians)
	{
		return (m_fDegrees != _Radians.GetDegrees() ? true : false);
	}
}