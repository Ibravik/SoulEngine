/************************************************************************/
/**
* @LC	  10/19/2016
* @file   soAnimationDataManager.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/19/2016
* @brief  Declaration of class soAnimationDataManager. Este Manager esta 
		  encargado de cargar toda la informacion de esqueletos y animaciones
		  involuchadas en el pipeline de animacion. Su proposito es gestionar 
		  la informacion que alimentara el preproceso de calculo de matrices
		  en el compute shader de animacion para optimizar la animacion por
		  sorftware.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Definicion de constantes de la clase									*/
/************************************************************************/
#define BytesNumber 33554432

/************************************************************************/
/* Inclucion de cabeceras necesarias para compilar                      */
/************************************************************************/
#include "Soul_Graphics.h"
#include "soSkeletalModel.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soAnimationDataManager				        */
	/************************************************************************/
	class soAnimationDataManager : public soModule<soAnimationDataManager>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soAnimationDataManager();										/*!< Constructor Standard */
		~soAnimationDataManager();										/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		uint8* m_Data;  												/*!< Puntero al arreglo de memoria */
		uint8* m_ptrDataIndex;											/*!< Puntero al espacio actual */
		uint32 m_Index;													/*!< Numero de dato ingresado */
		uint32 m_FreeSpace;												/*!< Espacio en bytes disponible */
		uint32 m_TotalSpace;											/*!< Espacio total del buffer */

	public:
		soAPITexture m_GlobalAnimationBuffer;							/*!< Buffer general de todas las matrices que componen esqueletos y animaciones */
		soAPITexture m_OutputBuffer;									/*!< Buffer de salida del Compute shader */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();												/*!< Inicializacion del resource manager */
		virtual void OnShutDown()override;								/*!< Finaliza la ejeucion del manager */
		void PushData(void* _ptrData, uint32 _SizeOf);					/*!< Ingresa informacion en el arreglo */

	public:
		void InsertSkeletalMatrix(soJoint* _RootNode, SkeletalCB& _CB);	/*!< Ingresa la informacion del esqueleto al buffer general */
		void InsertAnimation(soAnimation* _Animation, SkeletalCB& _CB);	/*!< Ingresa una animacion en el buffer gral */
	};
}
