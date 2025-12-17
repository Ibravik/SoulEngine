/********************************************************************/
/**
* @LC	  9/16/2016
* @file   omSpehre.cpp
* @Author Aldo Muñoz & Raul Portillo (lehi.munoz@gmail.com)
* @date   10/14/2015
* @brief  Implementacion de la clase omSpehre
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
#include "soVector3D.h"
#include "soSphere.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soPlane                                       */
	/************************************************************************/
	soSphere::soSphere()
	{
		m_fRadius = 0.0f;

		m_SphereCenter.X = 0.0f;
		m_SphereCenter.Y = 0.0f;
		m_SphereCenter.Z = 0.0f;
	}

	soSphere::soSphere(const soVector3D& _Center, const float& _Radius)
	{
		m_fRadius = _Radius;

		m_SphereCenter.X = _Center.X;
		m_SphereCenter.Y = _Center.Y;
		m_SphereCenter.Z = _Center.Z;
	}

	soSphere::soSphere(const soSphere& _Sphere)
	{
		m_fRadius = _Sphere.m_fRadius;

		m_SphereCenter.X = _Sphere.m_SphereCenter.X;
		m_SphereCenter.Y = _Sphere.m_SphereCenter.Y;
		m_SphereCenter.Z = _Sphere.m_SphereCenter.Z;
	}

	soSphere::~soSphere()
	{

	}

	bool soSphere::Equal(const soSphere & _Sphere, float _Precision) const
	{
		if (m_SphereCenter.Equal(_Sphere.m_SphereCenter, _Precision) &&
			soMath::Abs(m_fRadius - _Sphere.m_fRadius) < _Precision)
		{
			return true;
		}
		return false;
	}

	bool soSphere::Equal(soSphere & _Sphere, float _Precision) const
	{
		if (m_SphereCenter.Equal(_Sphere.m_SphereCenter, _Precision) &&
			soMath::Abs(m_fRadius - _Sphere.m_fRadius) < _Precision)
		{
			return true;
		}
		return false;
	}

	bool soSphere::IsCollition(soPhysicObject* _PhysicObject)
	{
		soAABB* AABB = dynamic_cast<soAABB*>(_PhysicObject);
		if (AABB != NULL)
		{
			return soMath::IntersectSpheretoAABB(*this, *AABB);
		}

		soPlane* Plane = dynamic_cast<soPlane*>(_PhysicObject);
		if (Plane != NULL)
		{
			return soMath::IntersectSpheretoPlane(*this, *Plane);
		}

		soRay* Ray = dynamic_cast<soRay*>(_PhysicObject);
		if (Ray != NULL)
		{
			return soMath::IntersectSpheretoRay(*this, *Ray);
		}

		soSphere* Sphere = dynamic_cast<soSphere*>(_PhysicObject);
		if (Sphere != NULL)
		{
			return soMath::IntersectSpheretoSphere(*this, *Sphere);
		}

		return false;
	}

	void soSphere::operator=(soSphere& _Sphere)
	{
		m_V4D = _Sphere.m_V4D;
	}

	bool soSphere::operator== (const soSphere& _Sphere)
	{
		return (m_SphereCenter == _Sphere.m_SphereCenter && m_fRadius == _Sphere.m_fRadius ? true : false);
	}

	bool soSphere::operator!= (const soSphere& _Sphere)
	{
		return (m_SphereCenter != _Sphere.m_SphereCenter || m_fRadius != _Sphere.m_fRadius ? true : false);
	}
}

