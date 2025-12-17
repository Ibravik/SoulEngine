/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soAABB.cpp
* @Author Aldo Muñoz & Raul Portillo (lehi.munoz@gmail.com)
* @date   10/14/2015
* @brief  Implementacion de la clase soAABB
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"		/*!< Funciones matematicas basicas optimisadas para Windows */
#endif
#include "soPlatformMath.h"
#include "soAABB.h"

/************************************************************************/
/* Definicion de la clase soAABB                                        */
/************************************************************************/
namespace SoulSDK
{
	soAABB::soAABB()
	{
		m_AABBCenter = soVector3D(0,0,0);
		m_AABBMax = soVector3D(0.5f, 0.5f, 0.5f);
		m_AABBMin = soVector3D(-0.5f, -0.5f, -0.5f);
	}

	soAABB::soAABB(const soVector3D& _Center, const soVector3D& _AABBMax, const soVector3D& _AABBMin)
	{
		m_AABBCenter = _Center;

		//Calculamos el punto minimo y maximo de la caja
		m_AABBMax = _AABBMax;
		m_AABBMin = _AABBMin;
	}

	soAABB::soAABB(const soAABB& _AABB)
	{
		m_AABBCenter = _AABB.m_AABBCenter;
		m_AABBMax = _AABB.m_AABBMax;
		m_AABBMin = _AABB.m_AABBMin;
	}

	soAABB::~soAABB()
	{

	}

	bool soAABB::Equal(const soAABB & _AABB, float _Precision) const
	{
		if (m_AABBCenter.Equal(_AABB.m_AABBCenter, _Precision) &&
			m_AABBMax.Equal(_AABB.m_AABBMax, _Precision) &&
			m_AABBMin.Equal(_AABB.m_AABBMax, _Precision))
		{
			return true;
		}
		return false;
	}

	bool soAABB::Equal(soAABB & _AABB, float _Precision) const
	{
		if (m_AABBCenter.Equal(_AABB.m_AABBCenter, _Precision) &&
			m_AABBMax.Equal(_AABB.m_AABBMax, _Precision) &&
			m_AABBMin.Equal(_AABB.m_AABBMax, _Precision))
		{
			return true;
		}
		return false;
	}

	bool soAABB::IsCollition(soPhysicObject* _PhysicObject)
	{
		soAABB* AABB = dynamic_cast<soAABB*>(_PhysicObject);
		if (AABB != NULL)
		{
			return soMath::IntersectAABBtoAABB(*this, *AABB);
		}

		soPlane* Plane = dynamic_cast<soPlane*>(_PhysicObject);
		if (Plane != NULL)
		{
			return soMath::IntersectAABBtoPlane(*this, *Plane);
		}

		soRay* Ray = dynamic_cast<soRay*>(_PhysicObject);
		if (Ray != NULL)
		{
			return soMath::IntersectAABBtoRay(*this, *Ray);
		}

		soSphere* Sphere = dynamic_cast<soSphere*>(_PhysicObject);
		if (Sphere != NULL)
		{
			return soMath::IntersectAABBtoSphere(*this, *Sphere);
		}

		return false;
	}

	bool soAABB::operator== (const soAABB& _AABB)
	{
		return (m_AABBCenter == _AABB.m_AABBCenter && m_AABBMax == _AABB.m_AABBMax && m_AABBMin == _AABB.m_AABBMin ? true : false);
	}

	bool soAABB::operator!= (const soAABB& _AABB)
	{
		return (m_AABBCenter != _AABB.m_AABBCenter || m_AABBMax != _AABB.m_AABBMax || m_AABBMin != _AABB.m_AABBMin ? true : false);
	}
}
