
/************************************************************************/
/**
* @LC	  9/4/2016
* @file   soDXGIManager.h 
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/26/2015
* @brief  Declaration of soDXGIManager. Esta clase encapsula los recursos
		  manejados por directx. Para buscar la estandarizacion, esta clase
		  permite que se pueda hacer cambio de appi grafica sin ningun cambio 
		  en capas de abstraccion superiores.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once
#pragma warning(disable : 4273)

/************************************************************************/
/* Inclucion de cabeceras para compilacion                              */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"
#include "soDeviceSwapChain.h"
#include "soDepthStencil.h"
#include "soViewPort.h"
#include "soSampler.h"
#include "soVertexBuffer.h"
#include "soIndexBuffer.h"
#include "soFragmentShader.h"
#include "soBuffer.h"
#include "soRenderTarget.h"
#include "soVertexShader.h"
#include "soInputLayout.h"
#include "soRasterizerState.h"
#include "soBlendState.h"
#include "soStructuredBuffer.h"
#include "soComputeShader.h"

namespace SoulSDK
{	
	/************************************************************************/
	/* Declaracion de estructura de CB del API                              */
	/************************************************************************/
	struct CBAPI
	{
		soVector2D ClientSize;																												/*!< Tamaño del area cliente */
	};

	/************************************************************************/
	/* Declaracion de la clase soDXGIManager	                            */
	/************************************************************************/
	class soDXGIManager : public soModule<soDXGIManager, const InitGraphicInfo&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soDXGIManager();																													/*!< Constructor Standard */
		~soDXGIManager();																													/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		bool m_IsDirty;																														/*!< Señala que es necesario reingresar los elementos de la appi */
		ID3D11RenderTargetView* m_ActiveRTV[8];																								/*!< Active render target view */

	public:
		//Swap Chain, Device & Context
		soDeviceSwapChain m_Device;																											/*!< Objeto que contiene el Device, el context y la cadena de intercambio */

		//Depth Stencil view for use as a depth buffer and the texture associate with it
		soDepthStencil m_DepthStencil;																										/*!< Objeto con el DepthStencil view, state y textura correspondiente */

		//Render target default de la api del back buffer
		soRenderTarget m_DefaultBackBuffer;																									/*!< Render target del back buffer */
		
		//Tamaño del area cliente
		soRect m_ClientRect;

		CBAPI m_CBApi;																														/*!< Estructura que contiene la informacion relacionada con la api */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const InitGraphicInfo& _InitInfo);																					/*!< Inicializacion del resource manager */
		virtual void OnShutDown()override;																									/*!< Finaliza la ejeucion del manager */
		RESULT CreateBackBufferRenderTargetView();																							/*!< Crea el render target view del back buffer */
		void UpdateConstantBuffer();																										/*!< Actualiza la informacion del CB */

	public:
		RESULT Resize(const soRect& _ClientSize);																							/*!< Recrea todo lo necesario para el rezize de la pantalla */
		void CleanResources();																												/*!< Libera cualquier residuo de recursos de la ultima ejecucion */
		void CleanDepthStencil();																											/*!< Limpia el depth buffer de la cadena de intercambio */
		void SetViewPort(const soViewPort& _ViewPort);																						/*!< Setea El viewPort de la api */
		void SetBlendState(soBlendState& _BlendState, const float* _BlendFactor, uint32 SampleMask);										/*!< Setea un estado de blend para el render target */
		void SetRenderTargets(soRenderTarget* _RenderTargetArray, uint32 _ActiveRT, bool _SetDepthStencil = true);							/*!< Set de render targets para el pase de render */
		void SetVertexShader(soVertexShader& _VertexShader);																				/*!< Setea el Vertex Shader al device */
		void SetPixelShader(soFragmentShader& _PixelShader);																				/*!< Setea el pixel Shader al device */
		void SetComputeShader(soComputeShader& _ComputeShader);																				/*!< Setea el compute Shader al device */
		void SetRasterizer(soRasterizerState&_Rasterizer);																					/*!< Setea el razterizador al device */
		void ClearRenderTargetList(soRenderTarget* _RenderTargetArray, uint32 _ActiveRT);													/*!< Limpia el render target List */
		void ClearBackBuffer(soRenderTarget* _RenderTarget);																				/*!< Limpia el back buffer */
		void SetVertexBuffer(uint32 _StartSlot, uint32 _NumBuffers, soVertexBuffer& _VertexBuffer, uint32& _Stride, uint32& _Offset);		/*!< Setea el Vertex Buffer en el device */
		void SetIndexBuffer(soIndexBuffer& _soIndexBuffer, APIGI_FORMAT& _APIGI_FORMAT, uint32& _Offset);									/*!< Setea el Vertex Buffer en el device */
		void SetPrimitiveTopology(API_PRIMITIVE_TOPOLOGY& _API_PRIMITIVE_TOPOLOGY);															/*!< Setea la topologia primitiva (Manera de relacionar los vertices introducidos)*/
		void DrawIndexed(soIndexBuffer& _IndexBuffer, uint32& _StartIndexLocation, int32& StartVertexLocation);								/*!< Indica al device a llamar un pase de render, en otras palabras, pasar al uso de shaders */
		void Draw(uint32& _VertexCount, int32& StartVertexLocation);																		/*!< Indica al device a llamar un pase de render, en otras palabras, pasar al uso de shaders */
		void Dispatch(uint32 ThreadGroupCountX, uint32 ThreadGroupCountY, uint32 ThreadGroupCountZ);										/*!< Llama un pase de compute */
		void PSSetShaderResources(uint32 _StartSlot, uint32 _NumViews, soAPITexture* _TextureShaderResource);								/*!< Introduce informacion para el PS */
		void CSSetShaderResources(uint32 _StartSlot, uint32 _NumViews, soAPITexture& _TextureShaderResource);								/*!< Asigna textura al CS */
		void CSSetUnorderedAccessViews(uint32 _StartSlot, uint32 _NumViews, ID3D11UnorderedAccessView* _UnorderAccesView, uint32 _InitCount);/*!< Ingresa un unnorder acces view para el compute shader  */
		void PSSetSampler(uint32 _StartSlot, uint32 _NumSampler, soSampler& _Sampler);														/*!< Setea el sampler para una textura */
		void UpdateSubresource(soBuffer& _ConstantBuffer, const void* _srcData, uint32 SrcRowPitch = 0, uint32 SrcDepthPitch = 0);			/*!< Actualiza la informacion del constant buffer */
		void VSSetConstantBuffers(uint32 _StartSlot, uint32 _NumBuffers, soBuffer& _ConstantBuffer);										/*!< Actualiza la informacion del CB para el vs */
		void PSSetConstantBuffers(uint32 _StartSlot, uint32 _NumBuffers, soBuffer& _ConstantBuffer);										/*!< Actualiza la informacion del cb para el ps*/
		void CSSetConstantBuffers(uint32 _StartSlot, uint32 _NumBuffers, soBuffer& _ConstantBuffer);										/*!< Actualiza la informacion del cb para el ps*/
		void GenerateMips(soAPITexture& _Texture);																							/*!< Genera los MipMaps de la textura ingresada (solo si al generarse se desbloqueo esa capacidad) */
	};
}
#pragma warning(default : 4273)