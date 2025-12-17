/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soComputeShader.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/15/2016
* @brief  Implementacion de la clase soStaticVertexBuffer
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soBuffer.h"
#include "soComputeShader.h"

/************************************************************************/
/* Definicion de la clase soComputeShader				                */
/************************************************************************/
namespace SoulSDK
{
	soComputeShader::soComputeShader()
	{
		Connect(std::bind(&soComputeShader::OnStartUp, this, std::placeholders::_1));
		m_ptrComputeShader = NULL;
	}

	soComputeShader::~soComputeShader()
	{
		ShutDown();
	}

	RESULT soComputeShader::OnStartUp(const ShaderData& _ShaderData)
	{		
		if (FAIL(CompileShaderFromFile(_ShaderData)))
		{
			SOE_ThrowException("soComputeShader", "OnStartUp()", "FAIL to compile ComputeShader");
			return EC_FALSE;
		}

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateComputeShader(m_ptrShaderBlob->GetBufferPointer(), m_ptrShaderBlob->GetBufferSize(), NULL, &m_ptrComputeShader)))
		{
			SOE_ThrowException("soComputeShader", "OnStartUp()", "FAIL to create shader resource from compiled shader.");
			return EC_FALSE;
		}

		if (FAIL(CreateConstantBuffer()))
		{
			SOE_ThrowException("soComputeShader", "OnStartUp()", "FAIL to create computer shader constant buffer.");
			return EC_FALSE;
		}

		return EC_OK;
	}
	
	void soComputeShader::OnShutDown()
	{
		SAFE_RELEASE(m_ptrShaderBlob);
		SAFE_RELEASE(m_ptrComputeShader);
		SAFE_DELETE_ARRAY(m_ptrBuffer);
	}

}