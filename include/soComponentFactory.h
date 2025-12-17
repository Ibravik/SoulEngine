/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soWorldManager.h
* @Author Lehi Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Declaration of base class soComponentFactory
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
	class soComponentFactory : public soStandardClass<>
	{
		/********************************************************************/
		/* Constructores y destructor							    		*/
		/********************************************************************/
	public:																	/*!< Constructor Standard */
		soComponentFactory();												/*!< Destructor */
		~soComponentFactory();

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();													/*!< Inicializa los parametros de la fabrica  */
		virtual void OnShutDown() override;									/*!< Destruye los parametros de la fabrica */

	public:
		soComponent* CreateComponent(const ComponentData& _ComponentData);	/*!< Crea un nuevo componente completamente nuevo */
		soComponent* LoadComponent(soString& _AssetFileName);				/*!< Carga un componente preajustado desde un asset */
	};
}

