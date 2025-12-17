/********************************************************************/
/**
* @LC     11/21/2016
* @file   soAssetManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/10/2016
* @brief  Implementacion de la clase soAssetManager
* @bug	   No known bugs.
*/
/********************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soResourceManager.h"
#include "soStaticModelComponent.h"
#include "soStaticModel.h"
#include "soAssetManager.h"

/************************************************************************/
/* Definicion de la clase soAssetManager                           */
/************************************************************************/
namespace SoulSDK
{
	soAssetManager::soAssetManager()
	{
		Connect(std::bind(&soAssetManager::OnStartUp, this));
		m_ptrFile = NULL;
	}
	
	soAssetManager::~soAssetManager()
	{
	}

	void SoulSDK::soAssetManager::OnShutDown()
	{
		CloseFile();
	}

	RESULT soAssetManager::OnStartUp()
	{
		return EC_OK;
	}

	RESULT soAssetManager::OpenFile(soString _FilePath, uint8& _Status)
	{		
		CloseFile();		
		
		if (_Status &File_Write)
		{
			soString Extencion(TEXT(".SoulEngineAsset"));
			_FilePath += Extencion;
#ifdef UNICODE
			m_ptrFile = fopen(ws2s(_FilePath).c_str(), "wb");
#else
			m_ptrFile = fopen(_FilePath.c_str(), "wb");
#endif // UNICODE			
			if (m_ptrFile != NULL)
			{
				return EC_OK;
			}
			SOE_OutputLOG("soAssetManager", "OpenFile()", "Fail to load from file to write");
			return EC_FALSE;
		}

		if (_Status &File_Read)
		{
			if(CheckExtentionFile(_FilePath, soString(TEXT("SoulEngineAsset"))))
#ifdef UNICODE
				m_ptrFile = fopen(ws2s(_FilePath).c_str(), "rb");
#else
				m_ptrFile = fopen(_FilePath.c_str(), "rb");
#endif
			else
				return EC_FALSE;
			if (m_ptrFile != NULL)
			{
				return EC_OK;
			}
			SOE_OutputLOG("soAssetManager", "OpenFile()", "Fail to load from file to Read");
			return EC_FALSE;
		}
		SOE_OutputLOG("soAssetManager", "OpenFile()", "Invalid Open flag");
		return EC_FALSE;
	}
		
	void soAssetManager::CloseFile()
	{
		if (m_ptrFile)
		{
			fclose(m_ptrFile);
			m_ptrFile = NULL;
		}
	}
	
	RESULT soAssetManager::SaveToFile(void* _Data, BasicDataType _DataType)
	{
		if (m_ptrFile == NULL)
		{
			SOE_OutputLOG("soAssetManager", "SaveToFile()", "Unable to save data, file not open")
			return EC_FALSE;
		}

		uint32 length = 0;
		TCHAR* Char = NULL;

		switch (_DataType)
		{
		case SoulSDK::BDT_TCHAR:
			fwrite(_Data, 1, sizeof(TCHAR), m_ptrFile);
			break;
		case SoulSDK::BDT_uint8:
			fwrite(_Data, 1, sizeof(uint8), m_ptrFile);
			break;
		case SoulSDK::BDT_uint16:
			fwrite(_Data, 1, sizeof(uint16), m_ptrFile);
			break;
		case SoulSDK::BDT_uint32:
			fwrite(_Data, 1, sizeof(uint32), m_ptrFile);
			break;
		case SoulSDK::BDT_uint64:
			fwrite(_Data, 1, sizeof(uint64), m_ptrFile);
			break;
		case SoulSDK::BDT_int8:
			fwrite(_Data, 1, sizeof(int8), m_ptrFile);
			break;
		case SoulSDK::BDT_int16:
			fwrite(_Data, 1, sizeof(int16), m_ptrFile);
			break;
		case SoulSDK::BDT_int32:
			fwrite(_Data, 1, sizeof(int32), m_ptrFile);
			break;
		case SoulSDK::BDT_int64:
			fwrite(_Data, 1, sizeof(uint64), m_ptrFile);
			break;
		case SoulSDK::BDT_Float:
			fwrite(_Data, 1, sizeof(float), m_ptrFile);
			break;
		case SoulSDK::BDT_soString:			
			length = static_cast<uint32>((static_cast<soString*>(_Data))->size());

			fwrite(&length, 1, sizeof(uint32), m_ptrFile);

			Char = NULL;
			for (uint32 i = 0; i < length; i++)
			{
				Char = &(static_cast<soString*>(_Data))->at(i);
				fwrite(Char, 1, sizeof(TCHAR), m_ptrFile);
			}			
			break;
		default:
			SOE_OutputLOG("soAssetManager", "SaveToFile()", "Invalid data type");
			return EC_FALSE;
			break;
		}
		return EC_OK;
	}

	RESULT soAssetManager::ReadInFile(void* _DataOutput, BasicDataType _DataType)
	{
		if (m_ptrFile == NULL)
		{
			SOE_OutputLOG("soAssetManager", "ReadInFile()", "Unable to read data, file not open");
			return EC_FALSE;
		}

		uint32 length = 0;
		TCHAR Char;

		switch (_DataType)
		{
		case SoulSDK::BDT_TCHAR:
			fread(_DataOutput, sizeof(TCHAR), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_uint8:
			fread(_DataOutput, sizeof(uint8), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_uint16:
			fread(_DataOutput, sizeof(uint16), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_uint32:
			fread(_DataOutput, sizeof(uint32), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_uint64:
			fread(_DataOutput, sizeof(uint64), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_int8:
			fread(_DataOutput, sizeof(int8), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_int16:
			fread(_DataOutput, sizeof(int16), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_int32:
			fread(_DataOutput, sizeof(int32), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_int64:
			fread(_DataOutput, sizeof(int64), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_Float:
			fread(_DataOutput, sizeof(float), 1, m_ptrFile);
			break;
		case SoulSDK::BDT_soString:
			fread(&length, sizeof(uint32), 1, m_ptrFile);

			for (uint32 i = 0; i < length; i++)
			{
				fread(&Char, sizeof(TCHAR), 1, m_ptrFile);
				(static_cast<soString*>(_DataOutput))->push_back(Char);
			}
			break;
		default:
			SOE_OutputLOG("soAssetManager", "ReadInFile()", "Invalid data type")
				break;
		}
		return EC_OK;
	}

	soString soAssetManager::CreateFilePath(soString& _FilePath)
	{
		soString Path = _FilePath;
		while (Path.back() != L'\\')
		{
			Path.pop_back();
		}

		return Path;
	}

	RESULT soAssetManager::SaveActorData(soActor& _Actor, soString& _FilePath)
	{
		////Primero guardamos los componentes del actor en assets separados, esto permite que solo amacenemos las rutas en el asset final y los podamos cargar de manera independiente
		//std::vector<soString> _ComponentPath;
		//soString RootPath = CreateFilePath(_FilePath);

		//for (uint32 i = 0; i < static_cast<uint32>(_Actor.m_Components.size()); i++)
		//{	
		//	//Guardamos la ruta del componente
		//	_ComponentPath.push_back(RootPath + _Actor.m_Components.at(i)->m_ComponentName + soString(TEXT(".SoulEngineAsset")));

		//	//Guardamos el componente en un asset separado
		//	soStaticModelComponent* ModelComponent = dynamic_cast<soStaticModelComponent*>(_Actor.m_Components.at(i));
		//	if (ModelComponent != NULL)
		//	{
		//		uint64 Misc = CT_Mesh_Static;
		//		uint64 Asset = Soul_Component;
		//		SaveAssetToFile(_FilePath, _Actor.m_Components.at(i), Asset, Misc);
		//		continue;
		//	}
		//}

		////Ahora guardamos la informacion del asset del actor
		//uint8 Flags = File_Write;
		//if (FAIL(OpenFile(_FilePath, Flags)))
		//{
		//	return EC_FALSE;
		//}
		//Flags = 0;

		///***Tipo de asset***/
		//Flags |= Soul_Actor;
		//SaveToFile(&Flags, BDT_uint8);

		///***Nombre***/
		//SaveToFile(&_Actor.m_Name, BDT_soString);

		///***Transform***/
		//soQuaternions Rotation;
		//_Actor.m_Transform.GetRotator().GetQuat(Rotation);
		//for (uint8 i = 0; i < 4; i++)
		//{
		//	SaveToFile(&Rotation[i], BDT_Float);
		//}

		//soVector3D Position;
		//Position = _Actor.m_Transform.GetPosition();
		//for (uint8 i = 0; i < 3; i++)
		//{
		//	SaveToFile(&Position[i], BDT_Float);
		//}

		//soVector3D Scale;
		//Scale = _Actor.m_Transform.GetScale();
		//for (uint8 i = 0; i < 3; i++)
		//{
		//	SaveToFile(&Scale[i], BDT_Float);
		//}

		////Numero de componentes en el actor
		//uint32 NumComponents = static_cast<uint32>(_Actor.m_Components.size());
		//SaveToFile(&NumComponents, BDT_uint32);

		////Ruta a los componentes
		//for (uint32 i = 0; i < static_cast<uint32>(_ComponentPath.size()); i++)
		//{
		//	SaveToFile(&_ComponentPath.at(i), BDT_soString);
		//}

		//CloseFile();
		return EC_OK;
	}

	soActor* soAssetManager::LoadActorData()
	{
		//soActor* NewActor = SOE_NEW soActor;

		///***Nombre***/
		//ReadInFile(&NewActor->m_Name, BDT_soString);

		///***Transform***/
		//soQuaternions Rotation;		
		//for (uint8 i = 0; i < 4; i++)
		//{
		//	ReadInFile(&Rotation[i], BDT_Float);
		//}
		//NewActor->m_Transform.GetRotator().GetQuat(Rotation);

		//soVector3D Position;		
		//for (uint8 i = 0; i < 3; i++)
		//{
		//	ReadInFile(&Position[i], BDT_Float);
		//}
		//NewActor->m_Transform.SetPosition(Position);

		//soVector3D Scale;		
		//for (uint8 i = 0; i < 3; i++)
		//{
		//	ReadInFile(&Scale[i], BDT_Float);
		//}
		//NewActor->m_Transform.SetScale(Scale);

		////Numero de componentes
		//uint32 NumComponents;
		//ReadInFile(&NumComponents, BDT_uint32);

		////Ruta de cada componente
		//std::vector<soString> _ComponentPath;
		//for (uint32 i = 0; i < NumComponents; i++)
		//{
		//	soString Path;
		//	ReadInFile(&Path, BDT_soString);
		//	_ComponentPath.push_back(Path);
		//}
		//CloseFile();

		//for (uint32 i = 0; i < NumComponents; i++)
		//{
		//	NewActor->AddNewComponent(static_cast<soComponent*>(LoadAssetFromFile(_ComponentPath.at(i))));
		//}

		//return NewActor;

		return NULL;
	}

	RESULT soAssetManager::SaveComponentData(soComponent& _Component, uint64 _ComponentType, soString& _FilePath)
	{		
		//uint8 Flags = File_Write;
		//soString Path = CreateFilePath(_FilePath);
		//if (FAIL(OpenFile(soString(Path + _Component.m_Name), Flags)))
		//{
		//	return EC_FALSE;
		//}
		//Flags = 0;

		///***Tipo de asset***/
		//Flags |= Soul_Component;
		//SaveToFile(&Flags, BDT_uint8);

		///***Nombre***/
		//SaveToFile(&_Component.m_Name, BDT_soString);

		///***Component Type***/
		//SaveToFile(&_ComponentType, BDT_uint64);
		//		
		//switch (_ComponentType)
		//{
		//case CT_Camera_FirstPerson:
		//	break;
		//case CT_Camera_ThirdPerson:
		//	break;
		//case CT_Light_Static:
		//	break;
		//case CT_Light_Dynamic:
		//	break;
		//case CT_Mesh_Static:
		//	SaveStaticModelComponentAsset(*dynamic_cast<soStaticModelComponent*>(&_Component));
		//	break;
		//case CT_Mesh_Dynamic:
		//	break;
		//case CT_Camera_Free:
		//	break;
		//case CT_Collider_Box:
		//	break;
		//case CT_BillBoard:
		//	break;
		//default:
		//	break;
		//}

		//CloseFile();
		return EC_OK;
	}

	soComponent* soAssetManager::LoadComponentData()
	{
		//soString Name;
		///***Nombre***/
		//ReadInFile(&Name, BDT_soString);

		//uint64 _ComponentType;
		///***Component Type***/
		//ReadInFile(&_ComponentType, BDT_uint64);
		//
		//soComponent* NewComponent;

		//switch (_ComponentType)
		//{
		//case CT_Camera_FirstPerson:
		//	break;
		//case CT_Camera_ThirdPerson:
		//	break;
		//case CT_Light_Static:
		//	break;
		//case CT_Light_Dynamic:
		//	break;
		//case CT_Mesh_Static:
		//{
		//	/***Transform***/
		//	soQuaternions Rotation;
		//	for (uint8 i = 0; i < 4; i++)
		//	{
		//		ReadInFile(&Rotation[i], BDT_Float);
		//	}

		//	soVector3D Position;
		//	for (uint8 i = 0; i < 3; i++)
		//	{
		//		ReadInFile(&Position[i], BDT_Float);
		//	}

		//	soVector3D Scale;
		//	for (uint8 i = 0; i < 3; i++)
		//	{
		//		ReadInFile(&Scale[i], BDT_Float);
		//	}

		//	NewComponent = SOE_NEW soStaticModelComponent;
		//	NewComponent->m_Name = Name;
		//	(dynamic_cast<soStaticModelComponent*>(NewComponent))->m_Transform.SetRotator(soRotator(Rotation));
		//	(dynamic_cast<soStaticModelComponent*>(NewComponent))->m_Transform.SetPosition(Position);
		//	(dynamic_cast<soStaticModelComponent*>(NewComponent))->m_Transform.SetScale(Scale);
		//	soStaticModel* NewModel = LoadStaticModelComponentAsset(Name);
		//	(dynamic_cast<soStaticModelComponent*>(NewComponent))->SetModel(NewModel);

		//	return NewComponent;
		//}
		//break;
		//case CT_Mesh_Dynamic:
		//	break;
		//case CT_Camera_Free:
		//	break;
		//case CT_Collider_Box:
		//	break;
		//case CT_BillBoard:
		//	break;
		//default:
		//	return NULL;
		//	break;
		//}
		return NULL;
	}

	RESULT soAssetManager::SavePBRData(PBRData& _PBRData, soString& _FilePath)
	{
		uint8 Flags = File_Write;
		soString Path = CreateFilePath(_FilePath);
		if (FAIL(OpenFile(soString(Path + soString(TEXT("PBRData"))), Flags)))
		{
			return EC_FALSE;
		}
		Flags = 0;

		/***Tipo de asset***/
		Flags |= Soul_PBRData;
		SaveToFile(&Flags, BDT_uint8);

		//Final Exosition
		for (int8 k = 0; k < 4; k++)
		{
			SaveToFile(&_PBRData.FinalExposition[k], BDT_Float);
		}

		//Intensity_Scale_Bias_SampleRadius
		for (int8 k = 0; k < 4; k++)
		{
			SaveToFile(&_PBRData.Intensity_Scale_Bias_SampleRadius[k], BDT_Float);
		}

		//BloomThreshold
		for (int8 k = 0; k < 4; k++)
		{
			SaveToFile(&_PBRData.BloomThreshold[k], BDT_Float);
		}

		//BloomMuiltiplier_Exposure
		for (int8 k = 0; k < 4; k++)
		{
			SaveToFile(&_PBRData.BloomMuiltiplier_Exposure[k], BDT_Float);
		}

		CloseFile();

		return EC_OK;
	}

	PBRData* soAssetManager::LoadPBRData()
	{
		PBRData* _PBRData = SOE_NEW PBRData;

		//Final Exosition
		for (int8 k = 0; k < 4; k++)
		{
			ReadInFile(&_PBRData->FinalExposition[k], BDT_Float);
		}

		//Intensity_Scale_Bias_SampleRadius
		for (int8 k = 0; k < 4; k++)
		{
			ReadInFile(&_PBRData->Intensity_Scale_Bias_SampleRadius[k], BDT_Float);
		}

		//BloomThreshold
		for (int8 k = 0; k < 4; k++)
		{
			ReadInFile(&_PBRData->BloomThreshold[k], BDT_Float);
		}

		//BloomMuiltiplier_Exposure
		for (int8 k = 0; k < 4; k++)
		{
			ReadInFile(&_PBRData->BloomMuiltiplier_Exposure[k], BDT_Float);
		}

		CloseFile();

		return _PBRData;
	}

	soStaticModel* soAssetManager::LoadStaticModelComponentAsset(soString& _ResourceName)
	{
		//uint8 Flags = 0;
		//uint32 InfoNumber = 0;
		//soString Path;
		//ResourceParameters RP;

		//RP.ResourceName = _ResourceName;
		//soStaticModel* _NewModel = dynamic_cast<soStaticModel*>(soResourceManager::Instance().Create(RP, RT_3DStaticModel, NULL));
		//if (_NewModel == NULL)
		//{
		//	return NULL;
		//}

		///***Number of meshes***/
		//ReadInFile(&InfoNumber, BDT_uint32);
		//if (InfoNumber == 0)
		//{
		//	SAFE_DELETE(_NewModel);
		//	return NULL;
		//}

		///***Meshes info(include material Info)***/
		//for (uint32 i = 0; i < InfoNumber; i++)
		//{
		//	soMesh* _NewMesh = SOE_NEW soMesh;
		//	/***Vertex Number***/
		//	uint32 VertexNumber;
		//	ReadInFile(&VertexNumber, BDT_uint32);

		//	_NewMesh->m_VertexBuffer.CreatesVertexArray(VertexNumber);

		//	/***Vertex Info***/
		//	for (uint32 j = 0; j < VertexNumber; j++)
		//	{
		//		/***Position***/
		//		for (int8 k = 0; k < 4; k++)
		//		{
		//			ReadInFile(&_NewMesh->m_VertexBuffer.GetsVertexArrayElement(j).Position[k], BDT_Float);
		//		}
		//		/***TexCoords***/
		//		for (int8 k = 0; k < 2; k++)
		//		{
		//			ReadInFile(&_NewMesh->m_VertexBuffer.GetsVertexArrayElement(j).Texture0[k], BDT_Float);
		//		}
		//		/***Normals***/
		//		for (int8 k = 0; k < 3; k++)
		//		{
		//			ReadInFile(&_NewMesh->m_VertexBuffer.GetsVertexArrayElement(j).Normals[k], BDT_Float);
		//		}
		//		/***BiNormals***/
		//		for (int8 k = 0; k < 3; k++)
		//		{
		//			ReadInFile(&_NewMesh->m_VertexBuffer.GetsVertexArrayElement(j).BiNormals[k], BDT_Float);
		//		}
		//		/***Tangent***/
		//		for (int8 k = 0; k < 3; k++)
		//		{
		//			ReadInFile(&_NewMesh->m_VertexBuffer.GetsVertexArrayElement(j).Tangent[k], BDT_Float);
		//		}
		//	}

		//	if (FAIL(_NewMesh->m_VertexBuffer.CreateVertexBuffer()))
		//	{
		//		SAFE_DELETE(_NewModel);
		//		return NULL;
		//	}

		//	/***Index Number***/
		//	uint32 IndexNumber;
		//	ReadInFile(&IndexNumber, BDT_uint32);

		//	_NewMesh->m_IndexBuffer.CreateIndexArray(IndexNumber);

		//	/***Index Info***/
		//	for (uint32 j = 0; j < IndexNumber; j++)
		//	{
		//		ReadInFile(&_NewMesh->m_IndexBuffer.GetIndexArrayElement(j), BDT_uint32);
		//	}

		//	if (FAIL(_NewMesh->m_IndexBuffer.CreateIndexBuffer()))
		//	{
		//		SAFE_DELETE(_NewModel);
		//		return NULL;
		//	}

		//	/***Mesh Material component flag***/
		//	ReadInFile(&Flags, BDT_uint8);
		//				
		//	Texture2DLoadData TD;
		//	soTexture* NewTexture;

		//	if (Flags &MAT_PROP_ALBEDO)
		//	{
		//		ReadInFile(&Path, BDT_soString);
		//		RP.FilePath = Path;
		//		TD.FilePath = Path;
		//		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP, RT_Texture, &TD));
		//		if (NewTexture != NULL)
		//		{
		//			_NewMesh->m_Material.AssingTexture(NewTexture, TextureType_ALBEDO);
		//		}
		//		Path.clear();
		//	}

		//	if (Flags &MAT_PROP_METALLIC)
		//	{
		//		ReadInFile(&Path, BDT_soString);
		//		RP.FilePath = Path;
		//		TD.FilePath = Path;
		//		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP, RT_Texture, &TD));
		//		if (NewTexture != NULL)
		//		{
		//			_NewMesh->m_Material.AssingTexture(NewTexture, TextureType_METALLIC);
		//		}
		//		Path.clear();
		//	}

		//	if (Flags &MAT_PROP_NORMAL)
		//	{
		//		ReadInFile(&Path, BDT_soString);
		//		RP.FilePath = Path;
		//		TD.FilePath = Path;
		//		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP, RT_Texture, &TD));
		//		if (NewTexture != NULL)
		//		{
		//			_NewMesh->m_Material.AssingTexture(NewTexture, TextureType_NORMAL);
		//		}
		//		Path.clear();
		//	}

		//	if (Flags &MAT_PROP_ROUGHNESS)
		//	{
		//		ReadInFile(&Path, BDT_soString);
		//		RP.FilePath = Path;
		//		TD.FilePath = Path;
		//		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP, RT_Texture, &TD));
		//		if (NewTexture != NULL)
		//		{
		//			_NewMesh->m_Material.AssingTexture(NewTexture, TextureType_ROUGHNESS);
		//		}
		//		Path.clear();
		//	}

		//	if (Flags &MAT_PROP_IRRADIANCE)
		//	{
		//		ReadInFile(&Path, BDT_soString);
		//		RP.FilePath = Path;
		//		TD.FilePath = Path;
		//		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP, RT_Texture, &TD));
		//		if (NewTexture != NULL)
		//		{
		//			_NewMesh->m_Material.AssingTexture(NewTexture, TextureType_IRRADIANCE);
		//		}
		//		Path.clear();
		//	}

		//	if (Flags &MAT_PROP_ENVIRONMENT)
		//	{
		//		ReadInFile(&Path, BDT_soString);
		//		RP.FilePath = Path;
		//		TD.FilePath = Path;
		//		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP, RT_Texture, &TD));
		//		if (NewTexture != NULL)
		//		{
		//			_NewMesh->m_Material.AssingTexture(NewTexture, TextureType_ENVIRONMENT);
		//		}
		//		Path.clear();
		//	}
		//	_NewModel->m_MeshList.push_back(_NewMesh);
		//}
		//return _NewModel;

return NULL;
	}

	RESULT soAssetManager::SaveStaticModelComponentAsset(soStaticModelComponent& _StaticModel)
	{
		//uint8 Flags = 0;
		//uint32 InfoNumber = 0;
		//soString Path;
		//
		///***Transform***/
		//soQuaternions Rotation;
		//_StaticModel.m_Transform.GetRotator().GetQuat(Rotation);
		//for (uint8 i = 0; i < 4; i++)
		//{
		//	SaveToFile(&Rotation[i], BDT_Float);
		//}

		//soVector3D Position;
		//Position = _StaticModel.m_Transform.GetPosition();
		//for (uint8 i = 0; i < 3; i++)
		//{
		//	SaveToFile(&Position[i], BDT_Float);
		//}

		//soVector3D Scale;
		//Scale = _StaticModel.m_Transform.GetScale();
		//for (uint8 i = 0; i < 3; i++)
		//{
		//	SaveToFile(&Scale[i], BDT_Float);
		//}

		///***Number of meshes***/
		//InfoNumber = static_cast<uint32>(_StaticModel.GetModel()->m_MeshList.size());
		//SaveToFile(&InfoNumber, BDT_uint32);

		///***Meshes info(include material Info)***/
		//for (uint32 i = 0; i < static_cast<uint32>(_StaticModel.GetModel()->m_MeshList.size()); i++)
		//{
		//	/***Vertex Number***/
		//	InfoNumber = _StaticModel.GetModel()->m_MeshList.at(i)->m_VertexBuffer.GetsVertexArraySize();
		//	SaveToFile(&InfoNumber, BDT_uint32);
		//	/***Vertex Info***/
		//	for (uint32 j = 0; j < _StaticModel.GetModel()->m_MeshList.at(i)->m_VertexBuffer.GetsVertexArraySize(); j++)
		//	{
		//		/***Position***/
		//		for (int8 k = 0; k < 4; k++)
		//		{
		//			SaveToFile(&_StaticModel.GetModel()->m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).Position[k], BDT_Float);
		//		}
		//		/***TexCoords***/
		//		for (int8 k = 0; k < 2; k++)
		//		{
		//			SaveToFile(&_StaticModel.GetModel()->m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).Texture[k], BDT_Float);
		//		}
		//		/***Normals***/
		//		for (int8 k = 0; k < 3; k++)
		//		{
		//			SaveToFile(&_StaticModel.GetModel()->m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).Normals[k], BDT_Float);
		//		}
		//		/***BiNormals***/
		//		for (int8 k = 0; k < 3; k++)
		//		{
		//			SaveToFile(&_StaticModel.GetModel()->m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).BiNormals[k], BDT_Float);
		//		}
		//		/***Tangent***/
		//		for (int8 k = 0; k < 3; k++)
		//		{
		//			SaveToFile(&_StaticModel.GetModel()->m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).Tangent[k], BDT_Float);
		//		}
		//	}

		//	/***Index Number***/
		//	InfoNumber = _StaticModel.GetModel()->m_MeshList.at(i)->m_IndexBuffer.GetIndexArraySize();
		//	SaveToFile(&InfoNumber, BDT_uint32);
		//	/***Index Info***/
		//	for (uint32 j = 0; j < _StaticModel.GetModel()->m_MeshList.at(i)->m_IndexBuffer.GetIndexArraySize(); j++)
		//	{
		//		SaveToFile(&_StaticModel.GetModel()->m_MeshList.at(i)->m_IndexBuffer.GetIndexArrayElement(j), BDT_uint32);
		//	}

		//	/***Mesh Material component flag***/
		//	Flags = _StaticModel.GetModel()->m_MeshList.at(i)->m_Material.m_MaterialFlags;
		//	SaveToFile(&Flags, BDT_uint8);

		//	Flags = MAT_PROP_ALBEDO;
		//	/*	if (_StaticModel.GetModel()->m_MeshList.at(i)->m_Material.CheckMaterialProperty(Flags))
		//		{
		//			Path = _StaticModel.GetModel()->m_MeshList.at(i)->m_Material.GetTexture(TextureType_ALBEDO)->GetszFilePath();
		//			SaveToFile(&Path, BDT_soString);
		//		}
		//		Flags = MAT_PROP_METALLIC;
		//		if (_StaticModel.GetModel()->m_MeshList.at(i)->m_Material.CheckMaterialProperty(Flags))
		//		{
		//			Path = _StaticModel.GetModel()->m_MeshList.at(i)->m_Material.GetTexture(TextureType_METALLIC)->GetszFilePath();
		//			SaveToFile(&Path, BDT_soString);
		//		}
		//		Flags = MAT_PROP_NORMAL;
		//		if (_StaticModel.GetModel()->m_MeshList.at(i)->m_Material.CheckMaterialProperty(Flags))
		//		{
		//			Path = _StaticModel.GetModel()->m_MeshList.at(i)->m_Material.GetTexture(TextureType_NORMAL)->GetszFilePath();
		//			SaveToFile(&Path, BDT_soString);
		//		}
		//		Flags = MAT_PROP_ROUGHNESS;
		//		if (_StaticModel.GetModel()->m_MeshList.at(i)->m_Material.CheckMaterialProperty(Flags))
		//		{
		//			Path = _StaticModel.GetModel()->m_MeshList.at(i)->m_Material.GetTexture(TextureType_ROUGHNESS)->GetszFilePath();
		//			SaveToFile(&Path, BDT_soString);
		//		}
		//		Flags = MAT_PROP_IRRADIANCE;
		//		if (_StaticModel.GetModel()->m_MeshList.at(i)->m_Material.CheckMaterialProperty(Flags))
		//		{
		//			Path = _StaticModel.GetModel()->m_MeshList.at(i)->m_Material.GetTexture(TextureType_IRRADIANCE)->GetszFilePath();
		//			SaveToFile(&Path, BDT_soString);
		//		}
		//		Flags = MAT_PROP_ENVIRONMENT;
		//		if (_StaticModel.GetModel()->m_MeshList.at(i)->m_Material.CheckMaterialProperty(Flags))
		//		{
		//			Path = _StaticModel.GetModel()->m_MeshList.at(i)->m_Material.GetTexture(TextureType_ENVIRONMENT)->GetszFilePath();
		//			SaveToFile(&Path, BDT_soString);
		//		}*/
		//}
		return EC_OK;
	}

	void* soAssetManager::LoadAssetFromFile(const soString& _FilePath)
	{	
		uint8 Flags = File_Read;
		if (FAIL(OpenFile(_FilePath, Flags)))
		{
			return NULL;
		}
		Flags = 0;

		//Primero sacamos la llave de tipo de asset
		ReadInFile(&Flags, BDT_uint8);

		if (Flags & Soul_Actor)
		{
			//Tipo componente, primero se guarda su info de componente			
			return LoadActorData();
		}

		if (Flags &Soul_Component)
		{
			soComponent* NewComponent = LoadComponentData();
			return NewComponent;
		}

		if (Flags &Soul_PBRData)
		{
			PBRData* _PBRData = LoadPBRData();
			return _PBRData;
		}

		CloseFile();
		return NULL;
	}
	
	RESULT soAssetManager::SaveAssetToFile(soString& _FilePath, void* _Data, uint64 _AssetType, uint64 _MiscFlags)
	{	
		uint8 Flags = 0;
		Flags |= _AssetType;
		
		//Llamamos al tipo especifico de asset a guardar
		if (Flags & Soul_Actor)
		{
			//Tipo componente, primero se guarda su info de componente			
			return SaveActorData(*static_cast<soActor*>(_Data), _FilePath);
		}

		if (Flags & Soul_Component)
		{
			return SaveComponentData(*static_cast<soComponent*>(_Data), _MiscFlags, _FilePath);
		}

		if (Flags & Soul_PBRData)
		{
			return SavePBRData(*static_cast<PBRData*>(_Data), _FilePath);
		}

		return EC_FALSE;
	}
}