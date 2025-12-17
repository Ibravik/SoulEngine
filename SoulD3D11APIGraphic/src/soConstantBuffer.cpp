/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soConstantBuffer.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/14/2016
* @brief  Implementacion de la clase soConstantBuffer
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soBuffer.h"
#include "soDXGIManager.h"
#include "soConstantBuffer.h"

/************************************************************************/
/* Definicion de la clase soConstantBuffer				                */
/************************************************************************/
namespace SoulSDK
{
	soConstantBuffer::soConstantBuffer()
	{
		Connect(std::bind(&soConstantBuffer::OnStartUp, this, std::placeholders::_1));
		m_ptrConstantBuffer = NULL;
	}

	soConstantBuffer::~soConstantBuffer()
	{
		ShutDown();
	}

	RESULT soConstantBuffer::OnStartUp(const ConstantBufferData& _ConstantBufferData)
	{		
		if (_ConstantBufferData.ByteWidth == 0)
		{
			SOE_OutputLOG("soConstantBuffer", "OnStartUp()", "Empty data container.");
			return EC_FALSE;
		}

		BufferData bd;
		bd.D3D11_USAGE = API_USAGE_DYNAMIC;
		bd.ByteWidth = _ConstantBufferData.ByteWidth;
		bd.D3D11_BIND_FLAG = _ConstantBufferData.D3D11_BIND_FLAG;
		bd.CPUAccesFlag = API_CPU_ACCESS_WRITE;
		bd.D3D11_SUBRESOURCE_DATA.pSysMem = _ConstantBufferData.D3D11_SUBRESOURCE_DATA.pSysMem;

		m_ptrConstantBuffer = new soBuffer;
		if (FAIL(m_ptrConstantBuffer->StartUp(bd)))
		{
			SOE_OutputLOG("soConstantBuffer", "OnStartUp()", "FAIL to create constant buffer.");
			return EC_FALSE;
		}

		m_ptrData = bd.D3D11_SUBRESOURCE_DATA.pSysMem;
		m_ByteWidth = bd.ByteWidth;

		return EC_OK;
	}

	void soConstantBuffer::OnShutDown()
	{
		SAFE_DELETE(m_ptrConstantBuffer);
	}
	
	void soConstantBuffer::UpdateBuffer(const void* _ptrData)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		soDXGIManager::Instance().m_Device.m_ptrD3DContext->Map(m_ptrConstantBuffer->m_ptrBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	
		if (_ptrData == NULL)
		{
			memcpy(ms.pData, m_ptrData, m_ByteWidth);
		}
		else
		{
			memcpy(ms.pData, _ptrData, m_ByteWidth);
		}
		
		soDXGIManager::Instance().m_Device.m_ptrD3DContext->Unmap(m_ptrConstantBuffer->m_ptrBuffer, 0);
	}

	void* soConstantBuffer::MapDiscard()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		soDXGIManager::Instance().m_Device.m_ptrD3DContext->Map(m_ptrConstantBuffer->m_ptrBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		return mappedResource.pData;
	}

	void soConstantBuffer::Unmap()
	{
		soDXGIManager::Instance().m_Device.m_ptrD3DContext->Unmap(m_ptrConstantBuffer->m_ptrBuffer, 0);
	}

}