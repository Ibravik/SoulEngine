
/********************************************************************/
/**
* @LC	  10/9/2016
* @file   soKeyFrame.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/22/2016
* @brief  Implementacion de la clase soKeyFrame. Cada soKeyFrame representa
		  las transformaciones de un hueso durante la animacion.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include <assimp/scene.h>
#include "Soul_Core.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de estructuras contenedoras para cada tipo de keyFrame   */
	/************************************************************************/
	struct TranslationKeyStruct
	{
		soVector3D Translation;								/*!< Matriz transformacion de translacion */
		float KeyTime;										/*!< Tiempo en el cual entra este key */
		TranslationKeyStruct()
		{
			KeyTime = 0.0f;
		}
	};

	struct RotationKeyStruct
	{
		soQuaternion Rotation;								/*!< Quaternion de rotacion */
		float KeyTime;										/*!< Tiempo en el cual entra este key */
		RotationKeyStruct() 
		{
			KeyTime = 0.0f;
		}
	};

	struct ScalationKeyStruct
	{
		soVector3D Scalation;								/*!< Matriz transformacion de translacion */
		float KeyTime;										/*!< Tiempo en el cual entra este key */
		ScalationKeyStruct()
		{
			KeyTime = 0.0f;
		}
	};

	/************************************************************************/
	/* Declaracion de la clase soKeyFrame								    */
	/************************************************************************/
	class soKeyFrame : public soStandardClass<const aiNodeAnim&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soKeyFrame();										/*!< Constructor Standard */
		~soKeyFrame();										/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables de la clase                                 */
		/************************************************************************/
	public:
		soString m_Name;									/*!< Nombre del canal de animacion */
		std::vector<TranslationKeyStruct> m_TranslationKey;	/*!< Arreglo contenedor de cada KeyFrame de translacion */
		std::vector<RotationKeyStruct> m_RotationKey;		/*!< Arreglo contenedor de cada KeyFrame de Rotacion */
		std::vector<ScalationKeyStruct> m_ScaleKey;			/*!< Arreglo contenedor de cada KeyFrame de escalacion */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const aiNodeAnim& _aiNodeAnim);	/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;					/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};

}