/************************************************************************/
/**
* @LC	  11/30/2016
* @file   soCameraManager.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/14/2015
* @brief  Declaration of class soCameraManager
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para compilar                      */
/************************************************************************/
#include "soCamera.h"
#include "soEditorCamera.h"
#include "SoulEngineDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soCameraManager                              */
	/************************************************************************/
	class soCameraManager : public soModule<soCameraManager>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soCameraManager();											/*!< Constructor Standard */
		~soCameraManager();											/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		std::map<uint32, soCamera*> m_CameraList;					/*!< Lista de referencias a las camaras creadas en la escena */
		std::map<soString, uint32> m_CameraMap;						/*!< Diccionario con las referencias a las camaras */
	
	public:
		soCamera* m_ActiveCamera;									/*!< Referencia a la camara actualmente activa */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();											/*!< Inicializacion */
		virtual void OnShutDown()override;							/*!< Libera los recursos solicitados al crear camaras */

	public:
		soCamera* CreateStandardCamera(CameraData& _CameraData);	/*!< Creacion de una camara Standard*/
		soEditorCamera* CreateEditorCamera(CameraData& _CameraData);/*!< Creacion de una camara Standard*/
		RESULT DeleteCamera(soString _CameraNameID);				/*!< Borra una camara de la escena */
		soCamera* ChangeTo(soString _CameraNameID);					/*!< Cambia la camara actual por la solicitada */
		soCamera* ChangeTo(uint32 _CameraID);						/*!< Cambia la camara actual por la solicitada */
		void Update(float _DeltaTime);								/*!< Actualiza la indormacion de las camaras */
		void Resize();												/*!< Actualiza el aspect ratio despues de un cambio de tamaño de la ventana */
	};
}

