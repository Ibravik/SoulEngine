/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soStaticModelComponent.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Declaration of base class soStaticModelComponent. La informacion
		  del componente se gestina por el resource manager.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soComponent.h"
#include "soStaticModel.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soStaticModelComponent					    */
	/************************************************************************/
	class soStaticModelComponent :public soComponent
	{
		/********************************************************************/
		/* Constructores y destructor							    		*/
		/********************************************************************/
	public:
		soStaticModelComponent();										/*!< Constructor Standard */
		virtual ~soStaticModelComponent();								/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		SET_AND_GET(soStaticModel*, Model)								/*!< Informacion del modelo */

	public:
		std::vector<uint32> m_MaterialID;								/*!< ID de los materiales asignados al modelo de este componente */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const ComponentData& _ComponentData)override;	/*!< Inicializa los parametros del componente */
		virtual void OnShutDown()override;								/*!< Destruye los parametros del componente */

	public:
		void ChangeModel(soString ResourcePath);						/*!< Cambia el modelo asignado al componente */
		virtual void Update(float _DeltaTime) override;					/*!< Actualiza al Componente */
		void Render(uint32 _MatProp);									/*!< Renderea el Componente */

	
	};
}

