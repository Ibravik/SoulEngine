/********************************************************************/
/**
* @LC	  10/9/2016
* @file   soKeyFrame.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/22/2016
* @brief  Implementacion de la clase soKeyFrame
* @bug	   No inown bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soKeyFrame.h"

/************************************************************************/
/* Definicion de la clase soAnimationNode				                */
/************************************************************************/
namespace SoulSDK
{
	soKeyFrame::soKeyFrame()
	{
		Connect(std::bind(&soKeyFrame::OnStartUp, this, std::placeholders::_1));
	}

	soKeyFrame::~soKeyFrame()
	{		
		ShutDown();
	}

	RESULT soKeyFrame::OnStartUp(const aiNodeAnim& _AnimationChannel)
	{
		m_Name = soString(s2ws(std::string(_AnimationChannel.mNodeName.C_Str())));						/*!< Nombre del nodo afectado */

		for (uint32 i = 0; i < _AnimationChannel.mNumPositionKeys; i++)									/* Keyframes de traslacion */
		{
			soVector3D Translation;
			Translation.X = _AnimationChannel.mPositionKeys[i].mValue.x;
			Translation.Y = _AnimationChannel.mPositionKeys[i].mValue.y;
			Translation.Z = _AnimationChannel.mPositionKeys[i].mValue.z;

			TranslationKeyStruct NewTranslation;
			NewTranslation.Translation = Translation;
			float Time = static_cast<float>(_AnimationChannel.mRotationKeys[i].mTime);
			NewTranslation.KeyTime = Time;
			m_TranslationKey.push_back(NewTranslation);
		}

		for (uint32 i = 0; i < _AnimationChannel.mNumRotationKeys; i++)									/* Keyframes de rotacion */
		{
			soQuaternion Rotation;
			Rotation.X = _AnimationChannel.mRotationKeys[i].mValue.x;
			Rotation.Y = _AnimationChannel.mRotationKeys[i].mValue.y;
			Rotation.Z = _AnimationChannel.mRotationKeys[i].mValue.z;
			Rotation.W = _AnimationChannel.mRotationKeys[i].mValue.w;

			RotationKeyStruct NewRotation;
			NewRotation.Rotation = Rotation;
			float Time = static_cast<float>(_AnimationChannel.mRotationKeys[i].mTime);
			NewRotation.KeyTime = Time;
			m_RotationKey.push_back(NewRotation);
		}

		for (uint32 i = 0; i < _AnimationChannel.mNumScalingKeys; i++)									/* Keyframes de escalacion */
		{
			soVector3D Scale;
			Scale.X = _AnimationChannel.mScalingKeys[i].mValue.x;
			Scale.Y = _AnimationChannel.mScalingKeys[i].mValue.y;
			Scale.Z = _AnimationChannel.mScalingKeys[i].mValue.z;

			ScalationKeyStruct NewScale;
			NewScale.Scalation = Scale;
			float Time = static_cast<float>(_AnimationChannel.mScalingKeys[i].mTime);
			NewScale.KeyTime = Time;
			m_ScaleKey.push_back(NewScale);
		}

		return EC_OK;
	}

	void soKeyFrame::OnShutDown()
	{
	}
}