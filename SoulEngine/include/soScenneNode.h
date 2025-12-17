/********************************************************************/
/**
* LC	  11/21/2016
* @file   soScenneNode.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/13/2015
* @brief  Implementacion de la clase soScenneNode. Un nodo en la escena
		  representa cualquier actor dentro de esta. Cada nodo puede 
		  estar ligado a un nodo padre, mientras puede tener una
		  pluralidad de conecciones. Es importante recordar que
		  la transformacion de cada nodo esta intimamente relacionada
		  al nodo padre por medio de la matris transformacion.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "Soul_Core.h"

namespace SoulSDK
{
	/************************************************************************/
	/* class forward declaration                                            */
	/************************************************************************/
	class soActor;

	/************************************************************************/
	/* Declaracion de la clase soScenneNode								    */
	/************************************************************************/
	class soScenneNode : public soNode
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soScenneNode();									/*!< Constructor Standard */
		virtual ~soScenneNode();						/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		soActor* m_ptrActor;							/*!< Referencia al actor afiliado al nodo. el nodo solo puede contener un solo actor */
		bool m_InFrustum;								/*!< Indica si el nodo esta dentro del frustum */
		soScenneNode* m_Parent;							/*!< Referencia al nodo al que estoy emparentados */
		
		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:		
		void RemoveChild(soScenneNode* _Child);			/*!< Remueve un nodo hijo de la lista */
		void RemoveParent();							/*!< Asigna un padre al nodo */
		soMatrix4x4 GetParentTransform();				/*!< Toma la matriz transformacion del padre */
		soMatrix4x4 GetTransform();						/*!< Toma la Matriz transformacion del nodo */
		soMatrix4x4 GetFinalNodeTransform();			/*!< Toma la matris transformacion final del nodo */
		void Culling();									/*!< Genera una lista de nodos con actores dentro del frustum de la camara activa */
		RESULT OnStartUp();								/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;				/*!< Libera los recursos solicitados por la clase para su iniciacion */

	public:		
		void SetActor(soActor* _ptrActor);				/*!< Asigna un actor al nodo */		
		bool CheckMaterialProperty(uint8 _MaterialProp);/*!< Verifica si el actor contiene un modelo asociado y con este alguna propiedad especifica del material */
		void AddChild(soScenneNode* _Child);			/*!< Ingresa un nuevo nodo hijo */	
		void SetParent(soScenneNode* _Parent);			/*!< Asigna un padre al nodo */
		virtual void Visit(soNode* _prev);				/*!< Visita el nodo, actualizando las matrices  */
	};
}
