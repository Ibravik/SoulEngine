/********************************************************************/
/**
* @LC	  9/13/2016
* @file   soDXGIManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/14/2015
* @brief  Implementacion de la clase soDXGIManager
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soAPITexture.h"
#include "soDXGIManager.h" 

/************************************************************************/
/* Definicion de la clase soDirectXGraphics                             */
/************************************************************************/
namespace SoulSDK
{
	soDXGIManager::soDXGIManager()
	{
		Connect(std::bind(&soDXGIManager::OnStartUp, this, std::placeholders::_1));
		m_IsDirty = true;
	}

	soDXGIManager::~soDXGIManager()
	{
	}

	RESULT soDXGIManager::Resize(const soRect& _ClientSize)
	{
		if (!_ClientSize.m_Width || !_ClientSize.m_Height)
		{
			SOE_OutputLOG("soDXGIManager", "Resize()", "Invalid ClientSize.");
			return EC_FALSE;
		}

		m_DefaultBackBuffer.ShutDown();
		m_DepthStencil.ShutDown();

		m_ClientRect = _ClientSize;
		UpdateConstantBuffer();

		if (m_Device.m_ptrSwapChain)
		{
			//cadena de intercambio a reconstruir
			if (FAIL(m_Device.m_ptrSwapChain->ResizeBuffers(1, m_ClientRect.m_Width, m_ClientRect.m_Height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
			{
				SOE_ThrowException("soDXGIManager", "Resize()", "FAIL to resize SwapChain buffers.");
				return EC_FALSE;
			}

			/* Creacion del render target del back buffer							*/
			if (FAIL(CreateBackBufferRenderTargetView()))
			{
				SOE_ThrowException("soDXGIManager", "OnStartUp()", "FAIL to create back buffer render target view.");
				return EC_FALSE;
			}

			/************************************************************************/
			/* Create Depth Stencil View                                            */
			/************************************************************************/
			TextureData T2DInfo;
			T2DInfo.T2DData.TextureSize = m_ClientRect;
			T2DInfo.T2DData.TextureSize = _ClientSize;
			T2DInfo.T2DData.D3D11_BIND_FLAG = API_BIND_DEPTH_STENCIL;
			T2DInfo.T2DData.DXGI_FORMAT = APIGI_FORMAT_D24_UNORM_S8_UINT;
			DepthStencilData DSD;
			DSD._Texture2DData = T2DInfo;

			if (FAIL(m_DepthStencil.StartUp(DSD)))
			{
				SOE_ThrowException("soDXGIManager", "Resize()", "FAIL to resize DepthStencil.");
				return EC_FALSE;
			}

			m_IsDirty = true;
			return EC_OK;
		}

		return EC_FALSE;
	}
		
	void soDXGIManager::CleanResources()
	{
		m_Device.m_ptrD3DContext->VSSetShader(0, 0, 0);
		m_Device.m_ptrD3DContext->GSSetShader(0, 0, 0);
		m_Device.m_ptrD3DContext->PSSetShader(0, 0, 0);
		m_Device.m_ptrD3DContext->OMSetRenderTargets(0, 0, 0);
		ID3D11ShaderResourceView* nullSRV[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		m_Device.m_ptrD3DContext->VSSetShaderResources(0, 8, nullSRV);
		m_Device.m_ptrD3DContext->PSSetShaderResources(0, 8, nullSRV);
		m_Device.m_ptrD3DContext->CSSetShaderResources(0, 8, nullSRV);
		ID3D11UnorderedAccessView *nullUAV[1] = { 0 };
		m_Device.m_ptrD3DContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	}

	void soDXGIManager::CleanDepthStencil()
	{
		m_Device.m_ptrD3DContext->ClearDepthStencilView(m_DepthStencil.m_ptrDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 1);
	}

	RESULT soDXGIManager::OnStartUp(const InitGraphicInfo& _InitInfo)
	{
		/* Assign client rect size												*/
		if (!_InitInfo.SCData.ScreenDimentions.m_Width || !_InitInfo.SCData.ScreenDimentions.m_Height)
		{
			SOE_ThrowException("soDXGIManager", "OnStartUp()", "Invalid ClientSize.");
			return EC_FALSE;
		}
		m_ClientRect = _InitInfo.SCData.ScreenDimentions;
		UpdateConstantBuffer();

		/* Create Device and swap chain	& check Feature level				    */
		if (FAIL(m_Device.StartUp(_InitInfo.SCData)))
		{
			SOE_ThrowException("soDXGIManager", "OnStartUp()", "FAIL to create device and swap chain.");
			return EC_FALSE;
		}

		/* Creacion del render target del back buffer							*/
		if (FAIL(CreateBackBufferRenderTargetView()))
		{
			SOE_ThrowException("soDXGIManager", "OnStartUp()", "FAIL to create back buffer render target view.");
			return EC_FALSE;
		}

		/* Create Depth Stencil View                                            */
		if (FAIL(m_DepthStencil.StartUp(_InitInfo.DSData)))
		{
			SOE_ThrowException("soDXGIManager", "OnStartUp()", "FAIL to create depth stencil.");
			return EC_FALSE;
		}



		m_IsDirty = true;

		return EC_OK;
	}

	void soDXGIManager::OnShutDown()
	{
	}

	RESULT soDXGIManager::CreateBackBufferRenderTargetView()
	{
		ID3D11Texture2D* pBackBuffer = NULL;
		if (FAIL(m_Device.m_ptrSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		{
			SOE_ThrowException("soDXGIManager", "CreateBackBufferRenderTargetView()", "FAIL to create back buffer render target view.");
			return EC_FALSE;
		}

		if (FAIL(m_Device.m_ptrD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_DefaultBackBuffer.m_ptrRenderTargetView)))
		{
			SAFE_RELEASE(pBackBuffer);
			SOE_ThrowException("soDXGIManager", "CreateBackBufferRenderTargetView()", "FAIL to create back buffer render target view.");
			return EC_FALSE;
		}

		m_DefaultBackBuffer.m_ptrRenderTargetTexture = SOE_NEW soAPITexture;
		m_DefaultBackBuffer.m_ptrRenderTargetTexture->m_ptrTexture2D = pBackBuffer;

		D3D11_TEXTURE2D_DESC desc;
		pBackBuffer->GetDesc(&desc);

		m_DefaultBackBuffer.m_ptrRenderTargetTexture->m_TextureSize.m_Width = desc.Width;
		m_DefaultBackBuffer.m_ptrRenderTargetTexture->m_TextureSize.m_Height = desc.Height;

		return EC_OK;
	}

	void soDXGIManager::UpdateConstantBuffer()
	{
		m_CBApi.ClientSize.X = static_cast<float>(m_ClientRect.m_Width);
		m_CBApi.ClientSize.Y = static_cast<float>(m_ClientRect.m_Height);
	}

	void soDXGIManager::SetViewPort(const soViewPort& _ViewPort)
	{
		/* ViewPort */
		m_Device.m_ptrD3DContext->RSSetViewports(1, &_ViewPort.m_ViewPort);
	}

	void soDXGIManager::SetBlendState(soBlendState& _BlendState, const float* _BlendFactor, uint32 SampleMask)
	{
		m_Device.m_ptrD3DContext->OMSetBlendState(_BlendState.m_ptrBlendState, _BlendFactor, SampleMask);
	}

	void soDXGIManager::SetVertexBuffer(uint32 _StartSlot, uint32 _NumBuffers, soVertexBuffer& _VertexBuffer, uint32& _Stride, uint32& _Offset)
	{
		m_Device.m_ptrD3DContext->IASetVertexBuffers(_StartSlot, _NumBuffers, &_VertexBuffer.m_ptrVertexBuffer->m_ptrBuffer, &_Stride, &_Offset);
	}

	void soDXGIManager::SetIndexBuffer(soIndexBuffer& _soIndexBuffer, APIGI_FORMAT& _APIGI_FORMAT, uint32& _Offset)
	{
		m_Device.m_ptrD3DContext->IASetIndexBuffer(_soIndexBuffer.m_ptrIndexBuffer->m_ptrBuffer, static_cast<DXGI_FORMAT>(_APIGI_FORMAT), _Offset);
	}

	void soDXGIManager::SetPrimitiveTopology(API_PRIMITIVE_TOPOLOGY& _API_PRIMITIVE_TOPOLOGY)
	{
		m_Device.m_ptrD3DContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(_API_PRIMITIVE_TOPOLOGY));
	}

	void soDXGIManager::DrawIndexed(soIndexBuffer& _IndexBuffer, uint32& _StartIndexLocation, int32& StartVertexLocation)
	{
		m_Device.m_ptrD3DContext->DrawIndexed(_IndexBuffer.GetIndexArraySize(), _StartIndexLocation, StartVertexLocation);
	}

	void soDXGIManager::Draw(uint32& _VertexCount, int32& StartVertexLocation)
	{
		m_Device.m_ptrD3DContext->Draw(_VertexCount, StartVertexLocation);
	}

	void soDXGIManager::Dispatch(uint32 ThreadGroupCountX, uint32 ThreadGroupCountY, uint32 ThreadGroupCountZ)
	{
		m_Device.m_ptrD3DContext->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
	}

	void soDXGIManager::PSSetShaderResources(uint32 _StartSlot, uint32 _NumViews, soAPITexture* _TextureShaderResource)
	{
		if (_TextureShaderResource != NULL)
		{
			m_Device.m_ptrD3DContext->PSSetShaderResources(_StartSlot, _NumViews, &_TextureShaderResource->m_ptrSRTexture);
		}	
		else
		{
			m_Device.m_ptrD3DContext->PSSetShaderResources(_StartSlot, NULL, NULL);
		}
	}

	void soDXGIManager::CSSetShaderResources(uint32 _StartSlot, uint32 _NumViews, soAPITexture& _TextureShaderResource)
	{
		if (_TextureShaderResource.m_ptrSRTexture != NULL)
		{
			m_Device.m_ptrD3DContext->CSSetShaderResources(_StartSlot, _NumViews, &_TextureShaderResource.m_ptrSRTexture);			
		}		
	}

	void soDXGIManager::CSSetUnorderedAccessViews(uint32 _StartSlot, uint32 _NumViews, ID3D11UnorderedAccessView* _UnorderAccesView, uint32 _InitCount)
	{
		if (_UnorderAccesView != NULL)
		{
			m_Device.m_ptrD3DContext->CSSetUnorderedAccessViews(_StartSlot, _NumViews, &_UnorderAccesView, &_InitCount);
		}
	}

	void soDXGIManager::PSSetSampler(uint32 _StartSlot, uint32 _NumSamplers, soSampler& _Sampler)
	{
		m_Device.m_ptrD3DContext->PSSetSamplers(_StartSlot, _NumSamplers, &_Sampler.m_ptrSampler);
	}

	void soDXGIManager::UpdateSubresource(soBuffer& _ConstantBuffer, const void* _srcData, uint32 SrcRowPitch, uint32 SrcDepthPitch)
	{
		m_Device.m_ptrD3DContext->UpdateSubresource(_ConstantBuffer.m_ptrBuffer, 0, NULL, _srcData, SrcRowPitch, SrcDepthPitch);
	}

	void soDXGIManager::VSSetConstantBuffers(uint32 _StartSlot, uint32 _NumBuffers, soBuffer& _ConstantBuffer)
	{
		m_Device.m_ptrD3DContext->VSSetConstantBuffers(_StartSlot, _NumBuffers, &_ConstantBuffer.m_ptrBuffer);
	}

	void soDXGIManager::PSSetConstantBuffers(uint32 _StartSlot, uint32 _NumBuffers, soBuffer& _ConstantBuffer)
	{
		m_Device.m_ptrD3DContext->PSSetConstantBuffers(_StartSlot, _NumBuffers, &_ConstantBuffer.m_ptrBuffer);
	}

	void soDXGIManager::CSSetConstantBuffers(uint32 _StartSlot, uint32 _NumBuffers, soBuffer& _ConstantBuffer)
	{
		m_Device.m_ptrD3DContext->CSSetConstantBuffers(_StartSlot, _NumBuffers, &_ConstantBuffer.m_ptrBuffer);
	}

	void soDXGIManager::GenerateMips(soAPITexture& _Texture)
	{
		m_Device.m_ptrD3DContext->GenerateMips(_Texture.m_ptrSRTexture);
	}

	void soDXGIManager::SetRenderTargets(soRenderTarget* _RenderTargetArray, uint32 _ActiveRT, bool _SetDepthStencil /* = true */)
	{
		ZeroMemory(m_ActiveRTV, sizeof(void*) * 8);
		/*release the kraken case */
		if (_RenderTargetArray == NULL)
		{
			m_Device.m_ptrD3DContext->OMSetRenderTargets(NULL, NULL, NULL);
			return;
		}

		for (uint32 i = 0; i < _ActiveRT; i++)
		{
			m_ActiveRTV[i] = _RenderTargetArray[i].m_ptrRenderTargetView;
		}

		if (_SetDepthStencil)
		{
			m_Device.m_ptrD3DContext->OMSetDepthStencilState(m_DepthStencil.m_ptrDepthStencilState, 0);
			m_Device.m_ptrD3DContext->OMSetRenderTargets(8, m_ActiveRTV, m_DepthStencil.m_ptrDepthStencilView);
		}
		else
		{
			m_Device.m_ptrD3DContext->OMSetDepthStencilState(NULL, NULL);
			m_Device.m_ptrD3DContext->OMSetRenderTargets(8, m_ActiveRTV, NULL);
		}
	}
	
	void soDXGIManager::SetVertexShader(soVertexShader& _VertexShader)
	{
		m_Device.m_ptrD3DContext->VSSetShader(_VertexShader.m_ptrVertexShader, NULL, 0);
		m_Device.m_ptrD3DContext->IASetInputLayout(_VertexShader.m_InputLayout.m_ptrInputLayOut);
	}

	void soDXGIManager::SetPixelShader(soFragmentShader& _PixelShader)
	{
		m_Device.m_ptrD3DContext->PSSetShader(_PixelShader.m_ptrPixelShader, NULL, 0);
	}

	void soDXGIManager::SetComputeShader(soComputeShader& _ComputeShader)
	{
		m_Device.m_ptrD3DContext->CSSetShader(_ComputeShader.m_ptrComputeShader, NULL, 0);
	}

	void soDXGIManager::SetRasterizer(soRasterizerState&_Rasterizer)
	{
		m_Device.m_ptrD3DContext->RSSetState(_Rasterizer.m_ptrRasterizerState);
	}

	void soDXGIManager::ClearRenderTargetList(soRenderTarget* _RenderTargetArray, uint32 _ActiveRT)
	{
		/* clear own RenderTarget list												*/		
		for (uint32 i = 0; i < _ActiveRT; i++)
		{
			if (_RenderTargetArray[i].m_CleanRenderTarget)
			{
				m_Device.m_ptrD3DContext->ClearRenderTargetView(_RenderTargetArray[i].m_ptrRenderTargetView, (float*)&_RenderTargetArray[i].m_CleanColor);
			}
		}
	}

	void soDXGIManager::ClearBackBuffer(soRenderTarget* _RenderTarget)
	{
		SetRenderTargets(_RenderTarget, 1, false);
		ClearRenderTargetList(_RenderTarget, 1);
		SetRenderTargets(NULL, NULL, false);
	}

}