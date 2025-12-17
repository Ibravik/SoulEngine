/********************************************************************
LC			14/2016
Created:	10/10/2016
Filename:	soEngineDataStruct.h 
Author:		Lehi Muñoz

Purpose:	Cabecera que incluye las estructuras de datos para la
			inicializacion de componentes abstaidos del engine
*********************************************************************/
#pragma once

#include "Soul_Graphics.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaración del enum ComponentType				    				*/
	/************************************************************************/
	enum ComponentType
	{
		CT_Camera = 0x01,
		CT_Light = 0x02,
		CT_Mesh_Static = 0x04,
		CT_Mesh_Dynamic = 0x08,
		CT_BillBoard = 0x10,
		CT_Listener = 0x20,
		CT_AudioSource = 0x40,
		CT_Physic = 0x80
	};

	/************************************************************************/
	/* soLight                                                              */
	/************************************************************************/
	struct LightData
	{
		soVector3D positionView;
		float attenuationBegin;
		soVector3D color;
		float attenuationEnd;

		LightData()
		{
			attenuationBegin = 5.0f;
			attenuationEnd = 10.0f;
			color = soVector3D(1, 1, 1);
		}
	};

	/************************************************************************/
	/* soCamera                                                             */
	/************************************************************************/
	struct CameraData
	{
		soVector3D Position;								/*!< Indica la pocision de la camara en coordenadas de Mundo */
		soVector3D Up;										/*!< Indica la direccion hacia arriba con respecto a la camara */
		soVector3D Front;									/*!< Indica la direccion hacia adelante con respecto a la camara */
		soDegrees Fov;										/*!< Indica el angulo de apertura de la camara */
		float AspectRatio;									/*!< Indica la configuracion de resolucion del width con respecto al heigth de resolucion de la pantalla */
		float Near;											/*!< Indica el punto mas cercano que persive la camara */
		float Far;											/*!< Indica el punto mas lejano que persive la camara */
		soString CameraName;								/*!< Nombre de la camara, este se asigna como nombre del recurso */
		soDegrees MaxPitch;									/*!< Max Pitch Angle */
		soVector3D MaxVel;									/*!< Max Camera Velocity */
		soVector3D RotationVel;								/*!< Max Camera Velocity rotation in euler */
		bool InvertY;										/*!< Indica si esta invertida la vista en Y */
		bool EnableUpMovement;								/*!< Habilita el movimiento en el eje Y*/
		bool LockUp;										/*!< Lokea el sentido Up de la camara para que no cambie sin importar la rotacion */
		ViewPortData ViewportData;							/*!< Informacion del viewport ligado a la camara */

		CameraData() 										
		{
			Position = soVector3D(0,0,-200);
			Up = soVector3D(0, 1, 0);
			Front = soVector3D(0, 0, 1);
			Fov = 70.0f;
			AspectRatio = 21.0f / 9.0f;
			Near = 0.1f;
			Far = 300.0f;
			CameraName = TEXT("Default");
			MaxPitch = 89.0f;
			MaxVel = soVector4D(1, 1, 1, 0.1f);
			RotationVel = soVector3D(0.1f, 0.1f, 0.1f);
			InvertY = false;
			EnableUpMovement = false;
			LockUp = true;
			ViewportData.Height = (float)soDXGIManager::Instance().m_ClientRect.m_Height;
			ViewportData.Width = (float)soDXGIManager::Instance().m_ClientRect.m_Width;
		}
	};

	/************************************************************************/
	/* soMesh                                                               */
	/************************************************************************/
	struct MeshData
	{
		std::vector<aiMesh*> *MaterialMesh;					/*!< Lista de meshes generados por assimp que comparten un mismo material */
		soString FilePath;									/*!< Ruta al archivo */
		API_PRIMITIVE_TOPOLOGY APT;							/*!< Determina el tipo de topologia del mesh. */

		MeshData()
		{
			MaterialMesh = NULL;
			APT = API11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
	};

	/************************************************************************/
	/* soMaterial					                                        */
	/************************************************************************/
	struct MaterialData
	{
		aiMaterial* Material;								/*!< Material asignado por assimp */
		soString FilePath;									/*!< Ruta al archivo */
		uint32 ID;											/*!< ID asignado por el manager */
		soString Name;										/*!< Nombre del material */

		MaterialData()
		{
			ID = 0;
			Material = NULL;
		}
	};

	/************************************************************************/
	/* soBillboard                                                          */
	/************************************************************************/
	struct BillboardData
	{
		soString TexturePath;									/*!< Ruta al archivo */

		BillboardData()
		{
			
		}
	};

	/************************************************************************/
	/* soComponent										                    */
	/************************************************************************/
	struct ComponentData
	{
		soString ResourcePath;									/*!< Ruta al componente */
		uint64 _ComponentType;									/*!< Tipo de componente */
		void* userData;											/*!< Actual actor*/

		ComponentData()
		{
			_ComponentType = 0;
			userData = NULL;
		}
	};

	/************************************************************************/
	/* soActor										                        */
	/************************************************************************/
	struct soActorStartUp
	{
		soString sName;
		bool bEnable = true;
		bool bVisible = true;
	};

	/************************************************************************/
	/* soResource															*/
	/************************************************************************/
	struct ResourceParameters
	{
		soString ResourceName;								/*!< Nombre del recurso, asignado por el recurso */
		soString FilePath;									/*!< Ruta al archivo */
		uint8 ResourceType;									/*!< ID de Tipo de recurso */

		ResourceParameters()
		{
			ResourceName = TEXT("");
			FilePath = TEXT("");
			ResourceType = 50;
		}
	};

	/************************************************************************/
	/* soTerrain                                                            */
	/************************************************************************/
	struct TerrainData
	{
		float Width;										/*!< Ancho del mapa */
		float Height;										/*!< Alto del mapa */

		TerrainData()
		{
			Width = 256.0f;
			Height = 256.0f;
		}
	};

	/************************************************************************/
	/* soEditorGUIManager                                                   */
	/************************************************************************/
	struct ImGuiData
	{
		HWND hwnd;
		float Width;
		float Height;

		ImGuiData()
		{
			hwnd = 0;
		}
	};

	/************************************************************************/
	/* soFont																*/
	/************************************************************************/
	struct FontData
	{
		soString fontBinFilePath;
		soVector2D screenSize;

		FontData()
		{
			fontBinFilePath = soString(L"Resources\\BinFiles\\Font_Data.bin");
			screenSize = soVector2D(1080.0f, 768.0f);
		}
	};


	struct FontVertex
	{
		soVector4D Position;
		soVector2D Texture;
	};

	/************************************************************************/
	/* PBRData                                                              */
	/************************************************************************/
#pragma pack (push)
#pragma pack(16)
	struct PBRData
	{
		soVector4D FinalExposition;
		soVector4D Intensity_Scale_Bias_SampleRadius;
		soVector4D BloomThreshold;
		soVector4D BloomMuiltiplier_Exposure;
		int32 MipLevel[4];

		PBRData()
		{
			FinalExposition.X = 3.0f;
			Intensity_Scale_Bias_SampleRadius.X = 0.26f;
			Intensity_Scale_Bias_SampleRadius.Y = 1.0f;
			Intensity_Scale_Bias_SampleRadius.Z = 0.20f;
			Intensity_Scale_Bias_SampleRadius.W = 0.24f;
			BloomThreshold.X = 0.30f;
			BloomMuiltiplier_Exposure.X = 1.0f;
			BloomMuiltiplier_Exposure.Y = 1.0f;
			MipLevel[0] = 0;
			MipLevel[1] = 0;
		}
	};
#pragma pack(pop)
}