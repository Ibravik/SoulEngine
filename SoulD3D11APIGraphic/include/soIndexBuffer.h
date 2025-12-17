/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soIndexBuffer.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Declaration of soIndexBuffer.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclusión de cabeceras requeridas                                    */
/************************************************************************/
#include <assimp/scene.h>
#include "SoulD3D11APIGraphicDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Forward class declaration                                            */
	/************************************************************************/
	class soBuffer;

	/************************************************************************/
	/* Declaracion de la clase soIndexBuffer                                */
	/************************************************************************/
	class soIndexBuffer : public soStandardClass<const std::vector<aiMesh*>&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soIndexBuffer();														/*!< Constructor Standard */
		~soIndexBuffer();														/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		soBuffer* m_ptrIndexBuffer;												/*!< Constant buffer de indices */
		ARRAY(uint32, Index)													/*!< Arreglo de enteros con los indices */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT CreateMeshIndexInfo(const std::vector<aiMesh*>& MaterialMesh);	/*!< Obtiene la informacion de cada uno de los vertices del mesh (info proporcionada por assimp) */
		RESULT OnStartUp(const std::vector<aiMesh*>& _MaterialMesh);			/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;										/*!< Libera los recursos solicitados por la clase para su iniciacion */

	public:
		RESULT CreateIndexBuffer();												/*!< Genera el index buffer a partir de la informacion contenida, SOLO PARA ASSETS */
	};
}