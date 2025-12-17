/************************************************************************/
/**
* @LC	  10/14/2016
* @file   soMessageSystem.cpp
* @Author Raul Portillo (ruulaasz_@hotmail.com)
* @date   10/14/2016
* @brief  Definition of class soMessageSystem
* @bug	  No know Bugs
*/
/************************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soEntity.h"
#include "soAudioSystem.h"
#include "soMessageSystem.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soMessageSystem							    */
	/************************************************************************/
	soMessageSystem::soMessageSystem()
	{
		Connect(std::bind(&soMessageSystem::OnStartUp, this));
	}

	soMessageSystem::~soMessageSystem()
	{

	}

	void soMessageSystem::SendEntityMessage(soEntityMessage* _Message)
	{
		if (_Message->Target)
		{
			_Message->Target->QueueMessage(_Message);
		}
	}

	RESULT soMessageSystem::OnStartUp()
	{
		return EC_OK;
	}

	void soMessageSystem::OnShutDown()
	{

	}
}