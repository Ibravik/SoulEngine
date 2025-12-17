/********************************************************************/
/**
* @LC	  10/10/2016
* @file   soRenderPass.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/14/2015
* @brief  Implementacion de la clase soRenderPass
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "Soul_Core.h"
#include "soDXGIManager.h"
#include "soTexture.h"
#include "soRenderPass.h"

/************************************************************************/
/* Definicion de la clase soRenderPass		                            */
/************************************************************************/
namespace SoulSDK
{
	soRenderPass::soRenderPass()
	{
		Connect(std::bind(&soRenderPass::OnStartUp, this, std::placeholders::_1));
		m_RenderTargetNameMap.clear();
		m_RenderTargetID = 0;
		m_SetStencilView = true;
		m_ShaderTypes = 0;
	}

	soRenderPass::~soRenderPass()
	{
		ShutDown();
	}

	RESULT soRenderPass::AddRenderTarget(const RenderTargetData& _RenderTargetData)
	{
		if (FAIL(m_RenderTargetArray[m_RenderTargetID].StartUp(_RenderTargetData)))
		{
			SOE_ThrowException("soRenderTargetPass", "AddRenderTarget", "Fail to create the render target")
		}
		m_RenderTargetArray[m_RenderTargetID].m_ID = m_RenderTargetID;
		m_RenderTargetID++;

		return EC_OK;
	}

	soRenderTarget * soRenderPass::GetRenderTarget(soString _RenderTargetName)
	{
		if (m_RenderTargetNameMap.find(soString(_RenderTargetName)) != m_RenderTargetNameMap.end())
		{
			return &m_RenderTargetArray[m_RenderTargetNameMap.at(soString(_RenderTargetName))];
		}

		return NULL;
	}

	soRenderTarget * soRenderPass::GetRenderTarget(uint8 _RenderTargetID)
 	{
		if (_RenderTargetID < m_RenderTargetID)
		{
			return &m_RenderTargetArray[_RenderTargetID];
		}

		return NULL;
	}

	RESULT soRenderPass::OnStartUp(const RenderPassData& _RenderPassData)
	{	
		m_Name = _RenderPassData._Name;
		m_SetStencilView = _RenderPassData.SetStencilView;
				
		//rasterizer
		m_Rasterizer.StartUp(_RenderPassData._RasterizerStateInfo);
		
		if (!_RenderPassData._VertexShaderData.FilePath.empty() || !_RenderPassData._VertexShaderData.S_EntryPoint.empty() || !_RenderPassData._VertexShaderData.S_Profile.empty())
		{
			m_VertexShader.StartUp(_RenderPassData._VertexShaderData);
			m_ShaderTypes |= ST_VERTEX;
		}
			

		if (!_RenderPassData._FragmentShaderData.FilePath.empty() || !_RenderPassData._FragmentShaderData.S_EntryPoint.empty() || !_RenderPassData._FragmentShaderData.S_Profile.empty())
		{
			m_PixelShader.StartUp(_RenderPassData._FragmentShaderData);
			m_ShaderTypes |= ST_PIXEL;
		}			

		if (!_RenderPassData._ComputeShader.FilePath.empty() || !_RenderPassData._ComputeShader.S_EntryPoint.empty() || !_RenderPassData._ComputeShader.S_Profile.empty())
		{
			m_ComputeShader.StartUp(_RenderPassData._ComputeShader);
			m_ShaderTypes |= ST_COMPUTE;
		}			

		return EC_OK;
	}

	void soRenderPass::OnShutDown()
	{
		m_RenderTargetNameMap.clear();
	}

	void soRenderPass::Resize()
	{
		//Primero liberamos los anteriores Render tarjets y los recreamos con la nueva info del swapchain
		RenderTargetData RT;

		for (uint8 i = 0; i < m_RenderTargetID; i++)
		{
			RT._TextureData = m_RenderTargetArray[i].m_RenderTargetTextureData;
			RT._TextureData.T2DData.TextureSize = soDXGIManager::Instance().m_DefaultBackBuffer.m_ptrRenderTargetTexture->m_TextureSize;
			RT.Name = m_RenderTargetArray[i].m_Name;
			m_RenderTargetArray[i].ShutDown();
			m_RenderTargetArray[i].StartUp(RT);
		}		
	}

	void soRenderPass::Render()
	{
		/* Set DirectX Pass Sentings	*/	
		if (m_ShaderTypes & ST_COMPUTE)
		{
			soDXGIManager::Instance().SetComputeShader(m_ComputeShader);
			return;
		}

		soDXGIManager::Instance().SetRenderTargets(*&m_RenderTargetArray, m_RenderTargetID, m_SetStencilView);
		if (m_ShaderTypes & ST_VERTEX)
		{
			soDXGIManager::Instance().SetVertexShader(m_VertexShader);
		}
		
		if (m_ShaderTypes & ST_PIXEL)
		{
			soDXGIManager::Instance().SetPixelShader(m_PixelShader);
		}
				
		soDXGIManager::Instance().SetRasterizer(m_Rasterizer);
		soDXGIManager::Instance().ClearRenderTargetList(m_RenderTargetArray, m_RenderTargetID);
	}
}