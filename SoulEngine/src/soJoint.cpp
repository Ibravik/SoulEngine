/********************************************************************/
/**
* @LC	  10/9/2016
* @file   soJoint.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/22/2016
* @brief  Implementacion de la clase soJoint
* @bug	  No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soJoint.h"

/************************************************************************/
/* Definicion de la clase soJoint						                */
/************************************************************************/
namespace SoulSDK
{
	soJoint::soJoint()
	{
		Connect(std::bind(&soJoint::OnStartUp, this));
		m_ParentTransform = soMath::IdentityMatrix4x4();
		m_NodeTransformation = soMath::IdentityMatrix4x4();
		m_JoinOffsetMatrix = soMath::IdentityMatrix4x4();
	}

	soJoint::~soJoint()
	{
		ShutDown();
	}

	RESULT soJoint::OnStartUp()
	{
		return EC_OK;
	}

	void soJoint::OnShutDown()
	{
		SAFE_DELETE_STDVECTOR(m_conexions);
	}

	void soJoint::SetOffsetMatrix(aiMatrix4x4& _aiMatrix4x4)
	{
		m_JoinOffsetMatrix.m00 = _aiMatrix4x4.a1; m_JoinOffsetMatrix.m10 = _aiMatrix4x4.b1; m_JoinOffsetMatrix.m20 = _aiMatrix4x4.c1; m_JoinOffsetMatrix.m30 = _aiMatrix4x4.d1;
		m_JoinOffsetMatrix.m01 = _aiMatrix4x4.a2; m_JoinOffsetMatrix.m11 = _aiMatrix4x4.b2; m_JoinOffsetMatrix.m21 = _aiMatrix4x4.c2; m_JoinOffsetMatrix.m31 = _aiMatrix4x4.d2;
		m_JoinOffsetMatrix.m02 = _aiMatrix4x4.a3; m_JoinOffsetMatrix.m12 = _aiMatrix4x4.b3; m_JoinOffsetMatrix.m22 = _aiMatrix4x4.c3; m_JoinOffsetMatrix.m32 = _aiMatrix4x4.d3;
		m_JoinOffsetMatrix.m03 = _aiMatrix4x4.a4; m_JoinOffsetMatrix.m13 = _aiMatrix4x4.b4; m_JoinOffsetMatrix.m23 = _aiMatrix4x4.c4; m_JoinOffsetMatrix.m33 = _aiMatrix4x4.d4;
	}
}