/********************************************************************/
/**
* @LC	  9/16/2016
* @file   soPlatformMath.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/22/2015
* @brief  Implementacion de soPlatformMath
* @bug	  No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion	            */
/************************************************************************/
#include "soPlatformMath.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de funciones y constantes de soPlatformMath               */
	/************************************************************************/
	const float soPlatformMath::PI						= atan(1.0f) * 4;
	const float soPlatformMath::DOUBLE_PI				= soPlatformMath::PI * 2;
	const float soPlatformMath::TETRA_PI				= soPlatformMath::DOUBLE_PI * 2;
	const float soPlatformMath::HALF_PI					= soPlatformMath::PI * 0.5f;
	const float soPlatformMath::QUARTER_PI				= soPlatformMath::PI * 0.25f;
	const float soPlatformMath::INVERSE_PI				= 1.0f / soPlatformMath::PI;
	const float soPlatformMath::EULER_NUMBER			= 2.71828182845904523536f;
	const float soPlatformMath::LOG2					= 0.68724788563157123299f;
	const float soPlatformMath::EPSILON					= 0.0000000000085418781f;
	const float soPlatformMath::DEG_TO_RAD				= soPlatformMath::PI / 180.0f;
	const float soPlatformMath::RAD_TO_DEG				= 180.0f / soPlatformMath::PI;
	const float soPlatformMath::MAX_FLOAT32				= 3.4f * pow(10.0f, 38.0f);
	const int	soPlatformMath::MAX_INT32				= 0x7fffffff;
	const float soPlatformMath::MATH_PRECISION_DEC		= 0.1f;
	const float soPlatformMath::MATH_PRECISION_CEN		= 0.01f;
	const float soPlatformMath::MATH_PRECISION_MIL		= 0.001f;
	const float soPlatformMath::MATH_PRECISION_MIC		= 0.0001f;
	const float soPlatformMath::MATH_PRECISION_NANO		= 0.00001f;
}