/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soPlane.cpp
* @Author Aldo Muñoz & Raul Portillo (lehi.munoz@gmail.com)
* @date   10/14/2015
* @brief  Implementacion de la clase soPlane
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
#include "soPlane.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soPlane                                       */
	/************************************************************************/
	soPlane::soPlane()
	{
		m_W = 0.0f;

		m_Normal.X = 0.0f;
		m_Normal.Y = 0.0f;
		m_Normal.Z = 0.0f;
	}

	soPlane::soPlane(const soVector3D& _Normal, const float& _Wvalue)
	{
		m_W = _Wvalue;

		m_Normal = _Normal;
	}

	soPlane::soPlane(const soPlane& _Plane)
	{
		m_W = _Plane.m_W;

		m_Normal.X = _Plane.m_Normal.X;
		m_Normal.Y = _Plane.m_Normal.Y;
		m_Normal.Z = _Plane.m_Normal.Z;
	}

	soPlane::~soPlane()
	{

	}

	bool soPlane::Equal(const soPlane & _Plane, float _Presicion) const
	{
		if (m_Normal.Equal(_Plane.m_Normal, _Presicion) &&
			soMath::Abs(m_W - _Plane.m_W) < _Presicion)
		{
			return true;
		}
		return false;
	}

	bool soPlane::Equal(soPlane & _Plane, float _Presicion) const
	{
		if (m_Normal.Equal(_Plane.m_Normal, _Presicion) &&
			soMath::Abs(m_W - _Plane.m_W) < _Presicion)
		{
			return true;
		}
		return false;
	}

	bool soPlane::IsCollition(soPhysicObject* _PhysicObject)
	{
		soAABB* AABB = dynamic_cast<soAABB*>(_PhysicObject);
		if (AABB != NULL)
		{
			return soMath::IntersectPlanetoAABB(*this, *AABB);
		}

		soPlane* Plane = dynamic_cast<soPlane*>(_PhysicObject);
		if (Plane != NULL)
		{
			return soMath::IntersectPlanetoPlane(*this, *Plane);
		}

		soRay* Ray = dynamic_cast<soRay*>(_PhysicObject);
		if (Ray != NULL)
		{
			return soMath::IntersectPlanetoRay(*this, *Ray);
		}

		soSphere* Sphere = dynamic_cast<soSphere*>(_PhysicObject);
		if (Sphere != NULL)
		{
			return soMath::IntersectPlanetoSphere(*this, *Sphere);
		}

		return false;
	}

	bool soPlane::operator== (const soPlane& _Plane)
	{
		return (m_Normal == _Plane.m_Normal ? true : false);
	}

	bool soPlane::operator!= (const soPlane& _Plane)
	{
		return (m_Normal != _Plane.m_Normal ? true : false);
	}
}
