/************************************************************************/
/**
* @LC	  9/15/2016
* @file   soShader.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/15/2016
* @brief  Declaration of class soShader. Este es un template de funcionalidad
		  para los diferentes tipos de shaders disponibles en la api
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"
#include "soBuffer.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaración de la clase base soVertexShader                          */
	/************************************************************************/
	class soShader : public soStandardClass<const ShaderData&>
	{
		/************************************************************************************************************************/
		/* Constructores y destructor																							*/
		/************************************************************************************************************************/
	public:
		soShader()														/*!< Constructor Standar */
		{
			m_ptrShaderBlob = NULL;
			m_ptrBuffer = NULL;
		}

		~soShader()														/*!< Destructor */
		{
		}

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	protected:
		ID3DBlob* m_ptrShaderBlob;										/*!< Informacion Compilada del Shader */

	public:
		soBuffer* m_ptrBuffer;											/*!< Constant Buffer para los datos pedidos por el shader. */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	protected:
		RESULT CompileShaderFromFile(const ShaderData& _ShaderData)		/*!< Compila el Shader */
		{
			uint32 dwShaderFlags = _ShaderData.ShaderFlags;
#if defined( DEBUG ) || defined( _DEBUG )
			// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
			// Setting this flag improves the shader debugging experience, but still allows 
			// the shaders to be optimized and to run exactly the way they will run in 
			// the release configuration of this program.
			dwShaderFlags |= D3DCOMPILE_DEBUG;
			dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

			ID3DBlob* pErrorBlob;

			if (_ShaderData.FilePath.size() == 0)
			{
				SOE_ThrowException("soShader", "CompileShaderFromFile()", "Empty file path.");
				return EC_FALSE;
			}
#ifdef UNICODE		
			if (FAIL(D3DX11CompileFromFileW(_ShaderData.FilePath.c_str(), NULL, NULL, ws2s(_ShaderData.S_EntryPoint).c_str(), ws2s(_ShaderData.S_Profile).c_str(), dwShaderFlags, 0, NULL, &m_ptrShaderBlob, &pErrorBlob, NULL)))
			{
				if (pErrorBlob != NULL)
				{
					SOE_ThrowException("soShader", "CompileShaderFromFile()", (char*)pErrorBlob->GetBufferPointer());
					SAFE_RELEASE(pErrorBlob);
				}
				return EC_FALSE;
			}
#else
			if (FAIL(D3DX11CompileFromFile(_ShaderData.FilePath.c_str(), NULL, NULL, _ShaderData.S_EntryPoint.c_str(), _ShaderData.S_Profile.c_str(), dwShaderFlags, 0, NULL, &m_ptrShaderBlob, &pErrorBlob, NULL)))
			{
				if (pErrorBlob != NULL)
				{
					SOE_ThrowException("soShader", "CompileShaderFromFile()", (char*)pErrorBlob->GetBufferPointer());
					SAFE_RELEASE(pErrorBlob);
				}
				return EC_FALSE;
			}
#endif
			SAFE_RELEASE(pErrorBlob);

			return EC_OK;
		}

		RESULT CreateConstantBuffer()									/*!< Crea el constant buffer necesario para */
		{
			if (m_ptrShaderBlob == NULL)
			{
				SOE_ThrowException("soShader", "CreateConstantBuffer()", "Invalid shaderblob file");
				return EC_FALSE;
			}

			// Reflejamos la informacion del shader
			ID3D11ShaderReflection* pReflection = NULL;
			if (FAIL(D3DReflect(m_ptrShaderBlob->GetBufferPointer(), m_ptrShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflection)))
			{
				SOE_ThrowException("soShader", "CreateConstantBuffer()", "Invalid shaderblob file");
				return EC_FALSE;
			}

			// Tomamos la informacion de desc del shader
			D3D11_SHADER_DESC ShaderDesc;
			pReflection->GetDesc(&ShaderDesc);

			//Creamos el arreglo lineal que contendra la descripcion
			m_ptrBuffer = SOE_NEW soBuffer[ShaderDesc.ConstantBuffers];

			for (unsigned int i = 0; i < ShaderDesc.ConstantBuffers; i++)
			{
				ID3D11ShaderReflectionConstantBuffer* ConstantBuffer = pReflection->GetConstantBufferByIndex(i);
				D3D11_SHADER_BUFFER_DESC ConstantBufferDesc;
				ConstantBuffer->GetDesc(&ConstantBufferDesc);

				//Descripcion general de todos los buffers, solo cambia el byte width para cada uno
				BufferData BInfo;
				BInfo.D3D11_USAGE = API_USAGE_DEFAULT;
				BInfo.D3D11_BIND_FLAG = API_BIND_CONSTANT_BUFFER;
				BInfo.CPUAccesFlag = 0;
				BInfo.ByteWidth = ConstantBufferDesc.Size;

				if (BInfo.ByteWidth != 8 && BInfo.ByteWidth != 4)
				{
					if (FAIL(m_ptrBuffer[i].StartUp(BInfo)))
					{
						SOE_ThrowException("soShader", "CreateConstantBuffer()", "FAIL to create constant buffer");
						return EC_FALSE;
					}
				}
			}
			return EC_OK;
		}
	};
}