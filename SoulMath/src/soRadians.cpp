/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soRadians.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/30/2015
* @brief  Implementacion de onRadians
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
#include "soDegrees.h"
#include "soRadians.h"


namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones de la clase soRadians                        */
	/************************************************************************/
	soRadians::soRadians(const soRadians& _CopyObj)
	{
		m_fRadians = _CopyObj.GetRadians();
	}

	soRadians::soRadians(const soDegrees & _CopyObj)
	{
		m_fRadians = _CopyObj.GetRadians();
	}

	soRadians::~soRadians()
	{

	}

	float soRadians::GetRadians() const
	{
		return m_fRadians;
	}

	float soRadians::GetDegrees() const
	{
		return m_fRadians * soMath::RAD_TO_DEG;
	}

	void soRadians::Unwind()
	{
		while (m_fRadians >= soMath::DOUBLE_PI)
		{
			m_fRadians -= soMath::DOUBLE_PI;
		}
	}

	bool soRadians::Equal(const float& _Value, float _Precision) const
	{
		if (soMath::Abs(m_fRadians - _Value) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soRadians::Equal(float& _Value, float _Precision) const
	{
		if (soMath::Abs(m_fRadians - _Value) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soRadians::Equal(const soDegrees& _Euler, float _Precision) const
	{
		if (soMath::Abs(m_fRadians - _Euler.GetRadians()) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soRadians::Equal(soDegrees& _Euler, float _Precision) const
	{
		if (soMath::Abs(m_fRadians - _Euler.GetRadians()) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soRadians::Equal(const soRadians& _Radian, float _Precision) const
	{
		if (soMath::Abs(m_fRadians - _Radian.GetRadians()) <= _Precision)
		{
			return true;
		}
		return false;
	}

	bool soRadians::Equal(soRadians& _Radian, float _Precision) const
	{
		if (soMath::Abs(m_fRadians - _Radian.GetRadians()) <= _Precision)
		{
			return true;
		}
		return false;
	}

	void soRadians::operator= (const soDegrees& _Degrees)
	{
		float Radians = _Degrees.GetRadians();
		memcpy(&m_fRadians, &Radians, sizeof(float));
	}

	void soRadians::operator= (const float& _Value)
	{
		memcpy(&m_fRadians, &_Value, sizeof(float));
	}

	void soRadians::operator=(const soRadians& _Radians)
	{
		float Radians = _Radians.GetRadians();
		memcpy(&m_fRadians, &Radians, sizeof(float));
	}

	soRadians operator+(const soRadians & _Radians1, const soRadians & _Radians2)
	{
		return soRadians(_Radians1.GetRadians() + _Radians2.GetRadians());
	}

	soRadians operator+(soRadians & _Radians1, const soRadians & _Radians2)
	{
		return soRadians(_Radians1.GetRadians() + _Radians2.GetRadians());
	}

	soRadians operator+(const soRadians & _Radians1, soRadians & _Radians2)
	{
		return soRadians(_Radians1.GetRadians() + _Radians2.GetRadians());
	}

	soRadians operator+(soRadians & _Radians1, soRadians & _Radians2)
	{
		return soRadians(_Radians1.GetRadians() + _Radians2.GetRadians());
	}

	soRadians soRadians::operator+(const soDegrees & _Degrees)
	{
		return soRadians(m_fRadians + _Degrees.GetRadians());
	}

	soRadians soRadians::operator+(soDegrees & _Degrees)
	{
		return soRadians(m_fRadians + _Degrees.GetRadians());
	}

	soRadians operator+(const soRadians & _Radians1, const float & _Value)
	{
		return soRadians(_Radians1.GetRadians() + _Value);
	}

	soRadians operator+(const float & _Value, const soRadians & _Radians1)
	{
		return soRadians(_Radians1.GetRadians() + _Value);
	}

	soRadians operator-(const soRadians & _Radians1, const soRadians & _Radians2)
	{
		return soRadians(_Radians1.GetRadians() - _Radians2.GetRadians());
	}

	soRadians operator-(soRadians & _Radians1, const soRadians & _Radians2)
	{
		return soRadians(_Radians1.GetRadians() - _Radians2.GetRadians());
	}

	soRadians operator-(const soRadians & _Radians1, soRadians & _Radians2)
	{
		return soRadians(_Radians1.GetRadians() - _Radians2.GetRadians());
	}

	soRadians operator-(soRadians & _Radians1, soRadians & _Radians2)
	{
		return soRadians(_Radians1.GetRadians() - _Radians2.GetRadians());
	}

	soRadians soRadians::operator-(const soDegrees & _Degrees)
	{
		return soRadians(m_fRadians - _Degrees.GetRadians());
	}

	soRadians soRadians::operator-(soDegrees & _Degrees)
	{
		return soRadians(m_fRadians - _Degrees.GetRadians());
	}

	soRadians operator-(const soRadians & _Radians1, const float & _Value)
	{
		return soRadians(_Radians1.GetRadians() - _Value);
	}

	soRadians operator-(const float & _Value, const soRadians & _Radians1)
	{
		return soRadians(_Radians1.GetRadians() - _Value);
	}

	void soRadians::operator+=(const soDegrees & _Degrees)
	{
		m_fRadians += _Degrees.GetRadians();
	}

	void soRadians::operator+=(const soRadians & _Radians)
	{
		m_fRadians += _Radians.GetRadians();
	}

	void soRadians::operator+=(const float & _Value)
	{
		m_fRadians += _Value;
	}

	void operator+=(float & _Value, soRadians & _Radians)
	{
		_Radians += _Value;
	}

	void soRadians::operator-=(const soDegrees & _Degrees)
	{
		m_fRadians -= _Degrees.GetRadians();
	}

	void soRadians::operator-=(const soRadians & _Radians)
	{
		m_fRadians -= _Radians.GetRadians();
	}

	void soRadians::operator-=(const float & _Value)
	{
		m_fRadians -= _Value;
	}

	void operator-=(float & _Value, soRadians & _Radians)
	{
		_Radians -= _Value;
	}

	bool soRadians::operator==(const soDegrees & _Degrees)
	{
		return (m_fRadians == _Degrees.GetRadians() ? true : false);
	}

	bool soRadians::operator==(const soRadians & _Radians)
	{
		return (m_fRadians == _Radians.GetRadians() ? true : false);
	}

	bool soRadians::operator!=(const soDegrees & _Degrees)
	{
		return (m_fRadians != _Degrees.GetRadians() ? true : false);
	}

	bool soRadians::operator!=(const soRadians & _Radians)
	{
		return (m_fRadians != _Radians.GetRadians() ? true : false);
	}
}
