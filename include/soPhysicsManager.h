#pragma once

#include <Soul_StandardTools.h>
#include "PxPhysicsAPI.h"
#include "soActor.h"

namespace SoulSDK
{
	enum PhysicTypeFlag
	{
		PTF_RIGID_STATIC = 0,
		PTF_RIGID_DYNAMIC,
		PTF_PARTICLE_SYSTEM,
		PTF_PARTICLE_FLUID,
		PTF_CLOTH
	};

	enum PhysicGeometryFlag
	{
		PGF_CAPSULE = 0,
		PGF_BOX,
		PGF_PLANE,
		PGF_SPHERE
	};

	struct PhysXData 
	{
		int32 type; 
		int32 geometry;
		float mass;

		soActor* actor;
		
		PhysXData()
		{
			type = 0;
			geometry = 0;
			mass = 0;
			actor = NULL;
		}
	};


	class soPhysicsManager : public soModule<soPhysicsManager>
	{
	public:
		soPhysicsManager();
		virtual ~soPhysicsManager();

	private:
		physx::PxDefaultErrorCallback m_defaultErrorCallback;
		physx::PxDefaultAllocator m_defaultAllocatorCallback;
		physx::PxFoundation* m_pFoundation;
		physx::PxProfileZoneManager* m_pProfileZoneManager;
		physx::PxPhysics* m_pPhysics;
		physx::PxCooking* m_pCooking;
		physx::PxMaterial* m_pMaterial;
		physx::PxScene* m_pScene;
		physx::PxDefaultCpuDispatcher* m_CPUDispacher;

		std::vector<PhysXData*> m_componentList;
		std::vector<physx::PxParticleSystem*> m_particleSystemList;

		bool m_bRecordMemoryAllocations;
		bool m_bIsPlay;

		physx::PxParticleCreationData m_ParticleData;
				
	public:
		void AddComponet(PhysXData* _data);
		void RemoveActor(soActor* _actor);
		void CreateScene();
		void SetGravity(soVector3D _gravity);
		void Update(float _dt);
		void Render();
		void Play();

		//Pendiente
		soTransform GetActorTransformByID(uint32 _ID, int32 _type);

	private:
		RESULT OnStartUp();																												/*!< Inherit Module Start Up */
		virtual void OnShutDown() override;																								/*!< Inherit Module Shut Down */
		
		physx::PxRigidDynamic* CreateRigidDynamicActor(physx::PxMat44 _transform, PhysicGeometryFlag _type, soActor* _actor);
		physx::PxRigidStatic * CreateRigidStaticActor(physx::PxMat44 _transform, PhysicGeometryFlag _type, soActor* _actor);
		physx::PxParticleSystem* CreateParticleSystem();
		
		void Simulate(float _dT);
		void UpdateActors();
		void RemoveActors();
		void CreateActor(PhysXData* _data);
		void CreateActors();
	};
}

