/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soDepthStencil.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/29/2015
* @brief  Implementacion de la clase soDepthStencil
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soAPITexture.h"
#include "soDepthStencil.h"

/************************************************************************/
/* Definicion de la clase soDepthStencil								*/
/************************************************************************/
namespace SoulSDK
{
	soDepthStencil::soDepthStencil()
	{
		Connect(std::bind(&soDepthStencil::OnStartUp, this, std::placeholders::_1));
		m_ptrDepthStencilView = NULL;
		m_ptrDepthStencilState = NULL;
		m_ptrDepthTexture = NULL;
	}
	
	soDepthStencil::~soDepthStencil()
	{
		ShutDown();
	}

	RESULT soDepthStencil::OnStartUp(const DepthStencilData& _DepthStencilData)
	{			
		m_ptrDepthTexture = SOE_NEW soAPITexture;
		if (FAIL(m_ptrDepthTexture->Create(_DepthStencilData._Texture2DData)))
		{
			SOE_ThrowException("soDepthStencil", "OnStartUp()", "FAIL to create depth stencil texture.");
			return EC_FALSE;
		}

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = static_cast<DXGI_FORMAT>(_DepthStencilData._Texture2DData.T2DData.DXGI_FORMAT);
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateDepthStencilView(m_ptrDepthTexture->m_ptrTexture2D, &descDSV, &m_ptrDepthStencilView)))
		{
			SOE_ThrowException("soDepthStencil", "OnStartUp()", "FAIL to create depth stencil view.");
			return EC_FALSE;
		}

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
		ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		// Depth test parameters
		depthStencilStateDesc.DepthEnable = _DepthStencilData.DepthEnable;
		depthStencilStateDesc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(_DepthStencilData.D3D11_DEPTH_WRITE_MASK);
		depthStencilStateDesc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(_DepthStencilData.D3D11_COMPARISON_FUNC);

		// Stencil test parameters
		depthStencilStateDesc.StencilEnable = _DepthStencilData.StencilEnable;
		depthStencilStateDesc.StencilReadMask = _DepthStencilData.StencilReadMask;
		depthStencilStateDesc.StencilWriteMask = _DepthStencilData.StencilWriteMask;

		// Stencil operations if pixel is front-facing
		depthStencilStateDesc.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_DepthStencilData.FrontFaceStencilFailOp);
		depthStencilStateDesc.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_DepthStencilData.FrontFaceStencilDepthFailOp);
		depthStencilStateDesc.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_DepthStencilData.FrontFaceStencilPassOp);
		depthStencilStateDesc.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_DepthStencilData.FrontFaceStencilFunc);

		// Stencil operations if pixel is back-facing
		depthStencilStateDesc.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_DepthStencilData.BackFaceStencilFailOp);;
		depthStencilStateDesc.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_DepthStencilData.BackFaceStencilDepthFailOp);;
		depthStencilStateDesc.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_DepthStencilData.BackFaceStencilPassOp);;
		depthStencilStateDesc.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_DepthStencilData.BackFaceStencilFunc);;

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateDepthStencilState(&depthStencilStateDesc, &m_ptrDepthStencilState)))
		{
			SOE_ThrowException("soDepthStencil", "OnStartUp()", "FAIL to create depth stencil state.");
			return EC_FALSE;
		}

		return EC_OK;
	}

	void soDepthStencil::OnShutDown()
	{
		SAFE_RELEASE(m_ptrDepthStencilView);
		SAFE_RELEASE(m_ptrDepthStencilState);
		SAFE_DELETE(m_ptrDepthTexture);
	}

}