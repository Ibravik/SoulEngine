/************************************************************************/
/**
* @LC	  10/30/2016
* @file   soVRManager.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/30/2016
* @brief  Declaration of class soVRManager. Este manager se encarga especi
		  ficamente de la gestion de inputs y outputs relacionados con 
		  sistemas VR.

* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para compilar                      */
/************************************************************************/
#include <openvr.h>
#include "soActor.h"
#include "soCamera.h"
#include "soEditorControl.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Enum de definicion de controles de VR                                */
	/************************************************************************/
	enum VRDevice
	{
		VRC_HeadSet = 0,
		VRC_LeftControl,
		VRC_RightControl,
		VRC_TotalDevices,
	};

	/************************************************************************/
	/* Estructura de informacion por control                                */
	/************************************************************************/
	struct DeviceData
	{
		soRay m_Ray;																	/*!< Rayo generado por el dispositivo */
		soMatrix4x4 m_WorldMatrix;														/*!< Matrix de transformacion en el mundo */
		soActor* m_VRActor;																/*!< Actor relacionado con el dispositivo */
		uint32 m_DeviceID;																/*!< ID asignado por la appi */
		float m_DeltaPad;																/*!< Delta de movimiento en el pad */

		DeviceData()
		{
			m_DeltaPad = 0;
		}
	};

	/************************************************************************/
	/* Informacion para subscripcion de funciones                           */
	/************************************************************************/
	typedef std::function<void(void)> VRFunction;

	struct VRSuscriptionData
	{
		VRDevice DeviceID;																/*!< Id del dispositivo asociado */
		uint32 _Event;																	/*!< Evento al que se suscriben */
		uint32 button;																	/*!< Input al que será suscrito una función */
		VRFunction callbackfunction;													/*!< Puntero a funcion de callback */
	};
	
	/************************************************************************/
	/* Declaracion de la clase soAnimationDataManager				        */
	/************************************************************************/
	class soVRManager : public soModule<soVRManager>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soVRManager();																	/*!< Constructor Standard */
		~soVRManager();																	/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:				
		vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];	/*!< Traked posed description array */
		vr::IVRSystem* m_ptrVRSystem;													/*!< Puntero local al dispositivo VR */
		soRect m_RecomendedBufferSize;													/*!< Tamaño de resolucion recomendado por el dispositivo */		
		std::vector<VRSuscriptionData> m_SignedList;									/*!< Lista de funciones suscritas */

	public:
		soCamera* m_LeftEyeCamera;														/*!< Camara referente al ojo izquierdo */
		soCamera* m_RightEyeCamera;														/*!< Camara referente al ojo derecho */
		soRenderTarget m_RightEyeRT;													/*!< Render target ligado al ojo derecho */
		soRenderTarget m_LeftEyeRT;														/*!< Render target ligado al ojo izquierdo */ 
		soRenderTarget m_GUIVR;															/*!< Render Target para GUI */
		soRect m_ControlGUI;															/*!< Cuadro de GUI del control Izquierdo */		
		DeviceData m_DeviceData[VRC_TotalDevices];										/*!< Referencia Informacion de cada dispositivo asociado al VR */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();																/*!< Inicializacion del resource manager */
		virtual void OnShutDown()override;												/*!< Finaliza la ejeucion del manager */
		soMatrix4x4 GetEyeProyection(vr::Hmd_Eye nEye);									/*!< Toma la proyeccion del ojo indicado */
		soMatrix4x4 GetEyeOffsetMatrix(vr::Hmd_Eye nEye);								/*!< Toma la matriz offset del ojo */
		soMatrix4x4 ConvertSteamVRMatrixTosoMatrix4x4(const vr::HmdMatrix34_t &matPose);/*!< Convierte una matriz de VR device a la del motor */
		RESULT InitCompositor();														/*!< Inicializa el sincronizador */
		RESULT CreateEyeRenderTarget(soRect& _BackBufferSize);							/*!< Genera los render target especificos del VR */
		RESULT EyeCameraStartUp();														/*!< Genera las camaras que represenan los ojos en el visor 3D */
		RESULT EyeUpdate();																/*!< Actualiza las camaras que representan los ojos */
		void InputHandler();															/*!< Maneja los inputs*/
		void ProcessEvent(const vr::VREvent_t & event);									/*!< Procesa los eventos*/		
		void GenerateRay(uint32 _InternalID);											/*!< Genera un rayo a partir de posicion actual del control */
		void UpdateDevices();															/*!< Render de los VR Controllers*/
		uint32 GetDeviceID(VRDevice _InternalID);										/*!< Retorna el ID asignado al dispositivo por la appi */
		uint32 GetDeviceInternalID(uint32 _AppiID);										/*!< Retorna el ID asignado al dispositivo por la appi */
		
	public:
		void Update(float _DeltaTime);													/*!< Actualizacion del sistema de VR */		
		void Render();																	/*!< Render de cada ojo en el dispositivo VR */
		void SuscribeFunction(VRSuscriptionData& _data);								/*!< Suscribe funciones void a ejecutar dependientes a un input; por ej: void Fn(void);*/
	};
}

