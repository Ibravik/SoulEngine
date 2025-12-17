/********************************************************************/
/**
* LC	  10/10/2016
* @file   soRenderManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/25/2015
* @brief  Implementacion de la clase base soRenderBeast.El render se 
		  encarga de poner en la pantalla todo aquello que se requiera, 
		  desde una interzas hasta la escena completa. 
		  El pipeline de rendereo se decide aqui, asi como la api a 
		  utilizar
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once
//#define LIGHT_TEST
#define BLUR_TEST
//#define CompressedGBuffer

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soEffect.h"

/************************************************************************/
/* Declaracion de la clase soRenderBeast							    */
/************************************************************************/
namespace SoulSDK
{	
	/************************************************************************/
	/* Enum para facilitar el ingreso de propiedades como CB al shader      */
	/************************************************************************/
	enum CBSlots
	{
		Slot_0 = 0,
		Slot_1,
		Slot_2,
		Slot_3,
		Slot_4,
		Slot_5,
		Slot_6,
		Slot_7,
		Slot_8,
		Slot_9,
		Slot_10,
		Slot_11,
		Slot_12,
		Slot_13,
		Slot_14,
		Slot_15,
		Slot_16,
	};

	/************************************************************************/
	/* Enum para manejo de pases en PBR                                     */
	/************************************************************************/
	enum RenderPass
	{
		RP_GBuffer = 0,
#ifdef LIGHT_TEST
		RP_ComputeLight,
#endif
		RP_SSAO,
		RP_Luminance,
		RP_Bright,
		RP_BlurH,
		RP_BlurV,
		RP_AddBright,
		RP_ToneMaping,
	};

	/************************************************************************/
	/* Declaracion de enum que define los distintos efectos disponibles     */
	/************************************************************************/
	enum AvilableRenderEffect
	{
		ARE_PBR = 0,
		ARE_EditorSelection,
		ARE_GraphicDebug,
		ARE_FinalPass,
		ARE_Terrain,
		ARE_SkyBox,
		ARE_EditorGUI,
		ARE_MultyPassViewer,
		ARE_FontSystem,
		ARE_TOTAL_EFECTS
	};

	enum AvailableTechnique
	{
		AT_Standard = 0,
	};

	/************************************************************************/
	/* Declaracion de identificadores de render                             */
	/************************************************************************/
	enum RenderIDAply
	{
		RID_PBR = 0x01,
		RID_EditorSelection = 0x02,
		RID_GraphicDebuger = 0x04,
		RID_RenderToBackBuffer = 0x08,
		RID_Terrain = 0x10,
		RID_SkyBox = 0x20,
		RID_GUI = 0x40,
		RID_MultyPass = 0x80,
		RID_FontSystem = 0x100
	};

	/************************************************************************/
	/* Estructura de informacion para el compute ligth                      */
	/************************************************************************/
	struct FramebufferFlatElement
	{
		uint32 rb;
		uint32 ga;
	};

	/************************************************************************/
	/* Declaracion de la clase soRenderBeast							    */
	/************************************************************************/
	class soRenderManager : public soModule<soRenderManager>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soRenderManager();																																/*!< Constructor Standard */
		~soRenderManager();																																/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		soStaticModel* m_ScreenAlignedQuad;																												/*!< Referencia al plano alineado a la camara */
		soStaticModel* m_SkyBoxMesh;																													/*!< Mesh para SkyBox */
		soStaticModel* VRControler;																														/*!< Mesh para el control de VR */
		soTexture* m_SelectionTexture;																													/*!< Textura de seleccion para un modelo */
		soTexture* m_MeshSelectionTexture;																												/*!< Textura de seleccion para un modelo */
		soTexture* m_SkyBoxTexture;																														/*!< Textura de SkyBox */
		soTexture* m_FontTexture;																														/*!< Textura de Font */
		soSceneNode* m_SelectedNode;																													/*!< Nodo de seleccion en modo editor */
		soBlendState m_DefaultBlend;																													/*!< Blend default del render manager */
		soBlendState m_AditiveBlend;																													/*!< Blend especializado del render manager */
		soSampler m_DefaultSampler;																														/*!< Sampler default linear */
		soSampler m_PointSampler;																														/*!< Sampler default point */
		soRenderTarget* m_BackBuffer;																													/*!< Referencia al render target que se usara como BF*/
		soCamera* m_Camera;																																/*!< Referencia a la camara para render */
		std::map<uint8, soEffect*> m_EfectList;																											/*!< Lista de efectos con los que cuenta el Pipeline */
		std::vector<soSceneNode*>* m_SceneNodeList;																									/*!< Lista de modelos a renderear */
		soViewPort m_BlurViewPort;																														/*!< Viewport utilizado en el pase de blur */
		ID3D11Buffer* m_dynamicVertexBufferFont;																										/*!< Vertex Buffer Dinamico para las letras de la fuente */
#ifdef LIGHT_TEST
		soStructuredBuffer<FramebufferFlatElement> m_LitBufferCS;																						/*!< Buffer para Compute ligth */
		soStructuredBuffer<DirectionalLightCB> m_LightBuffer;																							/*!< Buffer con todas las luces en la escena */
		uint32 m_ActiveLights;																															/*!< Numero de luces activas en la escena */
		soTexture m_LightShaderResourceBuffer;																											/*!< Buffer de luces para el shader */
#endif // TEST

	public:
		FrameBuffer m_FrameBuffer;																														/*!< Estructura de CB por Frame */
		PBRData m_PBRData;																																/*!< Estructura de constant buffer para PBR */
		DirectionalLightCB m_LightCB;																													/*!< Buffer para una sola luz */
		soEditorControl* m_EditorControl;																												/*!< Puntero local al editorcontrol, este contiene info importante para el render de info relacionada con el actor seleccionado */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();																																/*!< Inicializacion de la maquina de render. Aqui es donde se establecen los efectos que aplicaran al render final */
		virtual void OnShutDown()override;																												/*!< Finalizacion de la maquina de render */
		RESULT LoadRenderResources();																													/*!< Carga el cuadro alineado a la pantalla y texturas default */
		RESULT InitPBR();																																/*!< Inicializacion del efector PBR */
		RESULT InitEditorSelection();																													/*!< Efecto de seleccion de modelos a travez de shader para modo editor */
		RESULT InitGraphicDebug();																														/*!< El efecto de graphic debug es solo añadir lineas para renderear */
		RESULT InitFinalPassEffect();																													/*!< Inicializacion del pase final de render al back buffer */
		RESULT InitTerrain();																															/*!< Inicializacion del Render de terreno */
		RESULT InitSkyBox();																															/*!< Inicializacion del efector SkyBox */
		RESULT InitEditorGUI();																															/*!< Inicializacion del pase de GUI para modo editor */
		RESULT InitMultyPassViewer();																													/*!< Inicializacion del pase de visualizacion de todas las faces */
		RESULT InitFontSystem();																														/*!< Inicializacion del sistema de fuentes */

		RESULT AplyPBR();																																/*!< Aplica el Efecto de PBR a la lista de modelos */
		RESULT AplyEditorSelection();																													/*!< El efecto de seleccion ocurre en modo editor cuando se selecciona un modelo en interfas */
		RESULT AplyGraphicDebug();																														/*!< Aplica el Graphic debug */
		RESULT AplyTerrain();																															/*!< Aplica el Efecto de terreno al render */
		RESULT AplySkyBox();																															/*!< Aplica el Efecto de SkyBox al render */
		soEffect* GetEffectByID(uint8 _ID);																												/*!< Retorna el efecto solicitado por ID */
		RESULT AplyEditorGUI();																															/*!< Aplica el pase de GUI en modo editor */
		RESULT ApplyMultyPassViewer();																													/*!< Aplica el pase de visualizacion de todas las faces */
		RESULT ApplyFontSystem();
		void UpdateCameraBuffer();																														/*!< Actualizacion de catos para el render manager */																									
#ifdef LIGHT_TEST
		void SetActiveLights(uint32 activeLights);																										/*!< Asigna un buffer segun el numero de luces maximo en la escena */
		void SetupLights();																																/*!< Actualiza la informacion del buffer de luces */
#endif // LIGHT_TEST
	public:
		RESULT RenderToBackBuffer(soAPITexture& _Tex, soRect& _Size, soRenderTarget* _BackBuffer = NULL, soMatrix4x4 World = soMath::IdentityMatrix4x4());/*!< Renderea sobre el back buffer segun el orden en que se llame, al tamaño especificado por el rect */
		void Render(std::vector<soSceneNode*>* _ActorNodeList, soRenderTarget* _BackBuffer = NULL, uint32 _RenderKey = 0);								/*!< Release The Kraken, con el ID se pueden seleccionar distintos pases para disminuir o aumentar lo que se desee renderear sobre el back buffer ingresado */
		void Resize(const soRect& _ClientSize);																											/*!< Actualiza cualquier cambio de tamaño del area cliente que afecte a las texturaas del Render target */
		std::vector <soRenderTarget*> GetPassRenderTargets(AvilableRenderEffect _EffectID, RenderPass _PassID);											/*!< Retorna el render target del pase y efecto especificado. En caso de no existir retorna el ultimo pase a Back Buffer */
	};
}