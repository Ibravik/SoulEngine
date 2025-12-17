/********************************************************************/
/**
* @LC	  10/22/2016
* @file   soBlendState.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/22/2016
* @brief  Implementacion de la clase soBlendState
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soBlendState.h"

/************************************************************************/
/* Definicion de la clase soConstantBuffer							    */
/************************************************************************/
namespace SoulSDK
{
	soBlendState::soBlendState()
	{
		Connect(std::bind(&soBlendState::OnStartUp, this, std::placeholders::_1));
		m_ptrBlendState = NULL;
	}

	soBlendState::~soBlendState()
	{
		ShutDown();
	}

	RESULT soBlendState::OnStartUp(const BlendData& _BlendData)
	{
		CD3D11_BLEND_DESC desc(D3D11_DEFAULT);
		
		desc.AlphaToCoverageEnable = _BlendData.AlphaToCoverageEnable;
		desc.IndependentBlendEnable = _BlendData.IndependentBlendEnable;

		for (uint8 i = 0; i < 8; i++)
		{
			desc.RenderTarget[i].BlendEnable = _BlendData.RenderTarget[i].BlendEnable;
			desc.RenderTarget[i].SrcBlend = static_cast<D3D11_BLEND>(_BlendData.RenderTarget[i].SrcBlend);
			desc.RenderTarget[i].DestBlend = static_cast<D3D11_BLEND>(_BlendData.RenderTarget[i].DestBlend);
			desc.RenderTarget[i].BlendOp = static_cast<D3D11_BLEND_OP>(_BlendData.RenderTarget[i].BlendOp);
			desc.RenderTarget[i].SrcBlendAlpha = static_cast<D3D11_BLEND>(_BlendData.RenderTarget[i].SrcBlendAlpha);
			desc.RenderTarget[i].DestBlendAlpha = static_cast<D3D11_BLEND>(_BlendData.RenderTarget[i].DestBlendAlpha);
			desc.RenderTarget[i].BlendOpAlpha = static_cast<D3D11_BLEND_OP>(_BlendData.RenderTarget[i].BlendOpAlpha);
		}		

		return soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateBlendState(&desc, &m_ptrBlendState);
	}

	void soBlendState::OnShutDown()
	{
		SAFE_RELEASE(m_ptrBlendState);
	}

}