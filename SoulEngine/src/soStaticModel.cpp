/********************************************************************/
/**
* @LC	  10/12/2016
* @file   soStaticModel.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   1/23/2016
* @brief  Implementacion de la clase soStaticModel
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "Soul_Graphics.h"
#include "soMesh.h"
#include "soStaticModel.h"
#include "soCameraManager.h"
#include "soGraphicDebugManager.h"
#include "soCamera.h"

/************************************************************************/
/* Definicion de la clase soStaticModel					                */
/************************************************************************/
namespace SoulSDK
{
	soStaticModel::soStaticModel()
	{
		m_IsStatic = (uint32)1;
	}

	soStaticModel::~soStaticModel()
	{
		ShutDown();
	}
}
