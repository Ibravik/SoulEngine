/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soActor.h
* @Author Lehi Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Declaration of base class soComponent. Un componente representa
		  una funcionalidad especifica en un actor.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulEngineDataStruct.h "

namespace SoulSDK
{	
	/************************************************************************/
	/* Declaracion de la clase soComponent								    */
	/************************************************************************/
	class soComponent : public soStandardClass<const ComponentData&>
	{
		/********************************************************************/
		/* Constructores y destructor							    		*/
		/********************************************************************/
	public:
		soComponent();													/*!< Constructor Standard */
		virtual ~soComponent();											/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		bool m_Update;													/*!< Indica si el componente se actualiza o no */
		bool m_Visible;													/*!< Indica si el componente se renderea o no (principalmente bilboards para indicar su tipo en modo editor) */
		uint64 m_ComponentType;											/*!< Guarda el tipo de componente */
		soTransform m_ComponentTransform;								/*!< Transformacion del componente (En algunos casos no aplica una transformacion completa al componente) */
		soString m_ComponentName;										/*!< Nombre del componente */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	protected:
		virtual RESULT OnStartUp(const ComponentData& _ComponentData);	/*!< Inicializa los parametros del componente */ 
		virtual void OnShutDown();										/*!< Destruye los parametros del componente  */

	public:		
		virtual void Update(float _DeltaTime);							/*!< Actualiza al Componente */
		virtual void Render();											/*!< Renderea el Componente */
	};
}

