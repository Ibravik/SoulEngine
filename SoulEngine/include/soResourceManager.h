/************************************************************************/
/**
* @LC	  10/10/2016
* @file   soResourceManager.h
* @Author Aldo Muñoz & (lehi.munoz@gmail.com)
* @date   10/14/2015
* @brief  Declaration of class soResourceManager
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para compilar                      */
/************************************************************************/
#include "Soul_Core.h"
#include "soResourceFactory.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soResource                                   */
	/************************************************************************/
	class soResourceManager : public soModule<soResourceManager>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soResourceManager();													/*!< Constructor Standard */
		~soResourceManager();													/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		std::map<uint8, std::vector<soResource*>*> m_ResourceMap;				/*!< Mapa de identificacion de las distintas listas de recursos */
		uint32 m_ResourceID;													/*!< ID del recurso */
		soResourceFactory m_ResourceFactory;									/*!< Fabrica de recursos */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();														/*!< Inicializacion del resource manager */
		virtual void OnShutDown()override;										/*!< Finaliza la ejeucion del manager */
		void CheckValidResourceName(ResourceParameters& _ResourceParameters);	/*!< Verifica si el recurso que se pidio ya existe o no */
		soResource* CheckByFilePath(ResourceParameters& _ResourceParameters);	/*!< Verifica si el recurso que se pidio ya existe o no */
		INLINE void GrowID() { m_ResourceID++; }								/*!< Modifica el id para asignarlo a un nuevo recurso */

	public:
		soResource* Load(ResourceParameters& _ResourceParameters);				/*!< Carga un recurso a partir de la ruta especificada */
		soResource* Create(ResourceParameters& _ResourceParameters);			/*!< Carga un recurso a partir de la ruta especificada */
		void Destroy(soResource* _Resource);									/*!< Destruye el recurso introducido */
	};	
}
