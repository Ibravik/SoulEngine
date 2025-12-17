/********************************************************************/
/**
* @LC     10/12/2016
* @file   soTexture.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soTexture
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soTexture.h"

/************************************************************************/
/* Definicion de la clase soTexture									    */
/************************************************************************/
namespace SoulSDK
{
	soTexture::soTexture()
	{
	}
	
	soTexture::~soTexture()
	{
		ShutDown();
	}
	
	RESULT soTexture::StartUp(const TextureData& _TextureData)
	{
		return m_Texture.Create(_TextureData);
	}

	RESULT soTexture::Load(const ResourceParameters& _ResourceParameters)
	{
		SUPER::Load(_ResourceParameters);
		return m_Texture.Load(m_szFilePath);
	}
}