/********************************************************************/
/**
* @LC	  10/25/2016
* @file   soStructuredBuffer.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/25/2016
* @brief  Implementacion de la clase soStructuredBuffer. Este tipo de 
		  buffer se usa en el Compute shader 
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"
#include "soDXGIManager.h"

namespace SoulSDK 
{
	/************************************************************************/
	/* Declaracion de la clase soStructuredBuffer						    */
	/************************************************************************/
	template <class T>
	class soStructuredBuffer : public soStandardClass<const StructuredBufferData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soStructuredBuffer()												/*!< Constructor Standard */
		{
			Connect(std::bind(&soStructuredBuffer::OnStartUp, this, std::placeholders::_1));
			m_Buffer = NULL;
			m_ShaderResource = NULL;
			m_UnorderedAccess = NULL;
		}

		~soStructuredBuffer()												/*!< Destructor */
		{
			ShutDown();
		}

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private: 
		uint32 m_Elements;													/*!< Numero de elementos */

	public:
		ID3D11Buffer* m_Buffer;												/*!< Interfaz de acceso como buffer */
		ID3D11ShaderResourceView* m_ShaderResource;							/*!< Interfaz de acceso como recurso de shader */
		ID3D11UnorderedAccessView* m_UnorderedAccess;						/*!< Interfaz de acceso sin orden para CS */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const StructuredBufferData& _StructuredBufferData)	/*!< Inicializa la clase segun las especificaciones recibidas */
		{
			CD3D11_BUFFER_DESC desc(sizeof(T) * _StructuredBufferData.Elements, _StructuredBufferData.D3D11_BIND_FLAG,
				_StructuredBufferData.Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT,
				_StructuredBufferData.Dynamic ? D3D11_CPU_ACCESS_WRITE : 0,
				D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
				sizeof(T));

			soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateBuffer(&desc, 0, &m_Buffer);

			if (_StructuredBufferData.D3D11_BIND_FLAG & D3D11_BIND_UNORDERED_ACCESS) {
				soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateUnorderedAccessView(m_Buffer, 0, &m_UnorderedAccess);
			}

			if (_StructuredBufferData.D3D11_BIND_FLAG & D3D11_BIND_SHADER_RESOURCE) {
				soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateShaderResourceView(m_Buffer, 0, &m_ShaderResource);
			}

			m_Elements = _StructuredBufferData.Elements;

			return EC_OK;
		}

		virtual void OnShutDown() override									/*!< Libera los recursos solicitados por la clase para su iniciacion */
		{
			SAFE_RELEASE(m_Buffer);
			SAFE_RELEASE(m_ShaderResource);
			SAFE_RELEASE(m_UnorderedAccess);
		}

	public:
		T* MapDiscard()														/*!< Actualiza los datos del buffer (Solo Dynamicos) */
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			soDXGIManager::Instance().m_Device.m_ptrD3DContext->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			return static_cast<T*>(mappedResource.pData);
		}

		void Unmap()														/*!< Actualiza los datos del buffer (Solo Dynamicos) */
		{
			soDXGIManager::Instance().m_Device.m_ptrD3DContext->Unmap(m_Buffer, 0);
		}
	};
}