/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soWorldManager.h
* @Author Lehi Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Declaration of base class soWorldManager. El world manager es el 
		  encargado de gestionar toda la informacion de actores y su
		  interaccion.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para compilar                      */
/************************************************************************/
#include "soSceneGraph.h"
#include "soComponentFactory.h"
#include "soActor.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaración de la clase soWorldManager				    	      	*/
	/************************************************************************/
	class soWorldManager : public soModule<soWorldManager>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soWorldManager();									/*!< Constructor Standard */
		~soWorldManager();									/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		uint32 m_CurrentID;									/*!< ID de asignacion para los actores */
		soComponentFactory m_ComponentFactory;				/*!< Fabrica de componentes para el actor */

	public:
		std::vector<soActor*> m_ActorList;					/*!< Lista de actores activos */
		std::vector<soActor*> m_DeletedList;				/*!< Lista de actores para eliminar */
		soSceneGraph m_SceneGraph;							/*!< Grafo de la escena */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();									/*!< Todos los parametros iniciales del manager */
		virtual void OnShutDown() override;					/*!< Destrucion de los parametros del manager */
		
	public:
		RESULT CreateNewActor(const soString& _ActorName);	/*!< Crea un actor utilizando los componentes que la bandera indique */
		RESULT DeleteActor(uint32 _ActorID);				/*!< Crea un actor utilizando los componentes que la bandera indique */
		RESULT LoadActor(const soString& _AssetFileName);	/*!< Crea un actor a partir de un asset */
		soActor* GetActorByName(const soString& _ActorName);/*!< Retorna un actor por nombre */
		soActor* GetActorByID(uint64 _ActorID);				/*!< Retorna un actor por ID */
		void CheckValidActorName(soActor& _Actor);			/*!< Verifica si el nombre existe o no */

		void Update(float _DeltaTime);						/*!< Actualiza el mundo y todos sus elementos */
		void Render();										/*!< Renderea todos los elementos del mundo */
	};
}

