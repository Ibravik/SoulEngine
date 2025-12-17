/********************************************************************/
/**
* @LC:	  10/8/2016
* @file   soImputLayout.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soImputLayout
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include <d3dcompiler.h>
#include "soDXGIManager.h"
#include "soInputLayout.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soImputLayout		                            */
	/************************************************************************/
	soInputLayout::soInputLayout()
	{
		Connect(std::bind(&soInputLayout::OnStartUp, this, std::placeholders::_1));
		m_ptrInputLayoutDescArray = NULL;	/*!< Lista de elementos del descriptor */
		m_InputLayoutDescNumber = 0;		/*!< Numero de elementos del descriptor */
		m_ptrInputLayOut = NULL;			/*!< InputLayout */
	}

	soInputLayout::~soInputLayout()
	{
		ShutDown();
	}

	RESULT soInputLayout::CreateInputLayoutDescFrsoVertexShaderSignature(ID3DBlob* _pShaderBlob)
	{
		// Reflejamos la informacion del shader
		ID3D11ShaderReflection* pVertexShaderReflection = NULL;
		if (FAIL(D3DReflect(_pShaderBlob->GetBufferPointer(), _pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
		{
			SOE_OutputLOG("soInputLayout", "CreateInputLayoutDescFrsoVertexShaderSignature()", "FAIL shader blob reflection");
			return EC_FALSE;
		}

		// Tomamos la informacion de desc del shader
		D3D11_SHADER_DESC ShaderDesc;
		pVertexShaderReflection->GetDesc(&ShaderDesc);
		m_InputLayoutDescNumber = ShaderDesc.InputParameters;

		//Creamos el arreglo lineal que contendra la descripcion
		m_ptrInputLayoutDescArray = SOE_NEW D3D11_INPUT_ELEMENT_DESC[ShaderDesc.InputParameters];

		for (unsigned int i = 0; i < ShaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC ParamDesc;
			pVertexShaderReflection->GetInputParameterDesc(i, &ParamDesc);

			// rellenamos los elementos de inputs del descriptor
			D3D11_INPUT_ELEMENT_DESC ElementDesc;
			ElementDesc.SemanticName = ParamDesc.SemanticName;
			ElementDesc.SemanticIndex = ParamDesc.SemanticIndex;
			ElementDesc.InputSlot = 0;
			ElementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			ElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			ElementDesc.InstanceDataStepRate = 0;

			// Determinamos el formato de DXGI
			if (ParamDesc.Mask == 1)
			{
				if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ElementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ElementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (ParamDesc.Mask <= 3)
			{
				if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (ParamDesc.Mask <= 7)
			{
				if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (ParamDesc.Mask <= 15)
			{
				if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			if (ElementDesc.SemanticName[0] == 'C')
			{
				ElementDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			//Guardamos el elemento del Desc
			m_ptrInputLayoutDescArray[i] = ElementDesc;
		}
		return EC_OK;
	}

	void soInputLayout::OnShutDown()
	{
		SAFE_DELETE_ARRAY(m_ptrInputLayoutDescArray);
		SAFE_RELEASE(m_ptrInputLayOut);
	}

	RESULT soInputLayout::OnStartUp(ID3DBlob* _pShaderBlob)
	{			
		if (FAIL(CreateInputLayoutDescFrsoVertexShaderSignature(_pShaderBlob)))
		{
			SOE_OutputLOG("soInputLayout", "OnStartUp()", "FAIL to create Input lauyout descritpion.");
			return EC_FALSE;
		}

		if (m_ptrInputLayoutDescArray != NULL)
		{
			//Creamos el input layout a partr de la descripcion creada del blob del shader
			if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateInputLayout(m_ptrInputLayoutDescArray, m_InputLayoutDescNumber, _pShaderBlob->GetBufferPointer(), _pShaderBlob->GetBufferSize(), &m_ptrInputLayOut)))
			{
				SOE_OutputLOG("soInputLayout", "OnStartUp()", "FAIL to create Input lauyout shader resource.");
				return EC_FALSE;
			}
		}

		return EC_OK;
	}
}