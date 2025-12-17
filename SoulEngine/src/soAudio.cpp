/************************************************************************/
/**
* @LC	  14/10/2016
* @file   soAudio.cpp
* @Author Raul Portillo (ruulaasz_@hotmail.com)
* @date   5/10/2016
* @brief  Definition of base class soAudio
* @bug	  No know Bugs
*/
/************************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soAudio.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de la clase soAudio				      				*/
	/************************************************************************/
	soAudio::soAudio()
	{

	}

	soAudio::~soAudio()
	{
		ShutDown();
	}
	
	RESULT soAudio::StartUp(const AudioFileLoadData& _Data)
	{
		m_Audio = *m_Audio.Load(_Data);

		return EC_OK;
	}
}
