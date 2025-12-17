/************************************************************************/
/**
* @LC	  11/8/2016
* @file   soTerrain.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/8/2016
* @brief  Declaration of soTerrain. Esta clase gestiona lo relacionado al
		  manejo y edicion de terrenos. Encapsula la funcionalidad para 
		  generacion de mesh, texturas de altura, etc. necesarios para la 
		  generacion de terrenos.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once
#define UNIT_X_MT 4

/************************************************************************/
/* Inclusión de cabeceras requeridas                                    */
/************************************************************************/
#include "soMesh.h"
#include "soTexture.h"
#include "SoulEngineDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Estructuras de ayuda para el set del terreno	                        */
	/************************************************************************/
	struct HeightMapType
	{
		float x, y, z;
	};

	struct ModelType
	{
		float x, y, z;
	};

	/************************************************************************/
	/* Declaracion de la clase soTerrain									*/
	/************************************************************************/
	class soTerrain : public soModule<soTerrain>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soTerrain();							/*!< Constructor Standard */
		~soTerrain();							/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		soMesh m_TerrainData;					/*!< Guarda la informacion del grid para el terreno */

	public:

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();						/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;		/*!< Libera los recursos solicitados por la clase para su iniciacion */

	public:
		void Render();							/*!< Ingresa la informacion al render manager */
		void SetHeightMap(soString _FilePath);	/*!< Cambia el mapa de altura para modificar el terreno */
	};
}