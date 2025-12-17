/************************************************************************/
/**
* @LC     10/11/2016
* @file   soResource.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/8/2016
* @brief  Declaration of class soResource. Esta clase base ayuda a la gestion
		  de todo aquello que se define como recurso.
		  Recurso: Informacion compartida por mas de un objeto.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulEngineDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Definicion de Enum para clasificar los tipos de recursos             */
	/************************************************************************/
	enum eResourceType
	{
		RT_Default = 0,
		RT_Texture,		
		RT_3DStaticModel,
		RT_3DSkeletalModel,
		RT_Audio,
		RT_TOTAL
	};

	/************************************************************************/
	/* Declaracion de la clase soResource                                   */
	/************************************************************************/
	class soResource
	{
		/************************************************************************/
		/* Declaracion de construcctores y destructor                           */
		/************************************************************************/
	public:
		soResource();															/*!< Constructor Standard */
		virtual ~soResource();													/*!< Destructor Virtual */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		SET_AND_GET(int32, References)											/*!< Numero de referencias asignadas de un mismo recurso */
		SET_AND_GET(uint32, nID)												/*!< ID numerico asignado por el Resource manager */
		SET_AND_GET(soString, szResourceName)									/*!< Nombre del Recurso(asignado por el usuario, no tiene que ver con la ruta) */
		SET_AND_GET(soString, szFileName)										/*!< Nombre del archivo, se toma de la ruta */
		SET_AND_GET(soString, szFilePath)										/*!< Ruta de acceso del recurso */
		SET_AND_GET(uint8, ResourceType)										/*!< Indica el tipo de recurso */

		/************************************************************************/
		/* Funciones de ayuda de la clase                                       */
		/************************************************************************/
	private:
		void CreateFileName(soString _FilePath);								/*!< Toma el nombre del archivo a partir de la ruta */
		void AssignResourceData(const ResourceParameters& _ResourceParameters);	/*!< Asigna los datos que provienen del Resource parameters */

	public:
		virtual RESULT StartUp();												/*!< Inicializacion del recurso */
		virtual void ShutDown();												/*!< Destruccion del recurso */
		virtual RESULT Load(const ResourceParameters& _ResourceParameters);		/*!< Cargar desde Archivo, la ruta esta especificada en la estructura Resource identification */
		virtual RESULT Create(const ResourceParameters& _ResourceParameters);	/*!< Generas un recurso vacio */
	};
}