
/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soDepthStencil.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/29/2016
* @brief  Declaration of soDepthStencil. Esta es la textura utilizada por
		  el render para hacer calculos de profundidad. Si un pixel a
          pintar es menos profundo qeu otro, se sustituye.
* @bug	  No know Bugs 
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
	/* Forward class declaration                                            */
	/************************************************************************/
	class soAPITexture;
	
	/************************************************************************/
	/* Declaracion de la clase soDepthStencil						        */
	/************************************************************************/
	class soDepthStencil : public soStandardClass<const DepthStencilData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soDepthStencil();											/*!< Constructor Default */
		~soDepthStencil();											/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		ID3D11DepthStencilView* m_ptrDepthStencilView;				/*!< Puntero al DepthStencil View */
		ID3D11DepthStencilState* m_ptrDepthStencilState;			/*!< Puntero al deoth stencil state, que controla el uso de los buffers traseros */
		soAPITexture* m_ptrDepthTexture;							/*!< Textura usada por el Depth stencil */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const DepthStencilData& _DepthStencilData);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;							/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}