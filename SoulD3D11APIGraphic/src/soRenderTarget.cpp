/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soRenderTarjet.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soRenderTarjet
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soAPITexture.h"
#include "soRenderTarget.h"

/************************************************************************/
/* Definicion de la clase soRenderTarjet							    */
/************************************************************************/
namespace SoulSDK
{
	soRenderTarget::soRenderTarget()
	{
		Connect(std::bind(&soRenderTarget::OnStartUp, this, std::placeholders::_1));
		m_ptrRenderTargetView = NULL;
		m_ptrRenderTargetTexture = NULL;
		m_CleanRenderTarget = true;
		m_CleanColor = soVector4D(0, 0, 0, 1);
	}

	soRenderTarget::~soRenderTarget()
	{
		ShutDown();
	}

	RESULT soRenderTarget::OnStartUp(const RenderTargetData& _RenderTargetData)
	{		
		m_RenderTargetTextureData = _RenderTargetData._TextureData;
		m_CleanColor = _RenderTargetData._CleanColor;
		m_Name = _RenderTargetData.Name;

		m_ptrRenderTargetTexture = SOE_NEW soAPITexture;
		if (FAIL(m_ptrRenderTargetTexture->Create(_RenderTargetData._TextureData)))
		{
			SOE_ThrowException("soRenderTarjet", "OnStartUp()", "FAIL to create render target texture.");
			return EC_FALSE;
		}

		if (_RenderTargetData._TextureData.TextureType == API_Texture2D)
		{
			if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateRenderTargetView(m_ptrRenderTargetTexture->m_ptrTexture2D, NULL, &m_ptrRenderTargetView)))
			{
				SOE_ThrowException("soRenderTarjet", "OnStartUp()", "FAIL to create render target.");
				return EC_FALSE;
			}

			return EC_OK;
		}

		return EC_FALSE;
	}

	void soRenderTarget::OnShutDown()
	{
		SAFE_RELEASE(m_ptrRenderTargetView);
		SAFE_DELETE(m_ptrRenderTargetTexture);
	}

}