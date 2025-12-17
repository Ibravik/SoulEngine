/********************************************************************/
/**
* @LC	  10/9/2016
* @file   soJoint.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/22/2016
* @brief  Declaracion de la clase soJoint. Cada joint representa un
		  hueso, con sus respectivos vertices y pesos.
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
	/* Declaracion de la clase soJoint									    */
	/************************************************************************/
	class soJoint : public soNode
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soJoint();										/*!< Constructor Standard */
		~soJoint();										/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables de la clase                                 */
		/************************************************************************/
	public:		
		soMatrix4x4 m_NodeTransformation;				/*!< Matriz de transformacion relativa al nodo */
		soMatrix4x4 m_ParentTransform;					/*!< Matriz con la informacion de acumulado */
		soMatrix4x4 m_JoinOffsetMatrix;					/*!< Matriz de transformacion del espacio de mesh al espacio del hueso */			
		soJoint* m_Parent;								/*!< Referencia al nodo al que estoy emparentados */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase	                    */
		/************************************************************************/
	private:
		RESULT OnStartUp();								/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;				/*!< Libera los recursos solicitados por la clase para su iniciacion */		

	public:
		void SetOffsetMatrix(aiMatrix4x4& _aiMatrix4x4);/*!< Asigna una matriz offset al nodo */
	};
}

