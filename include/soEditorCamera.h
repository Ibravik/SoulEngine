/********************************************************************/
/**
* LC	  11/30/2016
* @file   soEditorCamera.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/30/2016
* @brief  Implementacion de la clase base soEditorCamera. La clase se 
		  encarga de gestionar una camara standar con informacion extra
		  para la camara en modo editor.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soCamera.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Manejo de camaras en modo editor                                     */
	/************************************************************************/
	enum OtographicPosition
	{
		OP_Top = 0,
		OP_Bottom,
		OP_Left,
		OP_Right,
		OP_Front,
		OP_Back,
		OP_Total,
	};

	/************************************************************************/
	/* Declaracion de la clase soEditorCamera							    */
	/************************************************************************/
	class soEditorCamera : public soCamera
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soEditorCamera();								/*!< Constructor Standard */
		virtual ~soEditorCamera();						/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		soVector3D m_OrthographicPos[OP_Total];			/*!< Posiciones de la camara en vista ortografica */
		soVector3D m_OrthographicUp[OP_Total];			/*!< Up en forma ortografica */
		soVector3D m_OrthographicFront[OP_Total];		/*!< Front en forma ortografica */

	public:
		bool m_Active;									/*!< Indica si la camara esta activa(de ser asi, renderea con la info de esta camara) */
		int32 m_ActiveOrthographic;						/*!< Proyeccion ortografica activa */
		soRenderTarget m_CameraRenderTarget;			/*!< Render target local de la camara */
		

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const CameraData&_CameraData);	/*!< Inicializa la clase segun las especificaciones recibidas */

	public:
		virtual void Update(float _DeltaTime)override;	/*!< Actualizacion de la camara */
		virtual void Resize()override;					/*!< Actualiza la informacion de la camara y tender target */
	};
}

