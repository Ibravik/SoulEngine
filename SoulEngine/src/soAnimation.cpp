/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soAnimation.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/22/2016
* @brief  Implementacion de la clase soAnimation
* @bug	   No inown bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soAnimation.h"

/************************************************************************/
/* Definicion de la clase soAnimationNode				                */
/************************************************************************/
namespace SoulSDK
{
	soAnimation::soAnimation()
	{
		Connect(std::bind(&soAnimation::OnStartUp, this, std::placeholders::_1));
		m_KeyFrameMap.clear();
	}

	soAnimation::~soAnimation()
	{	
		ShutDown();
	}

	RESULT soAnimation::OnStartUp(const aiAnimation& _Animation)
	{		
		m_Name = soString(s2ws(std::string(_Animation.mName.C_Str())));
		m_Duration = static_cast<float>(_Animation.mDuration);
		m_TicksPerSecond = static_cast<float>(_Animation.mTicksPerSecond);

		for (uint32 i = 0; i < _Animation.mNumChannels; i++)
		{
			soKeyFrame NewKeyFrame;
			NewKeyFrame.StartUp(*_Animation.mChannels[i]);
			m_KeyFrameInfo[NewKeyFrame.m_Name] = NewKeyFrame;
		}

		return EC_OK;
	}

	void soAnimation::OnShutDown()
	{
		m_KeyFrameInfo.clear();
		m_KeyFrameMap.clear();
	}
}
