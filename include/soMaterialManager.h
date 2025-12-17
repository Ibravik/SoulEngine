/************************************************************************/
/**
* @LC	  12/6/2016
* @file   soMaterialManager.h
* @Author Aldo Muñoz & (lehi.munoz@gmail.com)
* @date   12/6/2016
* @brief  Declaration of class soMaterialManager. Este se encarga de la
gestion de materiales para cada mesh, de modo que no se repitan
pero sea posible la creacion de nuevos materiales, de modo que
mismos modelos puedan verse distintos.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para compilar                      */
/************************************************************************/
#include "SoulEngineDataStruct.h"
#include "soMaterial.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soMaterialManager                            */
	/************************************************************************/
	class soMaterialManager : public soModule<soMaterialManager>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soMaterialManager();									/*!< Constructor Standard */
		~soMaterialManager();									/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:		
		uint32 m_CurrentID;										/*!< ID de asignacion actual */

	public:
		std::map<uint32, soMaterial*> m_MaterialMap;			/*!< Mapa de identificacion de Materiales */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();										/*!< Inicializacion del material manager */
		virtual void OnShutDown()override;						/*!< Finaliza la ejeucion del manager */
		soMaterial* CheckByName(soString& _MaterialName);		/*!< Verifica si el recurso que se pidio ya existe o no */
		INLINE void GrowID() { m_CurrentID++; }					/*!< Modifica el id para asignarlo a un nuevo recurso */

	public:
		uint32 CreateNewMaterial(MaterialData& _MaterialData);	/*!< Crea un nuevo material, si no existe retorna uno standard */
		soMaterial* GetMaterialByID(uint32 _ID);				/*!< Retorna un material segun el ID asignado */
		void DestroyMaterialByID(uint32 _ID);					/*!< Libera los recursos de un material */
	};
}

