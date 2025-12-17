/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soRenderTarjet.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/25/2015
* @brief  Implementacion de la clase soRenderTarjet. El render tarjet
		  representa una de las texturas sobre las que se renderea.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Forward Class declaration                                            */
	/************************************************************************/
	class soAPITexture;

	/************************************************************************/
	/* Declaracion de la clase soRenderTarjet							    */
	/************************************************************************/
	class soRenderTarget : public soStandardClass<const RenderTargetData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soRenderTarget();											/*!< Constructor Standard */
		~soRenderTarget();											/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		ID3D11RenderTargetView* m_ptrRenderTargetView;				/*!< Puntero al render target */
		soAPITexture* m_ptrRenderTargetTexture;						/*!< Textura perteneciente al render target */
		uint8 m_ID;													/*!< ID perteneciente al render target */
		soString m_Name;											/*!< Nombre del Render Target */
		TextureData m_RenderTargetTextureData;						/*!< Almacena la descripcion de la estructura de informacion para ta textura, de esta manera podemos reescalarla sin perder el formato original o pedir la info una vez mas */
		SamplerData m_RenderTargetTextureSamplerData;				/*!< Almacena la informacion del tipo de sampler que necesita el render target */
		bool m_CleanRenderTarget;									/*!< Identifica si debe o no de limpiar los render target view del pase (normalmente no si se trata del backbuffer )*/
		soVector4D m_CleanColor;									/*!< Color de limpieza de render target */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const RenderTargetData& _RenderTargetData);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;							/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}