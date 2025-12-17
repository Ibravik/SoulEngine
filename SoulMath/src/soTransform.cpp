/************************************************************************/
/**
* @LC	  16/09/2016
* @file   soTransform.h
* @Author Lehi Munoz (lehi.munoz@gmail.com)
* @date   8/09/2016
* @brief  Implementacion de soTransform
* @bug	   No known bugs.
*/
/********************************************************************/

/***********************************************************************************/
/* Header files inclusion														   */
/***********************************************************************************/
#if PLATFORM == WINDOWS_MATH
#include "soWindowsMath.h"
#endif
#include "soPlatformMath.h"
#include "soTransform.h"

namespace SoulSDK
{
	soTransform::soTransform()
	{
		m_ParentTransform = soMath::IdentityMatrix4x4();
		m_Transform = soMath::IdentityMatrix4x4();
		m_FinalTransform = soMath::IdentityMatrix4x4();
		m_LastParentTransform = soMath::IdentityMatrix4x4();
		m_LastTransform = soMath::IdentityMatrix4x4();
		m_Scaling = soVector3D(1, 1, 1);
		m_LastScaling = m_Scaling;
		m_CullingSphere.m_fRadius = 1.0f;
	}

	soTransform::~soTransform()
	{

	}

	soSphere soTransform::GetCullingSphere()
	{
		m_CullingSphere.m_SphereCenter = m_FinalTransform.GetColum(3);
		return m_CullingSphere;
	}

	void soTransform::Update()
	{
		bool RemakeFinal = false;

		//Verificacion de rotacion 
		m_Rotation = soQuaternion(m_Rotation.m_Yaw, m_Rotation.m_Pich, m_Rotation.m_Roll);
		if (m_Rotation != m_LastRotation)
		{
			m_LastRotation = m_Rotation;
			RemakeFinal = true;
		}

		//Verificacion de escala 
		if (m_Scaling != m_LastScaling)
		{
			m_LastScaling = m_Scaling;
			RemakeFinal = true;
		}

		//Verificacion de traslacion 
		if (m_Position != m_LastPosition)
		{
			m_LastPosition = m_Position;
			RemakeFinal = true;
		}

		//Verificacion de transformacion padre
		if (m_ParentTransform != m_LastParentTransform)
		{
			m_LastParentTransform = m_ParentTransform;
			RemakeFinal = true;
		}

		//Verificacion de transformacion
		if (m_Transform != m_LastTransform)
		{
			m_LastTransform = m_Transform;
			RemakeFinal = true;
		}

		if (RemakeFinal)
		{
			SetFinalTransform();
		}
	}

	void soTransform::operator=(soTransform& _Transform)
	{
		*this = _Transform;
	}

	void soTransform::SetFinalTransform()
	{
		soMatrix4x4 Rotation;
		m_Rotation.GetRotationMatrix(Rotation);
		m_Transform = soMath::TranslationMatrix4x4(m_Position) * Rotation * soMath::ScalarMatrix4x4(m_Scaling);
		m_FinalTransform = m_ParentTransform * m_Transform;
	}
}

