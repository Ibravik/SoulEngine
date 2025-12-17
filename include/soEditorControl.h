/************************************************************************/
/**
* @LC	  11/21/2016
* @file   soEditorControl.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/21/2016
* @brief  Declaration of class soEditorControl. Este controlador solo 
		  aplica en modo editor. Es la clase encargada de mostrar toda la 
		  informacion contenida en un actor, como transform, componentes,
		  etc.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "Soul_Core.h"
#include "soSceneNode.h"
#include "soStaticModelComponent.h"
#include "soSkeletalModelComponent.h"

namespace SoulSDK
{	
	/************************************************************************/
	/* Declaración de la clase base soEditorPositionControl                 */
	/************************************************************************/
	class soEditorControl : public soStandardClass<>
	{
		/************************************************************************************************************************/
		/* Constructores y destructor																							*/
		/************************************************************************************************************************/
	public:
		soEditorControl();										/*!< Constructor Standar */
		~soEditorControl();										/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:		
		std::vector<soSceneNode*> m_SphereTouced;				/*!< Lista de nodos con los que intersecta un rayo de seleccion */

	public:
		//Info general del nodo
		soSceneNode* m_SelectedNode;							/*!< Puntero al nodo seleccionado */

		//Info de edicion
		soStaticModelComponent* m_StaticModel;					/*!< Modelo estatico seleccionado */
		soSkeletalModelComponent* m_SkeletalModel;				/*!< Modelo skeletal seleccionado */
		int32 m_ActualSelectedMesh;								/*!< Mesh de seleccion actual */
		int32 m_ActualAnimation;								/*!< Animacion actual */
		int32 m_ActualSelectedComponent;						/*!< Componente seleccionado */
		bool m_EditMaterial;									/*!< Indica si la edicion de materiales esta activa */
		soComponentFactory m_ComponentFactory;					/*!< Fabrica de componentes para el actor */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();										/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown() override;						/*!< Libera los recursos solicitados por la clase para su iniciacion */
		void HitSelectorSphere(soRay& _Ray);					/*!< Crea una lista de esferas impactadas por un rayo de seleccion */
		soSceneNode* HitMesh(soRay& _Ray);						/*!< Verifica si el rayo de seleccion toca algun mesh de los nodos itersectados */		

	public:
		void ResetEditorData();									/*!< Resetea la informacion para la edicion */
		void SelectNodeModel(soRay _Ray);						/*!< Verifica si un nodo con componente de modelo es seleccionado */
		void SetSelectedNode(soSceneNode* _SelectedNode);		/*!< Setea un nodo seleccionado por medio de GUI */
		void AddComponentToSelectedActor(ComponentData & _Data);/*!< Añade un componente al actor seleccionado */
		void RemoveComponentToSelectedActor();					/*!< Añade un componente al actor seleccionado */
	};

}