/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soActor.h
* @Author Lehi Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Declaration of base class soActor. Se considera un actor a todo
		  objeto en la escena. Puede contener componentes que le dan
		  diferente funcionalidad.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulEngineDataStruct.h"
#include "soComponent.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soActor									    */
	/************************************************************************/
	class soActor : public soStandardClass<>
	{
		/********************************************************************/
		/* Constructores y destructor							    		*/
		/********************************************************************/
	public:
		soActor();															/*!< Constructor Standard */
		virtual ~soActor();													/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:		
		uint64 m_ComponentFlag;												/*!< Bandera del tipo de componentes que constituyen al actor */
		
	public:
		SET_AND_GET(uint32, ActorID)										/*!< ID asignado por el mundo para el actor (unico e irrepetible) */
		soString m_ActorName;												/*!< Nombre del actor */
		uint32 m_TotalComponentNumber;										/*!< Numero total de componentes en el actor */
		soTransform m_ActorTransform;										/*!< Componente transform del actor */
		std::vector<soComponent*> m_Components;								/*!< Lista de componentes */
		
	private:
		//soState m_State; //TO DO: Checar si sirve con la FSM

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();													/*!< Inicializa los parametros del actor */
		virtual void OnShutDown() override;									/*!< Destruye los parametros del actor */

	public:
		void AddNewComponent(soComponent* _Component);						/*!< Agrega un nuevo componente	*/	
		void RemoveComponentByID(uint32 _PosInList);						/*!< Remueve el componente en el lugar designado */
		std::vector<soComponent*> GetComponentbyType(uint64 _ComponentFlag);/*!< Retorna una lista de componentes del tipo dado */
		virtual void Update(float _DeltaTime);								/*!< Actualiza al actor y sus componentes */
		virtual void Render();												/*!< Renderea al actor y sus componentes */
	};
}

