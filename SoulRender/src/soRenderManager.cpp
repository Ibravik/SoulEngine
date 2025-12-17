/********************************************************************/
/**
* LC	  9/20/2016
* @file   soRenderManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soRenderBeast
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soCamera.h"
#include "soCameraManager.h"
#include "soTechnique.h"
#include "soRenderPass.h"
#include "soWorldManager.h"
#include "ShaderDefines.h"
#include "soRenderManager.h"
#include <soFont.h>

/************************************************************************/
/* Definicion de la clase soCamera									    */
/************************************************************************/
namespace SoulSDK
{
	soRenderManager::soRenderManager()
	{
		Connect(std::bind(&soRenderManager::OnStartUp, this));
		m_EfectList.clear();
		m_SceneNodeList = NULL;
		m_SelectedNode = NULL;
		m_dynamicVertexBufferFont = NULL;
#ifdef LIGHT_TEST
		m_ActiveLights = 0;
#endif // LIGHT_TEST	
	}
	
	soRenderManager::~soRenderManager()
	{
		m_dynamicVertexBufferFont->Release();
		m_dynamicVertexBufferFont = NULL;
	}

	RESULT soRenderManager::LoadRenderResources()
	{
		//Carga de textura default para seleccion de modelos 
		ResourceParameters RP;
		RP.FilePath = TEXT("Resources\\Textures\\Selection.dds");
		RP.ResourceName = TEXT("Selection");
		RP.ResourceType = RT_Texture;

		m_SelectionTexture = dynamic_cast<soTexture*>(soResourceManager::InstancePtr()->Load(RP));

		if (m_SelectionTexture == NULL)
		{
			SOE_OutputLOG("soRenderManager", "OnStartUp()", "m_SelectionTexture creation error.");
			return EC_FALSE;
		}

		RP.FilePath = TEXT("Resources\\Textures\\MeshSelection.dds");
		RP.ResourceName = TEXT("MeshSelection");
		RP.ResourceType = RT_Texture;

		m_MeshSelectionTexture = dynamic_cast<soTexture*>(soResourceManager::InstancePtr()->Load(RP));

		if (m_MeshSelectionTexture == NULL)
		{
			SOE_OutputLOG("soRenderManager", "OnStartUp()", "m_MeshSelectionTexture creation error.");
			return EC_FALSE;
		}

		//Carga de textura para SkyBox 
		RP.FilePath = TEXT("Resources\\Textures\\Clouds.dds");
		RP.ResourceName = TEXT("Clouds");
		RP.ResourceType = RT_Texture;

		m_SkyBoxTexture = dynamic_cast<soTexture*>(soResourceManager::InstancePtr()->Load(RP));

		if (m_SkyBoxTexture == NULL)
		{
			SOE_OutputLOG("soRenderManager", "OnStartUp()", "m_SkyBoxTexture creation error.");
			return EC_FALSE;
		}

		//Carga del cuadro alineado a la pantalla		
		RP.FilePath = TEXT("Resources\\Models\\QuadPerron.obj");
		RP.ResourceName = TEXT("ScreenAlignedQuad");
		RP.ResourceType = RT_3DStaticModel;
		m_ScreenAlignedQuad = dynamic_cast<soStaticModel*>(soResourceManager::InstancePtr()->Load(RP));

		if (m_ScreenAlignedQuad == NULL)
		{
			SOE_OutputLOG("soRenderManager", "OnStartUp()", "ScreenAlignedQuad creation error.");
			return EC_FALSE;
		}

		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_2);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_3);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_4);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_5);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_6);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_7);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_8);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_9);
		m_ScreenAlignedQuad->Update();

		//Carga de SkyBox Mesh
		RP.FilePath = TEXT("Resources\\Models\\SkyboxMesh.obj");
		RP.ResourceName = TEXT("SkyboxMesh");
		RP.ResourceType = RT_3DStaticModel;
		m_SkyBoxMesh = dynamic_cast<soStaticModel*>(soResourceManager::InstancePtr()->Load(RP));

		if (m_SkyBoxMesh == NULL)
		{
			SOE_OutputLOG("soRenderManager", "OnStartUp()", "SkyboxMesh creation error.");
			return EC_FALSE;
		}

		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(m_SkyBoxTexture, Slot_0);
		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);
		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_2);
		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_3);
		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_4);
		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_5);
		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_6);
		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_7);
		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_8);
		soMaterialManager::Instance().GetMaterialByID(m_SkyBoxMesh->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_9);
		m_SkyBoxMesh->Update();

		//Carga de textura para fuentes
		RP.FilePath = TEXT("Resources\\Textures\\Fonts\\CourierNewRegular.dds");
		RP.ResourceName = TEXT("Font");
		RP.ResourceType = RT_Texture;

		m_FontTexture = dynamic_cast<soTexture*>(soResourceManager::InstancePtr()->Load(RP));

		if (m_FontTexture == NULL)
		{
			SOE_OutputLOG("soRenderManager", "OnStartUp()", "m_FontTexture creation error.");
			return EC_FALSE;
		}

		return EC_OK;
	}
	
	RESULT soRenderManager::InitPBR()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		/* EFECTO */
		soEffect* PBR = SOE_NEW soEffect;
		EffectData ED;
		ED._Name = TEXT("PBR");
		if (FAIL(PBR->StartUp(ED)))
		{
			return EC_FALSE;
		}
		PBR->m_ID = ARE_PBR;

		/* TECNICA */
		TechniqueData TD;
		TD._Name = TEXT("Standard");
		if (FAIL(PBR->AddTechnique(TD)))
		{
			return EC_FALSE;
		}

		/* PASES */
		RenderPassData RPD;
		RasterizerStateData RSD;
		soRenderPass* WorkPass;
		RenderTargetData RTD;
		TextureData T2D;

		T2D.T2DData.TextureSize = AppiManager.m_DefaultBackBuffer.m_ptrRenderTargetTexture->m_TextureSize;
		T2D.T2DData.DXGI_FORMAT = APIGI_FORMAT_R16G16B16A16_FLOAT;
		T2D.T2DData.D3D11_BIND_FLAG = API_BIND_RENDER_TARGET | API_BIND_SHADER_RESOURCE;
		RTD._TextureData = T2D;

		/***GBUFFER PASS****/
		RPD._Name = TEXT("GBuffer");
		RPD.SetStencilView = true;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
#ifdef LIGHT_TEST
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\GBuffer.hlsl");
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\GBuffer.hlsl");
#else
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\GBufferStandard.hlsl");
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\GBufferStandard.hlsl");
#endif // LIGHT_TEST				
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);
#ifdef CompressedGBuffer
		RTD.Name = TEXT("RGB-Albedo A-Depth");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		RTD.Name = TEXT("RG-Normal B-Roughness A-Metallic");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		RTD.Name = TEXT("RGB-Position");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}
#else
		RTD.Name = TEXT("RGBA-Albedo");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		RTD.Name = TEXT("RGBA-Normal");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		RTD.Name = TEXT("RGBA-Position");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		RTD.Name = TEXT("RGBA-Rougness");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		RTD.Name = TEXT("RGBA-Metallic");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		RTD.Name = TEXT("RGBA-Depth");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}
#endif
#ifdef LIGHT_TEST
		RPD._VertexShaderData.FilePath.clear();
		RPD._VertexShaderData.S_EntryPoint.clear();
		RPD._VertexShaderData.S_Profile.clear();
		RPD._FragmentShaderData.FilePath.clear();
		RPD._FragmentShaderData.S_EntryPoint.clear();
		RPD._FragmentShaderData.S_Profile.clear();

		/***ComputeLight PASS****/
		RPD._Name = TEXT("ComputeLight");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._ComputeShader.FilePath = TEXT("Resources\\Shaders\\ComputeShaderTile.hlsl");
		RPD._ComputeShader.S_EntryPoint = TEXT("cs_main");
		RPD._ComputeShader.S_Profile = AppiManager.m_Device.m_szCSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}

		StructuredBufferData SBD;
		SBD.Elements = AppiManager.m_ClientRect.m_Width * AppiManager.m_ClientRect.m_Height;
		SBD.D3D11_BIND_FLAG = API_BIND_UNORDERED_ACCESS | API_BIND_SHADER_RESOURCE;
		m_LitBufferCS.StartUp(SBD);

		RPD._ComputeShader.FilePath.clear();
		RPD._ComputeShader.S_EntryPoint.clear();
		RPD._ComputeShader.S_Profile.clear();
#endif // LIGHT_TEST	

		/***SSAO PASS****/
		RPD._Name = TEXT("SSAO");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\SSAO.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\SSAO.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		RTD.Name = TEXT("SSAO");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		/***Luminancia PASS****/
		RPD._Name = TEXT("Luminance");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\Luminance.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\Luminance.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		RTD.Name = TEXT("Luminancia");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		T2D.T2DData.MipLevels = 4  ;
		T2D.T2DData.MiscFlags = API_RESOURCE_MISC_GENERATE_MIPS;
		RTD._TextureData = T2D;

		/***Bright PASS****/
		RPD._Name = TEXT("Bright");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\Bright.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\Bright.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;
		
		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		RTD.Name = TEXT("Bright");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}
#ifdef BLUR_TEST
		T2D.T2DData.ScalarViewPortHeigth = 0.5f;
		T2D.T2DData.ScalarViewPortWidth = 0.5f;
		RTD._TextureData = T2D;

		/***BlurH PASS****/
		RPD._Name = TEXT("BlurH");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\BlurHGeneral.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\BlurHGeneral.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		RTD.Name = TEXT("BlurH");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		/***BlurV PASS****/
		RPD._Name = TEXT("BlurV");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\BlurVGeneral.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\BlurVGeneral.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		RTD.Name = TEXT("BlurV");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		T2D.T2DData.ScalarViewPortHeigth = 1.0f;
		T2D.T2DData.ScalarViewPortWidth = 1.0f;
		RTD._TextureData = T2D;

		/***AddBright PASS****/
		RPD._Name = TEXT("AddBright");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\AddBrightGeneral.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\AddBrightGeneral.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		RTD.Name = TEXT("AddBright");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}		
#else
		T2D.T2DData.ScalarViewPortHeigth = 0.5f;
		T2D.T2DData.ScalarViewPortWidth = 0.5f;
		RTD._TextureData = T2D;

		/***BlurH PASS****/
		RPD._Name = TEXT("BlurH");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\BlurH.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\BlurH.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		/***BlurV PASS****/
		RPD._Name = TEXT("BlurV");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\BlurV.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\BlurV.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		T2D.T2DData.ScalarViewPortHeigth = 1.0f;
		T2D.T2DData.ScalarViewPortWidth = 1.0f;
		RTD._TextureData = T2D;

		/***AddBright PASS****/
		RPD._Name = TEXT("AddBright");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\AddBright.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\AddBright.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		WorkPass = PBR->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

#endif
		/***ToneMap PASS****/
		RPD._Name = TEXT("ToneMap");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
#ifdef LIGHT_TEST
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\ToneMaping.hlsl");
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\ToneMaping.hlsl");
#else
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\ToneMapingStandar.hlsl");
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\ToneMapingStandar.hlsl");
#endif // LIGHT_TEST
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(PBR->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
				
		m_EfectList[PBR->m_ID] = PBR;

		return EC_OK;
	}	

	RESULT soRenderManager::InitEditorSelection()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		/* Generacion del efecto para la lista */
		soEffect* ES = SOE_NEW soEffect;
		EffectData ED;
		ED._Name = TEXT("EditorSelector");
		if (FAIL(ES->StartUp(ED)))
		{
			return EC_FALSE;
		}
		ES->m_ID = ARE_EditorSelection;

		/* Generacion de la primera tecnica, vista en clase de shaders */
		TechniqueData TD;
		TD._Name = TEXT("Standard");
		if (FAIL(ES->AddTechnique(TD)))
		{
			return EC_FALSE;
		}

		/* Generacion de pases de prender necesarios para el efecto */
		RenderPassData RPD;
		RasterizerStateData RSD;
		RenderTargetData RTD;
		SamplerData Sampler;

		/***Final PASS****/
		RSD.D3D11_FILL_MODE = API_FILL_WIREFRAME;

		RPD._Name = TEXT("ES");
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD.SetStencilView = true;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\EditorSelection.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\EditorSelection.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(ES->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}

		m_EfectList[ES->m_ID] = ES;

		return EC_OK;
	}

	RESULT soRenderManager::InitGraphicDebug()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		/* Generacion del efecto para la lista */
		soEffect* GD = SOE_NEW soEffect;
		EffectData ED;
		ED._Name = TEXT("GD");
		if (FAIL(GD->StartUp(ED)))
		{
			return EC_FALSE;
		}
		GD->m_ID = ARE_GraphicDebug;

		/* Generacion de la primera tecnica, vista en clase de shaders */
		TechniqueData TD;
		TD._Name = TEXT("Standard");
		if (FAIL(GD->AddTechnique(TD)))
		{
			return EC_FALSE;
		}

		/* Generacion de pases de prender necesarios para el efecto */
		RenderPassData RPD;
		RasterizerStateData RSD;
		RenderTargetData RTD;
		SamplerData Sampler;

		/***Debug PASS****/
		RPD._Name = TEXT("GD");
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD.SetStencilView = true;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\GraphicDebug.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\GraphicDebug.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(GD->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}

		m_EfectList[GD->m_ID] = GD;

		return EC_OK;
	}

	RESULT soRenderManager::InitFinalPassEffect()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		/* Generacion del efecto para la lista */
		soEffect* Final = SOE_NEW soEffect;
		EffectData ED;
		ED._Name = TEXT("Final");
		if (FAIL(Final->StartUp(ED)))
		{
			return EC_FALSE;
		}
		Final->m_ID = ARE_FinalPass;

		/* Generacion de la primera tecnica, vista en clase de shaders */
		TechniqueData TD;
		TD._Name = TEXT("Standard");
		if (FAIL(Final->AddTechnique(TD)))
		{
			return EC_FALSE;
		}

		/* Generacion de pases de prender necesarios para el efecto */
		RenderPassData RPD;
		RasterizerStateData RSD;
		RenderTargetData RTD;
		SamplerData Sampler;

		/***Final PASS****/
		RPD._Name = TEXT("FinalPass");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\Final.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\Final.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;
		
		if (FAIL(Final->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}
		
		m_EfectList[Final->m_ID] = Final;

		return EC_OK;
	}
	
	RESULT soRenderManager::InitTerrain()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		/* Generacion del efecto para la lista */
		soEffect* Terrain = SOE_NEW soEffect;
		EffectData ED;
		ED._Name = TEXT("Terrain");
		if (FAIL(Terrain->StartUp(ED)))
		{
			return EC_FALSE;
		}
		Terrain->m_ID = ARE_Terrain;

		/* Generacion de la primera tecnica, vista en clase de shaders */
		TechniqueData TD;
		TD._Name = TEXT("Standard");
		if (FAIL(Terrain->AddTechnique(TD)))
		{
			return EC_FALSE;
		}

		/* Generacion de pases de prender necesarios para el efecto */
		RenderPassData RPD;
		RasterizerStateData RSD;
		RenderTargetData RTD;
		SamplerData Sampler;

		/***Terrain PASS****/
		RPD._Name = TEXT("Terrain");
		RPD.SetStencilView = true;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\Terrain.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\Terrain.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(Terrain->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}

		m_EfectList[Terrain->m_ID] = Terrain;

		return EC_OK;
	}

	RESULT soRenderManager::InitSkyBox()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		/* Generacion del efecto para la lista */
		soEffect* SkyBox = SOE_NEW soEffect;
		EffectData ED;
		ED._Name = TEXT("SkyBox");
		if (FAIL(SkyBox->StartUp(ED)))
		{
			return EC_FALSE;
		}
		SkyBox->m_ID = ARE_SkyBox;

		/* Generacion de la primera tecnica, vista en clase de shaders */
		TechniqueData TD;
		TD._Name = TEXT("Standard");
		if (FAIL(SkyBox->AddTechnique(TD)))
		{
			return EC_FALSE;
		}

		/* Generacion de pases de prender necesarios para el efecto */
		RenderPassData RPD;
		RasterizerStateData RSD;
		RenderTargetData RTD;
		SamplerData Sampler;

		/***Skybox PASS****/
		RPD._Name = TEXT("Skybox");
		RPD.SetStencilView = true;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\SkyBox.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\SkyBox.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;
		
		if (FAIL(SkyBox->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}

		m_EfectList[SkyBox->m_ID] = SkyBox;

		return EC_OK;
	}

	RESULT soRenderManager::InitEditorGUI()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		/* Generacion del efecto para la lista */
		soEffect* EditorGUI = SOE_NEW soEffect;
		EffectData ED;
		ED._Name = TEXT("EditorGUI");
		if (FAIL(EditorGUI->StartUp(ED)))
		{
			return EC_FALSE;
		}
		EditorGUI->m_ID = ARE_EditorGUI;

		/* Generacion de la primera tecnica, vista en clase de shaders */
		TechniqueData TD;
		TD._Name = TEXT("Standard");
		if (FAIL(EditorGUI->AddTechnique(TD)))
		{
			return EC_FALSE;
		}

		/* Generacion de pases de prender necesarios para el efecto */
		RenderPassData RPD;
		RasterizerStateData RSD;
		RenderTargetData RTD;
		SamplerData Sampler;

		/***EditorGUI PASS****/
		RPD._Name = TEXT("EditorGUI");
		RPD.SetStencilView = true;

		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		RSD.D3D11_FILL_MODE = API_FILL_SOLID;
		RSD.ScissorEnable = true;
		RSD.DepthClipEnable = true;
		RSD.AntialiasedLineEnable = false;
		RSD.DepthBiasClamp = 0.0f;
		RSD.FrontCounterClockwise = false;
		RSD.MultisampleEnable = false;
		RSD.SlopeScaledDepthBias = 0.0f;

		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\EditorGUI.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._VertexShaderData.ShaderFlags = 0;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\EditorGUI.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(EditorGUI->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}

		m_EfectList[EditorGUI->m_ID] = EditorGUI;

		return EC_OK;
	}

	RESULT soRenderManager::InitMultyPassViewer()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		/* Generacion del efecto para la lista */
		soEffect* MultyPassViewer = SOE_NEW soEffect;
		EffectData ED;
		ED._Name = TEXT("MultyPassViewer");
		if (FAIL(MultyPassViewer->StartUp(ED)))
		{
			return EC_FALSE;
		}
		MultyPassViewer->m_ID = ARE_MultyPassViewer;

		/* Generacion de la primera tecnica, vista en clase de shaders */
		TechniqueData TD;
		TD._Name = TEXT("Standard");
		if (FAIL(MultyPassViewer->AddTechnique(TD)))
		{
			return EC_FALSE;
		}

		/* Generacion de pases de prender necesarios para el efecto */
		RenderPassData RPD;
		RasterizerStateData RSD;
		RenderTargetData RTD;
		SamplerData Sampler;
		TextureData T2D;

		/***EditorGUI PASS****/
		RPD._Name = TEXT("MultyPassViewer");
		RPD.SetStencilView = false;

		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\MultyPassViewer.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._VertexShaderData.ShaderFlags = 0;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\MultyPassViewer.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		if (FAIL(MultyPassViewer->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}

		soRenderPass* WorkPass = MultyPassViewer->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		T2D.T2DData.TextureSize = AppiManager.m_DefaultBackBuffer.m_ptrRenderTargetTexture->m_TextureSize;
		T2D.T2DData.DXGI_FORMAT = APIGI_FORMAT_R8G8B8A8_UNORM;
		T2D.T2DData.D3D11_BIND_FLAG = API_BIND_RENDER_TARGET | API_BIND_SHADER_RESOURCE;
		RTD._TextureData = T2D;

		RTD.Name = TEXT("MultyPassViewer");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}

		m_EfectList[MultyPassViewer->m_ID] = MultyPassViewer;

		return EC_OK;
	}

	RESULT soRenderManager::InitFontSystem()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		/* Generacion del efecto para la lista */
		soEffect* FontSystem = SOE_NEW soEffect;
		EffectData ED;
		ED._Name = TEXT("FontSystem");
		if (FAIL(FontSystem->StartUp(ED)))
		{
			return EC_FALSE;
		}
		FontSystem->m_ID = ARE_FontSystem;

		/* Generacion de la primera tecnica, vista en clase de shaders */
		TechniqueData TD;
		TD._Name = TEXT("Standard");
		if (FAIL(FontSystem->AddTechnique(TD)))
		{
			return EC_FALSE;
		}

		/* Generacion de pases de prender necesarios para el efecto */
		RenderPassData RPD;
		RasterizerStateData RSD;
		RenderTargetData RTD;
		TextureData T2D;

		/***Font PASS****/
		RPD._Name = TEXT("FontSystem");
		RPD.SetStencilView = false;
		RSD.D3D11_CULL_MODE = API_CULL_NONE;
		

		RPD._RasterizerStateInfo = RSD;
		RPD._VertexShaderData.FilePath = TEXT("Resources\\Shaders\\Font.hlsl");
		RPD._VertexShaderData.S_EntryPoint = TEXT("vs_main");
		RPD._VertexShaderData.S_Profile = AppiManager.m_Device.m_szVSProfile;
		RPD._VertexShaderData.ShaderFlags = 0;
		RPD._FragmentShaderData.FilePath = TEXT("Resources\\Shaders\\Font.hlsl");
		RPD._FragmentShaderData.S_EntryPoint = TEXT("ps_main");
		RPD._FragmentShaderData.S_Profile = AppiManager.m_Device.m_szPSProfile;

		// Create Dynamic vertex buffer
		D3D11_BUFFER_DESC vertexDesc;
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		const int sizeOfSprite = sizeof(FontVertex) * 6;
		const int maxLetters = MAX_CHARACTERS_NUM;
		vertexDesc.ByteWidth = sizeOfSprite * maxLetters;

		AppiManager.m_Device.m_ptrD3DDevice->CreateBuffer(&vertexDesc, NULL, &m_dynamicVertexBufferFont);

		//Creacion del pase a renderear
		if (FAIL(FontSystem->GetTechnique(AT_Standard)->AddPass(RPD)))
		{
			return EC_FALSE;
		}

		//Creación de render target
		soRenderPass* WorkPass = FontSystem->GetTechnique(AT_Standard)->GetPass(RPD._Name);

		T2D.T2DData.TextureSize = AppiManager.m_DefaultBackBuffer.m_ptrRenderTargetTexture->m_TextureSize;
		T2D.T2DData.DXGI_FORMAT = APIGI_FORMAT_R8G8B8A8_UNORM;
		T2D.T2DData.D3D11_BIND_FLAG = API_BIND_RENDER_TARGET | API_BIND_SHADER_RESOURCE;
		RTD._TextureData = T2D;

		RTD.Name = TEXT("FontSystem");
		if (FAIL(WorkPass->AddRenderTarget(RTD)))
		{
			return EC_FALSE;
		}
		
		m_EfectList[FontSystem->m_ID] = FontSystem;

		return EC_OK;
	}

	RESULT soRenderManager::AplyPBR()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		soTexture _Dummy;
		soTechnique* PBR_Technique = m_EfectList.at(ARE_PBR)->GetTechnique(AT_Standard);
		uint32 NumBuffers = 1;
		
		if (m_SceneNodeList && !m_SceneNodeList->empty())
		{
			/************************************************************************/
			/* GBuffer                                                              */
			/************************************************************************/
			soRenderPass* WorkPass = PBR_Technique->GetPass(Slot_0);

			//Seteamos los componentes del pase
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			//Seteamos los constant buffer necesarios del shader
			AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
			AppiManager.VSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_0]);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);
#ifndef LIGHT_TEST
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_1], &m_LightCB);
			AppiManager.PSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_1]);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_2], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_2, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_2]);
#endif			
			//Seteamos las caracteristicas de cada modelo
			for (uint32 i = 0; i < static_cast<uint32>(m_SceneNodeList->size()); i++)
			{
				std::vector<soComponent*> MeshList = m_SceneNodeList->at(i)->m_ptrActor->GetComponentbyType(CT_Mesh_Static | CT_Mesh_Dynamic);

				for (uint32 j = 0; j < static_cast<uint32>(MeshList.size()); j++)
				{
					soStaticModelComponent* StaticModel = dynamic_cast<soStaticModelComponent*>(MeshList.at(j));

					if (StaticModel != NULL)
					{
						//Seteamos los constant buffer necesarios del shader por modelo
						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &StaticModel->m_ComponentTransform.m_FinalTransform);
						AppiManager.VSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_3], &StaticModel->GetModel()->m_IsStatic);
						AppiManager.VSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_3]);
#ifdef LIGHT_TEST
						AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_1], &StaticModel->GetModel()->m_IsRougthnes);
						AppiManager.PSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_1]);

						StaticModel->Render(MAT_PROP_ALBEDO | MAT_PROP_NORMAL | MAT_PROP_METALLIC | MAT_PROP_ROUGHNESS);
#else
						AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_3], &StaticModel->GetModel()->m_IsRougthnes);
						AppiManager.PSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_3]);

						StaticModel->Render(Shader_ALL_Slot);
#endif // LIGHT_TEST						
					}

					soSkeletalModelComponent* SkeletalModel = dynamic_cast<soSkeletalModelComponent*>(MeshList.at(j));
					if (SkeletalModel != NULL)
					{
						//Seteamos los constant buffer necesarios del shader por modelo
						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &SkeletalModel->m_ComponentTransform.m_FinalTransform);
						AppiManager.VSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_2], &SkeletalModel->m_AnimationData.BoneMatrix);
						AppiManager.VSSetConstantBuffers(Slot_2, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_2]);

						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_3], &SkeletalModel->GetModel()->m_IsStatic);
						AppiManager.VSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_3]);
#ifdef LIGHT_TEST
						AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_1], &SkeletalModel->GetModel()->m_IsRougthnes);
						AppiManager.PSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_1]);

						SkeletalModel->Render(MAT_PROP_ALBEDO | MAT_PROP_NORMAL | MAT_PROP_METALLIC | MAT_PROP_ROUGHNESS);
#else
						AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_3], &SkeletalModel->GetModel()->m_IsRougthnes);
						AppiManager.PSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_3]);

						SkeletalModel->Render(Shader_ALL_Slot);
#endif // LIGHT_TEST
					}
				}
			}
			//AppiManager.CleanResources();

#ifdef LIGHT_TEST
			/************************************************************************/
			/* Compute Tile Light												    */
			/************************************************************************/
			soTexture GAlbedo_GDepth, GNormal_GSpecular, GPositionZGrad;
			GAlbedo_GDepth.m_Texture = *PBR_Technique->GetPass(Slot_0)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			GNormal_GSpecular.m_Texture = *PBR_Technique->GetPass(Slot_0)->GetRenderTarget(Slot_1)->m_ptrRenderTargetTexture;
			GPositionZGrad.m_Texture = *PBR_Technique->GetPass(Slot_0)->GetRenderTarget(Slot_2)->m_ptrRenderTargetTexture;

			WorkPass = PBR_Technique->GetPass(RP_ComputeLight);

			//Seteamos los componentes del pase
			WorkPass->Render();

			//Seteamos los constant buffer necesarios del shader
			AppiManager.UpdateSubresource(WorkPass->m_ComputeShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
			AppiManager.CSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_ComputeShader.m_ptrBuffer[Slot_0]);

			AppiManager.UpdateSubresource(WorkPass->m_ComputeShader.m_ptrBuffer[Slot_1], &m_PBRData);
			AppiManager.CSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_ComputeShader.m_ptrBuffer[Slot_1]);

			//Texturas del GBuffer
			AppiManager.CSSetShaderResources(Slot_0, NumBuffers, GAlbedo_GDepth.m_Texture);
			AppiManager.CSSetShaderResources(Slot_1, NumBuffers, GNormal_GSpecular.m_Texture);
			AppiManager.CSSetShaderResources(Slot_2, NumBuffers, GPositionZGrad.m_Texture);

			//Aqui el scenne graph nos daria el numero total de luces, si este es diferente del que tenemos, reseteamos el buffer de luces
			SetActiveLights(1);//El resultado de la busqueda del scene graph es el 1  
							   //TODO: agregar una copia del buffer
			SetupLights();		//Aqui solo actualizamos el buffer 

			AppiManager.CSSetShaderResources(Slot_3, NumBuffers, m_LightShaderResourceBuffer.m_Texture);
			AppiManager.CSSetUnorderedAccessViews(Slot_0, NumBuffers, m_LitBufferCS.m_UnorderedAccess, 0);

			// Dispatch
			uint32 dispatchWidth = (m_FrameBuffer.ViewportDimensions[0] + COMPUTE_SHADER_TILE_GROUP_DIM - 1) / COMPUTE_SHADER_TILE_GROUP_DIM;
			uint32 dispatchHeight = (m_FrameBuffer.ViewportDimensions[1] + COMPUTE_SHADER_TILE_GROUP_DIM - 1) / COMPUTE_SHADER_TILE_GROUP_DIM;
			AppiManager.Dispatch(dispatchWidth, dispatchHeight, 1);

			GAlbedo_GDepth = GNormal_GSpecular = GPositionZGrad = _Dummy;
#endif // LIGHT_TEST

			/************************************************************************/
			/* SSAO                                                                 */
			/************************************************************************/
			soTexture Normals, Position;
			Normals.m_Texture = *PBR_Technique->GetPass(RP_GBuffer)->GetRenderTarget(Slot_1)->m_ptrRenderTargetTexture;
			Position.m_Texture = *PBR_Technique->GetPass(RP_GBuffer)->GetRenderTarget(Slot_2)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Normals, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Position, Slot_1);

			WorkPass = PBR_Technique->GetPass(RP_SSAO);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);
			AppiManager.CleanResources();

			Normals = Position = _Dummy;

			/************************************************************************/
			/* Luminance                                                            */
			/************************************************************************/
			soTexture GAlbedo;
			GAlbedo.m_Texture = *PBR_Technique->GetPass(RP_GBuffer)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&GAlbedo, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_Luminance);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			AppiManager.CleanResources();

			GAlbedo = _Dummy;

			/************************************************************************/
			/* Bright                                                               */
			/************************************************************************/
			soTexture LuminanceBuffer;
			LuminanceBuffer.m_Texture = *PBR_Technique->GetPass(RP_Luminance)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&LuminanceBuffer, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_Bright);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			AppiManager.CleanResources();

			LuminanceBuffer = _Dummy;
#ifdef BLUR_TEST
			/************************************************************************/
			/* MipSettings                                                          */
			/************************************************************************/
			m_PBRData.MipLevel[0] = 3;
			m_PBRData.MipLevel[1] = 4;
			uint32 width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width >> (m_PBRData.MipLevel[0]);
			uint32 height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height >> (m_PBRData.MipLevel[0]);
			ViewPortData VP;
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* Blur_0                                                               */
			/************************************************************************/
			soTexture Bright;
			Bright.m_Texture = *PBR_Technique->GetPass(RP_Bright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_0);

			/* H */
			WorkPass = PBR_Technique->GetPass(RP_BlurH);
			WorkPass->Render();
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);
			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);
			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);
			AppiManager.SetRenderTargets(NULL, 0, false);

			/* V */
			WorkPass = PBR_Technique->GetPass(RP_BlurV);
			WorkPass->Render();
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			AppiManager.CleanResources();

			Bright = _Dummy;
						
			/************************************************************************/
			/* MipSettings                                                          */
			/************************************************************************/
			m_PBRData.MipLevel[0]--;//2
			m_PBRData.MipLevel[1]--;//3			

			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width << (m_PBRData.MipLevel[0]);
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height << (m_PBRData.MipLevel[0]);
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);
			
			/************************************************************************/
			/* AddBright_0                                                          */
			/************************************************************************/
			soTexture BlurV, BlurH;
			Bright.m_Texture = *PBR_Technique->GetPass(RP_Bright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			BlurV.m_Texture = *PBR_Technique->GetPass(RP_BlurV)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			BlurH.m_Texture = *PBR_Technique->GetPass(RP_BlurH)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurV, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurH, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_2);

			WorkPass = PBR_Technique->GetPass(RP_AddBright);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_2);
			AppiManager.CleanResources();
			
			Bright = BlurV = BlurH = _Dummy;

			/************************************************************************/
			/* MipSettings                                                          */
			/************************************************************************/
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width >> (m_PBRData.MipLevel[0]);
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height >> (m_PBRData.MipLevel[0]);
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* Blur_1                                                               */
			/************************************************************************/
			Bright.m_Texture = *PBR_Technique->GetPass(RP_AddBright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_0);

			/* H */
			WorkPass = PBR_Technique->GetPass(RP_BlurH);
			WorkPass->Render();
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);
			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);
			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);
			AppiManager.SetRenderTargets(NULL, 0, false);

			/* V */
			WorkPass = PBR_Technique->GetPass(RP_BlurV);
			WorkPass->Render();
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);
			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			AppiManager.CleanResources();

			Bright = _Dummy;

			/************************************************************************/
			/* MipSettings                                                          */
			/************************************************************************/
			m_PBRData.MipLevel[0]--;//1
			m_PBRData.MipLevel[1]--;//2	
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width << (m_PBRData.MipLevel[0]);
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height << (m_PBRData.MipLevel[0]);
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* AddBright_1                                                          */
			/************************************************************************/
			Bright.m_Texture = *PBR_Technique->GetPass(RP_Bright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			BlurV.m_Texture = *PBR_Technique->GetPass(RP_BlurV)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			BlurH.m_Texture = *PBR_Technique->GetPass(RP_BlurH)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurV, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurH, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_2);

			WorkPass = PBR_Technique->GetPass(RP_AddBright);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_2);
			AppiManager.CleanResources();

			Bright = BlurV = BlurH = _Dummy;

			/************************************************************************/
			/* MipSettings                                                          */
			/************************************************************************/
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width >> (m_PBRData.MipLevel[0]);
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height >> (m_PBRData.MipLevel[0]);
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* Blur_2                                                               */
			/************************************************************************/
			Bright.m_Texture = *PBR_Technique->GetPass(RP_AddBright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_0);

			/* H */
			WorkPass = PBR_Technique->GetPass(RP_BlurH);
			WorkPass->Render();
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);
			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);
			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);
			AppiManager.SetRenderTargets(NULL, 0, false);

			/* V */
			WorkPass = PBR_Technique->GetPass(RP_BlurV);
			WorkPass->Render();
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			AppiManager.CleanResources();

			Bright = _Dummy;

			/************************************************************************/
			/* MipSettings                                                          */
			/************************************************************************/
			m_PBRData.MipLevel[0]--;//0
			m_PBRData.MipLevel[1]--;//1	
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width << (m_PBRData.MipLevel[0]);
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height << (m_PBRData.MipLevel[0]);
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* AddBright_2                                                          */
			/************************************************************************/
			Bright.m_Texture = *PBR_Technique->GetPass(RP_Bright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			BlurV.m_Texture = *PBR_Technique->GetPass(RP_BlurV)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			BlurH.m_Texture = *PBR_Technique->GetPass(RP_BlurH)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurV, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurH, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_2);

			WorkPass = PBR_Technique->GetPass(RP_AddBright);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_2);
			AppiManager.CleanResources();

			Bright = BlurV = BlurH = _Dummy;

			/************************************************************************/
			/* MipSettings                                                          */
			/************************************************************************/
			m_PBRData.MipLevel[0]++;
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width >> (m_PBRData.MipLevel[0]);
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height >> (m_PBRData.MipLevel[0]);
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* Blur_3                                                               */
			/************************************************************************/
			Bright.m_Texture = *PBR_Technique->GetPass(RP_AddBright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_0);

			/* H */
			WorkPass = PBR_Technique->GetPass(RP_BlurH);
			WorkPass->Render();
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);
			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);
			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);
			AppiManager.SetRenderTargets(NULL, 0, false);

			/* V */
			WorkPass = PBR_Technique->GetPass(RP_BlurV);
			WorkPass->Render();
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			AppiManager.CleanResources();

			Bright = _Dummy;

			/************************************************************************/
			/* MipSettings                                                          */
			/************************************************************************/
			m_PBRData.MipLevel[0]--;//0
			m_PBRData.MipLevel[1]--;//0
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width << (m_PBRData.MipLevel[0]);
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height << (m_PBRData.MipLevel[0]);
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* AddBright_3                                                          */
			/************************************************************************/
			Bright.m_Texture = *PBR_Technique->GetPass(RP_Bright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			BlurV.m_Texture = *PBR_Technique->GetPass(RP_BlurV)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			BlurH.m_Texture = *PBR_Technique->GetPass(RP_BlurH)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurV, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurH, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_2);

			WorkPass = PBR_Technique->GetPass(RP_AddBright);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_2);
			AppiManager.CleanResources();

			Bright = BlurV = BlurH = _Dummy;

#else
			/************************************************************************/
			/* ViewportSettings                                                     */
			/************************************************************************/
			m_PBRData.MipLevel[0] = 3;
			m_PBRData.MipLevel[1] = 4;
			uint32 width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width >> 1;
			uint32 height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height >> 1;
			ViewPortData VP;
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* BlurH                                                                */
			/************************************************************************/
			soTexture Bright;
			Bright.m_Texture = *PBR_Technique->GetPass(RP_Bright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_BlurH);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_PointSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_1], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_1]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			Bright = _Dummy;

			/************************************************************************/
			/* BlurV                                                                */
			/************************************************************************/
			soTexture BlurH;
			BlurH.m_Texture = *PBR_Technique->GetPass(RP_BlurH)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurH, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_BlurV);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_PointSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			BlurH = _Dummy;

			/************************************************************************/
			/* ViewportSettings                                                     */
			/************************************************************************/
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width;
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height;
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);
			
			/************************************************************************/
			/* AddBright                                                            */
			/************************************************************************/
			soTexture BlurV;
			BlurV.m_Texture = *PBR_Technique->GetPass(RP_BlurV)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			Bright.m_Texture = *PBR_Technique->GetPass(RP_Bright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurV, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_1);

			WorkPass = PBR_Technique->GetPass(RP_AddBright);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			BlurV = Bright = _Dummy;

			/************************************************************************/
			/* ViewportSettings                                                     */
			/************************************************************************/
			m_PBRData.MipLevel[0]--;
			m_PBRData.MipLevel[1]--;
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width >> 1;
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height >> 1;
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* BlurH_1                                                              */
			/************************************************************************/
			soTexture AddBright;
			AddBright.m_Texture = *PBR_Technique->GetPass(RP_AddBright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&AddBright, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_BlurH);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_PointSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			AddBright = _Dummy;

			/************************************************************************/
			/* BlurV_1                                                              */
			/************************************************************************/
			BlurH.m_Texture = *PBR_Technique->GetPass(RP_BlurH)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurH, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_BlurV);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_PointSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			BlurH = _Dummy;

			/************************************************************************/
			/* ViewportSettings                                                     */
			/************************************************************************/
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width;
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height;
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* AddBright_1                                                          */
			/************************************************************************/			
			BlurV.m_Texture = *PBR_Technique->GetPass(RP_BlurV)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			Bright.m_Texture = *PBR_Technique->GetPass(RP_Bright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurV, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_1);

			WorkPass = PBR_Technique->GetPass(RP_AddBright);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			BlurV = Bright = _Dummy;

			/************************************************************************/
			/* ViewportSettings                                                     */
			/************************************************************************/
			m_PBRData.MipLevel[0]--;
			m_PBRData.MipLevel[1]--;
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width >> 1;
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height >> 1;
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* BlurH_2                                                              */
			/************************************************************************/
			AddBright.m_Texture = *PBR_Technique->GetPass(RP_AddBright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&AddBright, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_BlurH);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_PointSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			AddBright = _Dummy;

			/************************************************************************/
			/* BlurV_2                                                              */
			/************************************************************************/
			BlurH.m_Texture = *PBR_Technique->GetPass(RP_BlurH)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurH, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_BlurV);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_PointSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);
			
			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			BlurH = _Dummy;

			/************************************************************************/
			/* ViewportSettings                                                     */
			/************************************************************************/
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width;
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height;
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* AddBright_2                                                          */
			/************************************************************************/
			BlurV.m_Texture = *PBR_Technique->GetPass(RP_BlurV)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			Bright.m_Texture = *PBR_Technique->GetPass(RP_Bright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurV, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Bright, Slot_1);

			WorkPass = PBR_Technique->GetPass(RP_AddBright);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			BlurV = Bright = _Dummy;

			/************************************************************************/
			/* ViewportSettings                                                     */
			/************************************************************************/
			m_PBRData.MipLevel[0]--;
			m_PBRData.MipLevel[1]--;
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width >> 1;
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height >> 1;
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

			/************************************************************************/
			/* BlurH_3                                                              */
			/************************************************************************/
			AddBright.m_Texture = *PBR_Technique->GetPass(RP_AddBright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&AddBright, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_BlurH);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_PointSampler);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			AddBright = _Dummy;

			/************************************************************************/
			/* BlurV_3                                                              */
			/************************************************************************/
			BlurH.m_Texture = *PBR_Technique->GetPass(RP_BlurH)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurH, Slot_0);

			WorkPass = PBR_Technique->GetPass(RP_BlurV);
			WorkPass->Render();
			AppiManager.PSSetSampler(Slot_0, Slot_1, m_PointSampler);
			
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);

			AppiManager.GenerateMips(*WorkPass->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture);

			BlurH = _Dummy;

			/************************************************************************/
			/* ViewportSettings                                                     */
			/************************************************************************/
			width = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width;
			height = m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height;
			VP.Width = (float)width;
			VP.Height = (float)height;
			m_BlurViewPort.ShutDown();
			m_BlurViewPort.StartUp(VP);

			AppiManager.SetViewPort(m_BlurViewPort);

#endif					
			/************************************************************************/
			/* ToneMaping                                                           */
			/************************************************************************/	
			soTexture SSAO;
			BlurV.m_Texture = *PBR_Technique->GetPass(RP_AddBright)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
			SSAO.m_Texture = *PBR_Technique->GetPass(RP_SSAO)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
#ifdef LIGHT_TEST			
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurH, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&SSAO, Slot_1);
#else
			GAlbedo.m_Texture = *PBR_Technique->GetPass(RP_GBuffer)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&GAlbedo, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&BlurV, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&SSAO, Slot_2);
#endif // LIGHT_TEST				
			WorkPass = PBR_Technique->GetPass(RP_ToneMaping);
			
			WorkPass->Render();

			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			AppiManager.SetRenderTargets(m_BackBuffer, 1, false);
#ifdef LIGHT_TEST
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);

			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_1], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_1]);

			AppiManager.m_Device.m_ptrD3DContext->PSSetShaderResources(Slot_2, NumBuffers, &m_LitBufferCS.m_ShaderResource);

#else
			AppiManager.UpdateSubresource(WorkPass->m_PixelShader.m_ptrBuffer[Slot_0], &m_PBRData);
			AppiManager.PSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_PixelShader.m_ptrBuffer[Slot_0]);
#endif // LIGHT_TEST
			((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);
			soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_2);
			AppiManager.CleanResources();

			SSAO = GAlbedo = BlurV = _Dummy;
		}
						
		return EC_OK;
	}
	
	RESULT soRenderManager::AplyEditorSelection()
	{		
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		m_SelectedNode = m_EditorControl->m_SelectedNode;
		
		if (m_SelectedNode != NULL)
		{				
			soTechnique* Selector_Technique = m_EfectList.at(ARE_EditorSelection)->GetTechnique(AT_Standard);

			/************************************************************************/
			/* EditorSelection                                                      */
			/************************************************************************/
			soRenderPass* WorkPass = Selector_Technique->GetPass(Slot_0);

			//Seteamos los componentes del pase
			WorkPass->Render();

			AppiManager.SetRenderTargets(m_BackBuffer, 1, true);

			AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

			//Seteamos los constant buffer necesarios del shader 
			uint32 NumBuffers = 1;
			AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
			AppiManager.VSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_0]);	

			std::vector<soComponent*> MeshList = m_SelectedNode->m_ptrActor->GetComponentbyType(CT_Mesh_Static | CT_Mesh_Dynamic);

			for (uint32 j = 0; j < static_cast<uint32>(MeshList.size()); j++)
			{
				soStaticModelComponent* StaticModel = dynamic_cast<soStaticModelComponent*>(MeshList.at(j));

				if (StaticModel != NULL)
				{
					if (m_EditorControl->m_EditMaterial)
					{
						if (m_EditorControl->m_StaticModel)
						{
							//Seteamos los constant buffer necesarios del shader por modelo
							AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &m_EditorControl->m_StaticModel->m_ComponentTransform.m_FinalTransform);
							AppiManager.VSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

							AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_3], &m_EditorControl->m_StaticModel->GetModel()->m_IsStatic);
							AppiManager.VSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_3]);

							AppiManager.PSSetShaderResources(Slot_6, NumBuffers, &m_MeshSelectionTexture->m_Texture);
							m_EditorControl->m_StaticModel->GetModel()->m_MeshList.at(m_EditorControl->m_ActualSelectedMesh)->Render(MAT_PROP_NOPROP, StaticModel->GetModel()->m_DefaultMaterialID[m_EditorControl->m_ActualSelectedMesh]);
						}

						//Seteamos los constant buffer necesarios del shader por modelo
						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &StaticModel->m_ComponentTransform.m_FinalTransform);
						AppiManager.VSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_3], &StaticModel->GetModel()->m_IsStatic);
						AppiManager.VSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_3]);

						AppiManager.PSSetShaderResources(Slot_6, NumBuffers, &m_SelectionTexture->m_Texture);
						for (int32 i = 0; i < static_cast<int32>(StaticModel->GetModel()->m_MeshList.size()); i++)
						{
							if (m_EditorControl->m_StaticModel != StaticModel)
							{								
								StaticModel->GetModel()->m_MeshList.at(i)->Render(MAT_PROP_NOPROP, StaticModel->GetModel()->m_DefaultMaterialID[i]);
							}
							else
							{
								if (m_EditorControl->m_ActualSelectedMesh != i)
								{
									StaticModel->GetModel()->m_MeshList.at(i)->Render(MAT_PROP_NOPROP, StaticModel->GetModel()->m_DefaultMaterialID[i]);
								}
							}
						}
					}
					else
					{
						//Seteamos los constant buffer necesarios del shader por modelo
						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &StaticModel->m_ComponentTransform.m_FinalTransform);
						AppiManager.VSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_3], &StaticModel->GetModel()->m_IsStatic);
						AppiManager.VSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_3]);

						AppiManager.PSSetShaderResources(Slot_6, NumBuffers, &m_SelectionTexture->m_Texture);
						for (uint32 i = 0; i < static_cast<int32>(StaticModel->GetModel()->m_MeshList.size()); i++)
						{
							StaticModel->GetModel()->m_MeshList.at(i)->Render(MAT_PROP_NOPROP, StaticModel->GetModel()->m_DefaultMaterialID[i]);
						}
					}
				}

				soSkeletalModelComponent* skeletalModel = dynamic_cast<soSkeletalModelComponent*>(MeshList.at(j));

				if (skeletalModel != NULL)
				{
					if (m_EditorControl->m_EditMaterial)
					{
						if (m_EditorControl->m_SkeletalModel)
						{
							//Seteamos los constant buffer necesarios del shader por modelo
							AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &m_EditorControl->m_SkeletalModel->m_ComponentTransform.m_FinalTransform);
							AppiManager.VSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

							AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_3], &m_EditorControl->m_SkeletalModel->GetModel()->m_IsStatic);
							AppiManager.VSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_3]);

							AppiManager.PSSetShaderResources(Slot_6, NumBuffers, &m_MeshSelectionTexture->m_Texture);
							m_EditorControl->m_SkeletalModel->GetModel()->m_MeshList.at(m_EditorControl->m_ActualSelectedMesh)->Render(MAT_PROP_NOPROP, skeletalModel->GetModel()->m_DefaultMaterialID[m_EditorControl->m_ActualSelectedMesh]);
						}

						//Seteamos los constant buffer necesarios del shader por modelo
						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &skeletalModel->m_ComponentTransform.m_FinalTransform);
						AppiManager.VSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_3], &skeletalModel->GetModel()->m_IsStatic);
						AppiManager.VSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_3]);

						AppiManager.PSSetShaderResources(Slot_6, NumBuffers, &m_SelectionTexture->m_Texture);
						for (int32 i = 0; i < static_cast<int32>(skeletalModel->GetModel()->m_MeshList.size()); i++)
						{
							if (m_EditorControl->m_SkeletalModel != skeletalModel)
							{
								skeletalModel->GetModel()->m_MeshList.at(i)->Render(MAT_PROP_NOPROP, skeletalModel->GetModel()->m_DefaultMaterialID[i]);
							}
							else
							{
								if (m_EditorControl->m_ActualSelectedMesh != i)
								{
									skeletalModel->GetModel()->m_MeshList.at(i)->Render(MAT_PROP_NOPROP, skeletalModel->GetModel()->m_DefaultMaterialID[i]);
								}
							}
						}
					}
					else
					{
						//Seteamos los constant buffer necesarios del shader por modelo
						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &skeletalModel->m_ComponentTransform.m_FinalTransform);
						AppiManager.VSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

						AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_3], &skeletalModel->GetModel()->m_IsStatic);
						AppiManager.VSSetConstantBuffers(Slot_3, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_3]);

						AppiManager.PSSetShaderResources(Slot_6, NumBuffers, &m_SelectionTexture->m_Texture);
						for (uint32 i = 0; i < static_cast<uint32>(skeletalModel->GetModel()->m_MeshList.size()); i++)
						{
							skeletalModel->GetModel()->m_MeshList.at(i)->Render(MAT_PROP_NOPROP, skeletalModel->GetModel()->m_DefaultMaterialID[i]);
						}
					}
				}
			}	

			AppiManager.SetRenderTargets(NULL, 0, false);
		}	
		
		return EC_OK;
	}

	RESULT soRenderManager::AplyGraphicDebug()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		soTechnique* Debug_Technique = m_EfectList.at(ARE_GraphicDebug)->GetTechnique(AT_Standard);

		if (!soGraphicDebugManager::Instance().IsEmpty())
		{
			/************************************************************************/
			/* GraphicDebug                                                         */
			/************************************************************************/
			soRenderPass* WorkPass = Debug_Technique->GetPass(Slot_0);

			//Seteamos los componentes del pase
			WorkPass->Render();

			AppiManager.SetRenderTargets(m_BackBuffer, 1, true);

			//Seteamos los constant buffer necesarios del shader 
			uint32 NumBuffers = 1;
			AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
			AppiManager.VSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_0]);

			//Seteamos las caracteristicas de las lineas
			soGraphicDebugManager::Instance().Render();

			AppiManager.SetRenderTargets(NULL, 0, false);
		}
		return EC_OK;
	}

	RESULT soRenderManager::RenderToBackBuffer(soAPITexture& _Tex, soRect& _Size, soRenderTarget* _BackBuffer /* = NULL */, soMatrix4x4 World /* = Math::IdentityMatrix4x4() */)
	{	
		soDXGIManager& AppiManager = soDXGIManager::Instance();
		AppiManager.SetBlendState(m_AditiveBlend, 0, 0xFFFFFFFF);

		soTexture Final, _Dummy;
		Final.m_Texture = _Tex;

		//Asignamos la textura como recurso que vamos a usar para renderear, en el registro 0
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Final, Texture_Slot_0);
		
		//Tomamos, por comodidad, una referencia al pase que usaremos
		soRenderPass* WorkPass = GetEffectByID(ARE_FinalPass)->GetTechnique(AT_Standard)->GetPass(Slot_0);

		//Seteamos todas las caracteristicas del pase (RenderTargetView, VShader, PShader, Rasterizer)
		WorkPass->Render();

		if (_BackBuffer)
		{
			AppiManager.SetRenderTargets(_BackBuffer, 1, false);
		}
		else
		{
			AppiManager.SetRenderTargets(m_BackBuffer, 1, false);
		}

		AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

		//Conversion de espacio de pantalla en pixeles a espacio en unidades(se toma en cuenta el tamaño de full hd por ahora)
		soMatrix4x4 Transform = World;
		uint32 NumBuffers = 1;

		if (Transform == soMath::IdentityMatrix4x4())
		{
			soVector3D Scalation, Translation;
			Scalation.X = static_cast<float>(_Size.m_Width) / static_cast<float>(AppiManager.m_ClientRect.m_Width);
			Scalation.Y = static_cast<float>(_Size.m_Height) / static_cast<float>(AppiManager.m_ClientRect.m_Height);
			Scalation.Z = 1.0f;

			float X, Y;
			X = 2.0f / static_cast<float>(AppiManager.m_ClientRect.m_Width);;
			Y = 2.0f / static_cast<float>(AppiManager.m_ClientRect.m_Height);;

			Translation.X = (-1.0f + (static_cast<float>(_Size.X + (_Size.m_Width / 2.0f)) * X));
			Translation.Y = (1.0f - (static_cast<float>(_Size.Y + (_Size.m_Height / 2.0f)) * Y));

			Transform = soMath::TranslationMatrix4x4(Translation) * soMath::ScalarMatrix4x4(Scalation);
			float NormalRender = 0.0f;
			AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_2], &NormalRender);
			AppiManager.VSSetConstantBuffers(Slot_2, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_2]);
		}
		else
		{
			float NormalRender = 1.0f;
			AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_2], &NormalRender);
			AppiManager.VSSetConstantBuffers(Slot_2, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_2]);
		}

		AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
		AppiManager.VSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_0]);

		AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &Transform);
		AppiManager.VSSetConstantBuffers(Slot_1, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

		//Seteamos e imprimimos las caracteristicas del modelo (Texturas del material, VBuffer, IBuffer, Topologia)
		((soModel*)m_ScreenAlignedQuad)->Render(MAT_PROP_ALBEDO, m_ScreenAlignedQuad->m_DefaultMaterialID);

		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Texture_Slot_0);

		AppiManager.SetRenderTargets(NULL, 0, false);

		Final = _Dummy;
		return EC_OK;
	}

	RESULT soRenderManager::AplyTerrain()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		soTechnique* Terrain_Technique = m_EfectList.at(ARE_Terrain)->GetTechnique(AT_Standard);

		/************************************************************************/
		/* EditorSelection                                                      */
		/************************************************************************/
		soRenderPass* WorkPass = Terrain_Technique->GetPass(Slot_0);

		//Seteamos los componentes del pase
		WorkPass->Render();

		AppiManager.SetRenderTargets(m_BackBuffer, 1, true);

		AppiManager.PSSetSampler(Slot_1, Slot_1, m_PointSampler);

		//Seteamos los constant buffer necesarios del shader 
		uint32 NumBuffers = 1;
		AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
		AppiManager.VSSetConstantBuffers(Slot_0, NumBuffers, WorkPass->m_VertexShader.m_ptrBuffer[Slot_0]);
		
		soTerrain::Instance().Render();

		AppiManager.SetRenderTargets(NULL, 0, false);
		
		return EC_OK;
	}

	RESULT soRenderManager::AplySkyBox()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		soTechnique* SkyBox_Technique = m_EfectList.at(ARE_SkyBox)->GetTechnique(AT_Standard);

		/************************************************************************/
		/* SkyBox	                                                            */
		/************************************************************************/
		soRenderPass* WorkPass = SkyBox_Technique->GetPass(Slot_0);
		WorkPass->Render();

		AppiManager.SetRenderTargets(m_BackBuffer, 1, true);

		AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

		//Seteamos los constant buffer necesarios del shader por modelo{
		float Scale = (m_FrameBuffer.mCameraNearFar.Y - m_FrameBuffer.mCameraNearFar.X);
		Scale -= Scale*0.05f;
		soMatrix4x4 World = soMath::TranslationMatrix4x4(m_FrameBuffer.viewPos) * soMath::ScalarMatrix4x4(soVector3D(Scale, Scale, Scale));
		
		AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_0], &m_FrameBuffer);
		AppiManager.VSSetConstantBuffers(Slot_0, Slot_1, WorkPass->m_VertexShader.m_ptrBuffer[Slot_0]);

		AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_1], &World);
		AppiManager.VSSetConstantBuffers(Slot_1, Slot_1, WorkPass->m_VertexShader.m_ptrBuffer[Slot_1]);

		((soModel*)m_SkyBoxMesh)->Render(MAT_PROP_ALBEDO, m_SkyBoxMesh->m_DefaultMaterialID);

		AppiManager.SetRenderTargets(NULL, 0, false);

		return EC_OK;
	}

	RESULT soRenderManager::AplyEditorGUI()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();
		AppiManager.ClearBackBuffer(&AppiManager.m_DefaultBackBuffer);

		soImGuiManager& _GUI = soImGuiManager::Instance();

		soTechnique* EditorGUI_Technique = m_EfectList.at(ARE_EditorGUI)->GetTechnique(AT_Standard);

		/************************************************************************/
		/* EditorGUI                                                            */
		/************************************************************************/
		soRenderPass* WorkPass = EditorGUI_Technique->GetPass(Slot_0);
		WorkPass->Render();

		AppiManager.SetRenderTargets(&AppiManager.m_DefaultBackBuffer, 1, false);
		AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

		AppiManager.UpdateSubresource(WorkPass->m_VertexShader.m_ptrBuffer[Slot_0], &_GUI.m_OtographicProyection);
		AppiManager.VSSetConstantBuffers(Slot_0, Slot_1, WorkPass->m_VertexShader.m_ptrBuffer[Slot_0]);

		_GUI.Render();
	
		AppiManager.SetRenderTargets(NULL, 0, false);

		return EC_OK;
	}

	RESULT soRenderManager::ApplyMultyPassViewer()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();		

		soTechnique* PBR_Technique = m_EfectList.at(ARE_PBR)->GetTechnique(AT_Standard);
		soTechnique* MultyPassViewer_Technique = m_EfectList.at(ARE_MultyPassViewer)->GetTechnique(AT_Standard);

		/************************************************************************/
		/* MultyPassViewer                                                      */
		/************************************************************************/
		soTexture Albedo, Normal, Roughness, Metallic, SSAO, Final, Dummy;

		Albedo.m_Texture = *PBR_Technique->GetPass(RP_GBuffer)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;
		Normal.m_Texture = *PBR_Technique->GetPass(RP_GBuffer)->GetRenderTarget(Slot_1)->m_ptrRenderTargetTexture;		
		Roughness.m_Texture = *PBR_Technique->GetPass(RP_GBuffer)->GetRenderTarget(Slot_3)->m_ptrRenderTargetTexture;
		Metallic.m_Texture = *PBR_Technique->GetPass(RP_GBuffer)->GetRenderTarget(Slot_4)->m_ptrRenderTargetTexture;
		SSAO.m_Texture = *PBR_Technique->GetPass(RP_SSAO)->GetRenderTarget(Slot_0)->m_ptrRenderTargetTexture;		
		Final.m_Texture = *m_BackBuffer->m_ptrRenderTargetTexture;

		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Albedo, Slot_0);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Normal, Slot_1);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Roughness, Slot_2);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Metallic, Slot_3);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&SSAO, Slot_4);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(&Final, Slot_5);

		soRenderPass* WorkPass = MultyPassViewer_Technique->GetPass(Slot_0);
		WorkPass->Render();

		AppiManager.ClearBackBuffer(WorkPass->GetRenderTarget(Slot_0));
		AppiManager.SetRenderTargets(WorkPass->GetRenderTarget(Slot_0), 1, false);
		AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

		((soModel*)m_ScreenAlignedQuad)->Render(Shader_ALL_Slot, m_ScreenAlignedQuad->m_DefaultMaterialID);

		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_0);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_1);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_2);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_3);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_4);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_5);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_6);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_7);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_8);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_9);
		soMaterialManager::Instance().GetMaterialByID(m_ScreenAlignedQuad->m_DefaultMaterialID[0])->AssingTexture(NULL, Slot_10);

		AppiManager.SetRenderTargets(NULL, 0, false);
		AppiManager.CleanResources();

		Albedo = Normal = Roughness = Metallic = SSAO = Final = Dummy;

		return EC_OK;
	}

	RESULT soRenderManager::ApplyFontSystem()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();
		soFont& Font = soFont::Instance();

		soTechnique* FontSystem_Technique = m_EfectList.at(ARE_FontSystem)->GetTechnique(AT_Standard);

		/************************************************************************/
		/* EditorGUI                                                            */
		/************************************************************************/
		soRenderPass* WorkPass = FontSystem_Technique->GetPass(Slot_0);
		WorkPass->Render(); //Setea el Pixel, Vertex y Render Target

		//Seteamos los componentes del pase
		//	Set vertex buffer
		UINT stride = sizeof(FontVertex);
		UINT offset = 0;
		AppiManager.m_Device.m_ptrD3DContext->IASetVertexBuffers(0, 1, &m_dynamicVertexBufferFont, &stride, &offset);

		AppiManager.m_Device.m_ptrD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		AppiManager.PSSetShaderResources(0, 1, &m_FontTexture->m_Texture);

		AppiManager.PSSetSampler(Slot_0, Slot_1, m_DefaultSampler);

		Font.Render(m_dynamicVertexBufferFont);

		AppiManager.SetRenderTargets(NULL, 0, false); // Se limpia el render target

		return EC_OK;
	}

	soEffect* soRenderManager::GetEffectByID(uint8 _ID)
	{
		return m_EfectList[_ID];
	}
#ifdef LIGHT_TEST
	void soRenderManager::SetActiveLights(uint32 activeLights)
	{
		if (activeLights != m_ActiveLights)
		{
			m_ActiveLights = activeLights;

			m_LightBuffer.ShutDown();

			StructuredBufferData SBD;
			SBD.D3D11_BIND_FLAG = D3D11_BIND_SHADER_RESOURCE;
			SBD.Elements = m_ActiveLights;
			SBD.Dynamic = true;

			m_LightBuffer.StartUp(SBD);
		}
	}

	void soRenderManager::SetupLights()
	{
		// Copy light list into shader buffer
		DirectionalLightCB* light = m_LightBuffer.MapDiscard();
		for (uint32 i = 0; i < m_ActiveLights; ++i)
		{
			light[i].LigthColor = m_LightCB.LigthColor;
			light[i].LigthPosition = m_LightCB.LigthPosition;
		}
		m_LightBuffer.Unmap();


		m_LightShaderResourceBuffer.m_Texture.m_ptrSRTexture = m_LightBuffer.m_ShaderResource;
	}
#endif // LIGHT_TEST
		
	RESULT soRenderManager::OnStartUp()
	{	
		if (FAIL(LoadRenderResources()))
		{
			return EC_FALSE;
		}

		if (FAIL(InitPBR()))
		{
			return EC_FALSE;
		}

		if (FAIL(InitSkyBox()))
		{
			return EC_FALSE;
		}

		if (FAIL(InitTerrain()))
		{
			return EC_FALSE;
		}

		if (FAIL(InitEditorSelection()))
		{
			return EC_FALSE;
		}

		if (FAIL(InitGraphicDebug()))
		{
			return EC_FALSE;
		}

		if (FAIL(InitFinalPassEffect()))
		{
			return EC_FALSE;
		}

		if (FAIL(InitEditorGUI()))
		{
			return EC_FALSE;
		}
		
		if (FAIL(InitMultyPassViewer()))
		{
			return EC_FALSE;
		}

		if (FAIL(InitFontSystem()))
		{
			return EC_FALSE;
		}

		BlendData BD;
		m_DefaultBlend.StartUp(BD);
		
		for (uint32 i = 0; i < 8; i++)
		{
			BD.RenderTarget[i].BlendEnable = true;
			BD.RenderTarget[i].SrcBlend = API11_BLEND_SRC_COLOR;
			BD.RenderTarget[i].DestBlend = API11_BLEND_DEST_COLOR;
			BD.RenderTarget[i].BlendOp = API11_BLEND_OP_MAX;
			BD.RenderTarget[i].SrcBlendAlpha = API11_BLEND_ONE;
			BD.RenderTarget[i].DestBlendAlpha = API11_BLEND_ONE;
			BD.RenderTarget[i].BlendOpAlpha = API11_BLEND_OP_ADD;
		}
		m_AditiveBlend.StartUp(BD);
		
		SamplerData SD;
		m_DefaultSampler.StartUp(SD);

		SD.D3D11_FILTER = API_FILTER_MIN_MAG_MIP_POINT;
		m_PointSampler.StartUp(SD);

		//Informacion de PBR
		soString AssetPath(TEXT("Resources\\Assets\\PBRData.SoulEngineAsset"));
		PBRData* NewData = static_cast<PBRData*>(soAssetManager::Instance().LoadAssetFromFile(AssetPath));

		if (NewData)
		{
			m_PBRData = *NewData;
			SAFE_DELETE(NewData);
		}
				
		return EC_OK;
	}

	void soRenderManager::OnShutDown()
	{
		std::map<uint8, soEffect*>::iterator It = m_EfectList.begin();
		while (It != m_EfectList.end())
		{
			SAFE_DELETE((It->second));
			m_EfectList.erase(It);
			It = m_EfectList.begin();
		}

		soDXGIManager::ShutDown();
	}
		
	void soRenderManager::Render(std::vector<soSceneNode*>* _Nodelist, soRenderTarget* _BackBuffer /* = NULL */, uint32 _RenderKey /* = 0 */)
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();
		m_Camera = soCameraManager::Instance().m_ActiveCamera;
		m_BackBuffer = _BackBuffer;
		m_SceneNodeList = _Nodelist;

		if (m_BackBuffer == NULL)
			m_BackBuffer = &AppiManager.m_DefaultBackBuffer;
		
		ViewPortData VP;
		VP.Width = (float)m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Width;
		VP.Height = (float)m_BackBuffer->m_ptrRenderTargetTexture->m_TextureSize.m_Height;
		m_BlurViewPort.ShutDown();
		m_BlurViewPort.StartUp(VP);

		//Directx settings
		AppiManager.SetViewPort(m_BlurViewPort);
		AppiManager.SetBlendState(m_DefaultBlend, 0, 0xFFFFFFFF);		
		AppiManager.CleanDepthStencil();
		AppiManager.ClearBackBuffer(m_BackBuffer);		

		UpdateCameraBuffer();

		if (_RenderKey & RID_PBR || _RenderKey == 0)
		{
			AplyPBR();
		}
		AppiManager.SetBlendState(m_AditiveBlend, 0, 0xFFFFFFFF);
		if (_RenderKey & RID_Terrain || _RenderKey == 0)
		{
			AplyTerrain();
		}
		if (_RenderKey & RID_SkyBox || _RenderKey == 0)
		{
			AplySkyBox();			
		}
		if (_RenderKey & RID_EditorSelection || _RenderKey == 0)
		{
			AplyEditorSelection();			
		}
		if (_RenderKey & RID_GraphicDebuger || _RenderKey == 0)
		{
			AplyGraphicDebug();
		}

		if (_RenderKey & RID_MultyPass || _RenderKey == 0)
		{
			ApplyMultyPassViewer();
		}

		if (_RenderKey & RID_GUI)
		{
			AplyEditorGUI();
		}
	
		if (_RenderKey & RID_FontSystem)
		{
			ApplyFontSystem();
		}
		
	}

	void soRenderManager::UpdateCameraBuffer()
	{
		soMatrix4x4 cameraProj = m_Camera->GetProjectionMatrix();
		soMatrix4x4 cameraView = m_Camera->GetViewMatrix();
		soVector4D ViewPos = m_Camera->m_Position; ViewPos.W = 1;

		soRect ClientSize = soRect(0, 0, (uint32)m_Camera->m_CameraViewPort.m_ViewPort.Width, (uint32)m_Camera->m_CameraViewPort.m_ViewPort.Height);
	
		m_FrameBuffer.mCameraView = soMath::TransposeMatrix(cameraView);
		m_FrameBuffer.mCameraProj = soMath::TransposeMatrix(cameraProj);
		m_FrameBuffer.mCameraViewProj = soMath::TransposeMatrix(cameraView * cameraProj);
		m_FrameBuffer.viewPos = ViewPos;
		m_FrameBuffer.mCameraNearFar = soVector4D(m_Camera->m_Near, m_Camera->m_Far, 0.0f, 0.0f);
		m_FrameBuffer.ViewportDimensions[0] = ClientSize.m_Width;
		m_FrameBuffer.ViewportDimensions[1] = ClientSize.m_Height;
	}

	void soRenderManager::Resize(const soRect& _ClientSize)
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		if (FAIL(AppiManager.Resize(_ClientSize)))
		{
			return;
		}	

		if (_ClientSize.m_Width > 1 && _ClientSize.m_Height > 1)
		{			
			/************************************************************************/
			/* Pass resize                                                          */
			/************************************************************************/
			for (uint8 i = 0; i < static_cast<uint8>(m_EfectList.size()) - 1; i++)
			{
				m_EfectList.at(i)->Resize();
			}

#ifdef LIGHT_TEST
			/************************************************************************/
			/* Ligth buffers                                                        */
			/************************************************************************/
			m_LitBufferCS.ShutDown();

			StructuredBufferData SBD;
			SBD.Elements = soDXGIManager::Instance().m_ClientRect.m_Width * soDXGIManager::Instance().m_ClientRect.m_Height;
			SBD.D3D11_BIND_FLAG = API_BIND_UNORDERED_ACCESS | API_BIND_SHADER_RESOURCE;
			m_LitBufferCS.StartUp(SBD);
#endif // LIGHT_TEST
		}
	}

	std::vector <soRenderTarget*> soRenderManager::GetPassRenderTargets(AvilableRenderEffect _EffectID, RenderPass _PassID)
	{
		std::vector<soRenderTarget*> RTList;
		soRenderPass* Pass = m_EfectList.at((uint8)_EffectID)->GetTechnique((uint8)AT_Standard)->GetPass((uint8)_PassID);

		if (Pass == NULL)
		{
			RTList.push_back(m_BackBuffer);
			return RTList;
		}

		for (uint8 i = 0; i < Pass->m_RenderTargetID; i++)
		{
			RTList.push_back(&Pass->m_RenderTargetArray[i]);
		}

		return RTList;
	}	
}