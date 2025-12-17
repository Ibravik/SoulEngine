/************************************************************************/
/**
* @LC	  11/21/2016
* @file   soSkeletalModelComponent.h
* @Author Lehi Munoz (lehi.munoz@gmail.com)
* @date   9/29/2016
* @brief  Declaration of base class soSkeletalModelComponent. Esta clase 
	      gestiona por separado la animacion en ejecucion asi como el tiempo
		  acumulativo para la misma.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soSkeletalModel.h"
#include "soComponent.h"

namespace SoulSDK
{	
	/************************************************************************/
	/* Declaracion de la clase soSkeletalModelComponent					    */
	/************************************************************************/
	class soSkeletalModelComponent : public soComponent
	{
		/********************************************************************/
		/* Constructores y destructor							    		*/
		/********************************************************************/
	public:
		soSkeletalModelComponent();
		virtual ~soSkeletalModelComponent();

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		Animation m_NextAnimation;										/*!< Puntero a la siguiente animacion */

	public:
		SET_AND_GET(soSkeletalModel*, Model)							/*!< Puntero al skeletal model proveido por el soResourceManager */
		AnimationData m_AnimationData;									/*!< Informacion de entrada y salidade la animacion */
		SkeletalCB m_SkeletalCB;										/*!< Constant Buffer para el preproceso de calculo de matrices de animacion */
		bool m_InFrustum;												/*!< Identifica si el modelo esta dentro del frustrum. si no es asi, no se actualizan sus matrices */
		std::vector<uint32> m_MaterialID;								/*!< ID de los materiales asignados al modelo de este componente */
					
		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		virtual RESULT OnStartUp(const ComponentData& _ComponentData);	/*!< Inicializa los parametros del componente */
		virtual void OnShutDown();										/*!< Destruye los parametros del componente */
		void SetFadeTime();												/*!< Asigna un tiempo de fade */
		void SetBlendAnimationSettings();								/*!< Asigna los valores para un blend */

	public:
		virtual void Update(float _DeltaTime) override;					/*!< Actualiza al Componente */
		void ChangeModel(soString ResourcePath);						/*!< Realiza un cambio de modelo segun la ruta especificada */
		void Render(uint32 _MatProp);									/*!< Renderea el Componente, los materiales asignados son los unicos que ingresan al shader */
		void PlayAnimation(soString _AnimationName);					/*!< Setea la animacion activa */
		void AddAnimation(soString& _FilePath);							/*!< Carga animaciones para el modelo de el mismo o algun otro archivo */
		soString GetAnimationName(uint32 _Index);						/*!< Retorna el nombre */
	};
}

