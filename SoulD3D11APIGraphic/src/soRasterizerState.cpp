/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soRasterizerState.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soRasterizerState
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soRasterizerState.h"

/************************************************************************/
/* Definicion de la clase soRasterizerState							    */
/************************************************************************/
namespace SoulSDK
{
	soRasterizerState::soRasterizerState()
	{
		Connect(std::bind(&soRasterizerState::OnStartUp, this, std::placeholders::_1));
		m_ptrRasterizerState = NULL;
	}

	soRasterizerState::~soRasterizerState()
	{
		ShutDown();
	}

	RESULT soRasterizerState::OnStartUp(const RasterizerStateData& _RasterizerStateData)
	{		
		// Setup rasterizer state.
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerDesc.AntialiasedLineEnable = _RasterizerStateData.AntialiasedLineEnable;
		rasterizerDesc.CullMode = static_cast<D3D11_CULL_MODE>(_RasterizerStateData.D3D11_CULL_MODE);
		rasterizerDesc.FillMode = static_cast<D3D11_FILL_MODE>(_RasterizerStateData.D3D11_FILL_MODE);
		rasterizerDesc.DepthBias = _RasterizerStateData.DepthBias;
		rasterizerDesc.DepthBiasClamp = _RasterizerStateData.DepthBiasClamp;
		rasterizerDesc.DepthClipEnable = _RasterizerStateData.DepthClipEnable;
		rasterizerDesc.FrontCounterClockwise = _RasterizerStateData.FrontCounterClockwise;
		rasterizerDesc.MultisampleEnable = _RasterizerStateData.MultisampleEnable;
		rasterizerDesc.ScissorEnable = _RasterizerStateData.ScissorEnable;
		rasterizerDesc.SlopeScaledDepthBias = _RasterizerStateData.SlopeScaledDepthBias;

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateRasterizerState(&rasterizerDesc, &m_ptrRasterizerState)))
		{
			SOE_ThrowException("soRasterizerState", "OnStartUp()", "FAIL to create rastericer state.");
			return EC_FALSE;
		}

		return EC_OK;
	}

	void soRasterizerState::OnShutDown()
	{
		SAFE_RELEASE(m_ptrRasterizerState);
	}
}
