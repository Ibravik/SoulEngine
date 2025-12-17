/********************************************************************/
/**
* @LC     6/8/2016
* @file   soResourceFactory.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/8/2016
* @brief  Implementacion de la clase soResourceFactory
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soTexture.h"
#include "soStaticModel.h"
#include "soSkeletalModel.h"
#include "soResourceFactory.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soResourceManager                             */
	/************************************************************************/
	soResourceFactory::soResourceFactory()
	{
	}

	soResourceFactory::~soResourceFactory()
	{
	}

	soResource * soResourceFactory::CreateResourceByType(const uint8 & _ResourceType)
	{
		switch (_ResourceType)
		{
		case RT_Default:
			return NULL;
			break;
		case RT_Texture:
			return SOE_NEW soTexture;
			break;	
		case RT_3DSkeletalModel:
			return SOE_NEW soSkeletalModel;
			break;
		case RT_3DStaticModel:
			return SOE_NEW soStaticModel;
			break;
		case RT_Audio:
			return NULL;
			break;			
		default:
			return NULL;
			break;
		}
	}
}