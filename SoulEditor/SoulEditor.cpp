/************************************************************************/
/* Inclucion de librerias necesarias para compilacion                   */
/************************************************************************/
#include "stdafx.h"
#include "SoulEditor.h"

/************************************************************************/
/* Definiciones de caracteristicas de compilacion                       */
/************************************************************************/
//#define VR_EDITOR										// Activa la Compativilidad con VR
//#define RUBEN
//#define RUBENPHYS			
#define MAX_LOAD_STRING 255								// Tamaño maximo de una cadena de caracteres para carga 
#define HD_WIDTH 1280
#define  HD_HEIGHT 720
#define MS_PER_UPDATE 16.7                           //Tiempo de update en milisegundos

//#define RUBENFSM

/************************************************************************/
/* Namespace propio de la aplicacion                                    */
/************************************************************************/
using namespace SoulSDK;

/************************************************************************/
/* Variables globales de la Ventana		                                */
/************************************************************************/
HWND g_hwnd;											// Global window handle
HINSTANCE hInst;										// current instance
WCHAR g_Title[MAX_LOAD_STRING];							// The title bar text
WCHAR g_WindowClass[MAX_LOAD_STRING];					// The main window class name
HACCEL g_hAccelTable;									// Tabla perteneciente a la ventana
RECT g_WindowRect;										// Tamaño de la ventana
RECT g_ClientRect;										// Tamaño del area cliente
soPoint g_ClientMousePosition;							// Posicion actual del raton respecto a la ventana
soPoint g_ClientMouseLastPosition;						// Posicion del raton en el frame anterior respecto a la ventana
soEditorControl g_EditorControl;						// Abstraccion de informacion para manejo de actores seleccionados 
soRay g_Ray;											// Rayo generado a partir de un click en pantalla o por medio de un control VR
bool g_IsWebWindow = true;

/************************************************************************/
/* Variables de control de perfomance                                   */
/************************************************************************/
unsigned int g_FPSCount = 0;							// Contador de frames per seccond
float g_FPS = 0;										// Medidor de frames per seccond

#if defined(PROFILE)
soProfiler g_editorProfiler;
#endif


soTimer g_editorTimer;

/************************************************************************/
/* Manejo de multi camaras para modo editor                             */
/************************************************************************/
uint8 g_TotalActiveCameras = 4;							// Numero de camaras activas maximo
soVector3D g_TranslationSpeed(100, 100, 100);			// Velocidad de traslacion de la camara 
soVector3D g_PanSpeed(800, 800, 800);					// Velocidad de paneo de la camara 
soVector3D g_RotationSpeed(1, 1, 1);					// Velocidad de rotacion de la camara 
float g_FOV = 70;										// Field of view de la camara
uint32 ActiveRenderPass = 0;							// Banderas de imprecion de pases 

/************************************************************************/
/* Informacion de eventos                                               */
/************************************************************************/
enum InputEvent											// Eventos de entrada 
{
	IE_RBUTTONDOWN = 0x01,								// Click Mouse derecho
	IE_LBUTTONDOWN = 0x02,								// Click Mouse izquierdo
	IE_MBUTTONDOWN = 0x04,								// Click Mouse central
	IE_KBW = 0x08,										// Click Keyboard W
	IE_KBA = 0x10,										// Click Keyboard A
	IE_KBS = 0x20,										// Click Keyboard S
	IE_KBD = 0x40,										// Click Keyboard D
														/*IE_KBSpace = 0x80,
														CT_BillBoard = 0x100,
														CT_Listener = 0x200,
														CT_AudioSource = 0x400,
														CT_Physic = 0x800*/
};
uint64 g_InputEvent = 0;

/************************************************************************/
/* Variables globales del tiempo                                        */
/************************************************************************/
LARGE_INTEGER Fq;										// Frecuencia del procesador
//LARGE_INTEGER g_L0;										// Lectura anterior	
//LARGE_INTEGER g_L1;										// Lectura actual
float g_lastTime;
float g_actualTime;
float g_DeltaTime = 0.0f;					 			// Tiempo entre Frames 
static float g_ElapsedTime = 0.0f;								// Acumulador de tiempo
static float g_ElapsedTimeForProfiler = 0.0f;

#ifdef VR_EDITOR
														/************************************************************************/
														/* Variables de prueba VR                                               */
														/************************************************************************/
vr::IVRSystem *g_pHMD;
vr::IVRRenderModels *g_pRenderModels;
soString g_strDriver;
soString g_strDisplay;
bool g_VRRenderOpenFileGUI = false;
uint32 g_VRGUI = 0;
soPlane g_GuiCollitionPlane;
soVector3D g_CollitionPointGUI;

void VRSelection()
{
	g_Ray = soVRManager::Instance().m_DeviceData[VRC_LeftControl].m_Ray;

	g_EditorControl.SelectNodeModel(g_Ray);
}

#endif // VR_EDITOR

/************************************************************************/
/* App Function												            */
/************************************************************************/
#pragma region AppFunctions

void UpdateFPS(float _DeltaTime)
{
	g_ElapsedTime += _DeltaTime;

	if (g_ElapsedTime >= 1000)
	{
		g_FPS = (float)g_FPSCount;
		g_FPSCount = 0;
		g_ElapsedTime = 0.0f;
	}
}

void UpdateApp() //Update Input
{
	//Actualizacion del mouse
	POINT MousePos;
	GetCursorPos(&MousePos);
	ScreenToClient(g_hwnd, &MousePos);
	g_ClientMouseLastPosition = g_ClientMousePosition;
	g_ClientMousePosition.X = MousePos.x;
	g_ClientMousePosition.Y = MousePos.y;
}

RESULT InitEditorCamera()
{
	/* Creacion de Camara principal	                                        */
	CameraData CamInfo;
	CamInfo.AspectRatio = (float)(g_ClientRect.right) / (float)(g_ClientRect.bottom);
	CamInfo.Far = 10000;
	CamInfo.Near = 1;
	CamInfo.Front = soVector3D(0.0f, -1.0f, 1.0f);
	CamInfo.EnableUpMovement = true;
	CamInfo.Position = soVector3D(0, 400, -400);

	soString CamName = TEXT("EditorCamera");
	for (uint32 i = 0; i < 5; i++)
	{
		CamInfo.CameraName = CamName += uint2ws(i);
		if (soCameraManager::Instance().CreateEditorCamera(CamInfo) == NULL)
		{
			return EC_FALSE;
		}
	}

	return EC_OK;
}

RESULT InitModules()
{
	RECT rect;
	InitGraphicInfo _InitGraphicInfo(soRect(g_ClientRect.left, g_ClientRect.top, g_ClientRect.right, g_ClientRect.bottom), g_hwnd);
	if (FAIL(soDXGIManager::StartUp(_InitGraphicInfo)))
	{
		return EC_FALSE;
	}

	//IA
	if (FAIL(soAIManager::StartUp()))
	{
		return EC_FALSE;
	}

	//ResourceManager
	if (FAIL(soResourceManager::StartUp()))
	{
		return EC_FALSE;
	}

	//FileManager
	if (FAIL(soAssetManager::StartUp()))
	{
		return EC_FALSE;
	}

	//Camera Manager	
	if (FAIL(soCameraManager::StartUp()))
	{
		return EC_FALSE;
	}

	/* Creacion de la camara principal                                      */
	if (FAIL(InitEditorCamera()))
	{
		return EC_FALSE;
	}

	//WorldManager
	if (FAIL(soWorldManager::StartUp()))
	{
		return EC_FALSE;
	}

	//Material Manager
	if (FAIL(soMaterialManager::StartUp()))
	{
		return EC_FALSE;
	}

	//Font System
	FontData data;
	data.fontBinFilePath = soString(L"Resources\\BinFiles\\CourierNewRegular_Data.bin");
	if (GetWindowRect(g_hwnd, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		data.screenSize = soVector2D(static_cast<float>(width), static_cast<float>(height));
	}

	if (FAIL(soFont::StartUp(data)))
	{
		return EC_FALSE;
	}

	//soFont::Instance().DrawFontText(300, 500, soString(L"Hola Mundo......:!\nBuenas Nochiiis"));

	//Render Manager
	if (FAIL(soRenderManager::StartUp()))
	{
		return EC_FALSE;
	}
	g_EditorControl.StartUp();
	soRenderManager::Instance().m_EditorControl = &g_EditorControl;

	if (FAIL(soGraphicDebugManager::StartUp()))
	{
		return EC_FALSE;
	}

#ifdef VR_EDITOR
	//Files Manager
	/*if (FAIL(soFilesManager::StartUp(L"Resources\\Models\\VelaModel")))
	{
		return EC_FALSE;
	}*/

	//Files Manager
	//soFilesManager::StartUp(L"Resources\\Models");
	//soFilesManager::Instance().GetDirectoryFilesName();
	//soString cancer = soFilesManager::Instance().GetPathByID(1);
#endif

	//Editor GUI
	ImGuiData ED;
	ED.hwnd = g_hwnd;

	if (GetWindowRect(g_hwnd, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		ED.Width = static_cast<float>(width);
		ED.Height = static_cast<float>(height);
	}

	if (FAIL(soImGuiManager::StartUp(ED)))
	{
		return EC_FALSE;
	}

#ifdef RUBENFSM
	g_testActor = new SoulSDK::soActor();
	g_testActor->m_FSM.AddState(L"Idle", MachineTest);
	g_testActor->m_FSM.AddState(L"Attack", MachineTest2);
	g_testActor->m_FSM.AssignTransition(L"Idle", TransitionTest, L"Attack");

	g_testActor->m_FSM.SetInitState(L"Idle");
#endif


#ifdef RUBEN
	soOISManager::StartUp((const size_t)g_hwnd);
	soSuscription::StartUp();

	soOISManager::Instance().SetEventCallback(g_eventHandler);
	soOISManager::Instance().CreateComponents();

	//Default Inputs
	soOISManager::Instance().SetGamepadMap();

	//Owned Inputs
	//SoulSDK::soInputMap inputMap;
	//SoulSDK::GamepadInputData inputData;
	//inputData.A = SoulSDK::OISButtonState::OISBS_B;
	//inputData.B = SoulSDK::OISButtonState::OISBS_A;

	//inputMap.CreateGamepadMap(inputData);
	//soOISManager::Instance().SetGamepadMap(inputMap.GetButtonMap(), inputMap.GetAxisMap(), inputMap.GetDpadMap());

	soActor* asd = new soActor();
	soSuscription::Instance().Suscribe(asd, SoulSDK::OISButtonState::OISBS_A, &Call);
	soSuscription::Instance().Suscribe(asd, SoulSDK::OISButtonState::OISBS_B, &Call2);

	soOISManager::Instance().SetSignedActors(soSuscription::Instance().GetSignedList());

#endif // RUBEN

#ifdef VR_EDITOR
	if (FAIL(soVRManager::StartUp()))
	{
		return EC_FALSE;
	}
	VRSuscriptionData VR;
	VR.button = vr::k_EButton_Axis1;
	VR.DeviceID = VRC_LeftControl;
	VR._Event = vr::VREvent_ButtonPress;
	VR.callbackfunction = &VRSelection;
	soVRManager::Instance().SuscribeFunction(VR);

#endif // VR_EDITOR

#ifdef RUBENPHYS
	if (FAIL(soPhysicsManager::StartUp()))
	{
		return EC_FALSE;
	}
	soPhysicsManager::Instance().CreateScene();
#endif

	if (FAIL(soTerrain::StartUp()))
	{
		return EC_FALSE;
	}

	return EC_OK;
}

void FinishApp()
{
	soFont::ShutDown();
	soMaterialManager::ShutDown();
	soImGuiManager::ShutDown();
	soTerrain::ShutDown();
	soGraphicDebugManager::ShutDown();
#ifdef RUBENPHYS
	soPhysicsManager::ShutDown();
#endif	
	soWorldManager::ShutDown();
	soRenderManager::ShutDown();
	soCameraManager::ShutDown();
	soAssetManager::ShutDown();
	soResourceManager::ShutDown();
	soAIManager::ShutDown();
#ifdef VR_EDITOR
	//soFilesManager::ShutDown();
	soVRManager::ShutDown();

#endif // VR_EDITOR

#ifdef RUBEN
	soSuscription::ShutDown();
	soOISManager::ShutDown();
#endif


}

soString LoadFilePath()
{
	OPENFILENAME ofn;
	TCHAR CurrentDirectory[MAX_PATH];
	memset(&ofn, 0, sizeof(ofn));
	soString FilePath;
	TCHAR FileName[MAX_PATH];

	//Obtenemos el directorio actual, pera reestablecerlo cuando lo requiramos
	GetCurrentDirectory(MAX_PATH, CurrentDirectory);

	//Rellenamos la información de la estructura para establecer la carpeta inicial y los tipos de archivo soportados
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hwnd;
	ofn.lpstrInitialDir = TEXT("Resources");
	ofn.lpstrFile = FileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//Abrimos el dialogo para seleccionar un archivo
	if (GetOpenFileName(&ofn))
	{//El usuario seleccionó un archivo
		if (_tcslen(FileName) > 0)
		{//El nombre del archivo no está vacio

			for (size_t i = 0; FileName[i] != '\0'; i++)
			{
				FilePath.push_back(FileName[i]);
			}
		}
	}

	//Restablecemos la ruta inicial de la aplicación (esto es porque el diálogo cambia la carpeta de trabajo)
	//NOTA: Si no hicieramos esto aquí, cualquier próxima carga de archivos sin ruta completa fallará dentro de la aplicación, al igual que las escrituras estarán fuera de lugar
	SetCurrentDirectory(CurrentDirectory);
	return FilePath;
}

soString SaveFilePath()
{
	OPENFILENAME ofn;
	TCHAR CurrentDirectory[MAX_PATH];
	memset(&ofn, 0, sizeof(ofn));
	soString FilePath;
	TCHAR FileName[MAX_PATH];

	//Obtenemos el directorio actual, pera reestablecerlo cuando lo requiramos
	GetCurrentDirectory(MAX_PATH, CurrentDirectory);

	//Rellenamos la información de la estructura para establecer la carpeta inicial y los tipos de archivo soportados
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrInitialDir = TEXT("Resources");
	ofn.lpstrFile = FileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//Abrimos el dialogo para seleccionar un archivo
	if (GetSaveFileName(&ofn))
	{//El usuario seleccionó un archivo
		if (_tcslen(FileName) > 0)
		{//El nombre del archivo no está vacio

			for (size_t i = 0; FileName[i] != '\0'; i++)
			{
				FilePath.push_back(FileName[i]);
			}
		}
	}

	//Restablecemos la ruta inicial de la aplicación (esto es porque el diálogo cambia la carpeta de trabajo)
	//NOTA: Si no hicieramos esto aquí, cualquier próxima carga de archivos sin ruta completa fallará dentro de la aplicación, al igual que las escrituras estarán fuera de lugar
	SetCurrentDirectory(CurrentDirectory);
	return FilePath;
}

void UpdateCameraMovement()
{
	soCamera* tempCamera = soCameraManager::Instance().m_ActiveCamera;

	tempCamera->m_MaxRotationVel = g_RotationSpeed;
	if (!tempCamera->m_Orthographic)
	{
		if (g_InputEvent & IE_RBUTTONDOWN)
		{
			if (g_ClientMousePosition.X < g_ClientMouseLastPosition.X)
			{
				tempCamera->Yaw(false);
			}
			else if (g_ClientMousePosition.X > g_ClientMouseLastPosition.X)
			{
				tempCamera->Yaw(true);
			}

			if (g_ClientMousePosition.Y < g_ClientMouseLastPosition.Y)
			{
				tempCamera->Pitch(false);
			}
			else if (g_ClientMousePosition.Y > g_ClientMouseLastPosition.Y)
			{
				tempCamera->Pitch(true);
			}
		}
	}

	tempCamera->m_MaxVelocity = g_PanSpeed;
	if (g_InputEvent & IE_MBUTTONDOWN)
	{
		if (g_ClientMousePosition.X < g_ClientMouseLastPosition.X)
		{
			tempCamera->MoveLeft();
		}
		else if (g_ClientMousePosition.X > g_ClientMouseLastPosition.X)
		{
			tempCamera->MoveRight();
		}

		if (g_ClientMousePosition.Y < g_ClientMouseLastPosition.Y)
		{
			tempCamera->MoveUp();
		}
		else if (g_ClientMousePosition.Y > g_ClientMouseLastPosition.Y)
		{
			tempCamera->MoveDown();
		}
	}

	tempCamera->m_MaxVelocity = g_TranslationSpeed;
	if (g_InputEvent & IE_KBW)
	{
		tempCamera->MoveForward();
	}

	if (g_InputEvent & IE_KBA)
	{
		tempCamera->MoveLeft();
	}

	if (g_InputEvent & IE_KBS)
	{
		tempCamera->MoveBackward();
	}

	if (g_InputEvent & IE_KBD)
	{
		tempCamera->MoveRight();
	}
}
#pragma endregion

/************************************************************************/
/* Funciones de manejo de interfaz										*/
/************************************************************************/
#pragma region Variables
/************************************************************************/
/* Manejo de viewports                                                  */
/************************************************************************/
uint32 g_ActiveCamera = 0;
soRenderTarget* g_PaintRenderTarget = NULL;

/************************************************************************/
/* Variables de ventanas                                                */
/************************************************************************/
enum GUIFlags
{
	GUI_General = 0,
	GUI_PBR,
	GUI_Actor,
	GUI_EditorCamera,
	GUI_ViewPort,
	GUI_DirectionaLight,
	GUI_MaterialList,
#ifdef VR_EDITOR
	GUI_ViewportVR,
#endif // VR_EDITOR
#ifdef RUBENPHYS
	GUI_Physics,
#endif //Physics GUI
#if defined(PROFILE)
	GUI_Profile,
#endif
	GUI_ScenneGraph,
	GUI_Total
};
bool g_GUIFlags[]
{ 
	true, 
	false, 
	false,    
	false, 
	true , 
	false, 
	false, 
#ifdef VR_EDITOR
	true,
#endif // VR_EDITOR
#ifdef RUBENPHYS
	false,
#endif
#if defined(PROFILE)
	false,
#endif
	true
};

ImVec2 g_GeneralGUISize;
ImVec2 g_GeneralGUIPos;

/************************************************************************/
/* Estados para los items de las ventanas                               */
/************************************************************************/
enum ItemState
{
	IS_ClickedLeft = 0x01,
	IS_ClickedRight = 0x02,
	IS_Active = 0x04,
	IS_Hovered = 0x08,
	IS_Drag = 0x10,
	/*CT_Mesh_Dynamic = 0x20,
	CT_Camera_Free = 0x40,
	CT_BillBoard = 0x100,
	CT_Listener = 0x200,
	CT_AudioSource = 0x400,
	CT_Physic = 0x800*/
};
uint32 g_ItemState = 0;

/************************************************************************/
/* Manejo de scene graph                                                */
/************************************************************************/
soSceneNode* NewParent = NULL;
soSceneNode* NewChild = NULL;
bool g_SetParent = false;

/************************************************************************/
/* Manejo de botones e inputs                                           */
/************************************************************************/
uint32 g_InputID = 0;
const soString g_InputIDString = TEXT("##");
int32 g_SelectedMaterial = 0;
bool g_SetMaterial = false;

#pragma endregion

#pragma region Funciones
static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(200.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void SetDragAndInputFloat(const char* _InputName, float* _Var, float _Speed = 1.0f, float _Min = 0.0f, float _Max = 0.0f)
{
	ImGui::PushItemWidth(80);
	ImGui::DragFloat(ws2s(s2ws(std::string(_InputName)) + g_InputIDString + uint2ws(g_InputID)).c_str(), _Var, _Speed, _Min, _Max);
	g_InputID++;
	ImGui::PopItemWidth();
	ImGui::SameLine(); ShowHelpMarker("Ctrl + Lclick/Double Lclick to input with keyboard");
}

void InputString(soString* _String, char* _Buffer)
{
	ImGui::PushItemWidth(ImGui::GetWindowSize().x / 2.0f);
	ImGui::Text(ws2s(*_String).c_str());
	ImGui::SameLine();
	if (ImGui::InputText(ws2s(g_InputIDString + uint2ws(g_InputID)).c_str(), _Buffer, 30, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		*_String = soString(s2ws(std::string(_Buffer)));
	}
	g_InputID++;
	ImGui::PopItemWidth();
	ImGui::SameLine(); ShowHelpMarker("Input box to change string");
}

void Transform(soTransform& _Transform)
{
	static float PDragSpeed = 1.0f;
	static float SDragSpeed = 1.0f;
	static float RDragSpeed = 1.0f;

	ImGui::Text("Transform");
	if (ImGui::BeginPopupContextItem(ws2s(g_InputIDString + uint2ws(g_InputID)).c_str()))
	{
		SetDragAndInputFloat("Posittion Drag Speed", &PDragSpeed);
		SetDragAndInputFloat("Scalar Drag Speed", &SDragSpeed);
		SetDragAndInputFloat("Rotation Drag Speed", &RDragSpeed);
		if (ImGui::Button("Reset"))
		{
			PDragSpeed = 1.0f;
			SDragSpeed = 1.0f;
			RDragSpeed = 1.0f;
		}
		ImGui::EndPopup();
	}
	g_InputID++;
	ImGui::SameLine(); ShowHelpMarker("Right click to Transform Speed Options");
	if (ImGui::Button(ws2s(soString(TEXT("Focus In ViewPort")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
	{
		//Calculo de la direccion desde la pocicion actual
		soCamera* tempCamera = soCameraManager::Instance().m_ActiveCamera;
		soVector3D Dir = _Transform.m_Position - tempCamera->m_Position;
		soMath::NormalizeVector(Dir);
		tempCamera->m_FrontDir = Dir;
		tempCamera->IsDirty();
	}
	g_InputID++;
	ImGui::SameLine(); ShowHelpMarker("Set the focus active camera to transform position");
	ImGui::SameLine();
	if (ImGui::Button(ws2s(soString(TEXT("Reset Transform")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
	{
		_Transform.m_Scaling = soVector3D(1, 1, 1);
		_Transform.m_Position = soVector3D(0, 0, 0);
		_Transform.m_Rotation = soRotator();
	}
	g_InputID++;
	ImGui::SameLine(); ShowHelpMarker("Reser transform to original values");

	ImGui::Text("Position");
	{
		SetDragAndInputFloat("X", &_Transform.m_Position.X, PDragSpeed);
		ImGui::SameLine();
		SetDragAndInputFloat("Y", &_Transform.m_Position.Y, PDragSpeed);
		ImGui::SameLine();
		SetDragAndInputFloat("Z", &_Transform.m_Position.Z, PDragSpeed);
	}

	ImGui::Text("Scale");
	{
		SetDragAndInputFloat(" ", &_Transform.m_Scaling.X, SDragSpeed);
		_Transform.m_Scaling.Y = _Transform.m_Scaling.X;
		_Transform.m_Scaling.Z = _Transform.m_Scaling.X;
	}

	ImGui::Text("Rotation");
	{
		SetDragAndInputFloat("X", &_Transform.m_Rotation.m_Pich.m_fDegrees, RDragSpeed, 0.0f, 360.0f);
		ImGui::SameLine();
		SetDragAndInputFloat("Y", &_Transform.m_Rotation.m_Yaw.m_fDegrees, RDragSpeed, 0.0f, 360.0f);
		ImGui::SameLine();
		SetDragAndInputFloat("Z", &_Transform.m_Rotation.m_Roll.m_fDegrees, RDragSpeed, 0.0f, 360.0f);
	}
}

void ChangeMaterialTexture(soMaterial* _Material, uint8 _SlotTexture)
{
	soString Path = LoadFilePath();

	//Revisamos si se debe de cargar un archivo de mapa
	if (!Path.empty())
	{
		//Creamos las texturas como recursos para el modelo 
		ResourceParameters RP;
		RP.FilePath = Path;
		RP.ResourceType = RT_Texture;

		soTexture* NewTexture = static_cast<soTexture*>(soResourceManager::Instance().Load(RP));
		if (NewTexture != NULL && g_EditorControl.m_SelectedNode != NULL)
		{
			_Material->AssingTexture(NewTexture, _SlotTexture);
		}
	}
}

void DisplayMaterial(uint32 _MatID)
{
	ImGui::Columns(2, "Materian Info", true);
	for (uint8 j = 0; j < 4; j++)
	{
		soMaterial* ActualMaterial = soMaterialManager::Instance().GetMaterialByID(_MatID);
		if (ActualMaterial->m_Textures[j])
		{
			if (ImGui::ImageButton((ImTextureID*)ActualMaterial->m_Textures[j]->m_Texture.m_ptrSRTexture, ImVec2(200, 200)))
			{
				ChangeMaterialTexture(ActualMaterial, j);
			}
			ImGui::NextColumn();
			//informacion de la textura
			switch (j)
			{
			case 0:
				ImGui::Text("Chanel : Albedo");
				break;
			case 1:
				ImGui::Text("Chanel : Metallic");
				break;
			case 2:
				ImGui::Text("Chanel : Normal");
				break;
			case 3:
				ImGui::Text("Chanel : Rougnnes");
				break;
			default:
				break;
			}
			ImGui::Text("Size : %d X %d", ActualMaterial->m_Textures[j]->m_Texture.m_TextureSize.m_Width, ActualMaterial->m_Textures[j]->m_Texture.m_TextureSize.m_Height);
			ImGui::Text("Format : %s", ws2s(ActualMaterial->m_Textures[j]->m_Texture.m_Format).c_str());
			ImGui::Text("File Name : %s", ws2s(ActualMaterial->m_Textures[j]->GetszFileName()).c_str());
		}
		else
		{
			ImGui::Text("No Aviliable Texture");
			ImGui::Text("No Aviliable Texture");
			if (ImGui::Button("Assign Texture"))
			{
				ChangeMaterialTexture(ActualMaterial, j);
			}
			ImGui::Text("No Aviliable Texture");
			ImGui::Text("No Aviliable Texture");
			ImGui::NextColumn();
			ImGui::Text("Size : NotAvilable X NotAvilable");
			ImGui::Text("Format : NotAvilable");
			ImGui::Text("File Name : NotAvilable");
		}
		ImGui::NextColumn();
	}

	ImGui::Columns(1);
}

void Textures(soModel* _Model, std::vector<uint32>& _MaterialIDs)
{
	if (g_SetMaterial)
	{
		_MaterialIDs[g_EditorControl.m_ActualSelectedMesh] = g_SelectedMaterial;
		g_SetMaterial = false;
	}

	if (ImGui::TreeNode(ws2s(soString(TEXT("Material Editor")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
	{
		soString ComboList;
		for (uint32 i = 0; i < _Model->m_MeshList.size(); i++)
		{
			ComboList += _Model->m_MeshList.at(i)->m_MeshName;
			ComboList += TEXT('\0');
		}

		ImGui::Text(" ");
		ImGui::Combo("Mesh List", &g_EditorControl.m_ActualSelectedMesh, ws2s(ComboList).c_str());
		ImGui::Text(" ");

		char MeshName[30] = "";
		InputString(&_Model->m_MeshList.at(g_EditorControl.m_ActualSelectedMesh)->m_MeshName, MeshName);

		ImVec2 Size = ImGui::GetWindowSize();
		Size.x /= 4.0f;
		Size.y = Size.x;

		ImGui::PushID(ws2s(g_InputIDString + uint2ws(g_InputID)).c_str());
		g_InputID++;
		if (ImGui::BeginMenu(ws2s(soString(TEXT("Material Options")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
		{
			if (ImGui::MenuItem(ws2s(soString(TEXT("Assign New Material")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				MaterialData MD;
				MD.Name = soString(TEXT("Default")) + uint2ws(g_InputID);
				g_InputID++;
				_MaterialIDs[g_EditorControl.m_ActualSelectedMesh] = soMaterialManager::Instance().CreateNewMaterial(MD);
			}
			g_InputID++;
			ImGui::SameLine(); ShowHelpMarker("Create new material for this component");

			if (ImGui::MenuItem("Assign Existing Material"))
			{
				g_GUIFlags[GUI_MaterialList] = true;
			}
			ImGui::SameLine(); ShowHelpMarker("Assign an existing material for this component");

			if (ImGui::MenuItem("Glossines / Rougnnes"))
			{
				_Model->m_IsRougthnes--;
				if (_Model->m_IsRougthnes != 0)
				{
					_Model->m_IsRougthnes = (uint32)1;
				}
			}
			ImGui::SameLine(); ShowHelpMarker("Change shader rougness mat for glossines");

			ImGui::EndMenu();
		}
		ImGui::PopID();
		g_InputID++;

		DisplayMaterial(_MaterialIDs[g_EditorControl.m_ActualSelectedMesh]);

		ImGui::TreePop();
	}
	g_InputID++;
}

static soSkeletalModelComponent TempSkeletal;
void AnimationGUI(soSkeletalModelComponent* _Component)
{
	static soActor TempActor;
	static soSceneNode TempNode;

	if (ImGui::TreeNode(ws2s(soString(TEXT("Animation Editor")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
	{
		if (ImGui::Button(ws2s(soString(TEXT("Add Animation")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
		{
			_Component->AddAnimation(LoadFilePath());
			TempSkeletal = *_Component;
		}
		g_InputID++;
		if (!_Component->GetModel()->m_AnimationMap.empty())
		{
			soString ComboList;
			std::map<soString, soAnimation>::iterator it = _Component->GetModel()->m_AnimationMap.begin();

			while (it != _Component->GetModel()->m_AnimationMap.end())
			{
				ComboList += (*it).second.m_Name;
				ComboList += TEXT('\0');
				it++;
			}

			ImGui::Text(" ");
			ImGui::Combo("Animation List", &g_EditorControl.m_ActualAnimation, ws2s(ComboList).c_str());
			ImGui::Text(" ");

			char MeshName[30] = "";
			InputString(&_Component->GetModel()->m_AnimationMap.at(_Component->GetAnimationName(g_EditorControl.m_ActualAnimation)).m_Name, MeshName);

			//Calculamos el tamaño de los nuevos viewports
			ImVec2 ViewPortAnimSize = ImVec2(200, 200);

			soCameraManager::Instance().ChangeTo(4);
			dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera)->m_Active = true;
			soCamera* tempCamera = soCameraManager::Instance().m_ActiveCamera;

			if (ViewPortAnimSize.x != tempCamera->m_CameraViewPort.m_ViewPort.Width || ViewPortAnimSize.y != tempCamera->m_CameraViewPort.m_ViewPort.Height)
			{
				tempCamera->m_CameraViewPort.m_ViewPort.Width = ViewPortAnimSize.x;
				tempCamera->m_CameraViewPort.m_ViewPort.Height = ViewPortAnimSize.y;
				tempCamera->Resize();
			}
			_Component->PlayAnimation(_Component->GetAnimationName(g_EditorControl.m_ActualAnimation));
			TempSkeletal.PlayAnimation(TempSkeletal.GetAnimationName(g_EditorControl.m_ActualAnimation));
			TempSkeletal.Update(g_DeltaTime);

			TempActor.AddNewComponent(&TempSkeletal);
			TempNode.SetActor(&TempActor);
			std::vector<soSceneNode*> NodeList;
			NodeList.push_back(&TempNode);

			soRenderManager::Instance().Render(&NodeList, &(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_CameraRenderTarget, RID_PBR);

			ImGui::Columns(2, "Animation info", true);
			ImGui::ImageButton((ImTextureID*)(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_CameraRenderTarget.m_ptrRenderTargetTexture->m_ptrSRTexture, ViewPortAnimSize);

			//Imputs sobre la ventana
			if (ImGui::IsItemHovered())
			{
				//Actualizacion de movimiento de la camara
				UpdateCameraMovement();
			}

			ImGui::NextColumn();

			ImGui::Text("Duration : %f", TempSkeletal.m_AnimationData.InAnimation._Animation->m_Duration);
			ImGui::Text("Ticks/s : %f", TempSkeletal.m_AnimationData.InAnimation._Animation->m_TicksPerSecond);
			ImGui::ProgressBar(TempSkeletal.m_AnimationData.InAnimation.AnimationTime / TempSkeletal.m_AnimationData.InAnimation._Animation->m_Duration, ImVec2(0.0f, 0.0f));
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::Text("Progress");

			ImGui::Columns(1);
		}
		TempActor.m_Components.clear();
		TempNode.SetActor(NULL);
		ImGui::TreePop();
	}
	g_InputID++;
}

void EditorStaticModel(soStaticModelComponent* _Component)
{
	if (ImGui::Button(ws2s(soString(TEXT("Change Model")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
	{
		g_InputID++;

		_Component->ChangeModel(LoadFilePath());
	}

	char ComponentName[30] = "";
	InputString(&_Component->m_ComponentName, ComponentName);
	Transform(_Component->m_ComponentTransform);

	if (g_EditorControl.m_StaticModel == NULL)
	{
		g_EditorControl.m_StaticModel = _Component;
	}

	g_EditorControl.m_EditMaterial = true;

	Textures(_Component->GetModel(), _Component->m_MaterialID);
}

void EditorSkeletalModel(soSkeletalModelComponent* _Component)
{
	if (ImGui::Button(ws2s(soString(TEXT("Change Model")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
	{
		g_InputID++;

		_Component->ChangeModel(LoadFilePath());

		TempSkeletal = *_Component;
	}

	if (TempSkeletal.GetModel() == NULL)
	{
		TempSkeletal = *_Component;
	}

	char ComponentName[30] = "";
	InputString(&_Component->m_ComponentName, ComponentName);
	Transform(_Component->m_ComponentTransform);

	if (g_EditorControl.m_SkeletalModel == NULL)
	{
		g_EditorControl.m_SkeletalModel = _Component;
	}
	g_EditorControl.m_EditMaterial = true;

	Textures(_Component->GetModel(), _Component->m_MaterialID);
	AnimationGUI(_Component);
}

void EditorPhysic(soPhysicComponent* _Component)
{
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Component Physics");

	ImGui::Combo("Type", &_Component->m_type, "PTF_RIGID_STATIC\0PTF_RIGID_DYNAMIC\0");
	ImGui::Combo("Geometry", &_Component->m_geoType, "PGF_CAPSULE\0PGF_BOX\0");
	SetDragAndInputFloat("Mass", &_Component->m_mass, 0.01f, 0.0f, 1000.0f);
	SetDragAndInputFloat("Scale", &_Component->m_Scale, 0.01f, 1.0f, 1000.0f);
}

void ComponentGUI(soComponent* _Component)
{
	switch (_Component->m_ComponentType)
	{
	case SoulSDK::CT_Camera:
		break;
	case SoulSDK::CT_Light:
		break;
	case SoulSDK::CT_Mesh_Static:
		EditorStaticModel(dynamic_cast<soStaticModelComponent*>(_Component));
		break;
	case SoulSDK::CT_Mesh_Dynamic:
		EditorSkeletalModel(dynamic_cast<soSkeletalModelComponent*>(_Component));
		break;
	case SoulSDK::CT_BillBoard:
		break;
	case SoulSDK::CT_Listener:
		break;
	case SoulSDK::CT_AudioSource:
		break;
	case SoulSDK::CT_Physic:
		EditorPhysic(dynamic_cast<soPhysicComponent*>(_Component));
		break;
	default:
		break;
	}
}

void RenderTargetMenu(std::vector<soRenderTarget*> RTList)
{
	for (uint32 i = 0; i < (uint32)RTList.size(); i++)
	{
		if (ImGui::MenuItem(ws2s(RTList.at(i)->m_Name).c_str()))
		{
			ActiveRenderPass = RID_PBR | RID_MultyPass;
			g_PaintRenderTarget = RTList.at(i);
			return;
		}
	}
}



//Se cambio el active camera por solo 1 instancia
void AxesGUI()
{
	soGraphicDebugManager* ptrGDM = soGraphicDebugManager::InstancePtr();
	soCamera* tempCamera = soCameraManager::Instance().m_ActiveCamera;

	float AspectRatio = (tempCamera->m_CameraViewPort.m_ViewPort.Width / tempCamera->m_CameraViewPort.m_ViewPort.Height);

	if (tempCamera->m_Orthographic)
	{
		float Resize = 40.0f * AspectRatio;
		float Front = 10.0f * AspectRatio;
		float Right = 400.0f * AspectRatio;
		float Up = 200.0f * AspectRatio;
		
		//soGraphicDebugManager::Instance().AddDebugPositionControl(soCameraManager::Instance().m_ActiveCamera->m_Position + soCameraManager::Instance().m_ActiveCamera->m_FrontDir * Front + soCameraManager::Instance().m_ActiveCamera->m_RightDir * -Right + soCameraManager::Instance().m_ActiveCamera->m_CurrentUp * -Up, false, Resize, soColorRGBA(0xFF0000FF), soColorRGBA(0xFF00FF00), soColorRGBA(0xFFFF0000));
		ptrGDM->AddDebugPositionControl(tempCamera->m_Position + tempCamera->m_FrontDir * Front + tempCamera->m_RightDir * -Right + tempCamera->m_CurrentUp * -Up, false, Resize, soColorRGBA(0xFF0000FF), soColorRGBA(0xFF00FF00), soColorRGBA(0xFFFF0000));
		
	}
	else
	{
		float Resize = 1.0f * AspectRatio;
		float Front = 10.0f * AspectRatio;
		float Right = 9.0f * AspectRatio;
		float Up = 5.0f * AspectRatio;
		//soGraphicDebugManager::Instance().AddDebugPositionControl(soCameraManager::Instance().m_ActiveCamera->m_Position + soCameraManager::Instance().m_ActiveCamera->m_FrontDir * Front + soCameraManager::Instance().m_ActiveCamera->m_RightDir * -Right + soCameraManager::Instance().m_ActiveCamera->m_CurrentUp * -Up, false, Resize, soColorRGBA(0xFF0000FF), soColorRGBA(0xFF00FF00), soColorRGBA(0xFFFF0000));
		ptrGDM->AddDebugPositionControl(tempCamera->m_Position + tempCamera->m_FrontDir * Front + tempCamera->m_RightDir * -Right + tempCamera->m_CurrentUp * -Up, false, Resize, soColorRGBA(0xFF0000FF), soColorRGBA(0xFF00FF00), soColorRGBA(0xFFFF0000));
	}
}

void GUIResize() //temporal
{
	//GeneralGUI
	g_GeneralGUISize.x = (float)soDXGIManager::Instance().m_ClientRect.m_Width / 5.0f;
	g_GeneralGUISize.y = (float)soDXGIManager::Instance().m_ClientRect.m_Height - ImGui::GetFontSize();
	g_GeneralGUIPos.x = 0.0f;
	g_GeneralGUIPos.y = ImGui::GetFontSize();
}

void CreateNewActor()
{
	soString ActorName(TEXT("Default"));
	soWorldManager::Instance().CreateNewActor(ActorName);
}

void LoadActorAsset()
{
	soString Path = soString(LoadFilePath());

	if (!Path.empty())
	{
		soWorldManager::Instance().LoadActor(Path);
	}
}

void SelectActorByID(uint32 _ID)
{
	g_EditorControl.SetSelectedNode(soWorldManager::Instance().m_SceneGraph.FindNodeByActorID(_ID));

	if (g_EditorControl.m_SelectedNode)
	{
		g_GUIFlags[GUI_Actor] = true;
		return;
	}
	g_GUIFlags[GUI_Actor] = false;
}

void SelectActorByMesh()
{
	g_EditorControl.SelectNodeModel(g_Ray);

	if (g_EditorControl.m_SelectedNode)
	{
		g_GUIFlags[GUI_Actor] = true;
		return;
	}
	g_GUIFlags[GUI_Actor] = false;
}

void SavePBRData()
{
	soString AssetPath(TEXT("Resources\\Assets\\"));
	soAssetManager::Instance().SaveAssetToFile(AssetPath, &soRenderManager::Instance().m_PBRData, Soul_PBRData, 0);
}

uint32 ItemStatus()
{
	uint32 ItemState = 0;
	if (ImGui::IsItemClicked(0)) ItemState |= IS_ClickedLeft;
	if (ImGui::IsItemClicked(1)) ItemState |= IS_ClickedRight;
	if (ImGui::IsItemActive()) ItemState |= IS_Active;
	if (ImGui::IsItemHovered()) ItemState |= IS_Hovered;

	return ItemState;
}

void SetParent(soSceneNode* GraphNode)
{
	if (g_SetParent)
	{
		NewParent = GraphNode;

		if (NewChild == NewParent)
		{
			NewChild = NewParent = NULL;
		}

		g_SetParent = false;
	}
}

void SetSceneGraphInfo(soSceneNode* GraphNode)
{
	g_ItemState = ItemStatus();

	//Si me hicieron un click
	if (g_ItemState & IS_ClickedLeft)
	{
		SelectActorByID(GraphNode->m_ptrActor->GetActorID());
	}

	//Si hubo drag y sobre quien se hizo drop
	if (g_ItemState & IS_Hovered)
	{
		SetParent(GraphNode);
	}

	//Si me hicieron un drop
	else if (ImGui::IsMouseReleased(0) && NewChild)
	{
		g_SetParent = true;
	}

	//Si me hacen drag
	if (g_ItemState & IS_Active)
	{
		if (ImGui::IsMouseDragging(0))
		{
			if (GraphNode->m_ptrActor->GetActorID() != soWorldManager::Instance().m_SceneGraph.m_ScenneRoot.m_ptrActor->GetActorID())
			{
				ImGui::SetNextWindowPos(ImGui::GetIO().MousePos, ImGuiSetCond_Always);
				ImGui::SetNextWindowSize(ImVec2(ImGui::GetFontSize() * (float)GraphNode->m_ptrActor->m_ActorName.size(), ImGui::GetFontSize()));
				ImGui::Begin("", &g_GUIFlags[GUI_General], ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
				NewChild = GraphNode;
				ImGui::Text(ws2s(NewChild->m_ptrActor->m_ActorName).c_str());
				ImGui::End();
			}
		}
	}
}

void ChangeNodeParent()
{
	if (NewParent && NewChild)
	{
		NewParent->AddChild(NewChild);
		NewChild = NewParent = NULL;
	}
}

void SetChildInfo(soSceneNode* GraphNode)
{
	//Agregamos los nodos hijos a la lista
	for (uint32 i = 0; i < static_cast<uint32>(GraphNode->m_conexions.size()); i++)
	{
		if (ImGui::TreeNode(ws2s(((soSceneNode*)GraphNode->m_conexions.at(i))->m_ptrActor->m_ActorName).c_str()))
		{
			SetSceneGraphInfo(((soSceneNode*)GraphNode->m_conexions.at(i)));

			SetChildInfo(((soSceneNode*)GraphNode->m_conexions.at(i)));

			ImGui::TreePop();
		}
		else
		{
			SetSceneGraphInfo(((soSceneNode*)GraphNode->m_conexions.at(i)));
		}
	}
}

void VisualizeViewPort(soRenderTarget* _RenderTarget, ImVec2 _Size)
{
	//Viewport Camera resize
	soCamera* tempCamera = soCameraManager::Instance().m_ActiveCamera;

	if (_Size.x != tempCamera->m_CameraViewPort.m_ViewPort.Width || _Size.y != tempCamera->m_CameraViewPort.m_ViewPort.Height)
	{
		tempCamera->m_CameraViewPort.m_ViewPort.Width = _Size.x;
		tempCamera->m_CameraViewPort.m_ViewPort.Height = _Size.y;
		tempCamera->Resize();
	}

	ImGui::PushID(ws2s(g_InputIDString + uint2ws(g_InputID)).c_str());
	g_InputID++;
	if (ImGui::BeginMenu(ws2s(soString(TEXT("Viewport Settings")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
	{
		if (ImGui::BeginMenu(ws2s(soString(TEXT("Projection")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
		{
			if (ImGui::MenuItem(ws2s(soString(TEXT("Perspective")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				tempCamera->SetOrthographic(false);
			}

			if (ImGui::BeginMenu(ws2s(soString(TEXT("Ortographic")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				if (ImGui::MenuItem(ws2s(soString(TEXT("Top")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
				{
					(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_ActiveOrthographic = OP_Top;
					tempCamera->SetOrthographic(true);
					tempCamera->IsDirty();
				}
				if (ImGui::MenuItem(ws2s(soString(TEXT("Bottom")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
				{
					(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_ActiveOrthographic = OP_Bottom;
					tempCamera->SetOrthographic(true);
					tempCamera->IsDirty();
				}
				if (ImGui::MenuItem(ws2s(soString(TEXT("Left")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
				{
					(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_ActiveOrthographic = OP_Left;
					tempCamera->SetOrthographic(true);
					tempCamera->IsDirty();
				}
				if (ImGui::MenuItem(ws2s(soString(TEXT("Right")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
				{
					(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_ActiveOrthographic = OP_Right;
					tempCamera->SetOrthographic(true);
					tempCamera->IsDirty();
				}
				if (ImGui::MenuItem(ws2s(soString(TEXT("Front")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
				{
					(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_ActiveOrthographic = OP_Front;
					tempCamera->SetOrthographic(true);
					tempCamera->IsDirty();
				}
				if (ImGui::MenuItem(ws2s(soString(TEXT("Back")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
				{
					(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_ActiveOrthographic = OP_Back;
					tempCamera->SetOrthographic(true);
					tempCamera->IsDirty();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(ws2s(soString(TEXT("Camera FOV")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
		{
			if (ImGui::DragFloat(ws2s(g_InputIDString + uint2ws(g_InputID)).c_str(), &g_FOV, 1.0f, 30.0f, 120.0f))
			{
				tempCamera->m_Fov = g_FOV;
				tempCamera->IsDirty();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	ImGui::PopID();
	g_InputID++;

	if (!_RenderTarget)
	{
		ImGui::Image((ImTextureID*)(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_CameraRenderTarget.m_ptrRenderTargetTexture->m_ptrSRTexture, _Size);
	}
	else
	{
		ImGui::Image((ImTextureID*)_RenderTarget->m_ptrRenderTargetTexture->m_ptrSRTexture, _Size);
	}

	//Imputs sobre la ventana
	if (ImGui::IsItemHovered())
	{
		//Toamos la pocision del raton
		ImVec2 MousePos = ImGui::GetMousePos();

		if (ImGui::IsWindowFocused() && ImGui::IsMouseClicked(0))
		{
			//Actualizamos la camara activa
			g_ActiveCamera = tempCamera->m_ID;

			//Informacion del viewport sobre la ventana
			ImGui::SetWindowFocus("--ViewPort--");
			ImGuiContext* ImGuiCtx = ImGui::GetCurrentContext();
			ImRect ViewportInfo = ImGuiCtx->CurrentWindow->DC.LastItemRect;

			//Posicion dentro de la ventana
			ImVec2 LocalMouse = MousePos;
			LocalMouse.x = LocalMouse.x - (ViewportInfo.Min.x);
			LocalMouse.y = LocalMouse.y - (ViewportInfo.Min.y);

			//Convercion de espacio de ventana a espacio de pantalla
			LocalMouse.x = (LocalMouse.x / (ViewportInfo.GetSize().x)) * tempCamera->m_CameraViewPort.m_ViewPort.Width;
			LocalMouse.y = (LocalMouse.y / (ViewportInfo.GetSize().y)) * tempCamera->m_CameraViewPort.m_ViewPort.Height;

			g_Ray = tempCamera->CreateRay(soPoint((int32)LocalMouse.x, (int32)LocalMouse.y));

			//Seleccion de modelos por click en interfaz
			SelectActorByMesh();
		}

		//Actualizacion de movimiento de la camara
		UpdateCameraMovement();
	}
}
#pragma endregion

#pragma region GUI
void ApiMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("ViewPort")) g_GUIFlags[GUI_ViewPort] = true;	
#ifdef VR_EDITOR
			if (ImGui::MenuItem("Viewport VR")) g_GUIFlags[GUI_ViewportVR] = true;
#endif
			if (ImGui::MenuItem("Camera Settings")) g_GUIFlags[GUI_EditorCamera] = true;
			if (ImGui::MenuItem("PBR Settings")) g_GUIFlags[GUI_PBR] = true;
			if (ImGui::MenuItem("Light Settings")) g_GUIFlags[GUI_DirectionaLight] = true;
			if (ImGui::MenuItem("Material Settings")) g_GUIFlags[GUI_MaterialList] = true;
#ifdef RUBENPHYS
			if (ImGui::MenuItem("Physics")) g_GUIFlags[GUI_Physics] = true;
#endif
#if defined(PROFILE)
			if(ImGui::MenuItem("Profile")) g_GUIFlags[GUI_Profile] = true;
#endif
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			ImGui::Text("Desarrollado por :");
			ImGui::Text("Aldo Lehi Munoz Roquet");
			ImGui::Text("Ruben Rodriguez Serrano");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void GeneralGUI()
{
	ImGui::SetNextWindowSize(g_GeneralGUISize, ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(g_GeneralGUIPos);
	ImGui::Begin("--General GUI--", &g_GUIFlags[GUI_General], ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
	{
		//Perfomance		
		ImGui::Text("Application average  %.3f ms/frame (%.1f FPS)", 1000.0f / g_FPS, g_FPS);
		ImGui::Text("Actors in Scenne     %d ", soWorldManager::Instance().m_ActorList.size());
		ImGui::Text("Models in Scenne     %d ", soWorldManager::Instance().m_SceneGraph.m_RenderedModelNum);
		ImGui::Text("Mesh in Scenne       %d ", soWorldManager::Instance().m_SceneGraph.m_RenderedMeshNum);
	}
	ImGui::End();
}


void VRGUI()
{
	ImGui::SetNextWindowSize(ImVec2(100,50), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("--ScenneGraph GUI--", &g_GUIFlags[GUI_ScenneGraph], ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
	{
		//Perfomance		
		ImGui::Text("Actors in Scenne     %d ", soWorldManager::Instance().m_ActorList.size());
		if (ImGui::Button("Create Empty Actor")) CreateNewActor();
		if (ImGui::Button("Load Actor from Asset")) LoadActorAsset();

		//Desplegamos la info del ScenneGraph
		ImGui::Text("--SceneGraph--");
		if (ImGui::TreeNode(ws2s(soWorldManager::Instance().m_SceneGraph.m_ScenneRoot.m_ptrActor->m_ActorName).c_str()))
		{
			SetSceneGraphInfo(&soWorldManager::Instance().m_SceneGraph.m_ScenneRoot);
			SetChildInfo(&soWorldManager::Instance().m_SceneGraph.m_ScenneRoot);

			ImGui::TreePop();
		}
		else
		{
			SetSceneGraphInfo(&soWorldManager::Instance().m_SceneGraph.m_ScenneRoot);
		}
		ChangeNodeParent();
	}
	ImGui::End();
}


#ifdef VR_EDITOR
void VRViewPort()
{
	if (!g_GUIFlags[GUI_ViewportVR])
	{
		return;
	}

	static bool no_border = false;
	static bool no_resize = false;
	static bool no_move = false;
	uint32 window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("VR Viewport", &g_GUIFlags[GUI_ViewportVR], window_flags);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Window Settings"))
			{
				ImGui::Checkbox("Show border", &no_border);
				ImGui::Checkbox("Lock resize", &no_resize);
				ImGui::Checkbox("Lock move", &no_move);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImVec2 ViewPortGUISize = ImGui::GetWindowSize();

		ImGui::Image((ImTextureID*)soVRManager::Instance().m_RightEyeRT.m_ptrRenderTargetTexture->m_ptrSRTexture, ViewPortGUISize);
	}
	ImGui::End();
}
#endif
void ViewPortGUI()
{
	static bool MultipleViewports = false;
	static ImVec2 VPScale[]{ ImVec2(0,0), ImVec2(1,0), ImVec2(0,1), ImVec2(1,1) };

	//Verificacion de ventana activa
	if (!g_GUIFlags[GUI_ViewPort])
	{
		return;
	}

	static bool no_border = false;
	static bool no_resize = false;
	static bool no_move = false;
	uint32 window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;

	

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("--ViewPort--", &g_GUIFlags[GUI_ViewPort], window_flags);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Window Settings"))
			{
				ImGui::Checkbox("Show border", &no_border);
				ImGui::Checkbox("Lock resize", &no_resize);
				ImGui::Checkbox("Lock move", &no_move);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginMenu(ws2s(soString(TEXT("Active Pass")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
		{
			if (ImGui::BeginMenu(ws2s(soString(TEXT("G-Buffer")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				RenderTargetMenu(soRenderManager::Instance().GetPassRenderTargets(ARE_PBR, RP_GBuffer));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ws2s(soString(TEXT("SSAO")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				RenderTargetMenu(soRenderManager::Instance().GetPassRenderTargets(ARE_PBR, RP_SSAO));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ws2s(soString(TEXT("Luminance")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				RenderTargetMenu(soRenderManager::Instance().GetPassRenderTargets(ARE_PBR, RP_Luminance));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ws2s(soString(TEXT("Bright")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				RenderTargetMenu(soRenderManager::Instance().GetPassRenderTargets(ARE_PBR, RP_Bright));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ws2s(soString(TEXT("BlurH")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				RenderTargetMenu(soRenderManager::Instance().GetPassRenderTargets(ARE_PBR, RP_BlurH));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ws2s(soString(TEXT("BlurV")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				RenderTargetMenu(soRenderManager::Instance().GetPassRenderTargets(ARE_PBR, RP_BlurV));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ws2s(soString(TEXT("AddBright")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				RenderTargetMenu(soRenderManager::Instance().GetPassRenderTargets(ARE_PBR, RP_AddBright));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ws2s(soString(TEXT("MultyPass")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				RenderTargetMenu(soRenderManager::Instance().GetPassRenderTargets(ARE_MultyPassViewer, RenderPass(0)));
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem(ws2s(soString(TEXT("Final Pass")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
			{
				ActiveRenderPass = 0;
				g_PaintRenderTarget = NULL;
			}
			ImGui::EndMenu();
		}

		//Actualizacion de la posicion y el tamaño del viewport
		ImVec2 ViewPortGUISize = ImGui::GetWindowSize();

		//Si preionan espacio, Verifico el cambio de visualizacion para el siguiente frame
		if (ImGui::IsWindowFocused() && ImGui::IsKeyReleased(VK_SPACE))
		{
			MultipleViewports = !MultipleViewports;

			g_PaintRenderTarget = NULL;
		}

		//Estilos para el render del viewport
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));

		//Multiple ViewPort
		if (MultipleViewports)
		{
			//Calculamos el tamaño de los nuevos viewports
			ImVec2 MultViewPortGUISize = ImVec2(ViewPortGUISize.x / 2.0f, ViewPortGUISize.y / 2.0f);

			//Inicializo la configuracion para la visualizacion
			ImGui::Columns(2, "ViewPort Colums", false);

			//Visualizacion de cada viewport
			for (uint32 i = 0; i < g_TotalActiveCameras; i++)
			{
				//Actualizamos la situacion de cada camara
				soCameraManager::Instance().ChangeTo(i);
				dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera)->m_Active = true;

				VisualizeViewPort(&(dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera))->m_CameraRenderTarget, MultViewPortGUISize);

				ImGui::NextColumn();
			}

			ImGui::Columns(1);
		}
		else
		{
			//Actualizamos la situacion de las camaras 
			for (uint32 i = 0; i < g_TotalActiveCameras; i++)
			{
				soCameraManager::Instance().ChangeTo(i);
				dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera)->m_Active = false;
			}

			//Camara activa
			soCameraManager::Instance().ChangeTo(g_ActiveCamera);
			dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera)->m_Active = true;

			VisualizeViewPort(g_PaintRenderTarget, ViewPortGUISize);
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
	ImGui::End();
	
}

void PBRGUI()
{
	if (!g_GUIFlags[GUI_PBR])
	{
		return;
	}

	static bool no_border = true;
	static bool no_resize = true;
	static bool no_move = true;
	static bool Autorezise = false;
	uint32 window_flags = ImGuiWindowFlags_MenuBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (Autorezise)   window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("--PBR Menu--", &g_GUIFlags[GUI_PBR], window_flags);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Window Settings"))
			{
				ImGui::Checkbox("Show border", &no_border);
				ImGui::Checkbox("Lock resize", &no_resize);
				ImGui::Checkbox("Lock move", &no_move);
				ImGui::Checkbox("AutoResize", &Autorezise);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::Button("Save Settings")) SavePBRData();

		if (ImGui::CollapsingHeader("General"))
		{
			ImGui::SliderFloat("Final Exposition", &soRenderManager::Instance().m_PBRData.FinalExposition.X, 0.0f, 100.0f);
		}
		if (ImGui::CollapsingHeader("SSAO"))
		{
			ImGui::SliderFloat("Intensity", &soRenderManager::Instance().m_PBRData.Intensity_Scale_Bias_SampleRadius.X, 0.0f, 2.0f);
			ImGui::SliderFloat("Scale", &soRenderManager::Instance().m_PBRData.Intensity_Scale_Bias_SampleRadius.Y, 0.0f, 2.0f);
			ImGui::SliderFloat("Bias", &soRenderManager::Instance().m_PBRData.Intensity_Scale_Bias_SampleRadius.Z, 0.0f, 1.0f);
			ImGui::SliderFloat("Sample Radius", &soRenderManager::Instance().m_PBRData.Intensity_Scale_Bias_SampleRadius.W, 0.0f, 0.25f);
		}
		if (ImGui::CollapsingHeader("Bright"))
		{
			ImGui::SliderFloat("BloomThreshold", &soRenderManager::Instance().m_PBRData.BloomThreshold.X, 0.0f, 1.0f);
		}
		if (ImGui::CollapsingHeader("Tone Maping"))
		{
			ImGui::SliderFloat("BloomMuiltiplier", &soRenderManager::Instance().m_PBRData.BloomMuiltiplier_Exposure.X, 0.0f, 10.0f);
			ImGui::SliderFloat("Exposure", &soRenderManager::Instance().m_PBRData.BloomMuiltiplier_Exposure.Y, 0.0f, 10.0f);
		}
	}
	ImGui::End();
}

void DirectionalLight()
{
	if (!g_GUIFlags[GUI_DirectionaLight])
	{
		return;
	}

	static bool no_border = true;
	static bool no_resize = true;
	static bool no_move = true;
	static bool Autorezise = false;
	uint32 window_flags = ImGuiWindowFlags_MenuBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (Autorezise)   window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("--Directional Light--", &g_GUIFlags[GUI_DirectionaLight], window_flags);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Window Settings"))
			{
				ImGui::Checkbox("Show border", &no_border);
				ImGui::Checkbox("Lock resize", &no_resize);
				ImGui::Checkbox("Lock move", &no_move);
				ImGui::Checkbox("AutoResize", &Autorezise);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::SliderFloat3("Light Position", &soRenderManager::Instance().m_LightCB.LigthPosition.X, -5000.0f, 5000.0f);
		ImGui::SliderFloat3("Light Color", &soRenderManager::Instance().m_LightCB.LigthColor.X, 0.0f, 1.0f);
	}
	ImGui::End();
}

void ActorGUI()
{
	if (!g_GUIFlags[GUI_Actor])
	{
		return;
	}
	else
	{
		if (!g_EditorControl.m_SelectedNode) // checar por que es null
		{
			g_GUIFlags[GUI_Actor] = false;
			return;
		}
	}
	
	static bool no_border = true;
	static bool no_resize = true;
	static bool no_move = true;
	static bool Autorezise = false;
	uint32 window_flags = ImGuiWindowFlags_MenuBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (Autorezise)   window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("--Actor Editor--", &g_GUIFlags[GUI_Actor], window_flags);
	{
		soCamera* tempCamera = soCameraManager::Instance().m_ActiveCamera;
		float Scale = (tempCamera->m_CameraViewPort.m_ViewPort.Width / tempCamera->m_CameraViewPort.m_ViewPort.Height) * 6.0f;
		soGraphicDebugManager::Instance().AddDebugPoint(g_EditorControl.m_SelectedNode->m_ptrActor->m_ActorTransform.m_Position, false, Scale, soColorRGBA(255, 0, 0, 255));

		//Info General
		ImGui::Separator();
		char ActorName[30] = "";
		InputString(&g_EditorControl.m_SelectedNode->m_ptrActor->m_ActorName, ActorName);

		ImGui::Separator();
		//Transform		
		Transform(g_EditorControl.m_SelectedNode->m_ptrActor->m_ActorTransform);
		ImGui::Separator();

		//Lista de componentes
		if (!g_EditorControl.m_SelectedNode->m_ptrActor->m_Components.empty())
		{
			if (ImGui::CollapsingHeader("Component List"))
			{
				g_EditorControl.m_StaticModel = NULL;
				g_EditorControl.m_SkeletalModel = NULL;
				for (uint32 n = 0; n < static_cast<uint32>(g_EditorControl.m_SelectedNode->m_ptrActor->m_Components.size()); n++)
				{
					if (ImGui::TreeNode(ws2s(g_EditorControl.m_SelectedNode->m_ptrActor->m_Components.at(n)->m_ComponentName + g_InputIDString + uint2ws(g_InputID)).c_str()))
					{
						ImGui::Separator();
						ComponentGUI(g_EditorControl.m_SelectedNode->m_ptrActor->m_Components.at(n));
						ImGui::Separator();
						ImGui::TreePop();
					}
					g_InputID++;
				}
			}
			else
			{
				g_SetMaterial = false;
				g_EditorControl.m_EditMaterial = false;
			}
		}

		//Window Menu
		ComponentData CD;
		bool AddNewComponent = false;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::BeginMenu("Add New Component"))
				{
					if (ImGui::MenuItem("Static Mesh"))
					{
						AddNewComponent = true;
						CD._ComponentType |= CT_Mesh_Static;
						CD.ResourcePath = LoadFilePath();
					}
					if (ImGui::MenuItem("Skeletal Mesh"))
					{
						AddNewComponent = true;
						CD._ComponentType |= CT_Mesh_Dynamic;
						CD.ResourcePath = LoadFilePath();
					}
					if (ImGui::MenuItem("Physic"))
					{
						AddNewComponent = true;
						CD._ComponentType |= CT_Physic;
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Load Component")) {}
				if (ImGui::MenuItem("Save As Asset"))
				{
					soAssetManager::Instance().SaveAssetToFile(SaveFilePath(), g_EditorControl.m_SelectedNode->m_ptrActor, Soul_Actor);
				}
				if (ImGui::MenuItem("Erase Actor"))
				{
					soWorldManager::Instance().DeleteActor(g_EditorControl.m_SelectedNode->m_ptrActor->GetActorID());
					g_EditorControl.SetSelectedNode(NULL);
					g_GUIFlags[GUI_Actor] = false;
				}
				if (ImGui::MenuItem("Exit"))g_GUIFlags[GUI_Actor] = false;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window Settings"))
			{
				ImGui::Checkbox("Show border", &no_border);
				ImGui::Checkbox("Lock resize", &no_resize);
				ImGui::Checkbox("Lock move", &no_move);
				ImGui::Checkbox("AutoResize", &Autorezise);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		if (AddNewComponent)
		{
			g_EditorControl.AddComponentToSelectedActor(CD);
		}
	}
	ImGui::End();
}

void EditorCameraSettingsGUI()
{
	if (!g_GUIFlags[GUI_EditorCamera])
	{
		return;
	}

	static bool no_border = true;
	static bool no_resize = true;
	static bool no_move = true;
	static bool Autorezise = false;
	uint32 window_flags = ImGuiWindowFlags_MenuBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (Autorezise)   window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("--Camera Settings--", &g_GUIFlags[GUI_EditorCamera], window_flags);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Window Settings"))
			{
				ImGui::Checkbox("Show border", &no_border);
				ImGui::Checkbox("Lock resize", &no_resize);
				ImGui::Checkbox("Lock move", &no_move);
				ImGui::Checkbox("AutoResize", &Autorezise);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		//Movimiento
		ImGui::DragFloat3("Radians Rotation Speed", &g_RotationSpeed.X, 0.1f, 0.0f, 10.0f);
		ImGui::DragFloat3("Unit Translation Speed", &g_TranslationSpeed.X, 1.0f, 0.0f, 800.0f);
		ImGui::DragFloat3("Unit Pan Speed", &g_PanSpeed.X, 1.0f, 0.0f, 800.0f);
	}
	ImGui::End();
}

void MaterialListGUI()
{
	if (!g_GUIFlags[GUI_MaterialList])
	{
		return;
	}

	static bool no_border = true;
	static bool no_resize = true;
	static bool no_move = true;
	static bool Autorezise = false;
	uint32 window_flags = ImGuiWindowFlags_MenuBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (Autorezise)   window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin(ws2s(soString(TEXT("Existing Materials")) + g_InputIDString + uint2ws(g_InputID)).c_str(), &g_GUIFlags[GUI_MaterialList], window_flags))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Window Settings"))
			{
				ImGui::Checkbox("Show border", &no_border);
				ImGui::Checkbox("Lock resize", &no_resize);
				ImGui::Checkbox("Lock move", &no_move);
				ImGui::Checkbox("AutoResize", &Autorezise);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		g_InputID++;

		if (ImGui::Button(ws2s(soString(TEXT("Set material to selected Mesh")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
		{
			if (g_EditorControl.m_SelectedNode)
			{
				g_SetMaterial = true;
			}
		}
		ImGui::SameLine(); ShowHelpMarker("Asigna el material actual al mesh seleccionado en caso de existir");
		if (ImGui::Button(ws2s(soString(TEXT("Create new material")) + g_InputIDString + uint2ws(g_InputID)).c_str()))
		{
			MaterialData MD;
			MD.Name = soString(TEXT("Default")) + uint2ws(g_InputID);
			g_InputID++;
			soMaterialManager::Instance().CreateNewMaterial(MD);
		}
		ImGui::SameLine(); ShowHelpMarker("Crea un nuevo material");

		soString ComboList;
		std::map<uint32, soMaterial*>::iterator it = soMaterialManager::Instance().m_MaterialMap.begin();

		while (it != soMaterialManager::Instance().m_MaterialMap.end())
		{
			ComboList += (*it).second->m_MaterialName;
			ComboList += TEXT('\0');
			it++;
		}

		ImGui::Text(" ");
		ImGui::Combo(ws2s(soString(TEXT("Material List")) + g_InputIDString + uint2ws(g_InputID)).c_str(), &g_SelectedMaterial, ws2s(ComboList).c_str());
		ImGui::Text(" ");

		DisplayMaterial(g_SelectedMaterial);
	}
	ImGui::End();
}

#ifdef RUBENPHYS
void PhysicsGUI()
{
	if (!g_GUIFlags[GUI_Physics])
	{
		return;
	}

	static bool no_border = true;
	static bool no_resize = true;
	static bool no_move = false;
	static bool Autorezise = false;
	uint32 window_flags = ImGuiWindowFlags_MenuBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (Autorezise)   window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("--Physics Controller--", &g_GUIFlags[GUI_Actor], window_flags);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Window Settings"))
			{
				ImGui::Checkbox("Show border", &no_border);
				ImGui::Checkbox("Lock resize", &no_resize);
				ImGui::Checkbox("Lock move", &no_move);
				ImGui::Checkbox("AutoResize", &Autorezise);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//TO-DO meter un text box de gravedad y un boton de apply

		if (ImGui::Button("Play"))
		{
			soPhysicsManager::Instance().Play();
		}
	}
	ImGui::End();
}
#endif // RUBENPHYS

#if defined(PROFILE)
void ProfileGUI()
{
	if (!g_GUIFlags[GUI_Profile])
	{
		return;
	}

	

	static bool no_border = true;
	static bool no_resize = true;
	static bool no_move = false;
	static bool Autorezise = false;
	uint32 window_flags = ImGuiWindowFlags_MenuBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (Autorezise)   window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("--Profile Info--", &g_GUIFlags[GUI_Actor], window_flags);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Window Settings"))
			{
				ImGui::Checkbox("Show border", &no_border);
				ImGui::Checkbox("Lock resize", &no_resize);
				ImGui::Checkbox("Lock move", &no_move);
				ImGui::Checkbox("AutoResize", &Autorezise);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//Obtenemos los tamaños de la ventana
		ImVec2 ProfileGUISize = ImGui::GetWindowSize();

		//Actualizamos el tamaño de la screen size para fonts
		//soFont::Instance().SetScreenSize(ProfileGUISize.x, ProfileGUISize.y);

		soRenderManager::Instance().Render(NULL, NULL, RID_FontSystem);
		soRenderTarget* renderTarget = soRenderManager::Instance().GetPassRenderTargets(ARE_FontSystem, RenderPass(0))[0];

		ImGui::Image((ImTextureID*)renderTarget->m_ptrRenderTargetTexture->m_ptrSRTexture, ProfileGUISize);
	}
	ImGui::End();
}
#endif

#pragma endregion

void RenderEditorGUI()
{
	//Reset del identificator de inputs
	g_InputID = 0;

	GUIResize();
	ViewPortGUI();
	ApiMenu();
	GeneralGUI();
	PBRGUI();
	ActorGUI();
	EditorCameraSettingsGUI();
	DirectionalLight();
	MaterialListGUI();
#ifdef VR_EDITOR
	VRViewPort();
#endif // VR_EDITOR
#ifdef RUBENPHYS
	PhysicsGUI();
#endif // RUBENPHYS
#if defined(PROFILE)
	ProfileGUI();
#endif
	VRGUI(); //No es de Realidad Virtual es para agregar un nuevo actor (Nombre confuso)
}

//
//if (ImGui::CollapsingHeader("Light"))
//{
//	ImGui::SliderFloat("Light Pos X", &soRenderManager::Instance().m_LightCB.LigthPosition.X, -300.0f, 300.0f);
//	ImGui::SliderFloat("Light Pos Y", &soRenderManager::Instance().m_LightCB.LigthPosition.Y, -300.0f, 300.0f);
//	ImGui::SliderFloat("Light Pos Z", &soRenderManager::Instance().m_LightCB.LigthPosition.Z, -300.0f, 300.0f);
//
//	ImGui::SliderFloat("Light Color R", &soRenderManager::Instance().m_LightCB.LigthColor.X, 0.0f, 1.0f);
//	ImGui::SliderFloat("Light Color G", &soRenderManager::Instance().m_LightCB.LigthColor.Y, 0.0f, 1.0f);
//	ImGui::SliderFloat("Light Colot B", &soRenderManager::Instance().m_LightCB.LigthColor.Z, 0.0f, 1.0f);
//
//	ImGui::SliderFloat("Light Final Exposition", &soRenderManager::Instance().m_PBRData.FinalExposition.X, 0.0f, 10.0f);
//}


//
//void TW_CALL LoadMetallicFromFile(void*)
//{
//	soString Path = LoadFilePath();
//
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!Path.empty())
//	{
//		//Creamos las texturas como recursos para el modelo 
//		ResourceParameters RP;
//		RP.FilePath = Path;
//		RP.ResourceType = RT_Texture;
//
//		soTexture* NewTexture = static_cast<soTexture*>(soResourceManager::Instance().Load(RP));
//		if (NewTexture != NULL && g_EditorControl.m_SelectedNode != NULL)
//		{
//			g_EditorControl.m_SelectedMeshList.at(g_EditorControl.m_ActualSelectedMesh)->m_Material.AssingTexture(NewTexture, Texture_Slot_1);
//		}
//	}
//}
//
//void TW_CALL LoadNormalFromFile(void*)
//{
//	soString Path = LoadFilePath();
//
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!Path.empty())
//	{
//		//Creamos las texturas como recursos para el modelo 
//		ResourceParameters RP;
//		RP.FilePath = Path;
//		RP.ResourceType = RT_Texture;
//
//		soTexture* NewTexture = static_cast<soTexture*>(soResourceManager::Instance().Load(RP));
//		if (NewTexture != NULL && g_EditorControl.m_SelectedNode != NULL)
//		{
//			g_EditorControl.m_SelectedMeshList.at(g_EditorControl.m_ActualSelectedMesh)->m_Material.AssingTexture(NewTexture, Texture_Slot_2);
//		}
//	}
//}
//
//void TW_CALL LoadRoughnessFromFile(void*)
//{
//	soString Path = LoadFilePath();
//
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!Path.empty())
//	{
//		//Creamos las texturas como recursos para el modelo 
//		ResourceParameters RP;
//		RP.FilePath = Path;
//		RP.ResourceType = RT_Texture;
//
//		soTexture* NewTexture = static_cast<soTexture*>(soResourceManager::Instance().Load(RP));
//		if (NewTexture != NULL && g_EditorControl.m_SelectedNode != NULL)
//		{
//			g_EditorControl.m_SelectedMeshList.at(g_EditorControl.m_ActualSelectedMesh)->m_Material.AssingTexture(NewTexture, Texture_Slot_3);
//		}
//	}
//}
//
//void TW_CALL LoadEnviromentFromFile(void*)
//{
//	soString Path = LoadFilePath();
//
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!Path.empty())
//	{
//		//Creamos las texturas como recursos para el modelo 
//		ResourceParameters RP;
//		RP.FilePath = Path;
//		RP.ResourceType = RT_Texture;
//
//		soTexture* NewTexture = static_cast<soTexture*>(soResourceManager::Instance().Load(RP));
//		if (NewTexture != NULL && g_EditorControl.m_SelectedNode != NULL)
//		{
//			g_EditorControl.m_SelectedMeshList.at(g_EditorControl.m_ActualSelectedMesh)->m_Material.AssingTexture(NewTexture, Texture_Slot_4);
//		}
//	}
//}
//
//void TW_CALL LoadIrradianceFromFile(void*)
//{
//	soString Path = LoadFilePath();
//
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!Path.empty())
//	{
//		//Creamos las texturas como recursos para el modelo 
//		ResourceParameters RP;
//		RP.FilePath = Path;
//		RP.ResourceType = RT_Texture;
//
//		soTexture* NewTexture = static_cast<soTexture*>(soResourceManager::Instance().Load(RP));
//		if (NewTexture != NULL && g_EditorControl.m_SelectedNode != NULL)
//		{
//			g_EditorControl.m_SelectedMeshList.at(g_EditorControl.m_ActualSelectedMesh)->m_Material.AssingTexture(NewTexture, Texture_Slot_5);
//		}
//	}
//}
//
//void TW_CALL LoadSkeletalModelAnimation(void*)
//{
//	soString Path = LoadFilePath();
//
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!Path.empty())
//	{
//		if (g_EditorControl.m_SkeletalModel)
//		{
//			soMessageSystem::Instance().SendEntityMessage(SOE_NEW soEntityMessage(NULL, g_EditorControl.m_SkeletalModel, MT_ADD_ANIMATION, SOE_NEW soString(Path)));
//		}
//	}
//}

//void TW_CALL SaveActorAsset(void*)
//{
//	soString Path = SaveFilePath();
//
//	if (!Path.empty())
//	{		
//		if (g_EditorControl.m_SelectedNode != NULL)
//		{
//			uint64 Type = Soul_Actor;
//			soAssetManager::Instance().SaveAssetToFile(Path, g_EditorControl.m_SelectedNode->m_ptrActor, Type);
//		}
//	}	
//}

//
//void ModelMaterialGUI()
//{
//	/* Generacion de interfaz                                               */
//	TwBar *MaterialProp = TwNewBar(TEXT("MaterialProp"));
//	
//	soString Text(TEXT(" Group='File' label='Selected Mesh' min = 0 max = "));
//	Text += int2ws((uint32)g_EditorControl.m_MeshNumber);
//	Text += soString(TEXT(" step = 1"));
//
//	TwAddVarRW(MaterialProp, TEXT("Roughness/Glossiness"), TW_TYPE_UINT32, &*g_EditorControl.m_Roughness, TEXT(" Group='Material Type' label='Roughness/Glossiness' "));
//	TwAddVarRW(MaterialProp, TEXT("SelectedMesh"), TW_TYPE_UINT32, &g_EditorControl.m_ActualSelectedMesh, Text.c_str());
//	TwAddButton(MaterialProp, TEXT("LoadBaseColor"), LoadBaseColorFromFile, NULL, TEXT(" Group='File' label='Load BaseColor From File' "));
//	TwAddButton(MaterialProp, TEXT("LoadMetallic"), LoadMetallicFromFile, NULL, TEXT(" Group='File' label='Load Metallic From File' "));
//	TwAddButton(MaterialProp, TEXT("LoadNormal"), LoadNormalFromFile, NULL, TEXT(" Group='File' label='Load Normal From File' "));
//	TwAddButton(MaterialProp, TEXT("LoadRoughness"), LoadRoughnessFromFile, NULL, TEXT(" Group='File' label='Load Roughness From File' "));
//	TwAddButton(MaterialProp, TEXT("LoadIrradiance"), LoadIrradianceFromFile, NULL, TEXT(" Group='File' label='Load Irradiance From File' "));
//	TwAddButton(MaterialProp, TEXT("LoadEnvironment"), LoadEnviromentFromFile, NULL, TEXT(" Group='File' label='Load Enviroment From File' "));
//
//	TwDefine(TEXT(" MaterialProp color='200 50 0' alpha=128 "));
//	TwDefine(TEXT(" MaterialProp position='0 500' "));
//	TwDefine(TEXT(" MaterialProp size='330 220' "));
//	TwDefine(TEXT(" MaterialProp valueswidth=90 "));
//	TwDefine(TEXT(" MaterialProp refresh=0.1 "));
//}
//
//void TW_CALL CopyStdStringToClient(soString & destinationClientString, const soString& sourceLibraryString)
//{
//	// Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
//	destinationClientString = sourceLibraryString;
//}
//
////soString s3 = TEXT("a STL string");
////
////void TW_CALL SetMyStdStringCB(const void *value, void *clientData)
////{
////	// Set: copy the value of s3 from AntTweakBar
////	const soString *srcPtr = static_cast<const soString*>(value);
////	s3 = *srcPtr;
////}
////void TW_CALL GetMyStdStringCB(void *value, void * /*clientData*/)
////{
////	// Get: copy the value of s3 to AntTweakBar
////	soString* destPtr = static_cast<soString*>(value);
////	TwCopyStdStringToLibrary(*destPtr, s3);
////}
//
//void TransformGUI()
//{
//	/* Generacion de interfaz                                               */
//	TwBar *Model_UI = TwNewBar(TEXT("Actor_GUI"));
//	TwType twComponents = TwDefineEnum(TEXT("Components"), g_twComponents, 3);
//	//TwAddVarCB(Model_UI, TEXT("s3"), TW_TYPE_STDSTRING, SetMyStdStringCB, GetMyStdStringCB, NULL, TEXT(" Group='Actor_Info' label='Actor_Name' "));
//
//	TwAddVarRO(Model_UI, TEXT("Name"), TW_TYPE_STDSTRING, &g_EditorControl.m_SelectedNode->m_ptrActor->m_Name, TEXT(" Group='Actor_Info' label='Name' "));
//	TwAddVarRO(Model_UI, TEXT("ID"), TW_TYPE_UINT32, &g_EditorControl.m_SelectedNode->m_ptrActor->GetID(), TEXT(" Group='Actor_Info' label='ID' "));
//	TwAddVarRW(Model_UI, TEXT("Select Component"), twComponents, &g_eComponents, TEXT(" Group='Add Component' label='Select Component' "));
//	TwAddButton(Model_UI, TEXT("AddComponent"), AddComponent, NULL, TEXT(" Group='Add Component' label='AddComponent' "));
//	TwAddButton(Model_UI, TEXT("DeleteActor"), DeleteSelected, NULL, TEXT(" Group='Actor_Info' label='DeleteActor' "));
//
//	TwAddVarRW(Model_UI, TEXT("TranslationX"), TW_TYPE_FLOAT, &g_EditorControl.m_NodePosition.X, TEXT(" Group='Transform' label='TranslationX' "));
//	TwAddVarRW(Model_UI, TEXT("TranslationY"), TW_TYPE_FLOAT, &g_EditorControl.m_NodePosition.Y, TEXT(" Group='Transform' label='TranslationY' "));
//	TwAddVarRW(Model_UI, TEXT("TranslationZ"), TW_TYPE_FLOAT, &g_EditorControl.m_NodePosition.Z, TEXT(" Group='Transform' label='TranslationZ' "));
//	TwAddVarRW(Model_UI, TEXT("ScalationX"), TW_TYPE_FLOAT, &g_EditorControl.m_NodeScale.X, TEXT(" Group='Transform' label='ScalationX' "));
//	TwAddVarRW(Model_UI, TEXT("ScalationY"), TW_TYPE_FLOAT, &g_EditorControl.m_NodeScale.Y, TEXT(" Group='Transform' label='ScalationY' "));
//	TwAddVarRW(Model_UI, TEXT("ScalationZ"), TW_TYPE_FLOAT, &g_EditorControl.m_NodeScale.Z, TEXT(" Group='Transform' label='ScalationZ' "));
//	TwAddVarRW(Model_UI, TEXT("Rotation"), TW_TYPE_QUAT4F, &g_EditorControl.m_NodeRotation, TEXT(" Group='Transform' label='Rotation' "));
//
//	TwAddVarRW(Model_UI, TEXT("Enable"), TW_TYPE_BOOLCPP, &g_EditorControl.m_NodeEnable, TEXT(" Group='Actor_Info' label='Enable' "));
//	TwAddVarRW(Model_UI, TEXT("Visible"), TW_TYPE_BOOLCPP, &g_EditorControl.m_NodeVisible, TEXT(" Group='Actor_Info' label='Visible' "));
//
//	if (g_EditorControl.m_StaticModel)
//	{
//		TwAddVarRO(Model_UI, TEXT("Model"), TW_TYPE_STDSTRING, &g_EditorControl.m_StaticModel->GetModel()->GetszFileName(), TEXT(" Group='StaticModel' label='Model' "));
//		TwAddButton(Model_UI, TEXT("ChangeModel"), ChangeStaticModel, NULL, TEXT(" Group='StaticModel' label='ChangeModel' "));
//		TwAddButton(Model_UI, TEXT("EditMaterial"), EditMaterial, NULL, TEXT(" Group='StaticModel' label='EditMaterial' "));
//	}
//
//	if (g_EditorControl.m_SkeletalModel)
//	{
//		TwAddVarRO(Model_UI, TEXT("Model"), TW_TYPE_STDSTRING, &g_EditorControl.m_SkeletalModel->GetModel()->GetszFileName(), TEXT(" Group='SkeletalModel' label='Model' "));
//		TwAddButton(Model_UI, TEXT("ChangeModel"), ChangeSkeletalModel, NULL, TEXT(" Group='SkeletalModel' label='ChangeModel' "));
//		TwAddButton(Model_UI, TEXT("EditMaterial"), EditMaterial, NULL, TEXT(" Group='SkeletalModel' label='EditMaterial' "));
//
//		soString Text(TEXT(" Group='Animation' label='Play Animation' min = 0 max = "));
//		Text += int2ws((uint32)g_EditorControl.m_SkeletalModel->GetModel()->m_AnimationMap.size());
//		Text += soString(TEXT(" step = 1"));
//
//		TwAddButton(Model_UI, TEXT("LoadAnimation"), LoadSkeletalModelAnimation, NULL, TEXT(" Group='Animation' label='Load Animation' "));
//		TwAddVarRW(Model_UI, TEXT("PlayAnimation"), TW_TYPE_UINT32, &g_EditorControl.m_AnimationIndex, Text.c_str());
//	}
//
//	if (g_EditorControl.m_Listener)
//	{
//		TwAddVarRO(Model_UI, TEXT("Listener"), TW_TYPE_STDSTRING, &g_EditorControl.m_Listener->m_Name, TEXT(" Group='Listeners' label='Listener' "));
//	}
//
//	if (g_EditorControl.m_AudioSource)
//	{
//		TwAddButton(Model_UI, TEXT("AddSound"), AddSound, NULL, TEXT(" Group='AudioSource' label='AddSound' "));
//		TwAddButton(Model_UI, TEXT("AddStream"), AddStream, NULL, TEXT(" Group='AudioSource' label='AddStream' "));
//
//		std::map<soString, soAudio*>::iterator it = g_EditorControl.m_AudioSource->m_AudioMap.begin();
//		while (it != g_EditorControl.m_AudioSource->m_AudioMap.end())
//		{
//			if ((*it).second->m_Audio.m_isStream)
//			{
//				TwAddVarRO(Model_UI, TEXT("Stream"), TW_TYPE_STDSTRING, &(*it).second->m_Audio.m_Name, TEXT(" Group='Stream' label='Stream' "));
//				TwAddButton(Model_UI, TEXT("ChangeStream"), ChangeStream, NULL, TEXT(" Group='Stream' label='ChangeStream' "));
//				TwAddButton(Model_UI, TEXT("Play"), PlayAudio, NULL, TEXT(" Group='Stream' label='Play' "));
//				TwAddButton(Model_UI, TEXT("TogglePause"), ToggleAudioPause, NULL, TEXT(" Group='Stream' label='TogglePause' "));
//			}
//			else
//			{
//				TwAddVarRO(Model_UI, TEXT("Audio"), TW_TYPE_STDSTRING, &(*it).second->m_Audio.m_Name, TEXT(" Group='Audio' label='Audio' "));
//				TwAddButton(Model_UI, TEXT("ChangeAudio"), ChangeAudio, NULL, TEXT(" Group='Audio' label='ChangeAudio' "));
//				TwAddButton(Model_UI, TEXT("Play"), PlayAudio, NULL, TEXT(" Group='Audio' label='Play' "));
//				TwAddButton(Model_UI, TEXT("TogglePause"), ToggleAudioPause, NULL, TEXT(" Group='Audio' label='TogglePause' "));
//			}
//			
//			it++;
//		}
//	}
//
//	TwDefine(TEXT(" Actor_GUI color='50 10 50' alpha=128 "));
//	TwDefine(TEXT(" Actor_GUI position='1550 0' "));
//	TwDefine(TEXT(" Actor_GUI size='330 500' "));
//	TwDefine(TEXT(" Actor_GUI valueswidth=fit "));
//	TwDefine(TEXT(" Actor_GUI refresh=0.1 "));
//}
//
//void PBRInterface()
//{
//	
//}

///************************************************************************/
///* Funciones de Movimiento de camaras                                   */
///************************************************************************/

/************************************************************************/
/* Funcion UPDATE de la aplicacion                                      */
/************************************************************************/
void Update(float _DeltaTime)
{
	//Actualizacion de camaras
	soCameraManager::Instance().Update(g_DeltaTime);

#ifdef VR_EDITOR	
	/* VRCamera Update*/
	soVRManager::Instance().Update(_DeltaTime);
	//soGUIManager::Instance().UpdateMousePosition(g_ClientMousePosition);

#endif // VR_EDITOR

	//World update
	soWorldManager::Instance().Update(g_DeltaTime);

#ifdef RUBENPHYS
	//Physic Update
	soPhysicsManager::Instance().Update(g_DeltaTime);
#endif
}

/************************************************************************/
/* Funcion Render de la aplicacion                                      */
/************************************************************************/
void Render()
{

	//g_editorProfiler.Begin(soString(L"ScenneRender"));

	//Scenne Render
	for (uint8 i = 0; i < g_TotalActiveCameras; i++)
	{
		if (g_GUIFlags[GUI_ViewPort])
		{
			soCameraManager::Instance().ChangeTo(i);
			if (dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera)->m_Active)
			{
				AxesGUI();

				//Actualizacion por camara activa
				uint8 MatProp = MAT_PROP_ALBEDO;
				soWorldManager::Instance().m_SceneGraph.Update();
				soWorldManager::Instance().m_SceneGraph.GenerateCustomNodeList(MatProp);

				soRenderManager::Instance().Render(&soWorldManager::Instance().m_SceneGraph.m_CullingNodeList, &dynamic_cast<soEditorCamera*>(soCameraManager::Instance().m_ActiveCamera)->m_CameraRenderTarget, ActiveRenderPass);
			}
		}		
	}

	//g_editorProfiler.End(soString(L"ScenneRender"));

#ifdef RUBENPHYS
	//Phys Debug Render
	soPhysicsManager::Instance().Render();
#endif

	//g_editorProfiler.ProfileBegin(soString(L"RenderEditorGui"));
	//Editor GUI render
	RenderEditorGUI();
	//g_editorProfiler.ProfileEnd(soString(L"RenderEditorGui"));

	//g_editorProfiler.ProfileBegin(soString(L"RenderManager"));
	//soRenderManager::Instance().Render(NULL, NULL, RID_GUI | RID_FontSystem);

	//g_editorProfiler.ProfileEnd(soString(L"RenderManager"));

	soRenderManager::Instance().Render(NULL, NULL, RID_GUI);

	//g_editorProfiler.ProfileBegin(soString(L"DXGIManager"));
	soDXGIManager::Instance().m_Device.m_ptrSwapChain->Present(0, 0); //Aqui se manda llamar el render del imgui
	//g_editorProfiler.ProfileEnd(soString(L"DXGIManager"));


#ifdef VR_EDITOR

	//Render to VR System
	uint8 MatProp = MAT_PROP_ALBEDO;

	//Actualizacion por camara activa
	soCameraManager::Instance().ChangeTo(soVRManager::Instance().m_LeftEyeCamera->m_ID);	
	soWorldManager::Instance().m_SceneGraph.Update();
	soWorldManager::Instance().m_SceneGraph.GenerateCustomNodeList(MatProp);
	soRenderManager::Instance().Render(&soWorldManager::Instance().m_SceneGraph.m_CullingNodeList, &soVRManager::Instance().m_LeftEyeRT, RID_PBR | RID_SkyBox);

	soCameraManager::Instance().ChangeTo(soVRManager::Instance().m_RightEyeCamera->m_ID);
	soWorldManager::Instance().m_SceneGraph.Update();
	soWorldManager::Instance().m_SceneGraph.GenerateCustomNodeList(MatProp);
	soRenderManager::Instance().Render(&soWorldManager::Instance().m_SceneGraph.m_CullingNodeList, &soVRManager::Instance().m_RightEyeRT, RID_PBR | RID_SkyBox);

	soVRManager::Instance().Render();

#endif // VR_EDITOR
}

///************************************************************************/
///* Funcion Bind Html		                                            */
///************************************************************************/
//void Save_Model(Awesomium::WebView* caller, const Awesomium::JSArray& args)
//{
//	OutputDebugString(L"Caca");
//
//	soString Path = SaveFilePath();
//
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!Path.empty())
//	{
//		if (g_EditorControl.m_SelectedNode != NULL)
//		{
//			uint64 Type = Soul_Actor;
//			soAssetManager::Instance().SaveAssetToFile(Path, g_EditorControl.m_SelectedNode->m_ptrActor, Type);
//		}
//	}
//}
//
//void Load_Model(Awesomium::WebView* caller, const Awesomium::JSArray& args)
//{
//	soString Path = LoadFilePath();
//
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!Path.empty())
//	{
//		soWorldManager::Instance().LoadActor(Path);
//	}
//}
//
//void Load_Static_Model(Awesomium::WebView* caller, const Awesomium::JSArray& args)
//{
//	soString Path = LoadFilePath();
//
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!Path.empty())
//	{
//		soActorStartUp ActorInfo;
//		soString Name = Path;
//		while (Name.back() != L'\\')
//		{
//			ActorInfo.sName.push_back(Name.back());
//			Name.pop_back();
//		}
//
//		std::reverse(ActorInfo.sName.begin(), ActorInfo.sName.end());
//
//		while (ActorInfo.sName.back() != L'.')
//		{
//			ActorInfo.sName.pop_back();
//		}
//		ActorInfo.sName.pop_back();
//
//		uint64 ComponentsFlags = 0;
//		ComponentsFlags |= CT_Mesh_Static;
//
//		soComponentStartUp ComponentsInfo;
//		ComponentsInfo.ResourcePath = Path;
//
//		soWorldManager::Instance().LoadActor(ComponentsInfo.ResourcePath);
//	}
//}
//
//Awesomium::JSValue OnHello2(Awesomium::WebView* caller, const Awesomium::JSArray& args)
//{
//	OutputDebugString(L"Hellooooo\n");
//
//	Awesomium::JSValue temp(5);
//
//	return temp;
//}
//
//Awesomium::JSValue OnHello4(Awesomium::WebView* caller, const Awesomium::JSArray& args)
//{
//	OutputDebugString(L"Cancer\n");
//
//	Awesomium::JSValue temp(10);
//
//	return temp;
//}
//
//Awesomium::JSValue Get_Files_Name(Awesomium::WebView* caller, const Awesomium::JSArray& args)
//{
//	Awesomium::JSArray tempJsArray;
//
//	uint32 numOfFiles = soFilesManager::Instance().GetDirectoryFilesName().size();
//
//	for (int32 i = 0; i < numOfFiles; i++)
//	{
//		Awesomium::JSValue JSvalue(Awesomium::WSLit(ws2s(*soFilesManager::Instance().GetDirectoryFilesName().at(i)).c_str()));
//		tempJsArray.Push(JSvalue);
//	}
//
//	Awesomium::JSValue tempResult(tempJsArray);
//
//	return tempResult;
//}
//
//Awesomium::JSValue Get_Num_Of_Files(Awesomium::WebView* caller, const Awesomium::JSArray& args)
//{
//	int32 tempNumOfFiles = soFilesManager::Instance().GetDirectoryFilesName().size();
//	Awesomium::JSValue tempResult(tempNumOfFiles);
//
//	return tempResult;
//}
//
//void CreateActorFromFileA(const soString& _path)
//{
//	//Revisamos si se debe de cargar un archivo de mapa
//	if (!_path.empty())
//	{
//		soComponentStartUp comp;
//		comp.ComponentsFlags |= CT_Mesh_Static;
//		comp.ResourcePath = _path;
//
//		soString name = GetFileName(_path);
//		soWorldManager::Instance().CreateNewActor(name, comp);
//	}
//}
//
//void Open_Files(Awesomium::WebView* caller, const Awesomium::JSArray& args)
//{
//	uint32 NumOfFile = soGUIManager::Instance().ExecuteJSWithReturnValueByID(L"GetNumForPath()", 1).m_jsValue.ToInteger();
//	OutputDebugString(int2ws(NumOfFile).c_str());
//	CreateActorFromFileA(soFilesManager::Instance().GetPathByID(NumOfFile));
//}

/************************************************************************/
/* Funcionalidad de la ventana							                */
/************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (soImGuiManager::IsStarted())
	{
		soImGuiManager::Instance().InputHandler(hWnd, message, wParam, lParam);
	}

	char tempChar;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_MBUTTONDOWN:
	{
		g_InputEvent |= IE_MBUTTONDOWN;
	}
	break;

	case WM_MBUTTONUP:
	{
		g_InputEvent &= ~(IE_MBUTTONDOWN);
	}
	break;

	case WM_MOUSEWHEEL:
	{
	}
	break;

	case WM_LBUTTONDOWN:
	{
		g_InputEvent |= IE_LBUTTONDOWN;
	}
	break;

	case WM_LBUTTONUP:
	{
		g_InputEvent &= ~(IE_LBUTTONDOWN);
	}
	break;

	case WM_RBUTTONDOWN:
	{
		g_InputEvent |= IE_RBUTTONDOWN;
	}
	break;

	case WM_RBUTTONUP:
	{
		g_InputEvent &= ~(IE_RBUTTONDOWN);
	}
	break;

	case WM_MOUSEMOVE:
	{
	}
	break;

	case WM_CHAR:
	{
	}
	break;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 0x57:
			g_InputEvent |= IE_KBW;
			break;
		case 0x41:
			g_InputEvent |= IE_KBA;
			break;
		case 0x53:
			g_InputEvent |= IE_KBS;
			break;
		case 0x44:
			g_InputEvent |= IE_KBD;
			break;
		default:
			break;
		}
	}
	break;

	case WM_KEYUP:
	{
		switch (wParam)
		{
		case 0x57:
			g_InputEvent &= ~(IE_KBW);
			break;
		case 0x41:
			g_InputEvent &= ~(IE_KBA);
			break;
		case 0x53:
			g_InputEvent &= ~(IE_KBS);
			break;
		case 0x44:
			g_InputEvent &= ~(IE_KBD);
			break;
		default:
			break;
		}
	}
	break;

	case WM_SIZE:
	{
		GetWindowRect(hWnd, &g_WindowRect);
		GetClientRect(g_hwnd, &g_ClientRect);

		if (soRenderManager::IsStarted())
		{
			soRenderManager::Instance().Resize(soRect(g_ClientRect.left, g_ClientRect.top, g_ClientRect.right, g_ClientRect.bottom));
			soCameraManager::Instance().Resize();
		}
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOULEDITOR));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_WindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	(void)(nCmdShow);
	hInst = hInstance; // Store instance handle in our global variable
	g_hwnd = CreateWindowW(g_WindowClass, g_Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1900, 1200, nullptr, nullptr, hInstance, nullptr);

	if (!g_hwnd)
	{
		return FALSE;
	}

	ShowWindow(g_hwnd, SW_SHOW);
	UpdateWindow(g_hwnd);

	return TRUE;
}

BOOL InitWindow(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, g_Title, MAX_LOAD_STRING);
	LoadStringW(hInstance, IDC_SOULEDITOR, g_WindowClass, MAX_LOAD_STRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	g_hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOULEDITOR));

	return TRUE;
}

#ifdef RUBENFSM
void MachineTest(void)
{
	OutputDebugString(L"Idle");
}

void MachineTest2(void)
{
	OutputDebugString(L"Attack");
}

bool TransitionTest(void)
{
	//Chequeo de la transición de estado
	return false;
}
#endif

#if defined(PROFILE)
void UpdateOutputProfiler(float _dT)
{
	g_ElapsedTimeForProfiler += _dT;

	if (g_ElapsedTimeForProfiler >= 1000)
	{
		std::string textBuffer;
		textBuffer = g_editorProfiler.GetOutputBuffer()->str();
		soFont::Instance().DrawFontText(50, 1030, s2ws(textBuffer));
		g_ElapsedTimeForProfiler = 0.0f;
	}

}
#endif

/************************************************************************/
/* Funcion MAIN de la aplicacion                                        */
/************************************************************************/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
	//Inicialización del contador
	g_editorTimer.Reset();
	
#if defined(PROFILE)
	//Init Profiler
	g_editorProfiler.Init(50);
#endif

	/* Inicializacion de la ventana                                         */
	if (!InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
	{
		return 0;
	}

	/* Inicializacion de sistemas de Directx                                */
	if (FAIL(InitModules()))
	{
		FinishApp();
		return 0;
	}

	//Get Current Ticks
	g_lastTime = g_editorTimer.getMilliseconds();
	double lag = 0.0f;

	/* Loop General de la aplicacion                                        */
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
#if defined(PROFILE)
		//Begin Main Loop Profiler
		g_editorProfiler.Begin(soString(L"MainLoop"));
#endif

		//Get Current Ticks
		g_actualTime = g_editorTimer.getMilliseconds();

		//DeltaTime Calculus in milliseconds
		g_DeltaTime = g_actualTime - g_lastTime;
		g_lastTime = g_actualTime;
		lag += g_DeltaTime;

#if defined(PROFILE)
		g_editorProfiler.SetDeltaTime(g_DeltaTime);
#endif

		//Message Inputs
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//Test Input
#ifdef RUBEN
		soOISManager::Instance().Update();
#endif // RUBEN	

		//Begin UpdateApp Profile
		//g_editorProfiler.ProfileBegin(soString(L"UpdateApp"));

		//Actualizacion de sistemas de la app
		UpdateApp();

		//End UpdateApp Profile
		//g_editorProfiler.ProfileEnd(soString(L"UpdateApp"));


		//Begin UpdateApp Profile
		//g_editorProfiler.ProfileBegin(soString(L"ImGuiNewFrame"));
		ImGui::NewFrame(); //Debe hacerse antes del update
		//End UpdateApp Profile
		//g_editorProfiler.ProfileEnd(soString(L"ImGuiNewFrame"));


		UpdateFPS(g_DeltaTime);

		
		while (lag >= MS_PER_UPDATE)
		{
			Update(lag);
			
			lag -= MS_PER_UPDATE;
		}

		//Begin Update Profile
		//g_editorProfiler.ProfileBegin(soString(L"EditorGUIUpdate"));
		//EditorGUIupdate
		soImGuiManager::Instance().Update(); 
		//End Update Profile
		//g_editorProfiler.ProfileEnd(soString(L"EditorGUIUpdate"));

#if defined(PROFILE)
		//Begin Update Profile
		g_editorProfiler.Begin(soString(L"Render"));
#endif
		Render();
#if defined(PROFILE)
		//End Update Profile
		g_editorProfiler.End(soString(L"Render"));
#endif

#if defined(PROFILE)
		//Render Profiler Text Buffer
		UpdateOutputProfiler(g_DeltaTime);
#endif

		//FPS Count
		g_FPSCount++;

#if defined(PROFILE)
		//End MainLoop Profile
		g_editorProfiler.End(soString(L"MainLoop"));
		g_editorProfiler.DumpOutputToBuffer();
#endif
	}

	FinishApp();
	return (int)msg.wParam;
}

