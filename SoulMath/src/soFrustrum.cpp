/********************************************************************/
/**
* @LC	  3/31/2016
* @file   soFrustrum.cpp
* @Author Aldo Muñoz(lehi.munoz@gmail.com)
* @date   10/14/201eFrustumPlanes::FP_Flor
* @brief  Implementacion de la clase soFrustrum
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
#include "soMatrix4D.h"
#include "soVector3D.h"
#include "soPlane.h"
#include "soAABB.h"
#include "soSphere.h"
#include "soFrustrum.h"

/************************************************************************/
/* Definicion de la clase soFrustrum                                    */
/************************************************************************/
namespace SoulSDK
{
	soFrustrum::soFrustrum()
	{
	}

	soFrustrum::~soFrustrum()
	{
	}

	void soFrustrum::ConstructFrustum(soMatrix4x4& _Projection, soMatrix4x4& _View)
	{
		soMatrix4x4 viewProjection = _View * _Projection;

		// Left plane
		Left.a = viewProjection._14 + viewProjection._11;
		Left.b = viewProjection._24 + viewProjection._21;
		Left.c = viewProjection._34 + viewProjection._31;
		Left.d = viewProjection._44 + viewProjection._41;

		// Right plane
		Right.a = viewProjection._14 - viewProjection._11;
		Right.b = viewProjection._24 - viewProjection._21;
		Right.c = viewProjection._34 - viewProjection._31;
		Right.d = viewProjection._44 - viewProjection._41;

		// Top plane
		Top.a = viewProjection._14 - viewProjection._12;
		Top.b = viewProjection._24 - viewProjection._22;
		Top.c = viewProjection._34 - viewProjection._32;
		Top.d = viewProjection._44 - viewProjection._42;

		// Bottom plane
		Bottom.a = viewProjection._14 + viewProjection._12;
		Bottom.b = viewProjection._24 + viewProjection._22;
		Bottom.c = viewProjection._34 + viewProjection._32;
		Bottom.d = viewProjection._44 + viewProjection._42;

		// Near plane
		Near.a = viewProjection._13;
		Near.b = viewProjection._23;
		Near.c = viewProjection._33;
		Near.d = viewProjection._43;

		// Far plane
		Far.a = viewProjection._14 - viewProjection._13;
		Far.b = viewProjection._24 - viewProjection._23;
		Far.c = viewProjection._34 - viewProjection._33;
		Far.d = viewProjection._44 - viewProjection._43;

		// Normalize planes
		for (int i = 0; i < 6; i++)
		{
			soMath::NormalizeVector(m_FrustumPlanes[i].m_Normal);
		}
	}

	bool soFrustrum::IsPointIn(const soVector3D& _Point)
	{
		int i;

		// Check if the point is inside all six planes of the view frustum.
		for (i = 0; i < 6; i++)
		{
			soVector4D Point = _Point;
			Point.W = 1;
			if (soMath::DotProduct(m_FrustumPlanes[i].m_V4D, Point) < 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	bool soFrustrum::IsAABBIn(const soAABB& _AABB)
	{
		//soVector4D Corner;

		//// Check if any one point of the cube is in the view frustum.
		//for (uint32 i = 0; i < 6; i++)
		//{
		//	Corner = _AABB.m_AABBMin;
		//	Corner.W = 1;
		//	if (soMath::DotProduct(m_planes[i], Corner) >= 0.0f)
		//	{
		//		continue;
		//	}

		//	Corner = soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMax.Z);
		//	Corner.W = 1;
		//	if (soMath::DotProduct(m_planes[i], Corner) >= 0.0f)
		//	{
		//		continue;
		//	}

		//	Corner = soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMin.Z);
		//	Corner.W = 1;
		//	if (soMath::DotProduct(m_planes[i], Corner) >= 0.0f)
		//	{
		//		continue;
		//	}

		//	Corner = soVector3D(_AABB.m_AABBMin.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMax.Z);
		//	Corner.W = 1;
		//	if (soMath::DotProduct(m_planes[i], Corner) >= 0.0f)
		//	{
		//		continue;
		//	}

		//	Corner = _AABB.m_AABBMax;
		//	Corner.W = 1;
		//	if (soMath::DotProduct(m_planes[i], Corner) >= 0.0f)
		//	{
		//		continue;
		//	}

		//	Corner = soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMax.Z);
		//	Corner.W = 1;
		//	if (soMath::DotProduct(m_planes[i], Corner) >= 0.0f)
		//	{
		//		continue;
		//	}

		//	Corner = soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMin.Y, _AABB.m_AABBMin.Z);
		//	Corner.W = 1;
		//	if (soMath::DotProduct(m_planes[i], Corner) >= 0.0f)
		//	{
		//		continue;
		//	}

		//	Corner = soVector3D(_AABB.m_AABBMax.X, _AABB.m_AABBMax.Y, _AABB.m_AABBMin.Z);
		//	Corner.W = 1;
		//	if (soMath::DotProduct(m_planes[i], Corner) >= 0.0f)
		//	{
		//		continue;
		//	}

		//	return false;
		//}

		return true;
	}

	bool  soFrustrum::IsSphereIn(const soSphere& _Sphere)
	{
		for (int i = 0; i < 6; i++)
		{
			if (soMath::DotProduct(m_FrustumPlanes[i].m_V4D, soVector4D(_Sphere.a, _Sphere.b, _Sphere.c, 1.0f)) + _Sphere.m_fRadius < 0)
			{
				// Outside the frustum, reject it!
				return false;
			}
		}
		return true;
	}
}
