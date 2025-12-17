/********************************************************************/
/**
* LC	  10/9/2016
* @file   soSceneGraph.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/12/2016
* @brief  Implementacion de la clase base soSceneGraph.El scene graph
		  es el objeto encargado de hacer culling para toda la informacion
		  de alimentacion para el render manager. 
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soSceneNode.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soSceneGraph				                    */
	/************************************************************************/
	class soSceneGraph : public soStandardClass<>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soSceneGraph();								/*!< Constructor Standard */
		~soSceneGraph();							/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:		
		std::vector<soSceneNode*> m_NodeList;		/*!< Lista general de nodos */
		
	public:
		soSceneNode m_ScenneRoot;					/*!< Nodo inicial del scenne graph */
		std::vector<soSceneNode*> m_CullingNodeList;/*!< Lista de nodos dentro del frustum, que contengan un modelo o algo para ser rendereados */
		std::vector<soSceneNode*> m_CustomNodeList;/*!< Lista de nodos con componentes especificos o propiedades de material especificas */
		uint32 m_RenderedMeshNum;					/*!< Numero de meshes rendereados durante el frame */
		uint32 m_RenderedModelNum;					/*!< Numero de modelos rendereados durante el frame */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();							/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;			/*!< Libera los recursos solicitados por la clase para su iniciacion */
		void CullingByFrustrum();					/*!< Culling actors by frustrum */

	public:
		void Update();								/*!< Actualizacion de scene graph, setea el parent transform y genera la lista de nodos dentro del frustum */
		void GenerateCustomNodeList(uint8 _MatProp);/*!< Genera una lista de nodos con una propiedad de material especifica*/
		void CreateNode(soActor* _ActorNode);		/*!< Ingresa un nuevo nodo a la escena, emparentandolo al nodo eespecificado, o al root en caso de no especificar uno */	
		soSceneNode* FindNodeByActorID(uint32 _ID);/*!< Retorna el nodo que contenga al actor en caso de existir */
		RESULT RemoveNodeByActorID(uint32 _ID);		/*!< Remueve el nodo asignado. Al hacerlo, asiga todos los hijos del nodo al nodo padre de este */
	};
}