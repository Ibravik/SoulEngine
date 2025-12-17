/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soSampler.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/29/2015
* @brief  Implementacion de la clase soSampler
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soSampler.h"

/************************************************************************/
/* Definicion de la clase soSampler										*/
/************************************************************************/
namespace SoulSDK
{
	soSampler::soSampler()
	{
		Connect(std::bind(&soSampler::OnStartUp, this, std::placeholders::_1));
		m_ptrSampler = NULL;
	}

	soSampler::~soSampler()
	{
		ShutDown();
	}

	RESULT soSampler::OnStartUp(const SamplerData& _SamplerData)
	{		
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));

		sampDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(_SamplerData.D3D11_TEXTURE_ADDRESS_MODE_U);
		sampDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(_SamplerData.D3D11_TEXTURE_ADDRESS_MODE_V);
		sampDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(_SamplerData.D3D11_TEXTURE_ADDRESS_MODE_W);
		sampDesc.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(_SamplerData.D3D11_COMPARISON_FUNC);
		sampDesc.Filter = static_cast<D3D11_FILTER>(_SamplerData.D3D11_FILTER);
		sampDesc.MaxAnisotropy = _SamplerData.MaxAnisotropy;
		sampDesc.MinLOD = _SamplerData.MinLOD;
		sampDesc.MaxLOD = _SamplerData.MaxLOD;
		sampDesc.MipLODBias = _SamplerData.MipLODBias;

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateSamplerState(&sampDesc, &m_ptrSampler)))
		{
			SOE_OutputLOG("soSampler", "OnStartUp()", "FAIL to create Sampler.");
			return EC_FALSE;
		}

		return EC_OK;
	}

	void soSampler::OnShutDown()
	{
		SAFE_RELEASE(m_ptrSampler);
	}

}