/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soRay.cpp
* @Author Aldo Muñoz & Raul Portillo (lehi.munoz@gmail.com)
* @date   10/14/2015
* @brief  Implementacion de la clase soRay
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
#include "soRay.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soRay				                            */
	/************************************************************************/
	soRay::soRay()
	{
		m_VecOrigin.X = 0.0f;
		m_VecOrigin.Y = 0.0f;
		m_VecOrigin.Z = 0.0f;

		m_VecDirection.X = 0.0f;
		m_VecDirection.Y = 0.0f;
		m_VecDirection.Z = 0.0f;
	}

	soRay::soRay(const soVector3D& _Origin, const soVector3D& _Direction)
	{
		m_VecOrigin = _Origin;
		m_VecDirection = _Direction;
	}

	soRay::soRay(const soRay& _Ray)
	{
		m_VecOrigin = _Ray.m_VecOrigin;
		m_VecDirection = _Ray.m_VecDirection;
	}

	soRay::~soRay()
	{

	}

	bool soRay::Equal(const soRay & _Ray, float _Presicion) const
	{
		if (m_VecDirection.Equal(_Ray.m_VecDirection, _Presicion) &&
			m_VecOrigin.Equal(_Ray.m_VecOrigin, _Presicion))
		{
			return true;
		}
		return false;
	}

	bool soRay::Equal(soRay & _Ray, float _Presicion) const
	{
		if (m_VecDirection.Equal(_Ray.m_VecDirection, _Presicion) &&
			m_VecOrigin.Equal(_Ray.m_VecOrigin, _Presicion))
		{
			return true;
		}
		return false;
	}

	bool soRay::IsCollition(soPhysicObject* _PhysicObject)
	{
		soAABB* AABB = dynamic_cast<soAABB*>(_PhysicObject);
		if (AABB != NULL)
		{
			return soMath::IntersectRaytoAABB(*this, *AABB);
		}

		soPlane* Plane = dynamic_cast<soPlane*>(_PhysicObject);
		if (Plane != NULL)
		{
			return soMath::IntersectRaytoPlane(*this, *Plane);
		}

		soSphere* Sphere = dynamic_cast<soSphere*>(_PhysicObject);
		if (Sphere != NULL)
		{
			return soMath::IntersectRaytoSphere(*this, *Sphere);
		}

		return false;
	}

	bool soRay::operator== (const soRay& _Ray)
	{
		return (m_VecDirection == _Ray.m_VecDirection && m_VecOrigin == _Ray.m_VecOrigin ? true : false);
	}

	bool soRay::operator!= (const soRay& _Ray)
	{
		return (m_VecDirection != _Ray.m_VecDirection || m_VecOrigin != _Ray.m_VecOrigin ? true : false);
	}
}
