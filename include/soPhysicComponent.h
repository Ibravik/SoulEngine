#pragma once

#include "soComponent.h"
#include <Soul_StandardTools.h>
#include "soActor.h"
#include "soPhysicsManager.h"

namespace SoulSDK 
{
	class soPhysicComponent : public soComponent
	{
	public:
		soPhysicComponent();
		virtual ~soPhysicComponent();

	public:
		soActor* m_actor;

		float m_Scale;
		int32 m_geoType;
		int32 m_type;
		float m_mass;
			
	private:		
		soAABB m_box;
		soSphere m_capsule;
		float m_min;
		float m_max;
		float m_radio;
		PhysXData m_data;

	public:
		virtual void Update(float _deltaTime) override;					/*!< Actualiza al Componente */
		void Render();
		float GetCapsuleRadio();
		soAABB GetAABB();
	
	private:
		RESULT OnStartUp(const ComponentData& _ComponentData)override;	/*!< Inicializa los parametros del componente */
		virtual void OnShutDown()override;								/*!< Destruye los parametros del componente */

		void UpdateBox();
		void UpdateCapsule();
	};
}

