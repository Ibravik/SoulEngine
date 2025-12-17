/********************************************************************/
/**
* @LC	  9/13/2016
* @file   soBuffer.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soBuffer
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soBuffer.h"

/************************************************************************/
/* Definicion de la clase soConstantBuffer							    */
/************************************************************************/
namespace SoulSDK
{
	soBuffer::soBuffer()
	{
		Connect(std::bind(&soBuffer::OnStartUp, this, std::placeholders::_1));
		m_ptrBuffer = NULL;
	}
	
	soBuffer::~soBuffer()
	{
		ShutDown();
	}

	RESULT soBuffer::OnStartUp(const BufferData& _BufferData)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = static_cast<D3D11_USAGE>(_BufferData.D3D11_USAGE);
		bd.ByteWidth = _BufferData.ByteWidth;
		bd.BindFlags = static_cast<D3D11_BIND_FLAG>(_BufferData.D3D11_BIND_FLAG);
		bd.CPUAccessFlags = _BufferData.CPUAccesFlag;

		D3D11_SUBRESOURCE_DATA* InitData = NULL;
		if (_BufferData.D3D11_SUBRESOURCE_DATA.pSysMem != NULL)
		{
			InitData = SOE_NEW D3D11_SUBRESOURCE_DATA;
			ZeroMemory(&*InitData, sizeof(InitData));
			InitData->pSysMem = _BufferData.D3D11_SUBRESOURCE_DATA.pSysMem;
			InitData->SysMemPitch = _BufferData.D3D11_SUBRESOURCE_DATA.SysMemPitch;
			InitData->SysMemSlicePitch = _BufferData.D3D11_SUBRESOURCE_DATA.SysMemSlicePitch;
		}

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateBuffer(&bd, &*InitData, &m_ptrBuffer)))
		{
			SOE_ThrowException("soBuffer", "OnStartUp()", "FAIL to create constant buffer.");
			SAFE_DELETE(InitData);
			return EC_FALSE;
		}

		SAFE_DELETE(InitData);
		return EC_OK;
	}

	void soBuffer::OnShutDown()
	{
		SAFE_RELEASE(m_ptrBuffer);
	}
}