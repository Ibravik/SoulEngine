/********************************************************************/
/**
* @LC     9/4/2016
* @file   soAPITexture.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soTexture
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma warning(disable : 4005)

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include <D3DX11tex.h>
#include "soDXGIManager.h"
#include "soSampler.h"
#include "soAPITexture.h"
 
/************************************************************************/
/* Definicion de la clase soTexture									    */
/************************************************************************/
namespace SoulSDK
{
	soAPITexture::soAPITexture()
	{
		Connect(std::bind(&soAPITexture::OnStartUp, this));
		m_ptrTexture2D = NULL;
		m_ptrTexture1D = NULL;
		m_ptrSRTexture = NULL;
	}

	soAPITexture::~soAPITexture()
	{
		ShutDown();
	}

	RESULT soAPITexture::OnStartUp()
	{
		return EC_OK;
	}

	void soAPITexture::OnShutDown()
	{
		SAFE_RELEASE(m_ptrTexture1D);
		SAFE_RELEASE(m_ptrTexture2D);
		SAFE_RELEASE(m_ptrSRTexture);
	}

	void soAPITexture::AssignFormat(uint32 _Format)
	{
		switch (_Format)
		{
		case DXGI_FORMAT_UNKNOWN:
			m_Format = UNKNOWN;
			break;
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
			m_Format = R32G32B32A32_TYPELESS;
			break;
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			m_Format = R32G32B32A32_FLOAT;
			break;
		case DXGI_FORMAT_R32G32B32A32_UINT:
			m_Format = R32G32B32A32_UINT;
			break;
		case DXGI_FORMAT_R32G32B32A32_SINT:
			m_Format = R32G32B32A32_SINT;
			break;
		case DXGI_FORMAT_R32G32B32_TYPELESS:
			m_Format = R32G32B32_TYPELESS;
			break;
		case DXGI_FORMAT_R32G32B32_FLOAT:
			m_Format = R32G32B32_FLOAT;
			break;
		case DXGI_FORMAT_R32G32B32_UINT:
			m_Format = R32G32B32_UINT;
			break;
		case DXGI_FORMAT_R32G32B32_SINT:
			m_Format = R32G32B32_SINT;
			break;
		case DXGI_FORMAT_R16G16B16A16_TYPELESS:
			m_Format = R16G16B16A16_TYPELESS;
			break;
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			m_Format = R16G16B16A16_FLOAT;
			break;
		case DXGI_FORMAT_R16G16B16A16_UNORM:
			m_Format = R16G16B16A16_UNORM;
			break;
		case DXGI_FORMAT_R16G16B16A16_UINT:
			m_Format = R16G16B16A16_UINT;
			break;
		case DXGI_FORMAT_R16G16B16A16_SNORM:
			m_Format = R16G16B16A16_SNORM;
			break;
		case DXGI_FORMAT_R16G16B16A16_SINT:
			m_Format = R16G16B16A16_SINT;
			break;
		case DXGI_FORMAT_R32G32_TYPELESS:
			m_Format = R32G32_TYPELESS;
			break;
		case DXGI_FORMAT_R32G32_FLOAT:
			m_Format = R32G32_FLOAT;
			break;
		case DXGI_FORMAT_R32G32_UINT:
			m_Format = R32G32_UINT;
			break;
		case DXGI_FORMAT_R32G32_SINT:
			m_Format = R32G32_SINT;
			break;
		case DXGI_FORMAT_R32G8X24_TYPELESS:
			m_Format = R32G8X24_TYPELESS;
			break;
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			m_Format = D32_FLOAT_S8X24_UINT;
			break;
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			m_Format = R32_FLOAT_X8X24_TYPELESS;
			break;
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			m_Format = X32_TYPELESS_G8X24_UINT;
			break;
		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
			m_Format = R10G10B10A2_TYPELESS;
			break;
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			m_Format = R10G10B10A2_UNORM;
			break;
		case DXGI_FORMAT_R10G10B10A2_UINT:
			m_Format = R10G10B10A2_UINT;
			break;
		case DXGI_FORMAT_R11G11B10_FLOAT:
			m_Format = R11G11B10_FLOAT;
			break;
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
			m_Format = R8G8B8A8_TYPELESS;
			break;
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			m_Format = R8G8B8A8_UNORM;
			break;
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			m_Format = R8G8B8A8_UNORM_SRGB;
			break;
		case DXGI_FORMAT_R8G8B8A8_UINT:
			m_Format = R8G8B8A8_UINT;
			break;
		case DXGI_FORMAT_R8G8B8A8_SNORM:
			m_Format = R8G8B8A8_SNORM;
			break;
		case DXGI_FORMAT_R8G8B8A8_SINT:
			m_Format = R8G8B8A8_SINT;
			break;
		case DXGI_FORMAT_R16G16_TYPELESS:
			m_Format = R16G16_TYPELESS;
			break;
		case DXGI_FORMAT_R16G16_FLOAT:
			m_Format = R16G16_FLOAT;
			break;
		case DXGI_FORMAT_R16G16_UNORM:
			m_Format = R16G16_UNORM;
			break;
		case DXGI_FORMAT_R16G16_UINT:
			m_Format = R16G16_UINT;
			break;
		case DXGI_FORMAT_R16G16_SNORM:
			m_Format = R16G16_SNORM;
			break;
		case DXGI_FORMAT_R16G16_SINT:
			m_Format = R16G16_SINT;
			break;
		case DXGI_FORMAT_R32_TYPELESS:
			m_Format = R32_TYPELESS;
			break;
		case DXGI_FORMAT_D32_FLOAT:
			m_Format = D32_FLOAT;
			break;
		case DXGI_FORMAT_R32_FLOAT:
			m_Format = R32_FLOAT;
			break;
		case DXGI_FORMAT_R32_UINT:
			m_Format = R32_UINT;
			break;
		case DXGI_FORMAT_R32_SINT:
			m_Format = R32_SINT;
			break;
		case DXGI_FORMAT_R24G8_TYPELESS:
			m_Format = R24G8_TYPELESS;
			break;
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			m_Format = D24_UNORM_S8_UINT;
			break;
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
			m_Format = R24_UNORM_X8_TYPELESS;
			break;
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			m_Format = X24_TYPELESS_G8_UINT;
			break;
		case DXGI_FORMAT_R8G8_TYPELESS:
			m_Format = R8G8_TYPELESS;
			break;
		case DXGI_FORMAT_R8G8_UNORM:
			m_Format = R8G8_UNORM;
			break;
		case DXGI_FORMAT_R8G8_UINT:
			m_Format = R8G8_UINT;
			break;
		case DXGI_FORMAT_R8G8_SNORM:
			m_Format = R8G8_SNORM;
			break;
		case DXGI_FORMAT_R8G8_SINT:
			m_Format = R8G8_SINT;
			break;
		case DXGI_FORMAT_R16_TYPELESS:
			m_Format = R16_TYPELESS;
			break;
		case DXGI_FORMAT_R16_FLOAT:
			m_Format = R16_FLOAT;
			break;
		case DXGI_FORMAT_D16_UNORM:
			m_Format = D16_UNORM;
			break;
		case DXGI_FORMAT_R16_UNORM:
			m_Format = R16_UNORM;
			break;
		case DXGI_FORMAT_R16_UINT:
			m_Format = R16_UINT;
			break;
		case DXGI_FORMAT_R16_SNORM:
			m_Format = R16_SNORM;
			break;
		case DXGI_FORMAT_R16_SINT:
			m_Format = R16_SINT;
			break;
		case DXGI_FORMAT_R8_TYPELESS:
			m_Format = R8_TYPELESS;
			break;
		case DXGI_FORMAT_R8_UNORM:
			m_Format = R8_UNORM;
			break;
		case DXGI_FORMAT_R8_UINT:
			m_Format = R8_UINT;
			break;
		case DXGI_FORMAT_R8_SNORM:
			m_Format = R8_SNORM;
			break;
		case DXGI_FORMAT_R8_SINT:
			m_Format = R8_SINT;
			break;
		case DXGI_FORMAT_A8_UNORM:
			m_Format = A8_UNORM;
			break;
		case DXGI_FORMAT_R1_UNORM:
			m_Format = R1_UNORM;
			break;
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
			m_Format = R9G9B9E5_SHAREDEXP;
			break;
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
			m_Format = R8G8_B8G8_UNORM;
			break;
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
			m_Format = G8R8_G8B8_UNORM;
			break;
		case DXGI_FORMAT_BC1_TYPELESS:
			m_Format = BC1_TYPELESS;
			break;
		case DXGI_FORMAT_BC1_UNORM:
			m_Format = BC1_UNORM;
			break;
		case DXGI_FORMAT_BC1_UNORM_SRGB:
			m_Format = BC1_UNORM_SRGB;
			break;
		case DXGI_FORMAT_BC2_TYPELESS:
			m_Format = BC2_TYPELESS;
			break;
		case DXGI_FORMAT_BC2_UNORM:
			m_Format = BC2_UNORM;
			break;
		case DXGI_FORMAT_BC2_UNORM_SRGB:
			m_Format = BC2_UNORM_SRGB;
			break;
		case DXGI_FORMAT_BC3_TYPELESS:
			m_Format = BC3_TYPELESS;
			break;
		case DXGI_FORMAT_BC3_UNORM:
			m_Format = BC3_UNORM;
			break;
		case DXGI_FORMAT_BC3_UNORM_SRGB:
			m_Format = BC3_UNORM_SRGB;
			break;
		case DXGI_FORMAT_BC4_TYPELESS:
			m_Format = BC4_TYPELESS;
			break;
		case DXGI_FORMAT_BC4_UNORM:
			m_Format = BC4_UNORM;
			break;
		case DXGI_FORMAT_BC4_SNORM:
			m_Format = BC4_SNORM;
			break;
		case DXGI_FORMAT_BC5_TYPELESS:
			m_Format = BC5_TYPELESS;
			break;
		case DXGI_FORMAT_BC5_UNORM:
			m_Format = BC5_UNORM;
			break;
		case DXGI_FORMAT_BC5_SNORM:
			m_Format = BC5_SNORM;
			break;
		case DXGI_FORMAT_B5G6R5_UNORM:
			m_Format = B5G6R5_UNORM;
			break;
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			m_Format = B5G5R5A1_UNORM;
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			m_Format = B8G8R8A8_UNORM;
			break;
		case DXGI_FORMAT_B8G8R8X8_UNORM:
			m_Format = B8G8R8X8_UNORM;
			break;
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
			m_Format = R10G10B10_XR_BIAS_A2_UNORM;
			break;
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
			m_Format = B8G8R8A8_TYPELESS;
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			m_Format = B8G8R8A8_UNORM_SRGB;
			break;
		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
			m_Format = B8G8R8X8_TYPELESS;
			break;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			m_Format = B8G8R8X8_UNORM_SRGB;
			break;
		case DXGI_FORMAT_BC6H_TYPELESS:
			m_Format = BC6H_TYPELESS;
			break;
		case DXGI_FORMAT_BC6H_UF16:
			m_Format = BC6H_UF16;
			break;
		case DXGI_FORMAT_BC6H_SF16:
			m_Format = BC6H_SF16;
			break;
		case DXGI_FORMAT_BC7_TYPELESS:
			m_Format = BC7_TYPELESS;
			break;
		case DXGI_FORMAT_BC7_UNORM:
			m_Format = BC7_UNORM;
			break;
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			m_Format = BC7_UNORM_SRGB;
			break;
		default:
			break;
		}
	}

	RESULT soAPITexture::Load(const soString& _FilePath)
	{		
#ifdef UNICODE	
		if (FAIL(D3DX11CreateShaderResourceViewFromFileW(soDXGIManager::Instance().m_Device.m_ptrD3DDevice, _FilePath.c_str(), NULL, NULL, &m_ptrSRTexture, NULL)))
		{
			SOE_OutputLOG("soAPITexture", "Load()", "FAIL to load texture from file.");
			return EC_FALSE;
		}

		D3DX11_IMAGE_INFO info;

		D3DX11GetImageInfoFromFileW(_FilePath.c_str(), NULL, &info, NULL);
#else
		if (FAIL(D3DX11CreateShaderResourceViewFromFile(soDXGIManager::Instance().m_Device.m_ptrD3DDevice, _FilePath.c_str(), NULL, NULL, &m_ptrSRTexture, NULL)))
		{
			SOE_OutputLOG("soAPITexture", "Load()", "FAIL to load texture from file.");
			return EC_FALSE;
		}

		D3DX11_IMAGE_INFO info;

		D3DX11GetImageInfoFromFile(_FilePath.c_str(), NULL, &info, NULL);
#endif
		m_TextureSize.m_Width = info.Width;
		m_TextureSize.m_Height = info.Height;
		AssignFormat(info.Format);

		return EC_OK;
	}

	RESULT soAPITexture::Create(const TextureData & _TextureData)
	{			
		if (_TextureData.TextureType == API_Texture2D)
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.Width = static_cast<uint32>(static_cast<float>(_TextureData.T2DData.TextureSize.m_Width) * _TextureData.T2DData.ScalarViewPortWidth);
			desc.Height = static_cast<uint32>(static_cast<float>(_TextureData.T2DData.TextureSize.m_Height) * _TextureData.T2DData.ScalarViewPortHeigth);
			desc.MipLevels = _TextureData.T2DData.MipLevels;
			desc.ArraySize = _TextureData.T2DData.ArraySize;
			desc.Format = static_cast<DXGI_FORMAT>(_TextureData.T2DData.DXGI_FORMAT);
			desc.SampleDesc.Count = _TextureData.T2DData.SampleDescCount;
			desc.SampleDesc.Quality = _TextureData.T2DData.SampleDescQuality;
			desc.Usage = static_cast<D3D11_USAGE>(_TextureData.T2DData.D3D11_USAGE);
			desc.BindFlags = static_cast<D3D11_BIND_FLAG>(_TextureData.T2DData.D3D11_BIND_FLAG);
			desc.CPUAccessFlags = _TextureData.T2DData.CPUAccesFlag;
			desc.MiscFlags = _TextureData.T2DData.MiscFlags;

			m_TextureSize = _TextureData.T2DData.TextureSize;

			if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateTexture2D(&desc, NULL, &m_ptrTexture2D)))
			{
				SOE_ThrowException("soAPITexture", "Create()", "FAIL to create new custom texture.");
				return EC_FALSE;
			}

			if (_TextureData.T2DData.D3D11_BIND_FLAG != API_BIND_DEPTH_STENCIL)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC SRVD;
				ZeroMemory(&SRVD, sizeof(SRVD));
				SRVD.Format = static_cast<DXGI_FORMAT>(_TextureData.T2DData.DXGI_FORMAT);
				SRVD.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
				SRVD.Texture2D.MostDetailedMip = 0;
				SRVD.Texture2D.MipLevels = _TextureData.T2DData.MipLevels;

				if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateShaderResourceView(m_ptrTexture2D, &SRVD, &m_ptrSRTexture)))
				{
					SOE_ThrowException("soAPITexture", "Create()", "FAIL to create shader resource from new custom texture.");
					return EC_FALSE;
				}
			}

			AssignFormat(desc.Format);

			return EC_OK;
		}

		if (_TextureData.TextureType == API_Texture1D)
		{
			D3D11_TEXTURE1D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.Width = _TextureData.T1DData.Width;			
			desc.MipLevels = _TextureData.T1DData.MipLevels;
			desc.ArraySize = _TextureData.T1DData.ArraySize;
			desc.Format = static_cast<DXGI_FORMAT>(_TextureData.T1DData.DXGI_FORMAT);
			desc.Usage = static_cast<D3D11_USAGE>(_TextureData.T1DData.D3D11_USAGE);
			desc.BindFlags = static_cast<D3D11_BIND_FLAG>(_TextureData.T1DData.D3D11_BIND_FLAG);
			desc.CPUAccessFlags = _TextureData.T1DData.CPUAccesFlag;
			desc.MiscFlags = _TextureData.T1DData.MiscFlags;

			if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateTexture1D(&desc, NULL, &m_ptrTexture1D)))
			{
				SOE_ThrowException("soAPITexture", "Create()", "FAIL to create new custom texture.");
				return EC_FALSE;
			}

			if (_TextureData.T1DData.D3D11_BIND_FLAG != API_BIND_DEPTH_STENCIL)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC SRVD;
				ZeroMemory(&SRVD, sizeof(SRVD));
				SRVD.Format = static_cast<DXGI_FORMAT>(_TextureData.T1DData.DXGI_FORMAT);
				SRVD.ViewDimension = D3D_SRV_DIMENSION_TEXTURE1D;
				SRVD.Texture1D.MostDetailedMip = 0;
				SRVD.Texture1D.MipLevels = _TextureData.T1DData.MipLevels;

				if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DDevice->CreateShaderResourceView(m_ptrTexture1D, &SRVD, &m_ptrSRTexture)))
				{
					SOE_ThrowException("soAPITexture", "Create()", "FAIL to create shader resource from new custom texture.");
					return EC_FALSE;
				}
			}

			return EC_OK;
		}

		
		return EC_FALSE;
	}

	RESULT soAPITexture::Lock1D(uint32& _depthPitch, void** _Data, uint32& _rowPitch)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DContext->Map(m_ptrTexture1D, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource)))
		{
			SOE_ThrowException("soAPITexture", "Lock1D", "FAIL to lock texture.");
			return EC_FALSE;
		}
		*_Data = mappedResource.pData;
		_rowPitch = mappedResource.RowPitch;
		_depthPitch = mappedResource.DepthPitch;

		return EC_OK;
	}

	RESULT soAPITexture::Unlock1D()
	{
		soDXGIManager::Instance().m_Device.m_ptrD3DContext->Unmap(m_ptrTexture1D, 0);
		return EC_OK;
	}

	RESULT soAPITexture::Lock2D(uint32& _depthPitch, void ** _pixels, uint32& _rowPitch)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		if (FAIL(soDXGIManager::Instance().m_Device.m_ptrD3DContext->Map(m_ptrTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		{
			SOE_ThrowException("soAPITexture", "Lock2D()", "FAIL to lock texture.");
			return EC_FALSE;
		}
		*_pixels = mappedResource.pData;
		_rowPitch = mappedResource.RowPitch;
		_depthPitch = mappedResource.DepthPitch;

		return EC_OK;
	}

	RESULT soAPITexture::Unlock2D()
	{
		soDXGIManager::Instance().m_Device.m_ptrD3DContext->Unmap(m_ptrTexture2D, 0);
		return EC_OK;
	}
}
#pragma warning(default : 4005)