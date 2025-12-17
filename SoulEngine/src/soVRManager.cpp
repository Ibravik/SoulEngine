/********************************************************************/
/**
* @LC     10/30/2016
* @file   soAnimationDataManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/30/2016
* @brief  Implementacion de la clase soVRManager
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soCameraManager.h"
#include "soWorldManager.h"
#include "soGraphicDebugManager.h"
#include "soVRManager.h"

/************************************************************************/
/* Definicion de la clase soResourceManager                             */
/************************************************************************/
namespace SoulSDK
{
	soVRManager::soVRManager()
	{
		Connect(std::bind(&soVRManager::OnStartUp, this));
		m_LeftEyeCamera = NULL;
		m_RightEyeCamera = NULL;
		m_ptrVRSystem = NULL;
		m_ControlGUI = soRect(0, 0, 1, 1);
	}

	soVRManager::~soVRManager()
	{
	}

	RESULT soVRManager::OnStartUp()
	{
		// Activacion de dispositivo VR
		vr::EVRInitError eError = vr::VRInitError_None;
		m_ptrVRSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);

		if (eError != vr::VRInitError_None)
		{
			SOE_OutputLOG("VRInit", "Init", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
			m_ptrVRSystem = NULL;
			return EC_FALSE;
		}

		// Generacion de camaras segu el ojo
		if (FAIL(EyeCameraStartUp()))
		{
			return EC_FALSE;
		}

		//Inicializacion de componentes del dispositivo VR
		if (FAIL(InitCompositor()))
		{
			return EC_FALSE;
		}

		//Inicializacion de GUI de dispositivos		
		soComponentFactory Factory;
		ComponentData CD;
		CD._ComponentType = CT_Mesh_Static;

		//HeadSet		
		if (FAIL(soWorldManager::Instance().CreateNewActor(TEXT("HeadSet"))))
		{
			SOE_OutputLOG("VRInit", "Init", "Load model fail");
			return EC_FALSE;
		}
		m_DeviceData[VRC_HeadSet].m_VRActor = soWorldManager::Instance().m_ActorList.back();
		CD.ResourcePath = TEXT("Resources\\Models\\HeadSetVR.fbx");
		m_DeviceData[VRC_HeadSet].m_VRActor->AddNewComponent(Factory.CreateComponent(CD));

		//LeftControl		
		if (FAIL(soWorldManager::Instance().CreateNewActor(TEXT("ControlVRLeft"))))
		{
			SOE_OutputLOG("VRInit", "Init", "Load model fail");
			return EC_FALSE;
		}
		m_DeviceData[VRC_LeftControl].m_VRActor = soWorldManager::Instance().m_ActorList.back();
		CD.ResourcePath = TEXT("Resources\\Models\\ControlVR.fbx");
		m_DeviceData[VRC_LeftControl].m_VRActor->AddNewComponent(Factory.CreateComponent(CD));

		//RigthControl	
		if (FAIL(soWorldManager::Instance().CreateNewActor(TEXT("ControlVRight"))))
		{
			SOE_OutputLOG("VRInit", "Init", "Load model fail");
			return EC_FALSE;
		}
		m_DeviceData[VRC_RightControl].m_VRActor = soWorldManager::Instance().m_ActorList.back();
		CD.ResourcePath = TEXT("Resources\\Models\\ControlVR.fbx");
		m_DeviceData[VRC_RightControl].m_VRActor->AddNewComponent(Factory.CreateComponent(CD));
		
		return EC_OK;
	}

	void soVRManager::OnShutDown()
	{
		if (m_ptrVRSystem)
		{
			vr::VR_Shutdown();
			m_ptrVRSystem = NULL;
		}
	}

	soMatrix4x4 soVRManager::GetEyeProyection(vr::Hmd_Eye nEye)
	{
		if (!m_ptrVRSystem)
		{
			return soMatrix4x4();
		}

		vr::HmdMatrix44_t mat = m_ptrVRSystem->GetProjectionMatrix(nEye, 0.02f, 1000.0f);

		soMatrix4x4 matrixObj(
			mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
			mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
			mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
			mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
		);

		return matrixObj;
	}

	soMatrix4x4 soVRManager::GetEyeOffsetMatrix(vr::Hmd_Eye nEye)
	{
		if (!m_ptrVRSystem)
		{
			return soMatrix4x4();
		}

		vr::HmdMatrix34_t matPose = m_ptrVRSystem->GetEyeToHeadTransform(nEye);

		soMatrix4x4 matrixObj(
			matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
			matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
			matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
			matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
		);

		return soMath::MatrixInverse4x4(matrixObj);
	}

	soMatrix4x4 soVRManager::ConvertSteamVRMatrixTosoMatrix4x4(const vr::HmdMatrix34_t &matPose)
	{
		soMatrix4x4 matrixObj(
			matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
			matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
			matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
			matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
		);

		return matrixObj;
	}

	RESULT soVRManager::InitCompositor()
	{
		if (!vr::VRCompositor())
		{
			SOE_ThrowException("VRInit", "Init", "%s - Failed to initialize VR Compositor!\n, __FUNCTION__");
			return EC_FALSE;
		}

		return EC_OK;
	}

	RESULT soVRManager::CreateEyeRenderTarget(soRect& _BackBufferSize)
	{
		TextureData T2D;
		T2D.T2DData.TextureSize = _BackBufferSize;
		T2D.T2DData.DXGI_FORMAT = APIGI_FORMAT_R8G8B8A8_UNORM;
		T2D.T2DData.D3D11_BIND_FLAG = API_BIND_RENDER_TARGET | API_BIND_SHADER_RESOURCE;

		RenderTargetData RTD;
		RTD._TextureData = T2D;

		RTD._CleanColor = soVector4D(0, 0, 0, 1);
		//"LeftEye"
		if (FAIL(m_LeftEyeRT.StartUp(RTD)))
		{
			return EC_FALSE;
		}

		RTD._CleanColor = soVector4D(0, 0, 0, 1);
		//"RightEye"
		if (FAIL(m_RightEyeRT.StartUp(RTD)))
		{
			return EC_FALSE;
		}

		//Init GUI Render Target
		RTD._CleanColor = soVector4D(0, 0, 0, 1);
		if (FAIL(m_GUIVR.StartUp(RTD)))
		{
			return EC_FALSE;
		}

		return EC_OK;
	}

	RESULT soVRManager::EyeCameraStartUp()
	{
		m_ptrVRSystem->GetRecommendedRenderTargetSize(&m_RecomendedBufferSize.m_Width, &m_RecomendedBufferSize.m_Height);

		CameraData CD;
		soMatrix4x4 Projection;

		CD.CameraName = TEXT("VRLeft");
		m_LeftEyeCamera = soCameraManager::Instance().CreateStandardCamera(CD);
		if (m_LeftEyeCamera == NULL)
		{
			return EC_FALSE;
		}

		Projection = GetEyeProyection(vr::Eye_Left);
		m_LeftEyeCamera->SetProjectionMatrix(Projection);

		CD.CameraName = TEXT("VRRigth");		
		m_RightEyeCamera = soCameraManager::Instance().CreateStandardCamera(CD);
		if (m_RightEyeCamera == NULL)
		{
			return EC_FALSE;
		}

		Projection = GetEyeProyection(vr::Eye_Right);
		m_RightEyeCamera->SetProjectionMatrix(Projection);

		soRect BFSize(0, 0, (uint32)m_LeftEyeCamera->m_CameraViewPort.m_ViewPort.Width, (uint32)m_LeftEyeCamera->m_CameraViewPort.m_ViewPort.Height);
		if (FAIL(CreateEyeRenderTarget(BFSize)))
		{
			return EC_FALSE;
		}

		return EC_OK;
	}
	
	RESULT soVRManager::EyeUpdate()
	{
		//Actualizamos proyeccion y vista
		soMatrix4x4 Projection;
		Projection = GetEyeProyection(vr::Eye_Left);
		m_LeftEyeCamera->SetProjectionMatrix(Projection);

		Projection = GetEyeProyection(vr::Eye_Right);
		m_RightEyeCamera->SetProjectionMatrix(Projection);

		soMatrix4x4 LeftHeadOffset = GetEyeOffsetMatrix(vr::Eye_Left);
		soMatrix4x4 RightHeadOffset = GetEyeOffsetMatrix(vr::Eye_Right);

		soMatrix4x4 View;

		View = soMath::TransposeMatrix(soMath::MatrixInverse4x4(m_DeviceData[VRC_HeadSet].m_WorldMatrix)) * LeftHeadOffset;
		m_LeftEyeCamera->SetViewMatrix(View);

		View = soMath::TransposeMatrix(soMath::MatrixInverse4x4(m_DeviceData[VRC_HeadSet].m_WorldMatrix)) * RightHeadOffset;
		m_RightEyeCamera->SetViewMatrix(View);

		return EC_OK;
	}

	void soVRManager::InputHandler()
	{
		vr::VREvent_t event;
		while (m_ptrVRSystem->PollNextEvent(&event, sizeof(event)))
		{
			ProcessEvent(event);
		}			
	}

	void soVRManager::ProcessEvent(const vr::VREvent_t & event)
	{
		//Checar si los eventos se ejecutan dependiendo el controller
		switch (event.eventType)
		{
		case vr::VREvent_TrackedDeviceActivated:
		{
		}
		break;
		case vr::VREvent_TrackedDeviceDeactivated:
		{
		}
		break;
		case vr::VREvent_TrackedDeviceUpdated:
		{
		}
		break;
		case vr::VREvent_ButtonPress:
			switch (event.data.controller.button)
			{
			case vr::k_EButton_System:				
				if (m_SignedList.empty())
					break;

				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			case vr::k_EButton_ApplicationMenu:
				if (m_SignedList.empty())
					break;

				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			case vr::k_EButton_Grip:
				if (m_SignedList.empty())
					break;

				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			case vr::k_EButton_Axis0:
				if (m_SignedList.empty())
					break;
				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			case vr::k_EButton_Axis1:
				if (m_SignedList.empty())
					break;

				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			default:
				break;
			}
			break;

		case vr::VREvent_ButtonUnpress:
			switch (event.data.controller.button)
			{
			case vr::k_EButton_System:
				if (m_SignedList.empty())
					break;

				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			case vr::k_EButton_ApplicationMenu:
				if (m_SignedList.empty())
					break;

				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			case vr::k_EButton_Grip:
				if (m_SignedList.empty())
					break;

				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			case vr::k_EButton_Axis0:
				if (m_SignedList.empty())
					break;
				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			case vr::k_EButton_Axis1:
				if (m_SignedList.empty())
					break;

				for (size_t i = 0; i < m_SignedList.size(); i++)
				{
					if (m_SignedList[i]._Event == event.eventType && m_SignedList[i].button == event.data.controller.button && GetDeviceID(m_SignedList[i].DeviceID) == event.trackedDeviceIndex)
					{
						m_SignedList[i].callbackfunction();
					}
				}
				break;
			default:
				break;
			}
			break;

		case vr::VREvent_TouchPadMove:
			break;
		default:
			break;
		}
	}

	void soVRManager::GenerateRay(uint32 _DeviceID)
	{
		soVector4D start = soVector4D(0, 0, -0.05f, 1) * m_DeviceData[_DeviceID].m_WorldMatrix;
		soVector4D end = soVector4D(0, 0, -1000.0f, 1) * m_DeviceData[_DeviceID].m_WorldMatrix;

		m_DeviceData[_DeviceID].m_Ray.m_VecOrigin = start;
		m_DeviceData[_DeviceID].m_Ray.m_VecDirection = end - start;
		soMath::NormalizeVector(m_DeviceData[_DeviceID].m_Ray.m_VecDirection);

		soGraphicDebugManager::Instance().AddDebugLine(start, end, false, soColorRGBA());
	}

	void soVRManager::UpdateDevices()
	{
		vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

		uint32 ControlIndex = 0;
		for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
		{
			if (!m_ptrVRSystem->IsTrackedDeviceConnected(nDevice))
				continue;

			if (m_ptrVRSystem->GetTrackedDeviceClass(nDevice) == vr::TrackedDeviceClass_HMD)
			{
				if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
				{
					m_DeviceData[ControlIndex].m_WorldMatrix = soMath::TransposeMatrix(ConvertSteamVRMatrixTosoMatrix4x4(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking));
					m_DeviceData[ControlIndex].m_VRActor->m_ActorTransform.m_ParentTransform = m_DeviceData[ControlIndex].m_WorldMatrix;
					m_DeviceData[ControlIndex].m_DeviceID = nDevice;
					ControlIndex++;
				}
			}

			else if (m_ptrVRSystem->GetTrackedDeviceClass(nDevice) == vr::TrackedDeviceClass_Controller)
			{
				if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
				{
					m_DeviceData[ControlIndex].m_WorldMatrix = soMath::TransposeMatrix(ConvertSteamVRMatrixTosoMatrix4x4(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking));
					m_DeviceData[ControlIndex].m_VRActor->m_ActorTransform.m_ParentTransform = m_DeviceData[ControlIndex].m_WorldMatrix;
					m_DeviceData[ControlIndex].m_DeviceID = nDevice;
					GenerateRay(ControlIndex);
					ControlIndex++;
				}
			}
		}
	}

	uint32 soVRManager::GetDeviceID(VRDevice _InternalID)
	{
		return m_DeviceData[_InternalID].m_DeviceID;
	}

	uint32 soVRManager::GetDeviceInternalID(uint32 _AppiID)
	{
		for (uint32 i = 0; i < VRC_TotalDevices; i++)
		{
			if (m_DeviceData[i].m_DeviceID == _AppiID)
			{
				return i;
			}
		}		
		return soMath::MAX_INT32;
	}

	void soVRManager::SuscribeFunction(VRSuscriptionData& _data)
	{
		m_SignedList.push_back(_data);
	}

	void soVRManager::Update(float _DeltaTime)
	{
		(void)_DeltaTime;
		UpdateDevices();
		EyeUpdate();
		InputHandler();
	}

	void soVRManager::Render()
	{
		vr::Texture_t leftEyeTexture = { m_LeftEyeRT.m_ptrRenderTargetTexture->m_ptrTexture2D, vr::TextureType_DirectX, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
		vr::Texture_t rightEyeTexture = { m_RightEyeRT.m_ptrRenderTargetTexture->m_ptrTexture2D, vr::TextureType_DirectX, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
	}
}
