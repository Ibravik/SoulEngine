/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soSwapChain.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/29/2016
* @brief  Declaration of soSwapChain. La cadena de intercambio es la 
		  encargada de cambiar el back buffer con el front buffer.
* @bug	  No know Bugs Segun dice el inge
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras para compilacion                              */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"

namespace SoulSDK
{		
	/************************************************************************/
	/* Declaracion de la clase soSwapChain							        */
	/************************************************************************/
	class soDeviceSwapChain : public soStandardClass<const SwapChainData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soDeviceSwapChain();																/*!< Constructor Default */
		~soDeviceSwapChain();																/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		//Driver type and feature level for shaders
		D3D_DRIVER_TYPE m_DriverType;						 								/*!< Tipo de driver para el device */
		D3D_FEATURE_LEVEL m_FeatureLevel;													/*!< Almacena la compativilidad del device con directx */

	public:
		IDXGISwapChain* m_ptrSwapChain;														/*!< Cadena de intercambio con el backBuffer */
		ID3D11Device* m_ptrD3DDevice;												 		/*!< Interfas del device, representa el hardware */
		ID3D11DeviceContext* m_ptrD3DContext;												/*!< Interfaz del device context, que abstrae la funcionalidad del device */
		soString m_szVSProfile;																/*!< Guarda el perfil maximo disponible de compilacion para el vertex shader */
		soString m_szPSProfile;														 		/*!< Guarda el perfil maximo disponible de compilacion para el pixel shader */
		soString m_szCSProfile;														 		/*!< Guarda el perfil maximo disponible de compilacion para el compute shader */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private: 
		DXGI_RATIONAL QueryRefreshRate(const soRect& _ScreenDimentions, const bool& _vSync);/*!< Devielve el refresh rate mas optimo deacuerdo a la configuracion de la ventana y monitor */
		void PerFeatureLevel();																/*!< Elige de manera automatica la vercion de PS, VS mas alta que soporte la tarjeta de video */
		RESULT OnStartUp(const SwapChainData& _SwapChainData);								/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;													/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}