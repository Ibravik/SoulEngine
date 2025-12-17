/************************************************************************/
/**
* @LC	  11/18/2016
* @file   soWorldManager.h
* @Author Lehi Muñoz (lehi.munoz@gmail.com)
* @date   11/18/2016
* @brief  Definition of base class soComponentFactory
* @bug	  No know Bugs
*/
/************************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soStaticModelComponent.h"
#include "soSkeletalModelComponent.h"
#include "soPhysicComponent.h"
#include "soAssetManager.h"
#include "soComponentFactory.h"

namespace SoulSDK
{
	soComponentFactory::soComponentFactory()
	{
		Connect(std::bind(&soComponentFactory::OnStartUp, this));
	}

	soComponentFactory::~soComponentFactory()
	{
		ShutDown();
	}

	soComponent * soComponentFactory::CreateComponent(const ComponentData& _ComponentData)
	{
		soComponent* NewComponent = NULL;

		switch (_ComponentData._ComponentType)
		{
		case CT_Camera:

			break;
		case SoulSDK::CT_Light:

			break;
		case SoulSDK::CT_Mesh_Static:
		{
			NewComponent = SOE_NEW soStaticModelComponent();
			if (FAIL(NewComponent->StartUp(_ComponentData)))
			{
				return NULL;
			}			
			NewComponent->m_ComponentName = soString(TEXT("Static Model"));
			return NewComponent;
		}
			break;
		case SoulSDK::CT_Mesh_Dynamic:
		{
			NewComponent = SOE_NEW soSkeletalModelComponent();
			if (FAIL(NewComponent->StartUp(_ComponentData)))
			{
				return NULL;
			}
			NewComponent->m_ComponentName = soString(TEXT("Skeletal Model"));
			return NewComponent;
		}
			break;
		case SoulSDK::CT_BillBoard:
			break;
		case SoulSDK::CT_Listener:
			break;
		case SoulSDK::CT_AudioSource:
			break;
		case SoulSDK::CT_Physic:
		{
			NewComponent = SOE_NEW soPhysicComponent();
			if (FAIL(NewComponent->StartUp(_ComponentData)))
			{
				return NULL;
			}
			NewComponent->m_ComponentName = soString(TEXT("Physic"));
			return NewComponent;
		}
			break;
		default:
		{
			SOE_OutputLOG("soComponentFactory", "CreateNewComponent", "Ivalid ComponentType ");
			return NULL;
		}			
			break;
		}

		SOE_OutputLOG("soComponentFactory", "CreateNewComponent", "Ivalid ComponentType ");
		return NULL;
	}

	soComponent* soComponentFactory::LoadComponent(soString& _AssetFileName)
	{
		soComponent* NewComponent = static_cast<soComponent*>(soAssetManager::Instance().LoadAssetFromFile(_AssetFileName));
		if (!NewComponent)
		{
			SOE_OutputLOG("soComponentFactory", "CreateNewComponent", "Failed to load file");
			return NULL;
		}

		return NewComponent;
	}
	
	RESULT soComponentFactory::OnStartUp()
	{
		return EC_OK;
	}

	void soComponentFactory::OnShutDown()
	{

	}
}
