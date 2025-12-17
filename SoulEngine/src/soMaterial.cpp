/********************************************************************/
/**
* @LC	  10/9/2016
* @file   soMaterial.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soMaterial
* @bug	  No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soResourceManager.h"
#include "soMaterialManager.h"
#include "soMaterial.h"

/************************************************************************/
/* Definicion de la clase soMaterial		                            */
/************************************************************************/
namespace SoulSDK
{
	soMaterial::soMaterial()
	{
		Connect(std::bind(&soMaterial::OnStartUp, this, std::placeholders::_1));

		for (uint8 i = 0; i < Texture_Slot_TotalSlots; i++)
		{
			m_Textures[i] = NULL;
		}
		m_MaterialFlags = 0;
		m_ID = 0;
		m_References = 0;
	}

	soMaterial::~soMaterial()
	{		
		ShutDown();
	}

	void soMaterial::LoadTexture(const MaterialData& _MaterialData, uint8 _MatProp, soString& _DefaultPath)
	{
		ResourceParameters RP;
		RP.ResourceType = RT_Texture;
		soTexture* NewTexture = NULL;
		aiString aiLoatedString;
		soString TexturePath;

		uint8 Slot;

		switch (_MatProp)
		{
		case 0x1:
			Slot = 0;
			break;
		case 0x2:
			Slot = 1;
			break;
		case 0x6:
			Slot = 2;
			break;
		case 0x8:
			Slot = 3;
			break;
		}

		if (_MaterialData.Material && aiReturn::aiReturn_SUCCESS == _MaterialData.Material->GetTexture((aiTextureType)_MatProp, 0, &aiLoatedString))
		{
			TexturePath = GetFileName(s2ws(std::string(aiLoatedString.C_Str())));
			TexturePath = _MaterialData.FilePath + TexturePath;

			if (_DefaultPath.empty())
			{
				_DefaultPath = TexturePath;
				if (_DefaultPath.at(_DefaultPath.size() - 2) == TEXT('_'))
				{
					_DefaultPath.pop_back();
				}
			}

			//Intento con el nombre adquirido del material
			RP.ResourceName = TexturePath + soString(TEXT(".dds"));
			RP.FilePath = TexturePath + soString(TEXT(".dds"));
			NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP));
			if (NewTexture != NULL)
			{
				AssingTexture(NewTexture, Slot);
				return;
			}			
		}
		//Intento con nomenclatura propia del motor
		if (!_DefaultPath.empty() && _DefaultPath.back() != TEXT('_'))
		{
			_DefaultPath.push_back(TEXT('_'));
		}
		RP.ResourceName = _DefaultPath + GetChanelInfo(_MatProp);
		RP.FilePath = _DefaultPath + GetChanelInfo(_MatProp);
		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP));
		if (NewTexture != NULL)
		{
			AssingTexture(NewTexture, Slot);
			return;
		}

		//En caso de no encontrar la textura, se carga una por default
		RP.FilePath = TEXT("Resources\\Textures\\Default_1.dds");
		RP.ResourceName = TEXT("Default_1");
		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP));
		if (NewTexture != NULL)
		{
			AssingTexture(NewTexture, Slot);
			return;
		}
	}

	soString soMaterial::GetChanelInfo(uint8 _MatProp)
	{
		switch (_MatProp)
		{
		case 0x1:
			return soString(TEXT("d.dds"));
			break;
		case 0x2:
			return soString(TEXT("m.dds"));
			break;
		case 0x6:
			return soString(TEXT("n.dds"));
			break;
		case 0x8:
			return soString(TEXT("r.dds"));
			break;
		default:
			break;
		}
	}

	RESULT soMaterial::OnStartUp(const MaterialData& _MaterialData)
	{
		m_ID = _MaterialData.ID;
		soString DefaultPath;

		if (!_MaterialData.Name.empty())
		{
			m_MaterialName = _MaterialData.Name;
		}
		else
		{
			m_MaterialName = uint2ws(m_ID);
		}
		
		/***Albedo***/
		LoadTexture(_MaterialData, 0x1, DefaultPath);
		/***Metallic***/
		LoadTexture(_MaterialData, 0x2, DefaultPath);
		/***Normal***/
		LoadTexture(_MaterialData, 0x6, DefaultPath);
		/***Roughness***/
		LoadTexture(_MaterialData, 0x8, DefaultPath);
		
		ResourceParameters RP;
		RP.ResourceType = RT_Texture;
		soTexture* NewTexture = NULL;

		/***Environment***/
		RP.FilePath = TEXT("Resources\\Textures\\Clouds.dds");
		RP.ResourceName = TEXT("grace_cube");
		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP));
		if (NewTexture != NULL)
		{
			AssingTexture(NewTexture, Texture_Slot_4);
		}

		/***Irradianse***/
		RP.FilePath = TEXT("Resources\\Textures\\Clouds_Diffuse.dds");
		RP.ResourceName = TEXT("grace_diffuse_cube");
		NewTexture = dynamic_cast<soTexture*>(soResourceManager::Instance().Load(RP));
		if (NewTexture != NULL)
		{
			AssingTexture(NewTexture, Texture_Slot_5);
		}
		
		return EC_OK;
	}

	void soMaterial::OnShutDown()
	{		
		for (uint8 i = 0; i < static_cast<uint8>(Texture_Slot_TotalSlots); i++)
		{
			if (m_Textures[i] != NULL)
			{
				soResourceManager::Instance().Destroy(m_Textures[i]);
			}
		}
	}

	void soMaterial::Render(uint32 _MatProp)
	{		
		//Seteamos las texturas para el pixel shader y el sampler
		SetShaderTexture(_MatProp);
	}

	void soMaterial::EndRender()
	{
		for (uint32 i = 0; i < static_cast<uint32>(Texture_Slot_TotalSlots); i++)
		{	
			uint32 NumTextures = 1;
			soDXGIManager::Instance().PSSetShaderResources(i, NumTextures, NULL);
		}
	}

	void soMaterial::AssingTexture(soTexture * _Texture, uint8 _TextureType)
	{
		if (m_Textures[_TextureType] != NULL && m_Textures[_TextureType]->GetReferences() > 0)
		{
			soResourceManager::Instance().Destroy(m_Textures[_TextureType]);
			m_Textures[_TextureType] = NULL;
		}

		switch (_TextureType)
		{
		case Texture_Slot_0:
			m_MaterialFlags &= ~(MAT_PROP_ALBEDO);
			m_MaterialFlags |= MAT_PROP_ALBEDO;
			break;
		case Texture_Slot_1:
			m_MaterialFlags &= ~(MAT_PROP_METALLIC);
			m_MaterialFlags |= MAT_PROP_METALLIC;
			break;
		case Texture_Slot_2:
			m_MaterialFlags &= ~(MAT_PROP_NORMAL);
			m_MaterialFlags |= MAT_PROP_NORMAL;
			break;
		case Texture_Slot_3:
			m_MaterialFlags &= ~(MAT_PROP_ROUGHNESS);
			m_MaterialFlags |= MAT_PROP_ROUGHNESS;
			break;
		case Texture_Slot_4:
			m_MaterialFlags &= ~(MAT_PROP_IRRADIANCE);
			m_MaterialFlags |= MAT_PROP_IRRADIANCE;
			break;
		case Texture_Slot_5:
			m_MaterialFlags &= ~(MAT_PROP_ENVIRONMENT);
			m_MaterialFlags |= MAT_PROP_ENVIRONMENT;
			break;
		default:
			break;			
		}

		m_Textures[_TextureType] = _Texture;
	}

	void soMaterial::SetShaderTexture(uint32 _Material)
	{
		uint32 Index = 0;
		for (uint32 i = 1; i != Shader_TotalSlot; )
		{
			if (_Material &i)
			{
				if (m_Textures[Index] != NULL)
				{
					soDXGIManager::Instance().PSSetShaderResources(Index, 1, &m_Textures[Index]->m_Texture);
				}
				else
				{
					soDXGIManager::Instance().PSSetShaderResources(Index, 1, NULL);
				}
			}
			Index++;
			i = 0;
			i |= (1u << Index);
		}
	}

	bool soMaterial::CheckMaterialProperty(uint8& _MatProp)
	{
		if (m_MaterialFlags &_MatProp)
			return true;
		return false;
	}

}
