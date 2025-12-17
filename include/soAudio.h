/************************************************************************/
/**
* @LC	  14/10/2016
* @file   soAudio.h
* @Author Raul Portillo (ruulaasz_@hotmail.com)
* @date   5/10/2016
* @brief  Declaration of base class soAudio
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soResource.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soAudio				 					    */
	/************************************************************************/
	class soAudio : public soResource
	{
	public:
		soAudio();
		~soAudio();

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
		soAudioFile m_Audio;
		soString m_FilePath;

	public:
		virtual RESULT StartUp(const AudioFileLoadData& _Data);	/*!< Inicializacion del recurso */
	};
}

