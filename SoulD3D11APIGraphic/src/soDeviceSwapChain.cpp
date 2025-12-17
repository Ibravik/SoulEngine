/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soSwapChain.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/29/2015
* @brief  Implementacion de la clase soSwapChain
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soDXGIManager.h"
#include "soDeviceSwapChain.h"

/************************************************************************/
/* Definicion de la clase soSampler										*/
/************************************************************************/
namespace SoulSDK
{
	soDeviceSwapChain::soDeviceSwapChain()
	{
		Connect(std::bind(&soDeviceSwapChain::OnStartUp, this, std::placeholders::_1));
		m_ptrSwapChain = NULL;
		m_ptrD3DDevice = NULL;												 	
		m_ptrD3DContext = NULL;
	}
	
	soDeviceSwapChain::~soDeviceSwapChain()
	{
		ShutDown();
	}

	DXGI_RATIONAL soDeviceSwapChain::QueryRefreshRate(const soRect& _ScreenDimentions, const bool& _vSync)
	{
		DXGI_RATIONAL refreshRate = { 60, 1 };
		if (_vSync)
		{
			IDXGIFactory* factory;
			IDXGIAdapter* adapter;
			IDXGIOutput* adapterOutput;
			DXGI_MODE_DESC* displayModeList;

			// Create a DirectX graphics interface factory.
			if (FAIL(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
			{
				SOE_ThrowException("soDeviceSwapChain", "QueryRefreshRate()", "Could not create DXGIFactory instance.");
			}

			if (FAIL(factory->EnumAdapters(0, &adapter)))
			{
				SOE_ThrowException("soDeviceSwapChain", "QueryRefreshRate()", "FAIL to enumerate adapters.");
			}

			if (FAIL(adapter->EnumOutputs(0, &adapterOutput)))
			{
				SOE_ThrowException("soDeviceSwapChain", "QueryRefreshRate()", "FAIL to enumerate adapter outputs.");
			}

			uint32 numDisplayModes;
			if (FAIL(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr)))
			{
				SOE_ThrowException("soDeviceSwapChain", "QueryRefreshRate()", "FAIL to query display mode list.");
			}

			displayModeList = new DXGI_MODE_DESC[numDisplayModes];
			if (FAIL(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList)))
			{
				SOE_ThrowException("soDeviceSwapChain", "QueryRefreshRate()", "FAIL to query display mode list.");
			}

			// Now store the refresh rate of the monitor that matches the width and height of the requested screen.
			for (uint32 i = 0; i < numDisplayModes; ++i)
			{
				if (displayModeList[i].Width == _ScreenDimentions.m_Width && displayModeList[i].Height == _ScreenDimentions.m_Height)
				{
					refreshRate = displayModeList[i].RefreshRate;
				}
			}

			SAFE_DELETE_ARRAY(displayModeList);
			SAFE_RELEASE(adapterOutput);
			SAFE_RELEASE(adapter);
			SAFE_RELEASE(factory);
		}

		return refreshRate;
	}

	void soDeviceSwapChain::PerFeatureLevel()
	{
		switch (m_FeatureLevel)
		{
		case D3D_FEATURE_LEVEL_11_0:
			m_szVSProfile = TEXT("vs_5_0");
			m_szPSProfile = TEXT("ps_5_0");
			m_szCSProfile = TEXT("cs_5_0");
			break;

		case D3D_FEATURE_LEVEL_10_1:
			m_szVSProfile = TEXT("vs_4_1");
			m_szPSProfile = TEXT("ps_4_1");
			m_szCSProfile = TEXT("cs_4_1");
			break;

		case D3D_FEATURE_LEVEL_10_0:
			m_szVSProfile = TEXT("vs_4_0");
			m_szPSProfile = TEXT("ps_4_0");
			m_szCSProfile = TEXT("cs_4_0");
			break;

		case D3D_FEATURE_LEVEL_9_3:
			m_szVSProfile = TEXT("vs_4_0_level_9_3");
			m_szPSProfile = TEXT("ps_4_0_level_9_3");
			m_szCSProfile = TEXT("");
			break;

		case D3D_FEATURE_LEVEL_9_2:
			m_szVSProfile = TEXT("vs_4_0_level_9_1");
			m_szPSProfile = TEXT("ps_4_0_level_9_1");
			m_szCSProfile = TEXT("");
			break;

		case D3D_FEATURE_LEVEL_9_1:
			m_szVSProfile = TEXT("vs_4_0_level_9_1");
			m_szPSProfile = TEXT("ps_4_0_level_9_1");
			m_szCSProfile = TEXT("");
			break;
		}
	}

	RESULT soDeviceSwapChain::OnStartUp(const SwapChainData& _SwapChainData)
	{		
		//Tipos de drivers disponibles para crear el device
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		uint32 numDriverTypes = ARRAYSIZE(driverTypes);

		//Compatibilidad del device con directx
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};
		uint32 numFeatureLevels = ARRAYSIZE(featureLevels);

		uint32 CreateDeviceFlags = 0;
#if _DEBUG
		CreateDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		//Caracteristicas del swapchain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));

		sd.BufferCount = _SwapChainData.BufferCount;
		sd.BufferDesc.Width = _SwapChainData.ScreenDimentions.m_Width;
		sd.BufferDesc.Height = _SwapChainData.ScreenDimentions.m_Height;
		sd.BufferDesc.Format = static_cast<DXGI_FORMAT>(_SwapChainData.DXGI_FORMAT);
		sd.BufferDesc.RefreshRate  = QueryRefreshRate(_SwapChainData.ScreenDimentions, _SwapChainData.vSync);
		sd.BufferUsage = static_cast<DXGI_USAGE>(_SwapChainData.DXGI_USAGE);
		sd.OutputWindow = _SwapChainData.OutputHWND;
		sd.SampleDesc.Count = _SwapChainData.SampleDescCount;
		sd.SampleDesc.Quality = _SwapChainData.SampleDescQuality;
		sd.Windowed = _SwapChainData.Windowed;
		sd.SwapEffect = static_cast<DXGI_SWAP_EFFECT>(_SwapChainData.DXGI_SWAP_EFFECT);

		RESULT HandleResult;
		for (unsigned int driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			m_DriverType = driverTypes[driverTypeIndex];
			HandleResult = D3D11CreateDeviceAndSwapChain(NULL, m_DriverType, NULL, CreateDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &m_ptrSwapChain, &m_ptrD3DDevice, &m_FeatureLevel, &m_ptrD3DContext);
			if (SUCCEEDED(HandleResult))
			{
				break;
			}
		}

		if (FAIL(HandleResult))
		{
			SOE_ThrowException("soDeviceSwapChain", "OnStartUp()", "FAIL to create device, context and swap chain.");
			return EC_FALSE;
		}

		PerFeatureLevel();

		return EC_OK;
	}

	void soDeviceSwapChain::OnShutDown()
	{
		if (m_ptrD3DContext) m_ptrD3DContext->ClearState();
		SAFE_RELEASE(m_ptrSwapChain);
		SAFE_RELEASE(m_ptrD3DDevice);
		SAFE_RELEASE(m_ptrD3DContext);
	}
}