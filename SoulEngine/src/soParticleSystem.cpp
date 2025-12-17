#include "soParticleSystem.h"
#include "soGraphicDebugManager.h"
#include "soPhysicsManager.h"

namespace SoulSDK
{
	soParticleSystem::soParticleSystem()
	{
		m_type = PhysicTypeFlag::PTF_PARTICLE_SYSTEM;
	}


	soParticleSystem::~soParticleSystem()
	{
	}

	void soParticleSystem::Render()
	{
		soGraphicDebugManager::Instance().AddDebugSphere(m_sphere, false, soColorRGBA(255, 0, 0, 0));
		//soGraphicDebugManager::Instance().AddDebugPoint(soVector3D(0.0f, 0.0f, 0.0f), false, 10.0f, soColorRGBA(255, 0, 0, 0));
	}
	void soParticleSystem::Update(float _dT)
	{
		//Update sphere
		m_sphere.m_SphereCenter = m_actor->m_ActorTransform.m_FinalTransform.GetColum(3);
		m_sphere.m_fRadius = 1.0f;

	}
	void soParticleSystem::Destroy()
	{
		
	}
}
