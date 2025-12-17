/************************************************************************/
/**
* @LC	  10/9/2016
* @file   soMesh.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Declaration of soStaticMesh. Un mesh es la representacion de una
		  serie de vertices e indices que conforman una geometria. Esta 
		  clase es compatible con modelos estaticos y dinamicos.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclusión de cabeceras requeridas                                    */
/************************************************************************/
#include <assimp/scene.h>
#include "SoulEngineDataStruct.h"
#include "soMaterial.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soMesh									    */
	/************************************************************************/
	class soMesh : public soStandardClass<const MeshData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soMesh();									/*!< Constructor standard */
		~soMesh();									/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		soVertexBuffer m_VertexBuffer;				/*!< Variable que contiene la informacion del vertex buffer del mesh */
		soIndexBuffer m_IndexBuffer;				/*!< Variable que contiene la info del index buffer del mesh */
		soString m_MeshName;						/*!< Nombre del mesh */
		GET_SAFE(soSphere, BoundingSphere)			/*!< Esfera de colision del mesh para culling */
		API_PRIMITIVE_TOPOLOGY m_APT;				/*!< Indica el tipo de topoogia del mesh */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const MeshData& _MeshInfo);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;			/*!< Libera los recursos solicitados por la clase para su iniciacion */

	public:
		void Render(uint32 _MatProp, uint32 _MatID);/*!< Renderea el mesh con las propiedades asignadas del material asignado */
	};
}