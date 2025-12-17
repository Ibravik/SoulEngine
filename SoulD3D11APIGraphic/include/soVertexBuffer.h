/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soVertexBuffer.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Declaration of soVertexBuffer 
Set up
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once
/************************************************************************************************************************/
/* Inclusión de cabeceras requeridas                                    												*/
/************************************************************************************************************************/
#include <assimp/scene.h>
#include "SoulD3D11APIGraphicDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Forward class declaration                                            */
	/************************************************************************/
	class soBuffer;

	/************************************************************************/
	/* Declaracion de la clase soVertexBuffer                               */
	/************************************************************************/
	class soVertexBuffer : public soStandardClass<const std::vector<aiMesh*>&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soVertexBuffer();														/*!< Constructor Standard */
		~soVertexBuffer();														/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		soBuffer* m_ptrVertexBuffer;											/*!< puntero al Vertex Bufer de DirectX */
		ARRAY(Vertex, sVertex)													/*!< Arreglo que contiene la informacion de los vertices de un mesh*/
		GET_SAFE(soSphere, BoundingSphere)										/*!< Esfera al rededor del mesh */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT CreateMeshVertexInfo(const std::vector<aiMesh*>& MaterialMesh);	/*!< Obtiene la informacion de cada uno de los vertices del mesh (info proporcionada por assimp) */
		RESULT OnStartUp(const std::vector<aiMesh*>& _MaterialMesh);			/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;										/*!< Libera los recursos solicitados por la clase para su iniciacion */	

	public:
		RESULT CreateVertexBuffer();											/*!< Genera el vertex buffer a partir de la informacion contenida, SOLO PARA ASSETS */	
		void ComputeBoundingSphere();											/*!< Retorna una esfera de colicion al rededor del arreglo de vertices */
	};
}