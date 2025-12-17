/************************************************************************/
/**
* @LC	  7/09/2016
* @file   soRotator.h
* @Author Alejandro Rodriguez (alejandro.rodriguez.navarro92@gmail.com)
* @date   7/09/2016
* @brief  Declaration of base class soActor
* @bug	  No know Bugs
*/
/************************************************************************/

#include "soRadians.h"
#include "soDegrees.h"
#include "soVector3D.h"
#include "soVector4D.h"
#include "soQuaternion.h"
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"		/*!< Funciones matematicas basicas optimisadas para Windows */
#endif
#include "soPlatformMath.h"
#include "soRotator.h"

namespace SoulSDK
{
	soRotator::soRotator()
	{
		m_Rotation = soQuaternion(0, 0, 0, 1);
	}
	
	soRotator::soRotator(const float& _Yaw, const float& _Pich, const float& _Roll)
	{
		m_Rotation = soQuaternion(_Yaw, _Pich, _Roll);
	}

	soRotator::soRotator(const soDegrees& _Yaw, const soDegrees& _Pich, const soDegrees& _Roll)
	{
		m_Rotation = soQuaternion(_Yaw, _Pich, _Roll);
	}

	soRotator::soRotator(const soRadians& _Yaw, const soRadians& _Pich, const soRadians& _Roll)
	{
		m_Rotation = soQuaternion(_Yaw, _Pich, _Roll);
	}

	soRotator::soRotator(const soVector4D& _InfoVector)
	{
		*this = soVector4D(_InfoVector);
	}

	soRotator::soRotator(const soQuaternion& _InfoVector)
	{
		m_Rotation = soQuaternion(_InfoVector);
	}

	soRotator::~soRotator()
	{
	}

	void soRotator::GetYaw(soDegrees& _YawOut)
	{
		soDegrees _PichOut, _RollOut;
		soMath::QuaternionToEulerAngle(m_Rotation, _YawOut, _PichOut, _RollOut);
	}

	void soRotator::GetYaw(soRadians& _YawOut)
	{
		soRadians _PichOut, _RollOut;
		soMath::QuaternionToRadianAngle(m_Rotation, _YawOut, _PichOut, _RollOut);
	}
	
	void soRotator::GetPich(soDegrees& _PichOut)
	{
		soDegrees _YawOut, _RollOut;
		soMath::QuaternionToEulerAngle(m_Rotation, _YawOut, _PichOut, _RollOut);
	}

	void soRotator::GetPich(soRadians& _PichOut)
	{
		soRadians _YawOut, _RollOut;
		soMath::QuaternionToRadianAngle(m_Rotation, _YawOut, _PichOut, _RollOut);
	}

	void soRotator::GetRoll(soDegrees& _RollOut)
	{
		soDegrees _YawOut, _PichOut;
		soMath::QuaternionToEulerAngle(m_Rotation, _YawOut, _PichOut, _RollOut);
	}

	void soRotator::GetRoll(soRadians& _RollOut)
	{
		soRadians _YawOut, _PichOut;
		soMath::QuaternionToRadianAngle(m_Rotation, _YawOut, _PichOut, _RollOut);
	}

	void soRotator::GetQuat(soQuaternion& _QuatOut)
	{
		_QuatOut = m_Rotation;
	}

	void soRotator::GetYawPichRoll(soDegrees& _YawOut, soDegrees& _PichOut, soDegrees& _RollOut)
	{
		soMath::QuaternionToEulerAngle(m_Rotation, _YawOut, _PichOut, _RollOut);
	}

	void soRotator::GetYawPichRoll(soRadians& _YawOut, soRadians& _PichOut, soRadians& _RollOut)
	{
		soMath::QuaternionToRadianAngle(m_Rotation, _YawOut, _PichOut, _RollOut);
	}

	void soRotator::GetRotationMatrix(soMatrix4x4& _RotationMatrix)
	{
		soMath::QuaternionToMatrixRotation4x4(m_Rotation, _RotationMatrix);
	}

	void soRotator::Yaw(soDegrees& _YawIn)
	{
		m_Yaw = _YawIn;
	}

	void soRotator::Yaw(soRadians& _YawIn)
	{
		m_Yaw = _YawIn;
	}

	void soRotator::Pich(soDegrees& _PichIn)
	{
		m_Pich = _PichIn;
	}

	void soRotator::Pich(soRadians& _PichIn)
	{
		m_Pich = _PichIn;
	}

	void soRotator::Roll(soDegrees& _RollIn)
	{
		m_Roll = _RollIn;
	}

	void soRotator::Roll(soRadians& _RollIn)
	{
		m_Roll = _RollIn;
	}

	void soRotator::YawPichRoll(soDegrees& _Yaw, soDegrees& _Pich, soDegrees& _Roll)
	{
		Yaw(_Yaw);
		Pich(_Pich);
		Roll(_Roll);
	}

	void soRotator::YawPichRoll(soRadians& _Yaw, soRadians& _Pich, soRadians& _Roll)
	{
		Yaw(_Yaw);
		Pich(_Pich);
		Roll(_Roll);
	}
	
	void soRotator::operator=(const soRotator& _RotIn)
	{
		memcpy(this, &_RotIn, sizeof(soRotator));
	}

	bool soRotator::operator!=(const soRotator& _RotatorIn)
	{
		return m_Rotation != soQuaternion(_RotatorIn.m_Rotation);
	}

	bool soRotator::operator!=(const soQuaternion& _QuatIn)
	{
		return m_Rotation != soQuaternion(_QuatIn);
	}

	bool soRotator::operator==(const soRotator& _RotatorIn)
	{
		return m_Rotation == soQuaternion(_RotatorIn.m_Rotation);
	}

	bool soRotator::operator==(const soQuaternion& _QuatIn)
	{
		return m_Rotation == soQuaternion(_QuatIn);
	}

	void soRotator::operator=(const soQuaternion& _QuatIn)
	{
		m_Rotation = _QuatIn;
	}

	void soRotator::operator=(const soVector4D& _VectorIn)
	{
		m_Rotation = soQuaternion(_VectorIn.X, _VectorIn.Y, _VectorIn.Z, _VectorIn.W);
	}
}
