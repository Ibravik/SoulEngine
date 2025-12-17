#include "soPhysicsManager.h"
#include "Soul_Math.h"
#include "soWorldManager.h"
#include "soComponent.h"
#include "soPhysicComponent.h"


namespace SoulSDK
{
	soPhysicsManager::soPhysicsManager()
	{
		Connect(std::bind(&soPhysicsManager::OnStartUp, this));
		m_pFoundation = NULL;
		m_pProfileZoneManager = NULL;
		m_pPhysics = NULL;
		m_pCooking = NULL;
		m_bRecordMemoryAllocations = false;
		m_bIsPlay = false;
	}


	soPhysicsManager::~soPhysicsManager()
	{
	}
	RESULT soPhysicsManager::OnStartUp()
	{
		//Creates Foundation
		m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_defaultAllocatorCallback, m_defaultErrorCallback);
		if (!m_pFoundation)
		{
			OutputDebugString(L"Fallo inicialización de Foundation");
		}

		//Creates Profile Zone Manager
		m_bRecordMemoryAllocations = true;

		m_pProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(m_pFoundation);
		if (!m_pProfileZoneManager)
		{
			OutputDebugString(L"Fallo inicializacion de Profile Zone Manager");
		}

		//Creates Physics
		m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxTolerancesScale(), m_bRecordMemoryAllocations, m_pProfileZoneManager);
		if (!m_pPhysics)
		{
			OutputDebugString(L"Fallo inicialización de Physics");
		}

		//Creates Cooking
		physx::PxTolerancesScale scale;
		m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxCookingParams(scale));
		if (!m_pCooking)
		{
			OutputDebugString(L"Fallo inicializacion de Cooking");
		}

		//Create Extensions
		if (!PxInitExtensions(*m_pPhysics))
		{
			OutputDebugString(L"Fallo inicializacion de Extensions");
		}

		//Set up default material
		m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.1f);
		if (!m_pMaterial)
		{
			OutputDebugString(L"Fallo creacion de material");
		}

		return EC_OK;
	}
	void soPhysicsManager::OnShutDown()
	{
		m_componentList.clear();
		m_CPUDispacher->release();
		m_pScene->release();
		m_pMaterial->release();
		m_pCooking->release();
		m_pPhysics->release();
		m_pProfileZoneManager->release();
		m_pFoundation->release();
	}
	
	void soPhysicsManager::CreateActor(PhysXData* _data)
	{
		physx::PxMat44 physMat;

		physMat.column0.x = _data->actor->m_ActorTransform.m_Transform.GetColum(0).X;
		physMat.column0.y = _data->actor->m_ActorTransform.m_Transform.GetColum(0).Y;
		physMat.column0.z = _data->actor->m_ActorTransform.m_Transform.GetColum(0).Z;
		physMat.column0.w = _data->actor->m_ActorTransform.m_Transform.GetColum(0).W;
						
		physMat.column1.x = _data->actor->m_ActorTransform.m_Transform.GetColum(1).X;
		physMat.column1.y = _data->actor->m_ActorTransform.m_Transform.GetColum(1).Y;
		physMat.column1.z = _data->actor->m_ActorTransform.m_Transform.GetColum(1).Z;
		physMat.column1.w = _data->actor->m_ActorTransform.m_Transform.GetColum(1).W;
						
		physMat.column2.x = _data->actor->m_ActorTransform.m_Transform.GetColum(2).X;
		physMat.column2.y = _data->actor->m_ActorTransform.m_Transform.GetColum(2).Y;
		physMat.column2.z = _data->actor->m_ActorTransform.m_Transform.GetColum(2).Z;
		physMat.column2.w = _data->actor->m_ActorTransform.m_Transform.GetColum(2).W;
					
		physMat.column3.x = _data->actor->m_ActorTransform.m_Transform.GetColum(3).X;
		physMat.column3.y = _data->actor->m_ActorTransform.m_Transform.GetColum(3).Y;
		physMat.column3.z = _data->actor->m_ActorTransform.m_Transform.GetColum(3).Z;
		physMat.column3.w = _data->actor->m_ActorTransform.m_Transform.GetColum(3).W;

		switch (_data->type)
		{
		case SoulSDK::PTF_RIGID_STATIC:
			m_pScene->addActor(*CreateRigidStaticActor(physMat, static_cast<PhysicGeometryFlag>(_data->geometry), _data->actor));
			break;
		case SoulSDK::PTF_RIGID_DYNAMIC:
			m_pScene->addActor(*CreateRigidDynamicActor(physMat, static_cast<PhysicGeometryFlag>(_data->geometry), _data->actor));
			break;
		case SoulSDK::PTF_PARTICLE_SYSTEM:
			m_pScene->addActor(*CreateParticleSystem());
			break;
		case SoulSDK::PTF_PARTICLE_FLUID:
			break;
		case SoulSDK::PTF_CLOTH:
			break;
		default:
			break;
		}
	}

	void soPhysicsManager::CreateActors()
	{
		for (uint32 i = 0; i < m_componentList.size(); i++)
		{
			CreateActor(m_componentList[i]);
		}
	}

	void soPhysicsManager::AddComponet(PhysXData* _data)
	{
		m_componentList.push_back(_data);
	}

	void soPhysicsManager::RemoveActor(soActor* _actor)
	{
		for (uint32 i = 0; i < m_componentList.size(); i++)
		{
			if (m_componentList[i]->actor->GetActorID() == _actor->GetActorID())
			{
				m_componentList.erase(m_componentList.begin() + i);
			}
		}
	}

	physx::PxRigidDynamic * soPhysicsManager::CreateRigidDynamicActor(physx::PxMat44 _transform, PhysicGeometryFlag _type, soActor* _actor)
	{
		physx::PxRigidDynamic* tempActor = m_pPhysics->createRigidDynamic(physx::PxTransform(_transform));
		physx::PxShape* tempShape = NULL;
		std::vector<soComponent*> tempComponentList;


		//TO-DO: falta hacer que la geometria concuerde con el debug del SoulEngine 
		switch (_type)
		{
		case SoulSDK::PGF_CAPSULE:
			tempComponentList = _actor->GetComponentbyType(CT_Physic);
			for (uint32 i = 0; i < tempComponentList.size(); i++)
			{
				soPhysicComponent* component = static_cast<soPhysicComponent*>(tempComponentList[i]);
				tempShape = tempActor->createShape(physx::PxCapsuleGeometry(component->GetCapsuleRadio(), 0.01f), *m_pMaterial); //NOTA: Que el Debug trabaje con el Height de la capsula 
				//tempActor->setMass(10.0f); //Asignación de masa
			}
			break;
		case SoulSDK::PGF_BOX:
			tempComponentList = _actor->GetComponentbyType(CT_Physic);
			for (uint32 i = 0; i < tempComponentList.size(); i++)
			{
				soPhysicComponent* component = static_cast<soPhysicComponent*>(tempComponentList[i]);
				//(Max - min) / 2 -> para los half
				tempShape = tempActor->createShape(physx::PxBoxGeometry((component->GetAABB().m_AABBMax.X - component->GetAABB().m_AABBMin.X) / 2, (component->GetAABB().m_AABBMax.Y - component->GetAABB().m_AABBMin.Y) / 2, (component->GetAABB().m_AABBMax.Z - component->GetAABB().m_AABBMin.Z) / 2), *m_pMaterial);
			}
			break;
		case SoulSDK::PGF_PLANE:
			break;
		default:
			break;
		}

		physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
		tempShape->setLocalPose(relativePose);

		//asignamos al physActor el world actor
		tempActor->userData = _actor;

		return tempActor;
	}

	physx::PxRigidStatic * soPhysicsManager::CreateRigidStaticActor(physx::PxMat44 _transform, PhysicGeometryFlag _type, soActor* _actor)
	{
		physx::PxRigidStatic* tempActor = m_pPhysics->createRigidStatic(physx::PxTransform(_transform));
		physx::PxShape* tempShape = NULL;
		std::vector<soComponent*> tempComponentList;

		switch (_type)
		{
		case SoulSDK::PGF_CAPSULE:
			tempComponentList = _actor->GetComponentbyType(CT_Physic);
			for (uint32 i = 0; i < tempComponentList.size(); i++)
			{
				soPhysicComponent* component = static_cast<soPhysicComponent*>(tempComponentList[i]);
				tempShape = tempActor->createShape(physx::PxCapsuleGeometry(component->GetCapsuleRadio(), 0.01f), *m_pMaterial);
			}
			break;
		case SoulSDK::PGF_BOX:
			tempComponentList = _actor->GetComponentbyType(CT_Physic);
			for (uint32 i = 0; i < tempComponentList.size(); i++)
			{
				soPhysicComponent* component = static_cast<soPhysicComponent*>(tempComponentList[i]);
				//(Max - min) / 2 -> para los half
				tempShape = tempActor->createShape(physx::PxBoxGeometry((component->GetAABB().m_AABBMax.X - component->GetAABB().m_AABBMin.X)/2, (component->GetAABB().m_AABBMax.Y - component->GetAABB().m_AABBMin.Y)/2, (component->GetAABB().m_AABBMax.Z - component->GetAABB().m_AABBMin.Z)/2), *m_pMaterial);
			}
			break;
		case SoulSDK::PGF_PLANE:
			break;
		default:
			break;
		}

		physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
		tempShape->setLocalPose(relativePose);

		//asignamos al physActor el world actor
		tempActor->userData = _actor;

		return tempActor;
	}

	physx::PxParticleSystem * soPhysicsManager::CreateParticleSystem()
	{
		if (m_pPhysics)
		{
			physx::PxParticleSystem* tempParticleSystem;

			//Create particle system
			tempParticleSystem = m_pPhysics->createParticleSystem(50, false);
			if (!tempParticleSystem)
			{
				OutputDebugString(L"Fallo creacion de sistema de particulas");
			}

			return tempParticleSystem;
		}

		return nullptr;
	}

	PX_FORCE_INLINE physx::PxSimulationFilterShader getSampleFilterShader()
	{
		return physx::PxDefaultSimulationFilterShader;
	}

	//TO-DO hacer que la escena reciba la gravedad del usuario
	void soPhysicsManager::CreateScene()
	{
		physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f); //gravedad default

		if (!sceneDesc.filterShader)
			sceneDesc.filterShader = getSampleFilterShader();

		if (!sceneDesc.cpuDispatcher)
		{
			m_CPUDispacher = physx::PxDefaultCpuDispatcherCreate(1);
			if (!m_CPUDispacher)
				OutputDebugString(L"PxDefaultCpuDispatcherCreate failed!");
			sceneDesc.cpuDispatcher = m_CPUDispacher;
		}
		
		//sceneDesc.flags |= PxSceneFlag::eENABLE_TWO_DIRECTIONAL_FRICTION;
		//sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
		//sceneDesc.flags |= PxSceneFlag::eENABLE_ONE_DIRECTIONAL_FRICTION;  
		//sceneDesc.flags |= PxSceneFlag::eADAPTIVE_FORCE;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
		//sceneDesc.flags |= PxSceneFlag::eDISABLE_CONTACT_CACHE;

		//The order in which collide and solve are run in a normal simulation time-step. 
		sceneDesc.simulationOrder = physx::PxSimulationOrder::eCOLLIDE_SOLVE;

		m_pScene = m_pPhysics->createScene(sceneDesc);
		if (!m_pScene)
			OutputDebugString(L"create Scene failed!");

		////Add particle system to scene
		//if (m_pParticleSystem)
		//	m_pScene->addActor(*m_pParticleSystem);

		//physx::PxU32 numofparticles = 50;
		//physx::PxVec3 position = physx::PxVec3(0.0f, 0.0f, 0.0f);
		//physx::PxVec3 velocity = physx::PxVec3(0.0f, 1.0f, 0.0f);

		////fill particle data info
		//m_ParticleData.numParticles = numofparticles;
		//m_ParticleData.indexBuffer = physx::PxStrideIterator<const physx::PxU32>(&numofparticles);
		//m_ParticleData.positionBuffer = physx::PxStrideIterator<const  physx::PxVec3>(&position);
		//m_ParticleData.velocityBuffer = physx::PxStrideIterator<const  physx::PxVec3>(&velocity, 0);

		////send info to particle system
		//if (!m_pParticleSystem->createParticles(m_ParticleData))
		//	OutputDebugString(L"Adding particles to particle system failed");
				
	}
	void soPhysicsManager::SetGravity(soVector3D _gravity)
	{
		m_pScene->setGravity(physx::PxVec3(_gravity.X, _gravity.Y, _gravity.Z));
	}
	void soPhysicsManager::Play()
	{
		if (m_componentList.empty())
			return;

		m_bIsPlay = !m_bIsPlay;

		if (m_bIsPlay)
		{
			CreateActors();
		}
		else
		{
			RemoveActors();
		}
	}
	void soPhysicsManager::Update(float _dt)
	{
		if (m_bIsPlay)
		{
			Simulate(_dt);
			UpdateActors();
		}
	}

	void soPhysicsManager::Render()
	{
		if (!m_componentList.empty())
		{
			for (size_t i = 0; i < m_componentList.size(); i++)
			{
				m_componentList[i]->actor->Render();
			}
		}
	}

	void soPhysicsManager::Simulate(float _dT)
	{
		if (_dT == 0.0f)
			return;

		m_pScene->simulate(_dT);
		m_pScene->fetchResults(true);
	}
	void soPhysicsManager::UpdateActors()
	{
		//obtenemos todos los actores dinamicos
		uint32 numOfDynamicActors = m_pScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);

		if (numOfDynamicActors > 0)
		{
			physx::PxActor** tempPhysActor = new physx::PxActor*[numOfDynamicActors];
			m_pScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC, tempPhysActor, sizeof(physx::PxActor) * numOfDynamicActors, 0);

			//actualizamos la matriz de cada uno
			for (uint32 i = 0; i < numOfDynamicActors; i++)
			{
				const physx::PxRigidDynamic* actualPhysActor = tempPhysActor[i]->is<physx::PxRigidDynamic>();

				physx::PxTransform trans = actualPhysActor->getGlobalPose();

				soTransform finalTrans;
				finalTrans.m_Position.X = trans.p.x;
				finalTrans.m_Position.Y = trans.p.y;
				finalTrans.m_Position.Z = trans.p.z;

				soRotator Rot(soQuaternion(trans.q.x, trans.q.y, trans.q.z, trans.q.w));

				Rot.GetYawPichRoll(finalTrans.m_Rotation.m_Yaw, finalTrans.m_Rotation.m_Pich, finalTrans.m_Rotation.m_Roll);

				soActor* tempActor = static_cast<soActor*>(tempPhysActor[i]->userData);
				tempActor->m_ActorTransform = finalTrans;
			}
		}

	}
	void soPhysicsManager::RemoveActors()
	{
		uint32 numOfDynamicActors = m_pScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);

		if (numOfDynamicActors > 0)
		{
			physx::PxActor** tempDynamicActors = new physx::PxActor*[numOfDynamicActors];
			m_pScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC, tempDynamicActors, sizeof(physx::PxActor) * numOfDynamicActors, 0);

			for (uint32 i = 0; i < numOfDynamicActors; i++)
			{
				m_pScene->removeActor(*tempDynamicActors[i]);
			}

			//m_pScene->removeActors(tempDynamicActors, numOfDynamicActors);
		}

		uint32 numOfStaticActors = m_pScene->getNbActors(physx::PxActorTypeFlag::eRIGID_STATIC);

		if (numOfStaticActors > 0)
		{
			physx::PxActor** tempStaticActors = new physx::PxActor*[numOfStaticActors];
			m_pScene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC, tempStaticActors, sizeof(physx::PxActor) * numOfStaticActors, 0);

			for (uint32 i = 0; i < numOfStaticActors; i++)
			{
				m_pScene->removeActor(*tempStaticActors[i]);
			}

			//m_pScene->removeActors(tempStaticActors, numOfDynamicActors);
		}
	}

	soTransform soPhysicsManager::GetActorTransformByID(uint32 _ID, int32 _type)
	{
		//EL ID debe ser mayor a 0 sino retornamos
		if (_ID == 0)
			return soTransform();

		if (PTF_RIGID_DYNAMIC == _type)
		{
			uint32 numOfActors = m_pScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);

			physx::PxActor** tempActor = new physx::PxActor*[numOfActors];

			m_pScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC, tempActor, sizeof(physx::PxActor) * numOfActors, 0);

			const physx::PxRigidDynamic* actualActor = tempActor[_ID - 1]->is<physx::PxRigidDynamic>(); //-1 ya que Scene maneja los actores desde 0 y el World desde 1

			physx::PxTransform trans = actualActor->getGlobalPose();

			soTransform finalTrans;
			finalTrans.m_Position.X = trans.p.x;
			finalTrans.m_Position.Y = trans.p.y;
			finalTrans.m_Position.Z = trans.p.z;

			soRotator Rot(soQuaternion(trans.q.x, trans.q.y, trans.q.z, trans.q.w));

			Rot.GetYawPichRoll(finalTrans.m_Rotation.m_Yaw, finalTrans.m_Rotation.m_Pich, finalTrans.m_Rotation.m_Roll);

			return finalTrans;
		}
		else
		{
			uint32 numOfActors = m_pScene->getNbActors(physx::PxActorTypeFlag::eRIGID_STATIC);

			physx::PxActor** tempActor = new physx::PxActor*[numOfActors];

			m_pScene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC, tempActor, sizeof(physx::PxActor) * numOfActors, 0);

			const physx::PxRigidStatic* actualActor = tempActor[_ID - 1]->is<physx::PxRigidStatic>(); //-1 ya que Scene maneja los actores desde 0 y el World desde 1

			physx::PxTransform trans = actualActor->getGlobalPose();

			soTransform finalTrans;
			finalTrans.m_Position.X = trans.p.x;
			finalTrans.m_Position.Y = trans.p.y;
			finalTrans.m_Position.Z = trans.p.z;

			soRotator Rot(soQuaternion(trans.q.x, trans.q.y, trans.q.z, trans.q.w));

			Rot.GetYawPichRoll(finalTrans.m_Rotation.m_Yaw, finalTrans.m_Rotation.m_Pich, finalTrans.m_Rotation.m_Roll);

			return finalTrans;
		}

		return soTransform();
	}
}

//Update de fisicas y si el delta es 0 no hay simulacion