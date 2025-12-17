/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soVertexShader.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soVertexShader
* @bug	  No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soInputLayout.h"
#include "soBuffer.h"
#include "soVertexShader.h"

/************************************************************************/
/* Definicion de la clase soVertexShader	                            */
/************************************************************************/
namespace SoulSDK
{
	soVertexShader::soVertexShader()
	{
		Connect(std::bind(&soVertexShader::OnStartUp, this, std::placeholders::_1));
		m_ptrVertexShader = NULL;
	}

	soVertexShader::~soVertexShader()
	{
		ShutDown();
	}	

	RESULT soVertexShader::OnStartUp(const ShaderData& _VertexShaderData)
	{		
		if (FAIL(CompileShaderFromFile(_VertexShaderData)))
		{
			SOE_ThrowException("soVertexShader", "OnStartUp()", "FAIL to compile VertexShader");
			return EC_FALSE;
		}

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateVertexShader(m_ptrShaderBlob->GetBufferPointer(), m_ptrShaderBlob->GetBufferSize(), NULL, &m_ptrVertexShader)))
		{
			SOE_ThrowException("soVertexShader", "OnStartUp()", "FAIL to create shader resource from compiled shader.");
			return EC_FALSE;
		}

		if (FAIL(m_InputLayout.StartUp(m_ptrShaderBlob)))
		{
			SOE_ThrowException("soVertexShader", "OnStartUp()", "FAIL to create input layout.");
			return EC_FALSE;
		}

		if (FAIL(CreateConstantBuffer()))
		{
			SOE_ThrowException("soVertexShader", "OnStartUp()", "FAIL to create vertex shader constant buffer.");
			return EC_FALSE;
		}

		return EC_OK;
	}

	void soVertexShader::OnShutDown()
	{
		SAFE_RELEASE(m_ptrShaderBlob);
		SAFE_RELEASE(m_ptrVertexShader);
		SAFE_DELETE_ARRAY(m_ptrBuffer);
	}	
}

#pragma warning(default : 4005)