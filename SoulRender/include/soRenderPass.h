/************************************************************************/
/**
* @LC	  10/10/2016
* @file   soRenderPass.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/15/2016
* @brief  Declaration of soRenderPass. Un pase representa una llamada 
	      de render. En	sistema de render diferido representa la añadidura 
		  de un efecto en la textura final.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras para compilacion                              */
/************************************************************************/
#include "SoulRenderDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de enum para identificar los shaders del pase		    */
	/************************************************************************/
	enum ShaderTypes
	{
		ST_PIXEL = 0x01,
		ST_VERTEX = 0x02,
		ST_COMPUTE = 0x04,
	};
	
	/************************************************************************/
	/* Declaracion de la clase soRenderPass						            */
	/************************************************************************/
	class soRenderPass : public soStandardClass<const RenderPassData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soRenderPass();														/*!< Constructor Standar */
		~soRenderPass();													/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private: 
		std::map<soString, uint8> m_RenderTargetNameMap;					/*!< Diccionario de render targets por nombre */		
		bool m_SetStencilView;												/*!< Identifica si se debe de setea*/
		uint32 m_ShaderTypes;												/*!< Identifica los tipos de shader que contiene el pase */
		
	public:
		soRenderTarget m_RenderTargetArray[8];								/*!< Arreglo de render targets del pase */
		uint8 m_RenderTargetID;												/*!< ID para los render targets */
		soString m_Name;													/*!< Nombre del pase */
		uint8 m_ID;															/*!< ID del pase */		
		soRasterizerState m_Rasterizer;										/*!< Rasterizador propio del pase */
		soFragmentShader m_PixelShader;										/*!< Pixel Shader perteneciente al pase */
		soVertexShader m_VertexShader;										/*!< Fragent Shader perteneciente al pase */
		soComputeShader m_ComputeShader;									/*!< Compute shader del pase */
		
		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/       
	private:
		RESULT OnStartUp(const RenderPassData& _RenderPassData);			/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;									/*!< Libera los recursos solicitados por la clase para su iniciacion */

	public:
		
		RESULT AddRenderTarget(const RenderTargetData& _RenderTargetData);	/*!< Añade un render Target al pase */
		soRenderTarget* GetRenderTarget(soString _RenderTargetName);		/*!< Toma un render target segun el nombre */
		soRenderTarget* GetRenderTarget(uint8 _RenderTargetID);				/*!< Toma un render target segun el ID */
		void Resize();														/*!< Aplica el cambio de tamaño de area cliente */
		void Render();														/*!< setea los render target, rasterizer state, viewports etc propios del passe */
	};
}