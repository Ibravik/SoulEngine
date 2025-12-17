/************************************************************************/
/**
* @LC	  11/21/2016
* @file   soAssetManager.h
* @Author Aldo Muñoz & (lehi.munoz@gmail.com)
* @date   9/10/2016
* @brief  Declaration of class soAssetManager. Los datos seran
		  encapsulados en forma binaria. Se tendra capacidad de guardar 
		  cualquier dato para su uso ulterior.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para compilar                      */
/************************************************************************/
#include "SoulEngineDataStruct.h"
#include "soActor.h"
#include "soStaticModelComponent.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de enum para facilitar la lectura y escritura			*/
	/************************************************************************/
	enum BasicDataType
	{
		BDT_TCHAR = 0,
		BDT_uint8,
		BDT_uint16,
		BDT_uint32,
		BDT_uint64,
		BDT_int8,
		BDT_int16,
		BDT_int32,
		BDT_int64,
		BDT_Float,
		BDT_soString,
		BDT_TotalTypes
	};

	enum FileStatus
	{
		File_Read = 0x01,
		File_Write = 0x02,
	};

	enum SoulAssetType
	{
		Soul_Actor = 0x01,
		Soul_Component = 0x02,
		Soul_PBRData = 0x04,
	};

	/************************************************************************/
	/* Declaracion de la clase soFileStreamManager                          */
	/************************************************************************/
	class soAssetManager : public soModule<soAssetManager>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soAssetManager();																					/*!< Constructor Standard */
		~soAssetManager();																					/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		FILE* m_ptrFile;																					/*!< Representa el archivo abierto durante la lectura o escritura */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();																					/*!< Inicializacion */
		virtual void OnShutDown()override;																	/*!< Finaliza la ejeucion del manager */
		RESULT OpenFile(soString _FilePath, uint8& _Status);												/*!< Carga un archivo de memoria */
		void CloseFile();																					/*!< Cierra el archivo en uso */
		RESULT SaveToFile(void* _Data, BasicDataType _DataType);											/*!< Guarda informacion en el documento segun el tipo especificado */
		RESULT ReadInFile(void* _DataOutput, BasicDataType _DataType);										/*!< Lee informacion desde el documento*/
		soString CreateFilePath(soString& _FilePath);														/*!< Genera la ruta a la carpeta raiz donde se guardara o cargara el asset */

		//PBRData
		RESULT SavePBRData(PBRData& _PBRData, soString& _FilePath);											/*!< Guarda la informacion de inicio del PBR */
		PBRData* LoadPBRData();																				/*!< Carga la informacion de PBR */

		//Actor
		RESULT SaveActorData(soActor& _Actor, soString& _FilePath);											/*!< Guarda la informacion de un actor */
		soActor* LoadActorData();																			/*!< Carga la informacion de un actor */

		//Componentes
		RESULT SaveComponentData(soComponent& _Component, uint64 _ComponentType, soString& _FilePath);		/*!< Guarda la informacion basica de todo componente */
		soComponent* LoadComponentData();																	/*!< Carga la informacion basica de todo componente */

		//StaticModelComponent
		soStaticModel* LoadStaticModelComponentAsset(soString& _ResourceName);								/*!< Funcionalidad para carga de modelos estaticos */
		RESULT SaveStaticModelComponentAsset(soStaticModelComponent& _StaticModel);							/*!< Funcionalidad para guardar un modelo en asset */

	public:
		void* LoadAssetFromFile(const soString& _FilePath);													/*!< Carga la informacion a partir de un archivo */
		RESULT SaveAssetToFile(soString& _FilePath, void* _Data, uint64 _AssetType, uint64 _MiscFlags = 0);	/*!< Guarda la informacion en un asset */
	};
}
