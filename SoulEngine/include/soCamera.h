/********************************************************************/
/**
* LC	  11/30/2016		
* @file   soCamera.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/25/2015
* @brief  Implementacion de la clase base soCamera
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulEngineDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soCamera									    */
	/************************************************************************/
	class soCamera : public soStandardClass<const CameraData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soCamera();											/*!< Constructor Standard */
		virtual ~soCamera();								/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	protected:				
		soVector3D m_Velocity;								/*!< Indica la velocidad(y direccion de manera implicita) de la camara */
		soMatrix3x3 m_Rotation;								/*!< indica la rotacion de la camara */
		soVector3D m_LookAt;								/*!< Indica el punto objetivo de la camara */
		soDegrees m_Yaw;									/*!< Yaw Angle */
		soDegrees m_Pitch;									/*!< Pitch Angle */
		float m_AspectRatio;								/*!< Indica la configuracion de resolucion del width con respecto al heigth de resolucion de la pantalla */
		bool m_IsDirty;										/*!< Indica si la camara necesita hacer update si se a movido */

	public:
		SET_AND_GET(soMatrix4x4, ProjectionMatrix)			/*!< Matriz de proyecion */
		SET_AND_GET(soMatrix4x4, ViewMatrix)				/*!< Matriz de Vista */
		soFrustrum m_Frustum;								/*!< Frustum perteneciente a la camara */
		soViewPort m_CameraViewPort;						/*!< Viewport ligado a la camara */
		soDegrees m_Fov;									/*!< Field of view */
		float m_Near;										/*!< Indica el punto mas cercano que persive la camara */
		float m_Far;										/*!< Indica el punto mas lejano que persive la camara */
		soDegrees m_MaxPitch;								/*!< Angulo maximo para pich */
		soVector3D m_CurrentUp;								/*!< Indica la direccion actual del up en cada frame (este es solo informativo, no afecta la construcccion de la matriz de vista) */
		soVector3D m_UpDir;									/*!< Indica la direccion hacia arriba en la camara */
		soVector3D m_FrontDir;								/*!< Indica la direccion de vista de la camra */
		soVector3D m_RightDir;								/*!< Vector perpendicuar a la direccion */		
		soVector3D m_Position;								/*!< Indica la pocision de la camara en coordenadas de Mundo */		
		soVector3D m_MaxVelocity;							/*!< Velocidad maxima de la camara (ejex, ejey, ejez)*/
		soVector3D m_MaxRotationVel;						/*!< Velocidad de rotacion maxima (Yaw, Pich, Roll)*/
		bool m_Orthographic;								/*!< Indica si la camara es ortografica */
		bool m_InvertY;										/*!< Indica si la vista sobre Y esta invertida */
		bool m_LockUp;										/*!< Indica si la camara puede cambiar su up o no */
		bool m_EnableUpMovement;							/*!< indica si la camara puede desplazarce sobre el eje Y */
		
		uint32 m_ID;										/*!< ID asignado por el camera manager */
		soString m_Name;									/*!< Nopmbre asignado por el usuario y manager */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	protected:
		void SetCameraAxes();								/*!< Actualiza los vectores de direccion right, lookat y up */
		RESULT OnStartUp(const CameraData&_CameraData);		/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;					/*!< Libera los recursos solicitados por la clase para su iniciacion */
		void ReBuildProyection();							/*!< Construccion de la matriz de proyeccion */
		
	public:
		void IsDirty();										/*!< Indica internamente que se necesita actualisar la camara */
		virtual void Resize();								/*!< Reconstruye la matriz de proyeccion al actualizar el aspectRatio */
		virtual void Update(float _DeltaTime);				/*!< Actualizacion de la camara */
		void SetOrthographic(bool _Orthographic);			/*!< Activa o desactiva el modo ortografico de la camara */
		void MoveForward(float _Scale = 1.0f);				/*!< Movimiento hacia adelante */
		void MoveBackward(float _Scale = 1.0f);				/*!< Movimiento hacia atras */
		void MoveLeft(float _Scale = 1.0f);					/*!< Movimiento hacia izquierda */
		void MoveRight(float _Scale = 1.0f);				/*!< Movimiento hacia derecha */
		void MoveUp(float _Scale = 1.0f);					/*!< Movimiento vertical arriba */
		void MoveDown(float _Scale = 1.0f);					/*!< Movimiento vertical abajo */
		void Yaw(bool _Positive);							/*!< Yaw Rotation */
		void Pitch(bool _Positive);							/*!< Pitch Rotation */
		void Roll(bool _Positive);							/*!< Roll rotation */
		soRay CreateRay(soPoint _ViewportPos);				/*!< Genera un rayo a partir de la posicion xy de la pantalla */
		bool SphereIncideFrustum(const soSphere& _Sphere);	/*!< Verifica si la esfera esta dentro del frustum */
		bool PointIncideFrustum(const soVector3D& _Point);	/*!< Verifica si la esfera esta dentro del frustum */
		bool AABBIncideFrustum(const soAABB& _AABB);		/*!< Verifica si la esfera esta dentro del frustum */
	};
}

