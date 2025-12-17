/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soFragmentShader.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soFragmentShader
* @bug	  No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soBuffer.h"
#include "soFragmentShader.h"

/************************************************************************/
/* Definicion de la clase soFragmentShader	                            */
/************************************************************************/
namespace SoulSDK
{
	soFragmentShader::soFragmentShader()
	{
		Connect(std::bind(&soFragmentShader::OnStartUp, this, std::placeholders::_1));
		m_ptrPixelShader = NULL;
		m_ptrShaderBlob = NULL;
		m_ptrBuffer = NULL;
	}
	
	soFragmentShader::~soFragmentShader()
	{
		ShutDown();
	}

	RESULT soFragmentShader::OnStartUp(const ShaderData& _FragmentShaderData)
	{
		if (FAIL(CompileShaderFromFile(_FragmentShaderData)))
		{
			SOE_ThrowException("soFragmentShader", "OnStartUp()", "FAIL to compile PixelShader");
			return EC_FALSE;
		}

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreatePixelShader(m_ptrShaderBlob->GetBufferPointer(), m_ptrShaderBlob->GetBufferSize(), NULL, &m_ptrPixelShader)))
		{
			SOE_ThrowException("soFragmentShader", "OnStartUp()", "FAIL to create shader resource from compiled shader.");
			return EC_FALSE;
		}

		if (FAIL(CreateConstantBuffer()))
		{
			SOE_ThrowException("soFragmentShader", "OnStartUp()", "FAIL to create pixel shader constant buffer.");
			return EC_FALSE;
		}

		return EC_OK;
	}

	void soFragmentShader::OnShutDown()
	{
		SAFE_RELEASE(m_ptrShaderBlob);
		SAFE_RELEASE(m_ptrPixelShader);
		SAFE_DELETE_ARRAY(m_ptrBuffer);
	}
}