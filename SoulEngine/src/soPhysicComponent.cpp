#include "soPhysicComponent.h"
#include "soGraphicDebugManager.h"
#include "soWorldManager.h"

namespace SoulSDK
{
	soPhysicComponent::soPhysicComponent()
	{
		Connect(std::bind(&soPhysicComponent::OnStartUp, this, std::placeholders::_1));
		m_type = PhysicTypeFlag::PTF_RIGID_DYNAMIC; //Es estatico de default
		m_geoType = PhysicGeometryFlag::PGF_CAPSULE;
		m_mass = 10.0f;
		m_radio = 1.0f;
		m_Scale = 1.0f;
		m_min = -1.0f;
		m_max = 1.0f;
		m_actor = NULL;
	}

	soPhysicComponent::~soPhysicComponent()
	{
		ShutDown();
	}
	void soPhysicComponent::Update(float _deltaTime)
	{
		SUPER::Update(_deltaTime);

		switch (m_geoType)
		{
		case SoulSDK::PGF_CAPSULE:
			UpdateCapsule();
			break;
		case SoulSDK::PGF_BOX:
			UpdateBox();
			break;
		case SoulSDK::PGF_PLANE: //Por el momento no hay plano
			break;
		default:
			break;
		}
	}

	void soPhysicComponent::Render()
	{
		switch (m_geoType)
		{
		case SoulSDK::PGF_CAPSULE:
			soGraphicDebugManager::Instance().AddDebugSphere(m_capsule, false, soColorRGBA(255, 0, 0, 0));
			break;
		case SoulSDK::PGF_BOX:
			soGraphicDebugManager::Instance().AddDebugAABB(m_box, false, soColorRGBA(0, 255, 0, 0));
			break;
		case SoulSDK::PGF_PLANE: //Por el momento no hay plano
			break;
		default:
			break;
		}
	}

	float soPhysicComponent::GetCapsuleRadio()
	{
		return m_capsule.m_fRadius;
	}

	soAABB soPhysicComponent::GetAABB()
	{
		return m_box;
	}
	
	void soPhysicComponent::OnShutDown()
	{
		if (soPhysicsManager::IsStarted())
		{
			//Remover el data del componentList de PhysciManager
			soPhysicsManager::Instance().RemoveActor(m_actor);
		}
		
		m_actor = NULL;
	}
	void soPhysicComponent::UpdateCapsule()
	{
		//Falta optimizar para que lo haga solo cuando haya un cambio
		m_capsule.m_SphereCenter = m_actor->m_ActorTransform.m_FinalTransform.GetColum(3);
		m_capsule.m_fRadius = m_radio * m_Scale;
	}

	void soPhysicComponent::UpdateBox()
	{
		//Falta optimizar para que lo haga solo cuando haya un cambio
		m_box.m_AABBCenter = m_actor->m_ActorTransform.m_FinalTransform.GetColum(3);

		m_box.m_AABBMax.X = (m_max * m_Scale) + m_actor->m_ActorTransform.m_FinalTransform.m03;
		m_box.m_AABBMax.Y = (m_max * m_Scale) + m_actor->m_ActorTransform.m_FinalTransform.m13;
		m_box.m_AABBMax.Z = (m_max * m_Scale) + m_actor->m_ActorTransform.m_FinalTransform.m23;

		m_box.m_AABBMin.X = (m_min * m_Scale) + m_actor->m_ActorTransform.m_FinalTransform.m03;
		m_box.m_AABBMin.Y = (m_min * m_Scale) + m_actor->m_ActorTransform.m_FinalTransform.m13;
		m_box.m_AABBMin.Z = (m_min * m_Scale) + m_actor->m_ActorTransform.m_FinalTransform.m23;
	}
	RESULT soPhysicComponent::OnStartUp(const ComponentData& _ComponentData)
	{
		SUPER::OnStartUp(_ComponentData);

		//Recibo el puntero al world actor
		m_actor = static_cast<soActor*>(_ComponentData.userData);

		m_data.actor = m_actor;
		m_data.geometry = m_geoType;
		m_data.type = m_type;
		m_data.mass = m_mass;

		//Creo el actor fisico con los datos recibidos
		soPhysicsManager::Instance().AddComponet(&m_data);

		return EC_OK;
	}
}