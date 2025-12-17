#pragma once
#include "PxPhysicsAPI.h"
#include "soActor.h"

namespace SoulSDK
{
	class soParticleSystem
	{
	public:
		soParticleSystem();
		virtual ~soParticleSystem();

	public:
		uint32 m_maxParticles;
		bool m_perParticleResetOffset;
		int32 m_type;						//Tipo de actor fisico (Particle System)
		soSphere m_sphere;                  //Esfera para saber de donde saldran las particulas

		soVector3D m_position;               //posiscion de la particula (puede que sea un vector para todas)
		soActor* m_actor;


	public:
		void Render();
		void Update(float _dT);
		void Destroy();

	};
}

