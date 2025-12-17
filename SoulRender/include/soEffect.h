/************************************************************************/
/**
* @LC	  10/10/2016
* @file   soEffect.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)     
* @date   6/29/2016
* @brief  Declaration of soEffect. Un efecto contiene tecnicas y pases para
		  el render final. 
* @bug	  No know Bugs Segun dice el inge
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras para compilacion                              */
/************************************************************************/
#include "SoulRenderDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* forward class declaration                                            */
	/************************************************************************/
	class soTechnique;
	
	/************************************************************************/
	/* Declaracion de la clase soEffect							            */
	/************************************************************************/
	class soEffect : public soStandardClass<const EffectData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soEffect();													/*!< Constructor Standar */
		~soEffect();												/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		std::map<soString, uint8> m_TechniqueNameMap;				/*!< Diccionario de render Pass por nombre */
		std::vector<soTechnique*> m_TechniqueList;					/*!< Lista de render pass de la tecnica */
		uint8 m_TechniqueID;										/*!< ID para los render pass */

	public:
		soString m_Name;											/*!< Nombre de la tecnica */
		uint8 m_ID;													/*!< ID de la tecnica  */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const EffectData& _EffectData);			/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;							/*!< Libera los recursos solicitados por la clase para su iniciacion */

	public:
		RESULT AddTechnique(const TechniqueData& _TechniqueData);	/*!< Añade un render Target al pase */
		soTechnique* GetTechnique(soString _TechniqueName);			/*!< Toma un render target segun el nombre */
		soTechnique* GetTechnique(uint8 _TechniqueID);				/*!< Toma un render target segun el ID */

		void Resize();												/*!< Aplica el cambio de tamaño de area cliente */
		void Render();												/*!< setea los render target, rasterizer state, viewports etc propios del passe */
	};
}
