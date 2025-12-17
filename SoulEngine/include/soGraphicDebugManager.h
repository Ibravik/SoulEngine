/************************************************************************/
/**
* @LC	  10/10/2016
* @file   soGraphicDebug.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   1/26/2016
* @brief  Declaration of soGraphicDebug. Genera la geometria basica de 
		  cajas y lineas en el render.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclusión de cabeceras requeridas                                    */
/************************************************************************/
#include "Soul_Graphics.h"
#include "soCamera.h"

/************************************************************************/
/* Definicion de constantes de la clase                                 */
/************************************************************************/
#define MAX_Vertex 50000

namespace SoulSDK
{
	/************************************************************************/
	/* Vertice especifico	                                                */
	/************************************************************************/
	struct DebugVertex
	{
		soVector4D Position;
		soColorRGBA Color;
	};

	/************************************************************************/
	/* Declaracion de la clase soGraphicDebug							    */
	/************************************************************************/
	class soGraphicDebugManager : public soModule<soGraphicDebugManager>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soGraphicDebugManager();																																/*!< Constructor Standard */
		~soGraphicDebugManager();																																/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables de la clase                                 */
		/************************************************************************/
	private:
		soConstantBuffer m_DynamicBuffer;																														/*!< Buffer Dinamico. Se crea cada frame y se libera cada frame */
		soConstantBuffer m_StaticBuffer;																														/*!< Bufer Estatico. Incrementa cuando se pide generar una linea de debug que permanesca de manera indefinida en la escena */
		soPool<DebugVertex> m_DynamicPool;																														/*!< Pool de vertices dinamicos */
		soPool<DebugVertex> m_StaticPool;																														/*!< Pool de vertices estaticos */

		/************************************************************************/
		/* Declaracion de funciones de debug grafico			                */
		/************************************************************************/
	private:
		void AddToStaticBuffer(soVector3D& _From, soVector3D& _To, soColorRGBA _Color);																			/*!< Ingresa informacion al buffer estatico */
		void AddToDynamicBuffer(soVector3D& _From, soVector3D& _To, soColorRGBA _Color);																		/*!< Ingresa Informacion al Buffer Dynamico */
		RESULT OnStartUp();																																		/*!< Inicializacion */
		virtual void OnShutDown()override;																														/*!< Finalizacion */

	public:
		void AddDebugPoint(const soVector3D& _Point, bool _IsStatic, float _Scale, soColorRGBA _Color);															/*!< Añade un punto a la escena, el bool indica si es un elemento estatico */
		void AddDebugLine(const soVector3D& _From, const soVector3D& _To, bool _IsStatic, soColorRGBA _Color);													/*!< Añade una linea a la escena, el bool indica si es un elemento estatico */
		void AddDebugAABB(const soAABB& _AABB, bool _IsStatic, soColorRGBA _Color);																				/*!< Añade una AABB a la escena, el bool indica si es un elemento estatico */
		void AddDebugCameraFrustum(soCamera& _Camera, bool _IsStatic);																								/*!< Añade la equivalencia a un frustum */
		void AddDebugSphere(const soSphere& _Sphere, bool _IsStatic, soColorRGBA _Color);																		/*!< Añade una esfera a la escena, el bool indica si es un elemento estatico */
		void AddDebugPositionControl(const soVector3D& _Center, bool _IsStatic, float _Scale, soColorRGBA _Color1, soColorRGBA _Color2, soColorRGBA _Color3);	/*!< Añade un punto de control, el bool indica si es un elemento estatico */
		
		bool IsEmpty();																																			/*!< Verifica si algun buffer tiene informacion para render */
		void Render();																																			/*!< Imprime Ambos Buffers graficos */
	};
}
