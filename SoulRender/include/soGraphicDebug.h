/************************************************************************/
/**
* @LC	  3/6/2016
* @file   soGraphicDebug.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   1/26/2016
* @brief  Declaration of soGraphicDebug. The porpuse of this class is verify
		  all the bugs in the scene.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclusión de cabeceras requeridas                                    */
/************************************************************************/
#include <d3dx11.h>
#include "Soul_Core.h"

/************************************************************************/
/* Definicion de constantes de la clase                                 */
/************************************************************************/
#define MAX_BUFFER_POINTS 100;

namespace SoulSDK
{
	/************************************************************************/
	/* Forward class delcaration                                            */
	/************************************************************************/
	class soVertexBuffer;
	class soVertexShader;
	class soFragmentShader;
	class soSkeletalMesh;

	/************************************************************************/
	/* Declaracion de la estructura del Constant Buffer                     */
	/************************************************************************/
	struct CBObject
	{
		soMatrix4x4 MatrixWorld = Math::IdentityMatrix4x4();
		soVector4D Color = soVector4D(0, 1, 0, 1);

		CBObject() {}
	};

	/************************************************************************/
	/* Declaracion de la clase soGraphicDebug							    */
	/************************************************************************/
	class soGraphicDebug
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soGraphicDebug();																					/*!< Constructor Standard */
		~soGraphicDebug();																					/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables de la clase                                 */
		/************************************************************************/
	private:
		soVertexBuffer m_DynamicBuffer;																		/*!< Buffer Dinamico. Se crea cada frame y se libera cada frame */
		soVertexBuffer m_StaticBuffer;																		/*!< Bufer Estatico. Incrementa cuando se pide generar una linea de debug que permanesca de manera indefinida en la escena */
		ID3D11Device* m_ID3D11Device;																		/*!< Puntero local al device */
		soVertexShader* m_VertexShader;																		/*!< Puntero local al shader asignado */
		soFragmentShader* m_PixelShader;																	/*!< Puntero local al shader asignado */
		bool m_bStaticBufferUpdated;																		/*!< Indica si se añadio informacion al buffer estatico */
		bool m_bDynamicBufferUpdated;																		/*!< Indica si se añadio informacion al buffer dinamico */
		CBObject m_CBObject;																				/*!< Constant Buffer del Debug Grafico */
		soVertexBuffer* m_StaticBufferBeigin;																/*!< Inicio del Pool de vertices del buffer estatico */
		soVertexBuffer* m_DynamicBufferBeigin;																/*!< Inicio del Pool de vertices del buffer dinamico */
		unsigned int m_StaticBufferVertexCount;																/*!< Contador de vertices ingresados en el buffer estatico */
		unsigned int m_DynamicBufferVertexCount;															/*!< Contador de vertices ingresados en el buffer dinamico */
		
		/************************************************************************/
		/* Declaracion de funciones de debug grafico			                */
		/************************************************************************/
	private:
		void AddToStaticBuffer(soVector3D& _From, soVector3D& _To);											/*!< Ingresa informacion al buffer estatico */
		void AddToDynamicBuffer(soVector3D& _From, soVector3D& _To);										/*!< Ingresa Informacion al Buffer Dynamico */
		void DestroyDynamicBuffer();																		/*!< Destrulle el buffer temporal, esta se llama por frame despues de renderear las lineas dinamicas */
		void DestroyBuffers();																				/*!< Libera los recursos usados por la clase */
		void UpdateDynamicBuffer();																			/*!< Actualiza el buffer */
		void UpdateStaticBuffer();																			/*!< Actualiza el buffer */
	public:
		void AddDebugPoint(float _X, float _Y, float _Z, bool _IsStatic);									/*!< Añade un punto a la escena, el bool indica si es un elemento estatico */
		void AddDebugPoint(const soVector3D& _Point, bool _IsStatic);										/*!< Añade un punto a la escena, el bool indica si es un elemento estatico */
		void AddDebugLine(float _X1, float _Y1, float _Z1, float _X2, float _Y2, float _Z2, bool _IsStatic);/*!< Añade una linea a la escena, el bool indica si es un elemento estatico */
		void AddDebugLine(const soVector3D& _From, const soVector3D& _To, bool _IsStatic);					/*!< Añade una linea a la escena, el bool indica si es un elemento estatico */
		void AddDebugAABB(const soVector3D& _AABBMin, const soVector3D _AABBMax, bool _IsStatic);			/*!< Añade una AABB a la escena, el bool indica si es un elemento estatico */
		void AddDebugAABB(const soAABB& _AABB, bool _IsStatic);												/*!< Añade una AABB a la escena, el bool indica si es un elemento estatico */
		void AddDebugFrustum(soFrustrum& _Frustum, bool _IsStatic);											/*!< Añade la equivalencia a un frustum */
		void AddDebugSkeletalMesh(soSkeletalMesh& _Mesh, bool _IsStatic, float _ScalingAABB = 1.0f);		/*!< Añade un esqueleto de debug */

		void InitGraphicDebug(ID3D11Device* _ID3D11Device, soVertexShader* _VS, soFragmentShader* _FS);		/*!< Inicializa los componentes necesarios para el debuger grafico */
		void UpdateGraphicDebug(float _DeltaTime);															/*!< Actualiza el GraphicDebuguer */
		void RenderGrapicDebug(ID3D11DeviceContext* _pImmediateContext);									/*!< Imprime Ambos Buffers graficos */
		void EndRender();																					/*!< Termina con el estado de render del debugguer */
	};
}
