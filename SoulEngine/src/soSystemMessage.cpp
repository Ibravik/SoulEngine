/************************************************************************/
/**
* @LC	  14/10/2016
* @file   soAudioFile.h
* @Author Raul Portillo (ruulaasz_@hotmail.com)
* @date   14/10/2016
* @brief  Definition of base class soSystemMessage and all its child classes
* @bug	  No know Bugs
*/
/************************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soSystemMessage.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soSystemMessage				      			*/
	/************************************************************************/
	soSystemMessage::soSystemMessage()
	{
		Connect(std::bind(&soSystemMessage::OnStartUp, this));
	}

	soSystemMessage::~soSystemMessage()
	{
		ShutDown();
	}

	RESULT soSystemMessage::OnStartUp()
	{
		return EC_OK;
	}

	void soSystemMessage::OnShutDown()
	{

	}
}
