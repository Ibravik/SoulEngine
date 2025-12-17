/************************************************************************/
/**
* @LC	  10/10/2016
* @file   soTechnique.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/29/2016
* @brief  Declaration of soTechnique. 

* @bug	  No know Bugs Segun dice el inge
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras para compilacion                              */
/************************************************************************/
#include "SoulRenderDataStruct.h "

namespace SoulSDK
{
	/************************************************************************/
	/* Forward class declaration                                            */
	/************************************************************************/
	class soRenderPass;
		
	/************************************************************************/
	/* Declaracion de la clase soTechnique						            */
	/************************************************************************/
	class soTechnique : public soStandardClass<const TechniqueData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soTechnique();
		~soTechnique();

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		std::map<soString, uint8> m_RenderPassNameMap;			/*!< Diccionario de render Pass por nombre */
		std::vector<soRenderPass*> m_RenderPassList;			/*!< Lista de render pass de la tecnica */
		uint8 m_RenderPassID;									/*!< ID para los render pass */

	public:
		soString m_Name;										/*!< Nombre de la tecnica */
		uint8 m_ID;												/*!< ID de la tecnica  */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const TechniqueData& _TechniqueData);	/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;						/*!< Libera los recursos solicitados por la clase para su iniciacion */
	public:
		RESULT AddPass(const RenderPassData& _RenderPassData);	/*!< Añade un render Target al pase */
		soRenderPass* GetPass(soString _PassName);				/*!< Toma un render target segun el nombre */
		soRenderPass* GetPass(uint8 _PassID);					/*!< Toma un render target segun el ID */

		void Resize();											/*!< Aplica el cambio de tamaño de area cliente */
		void Render();											/*!< setea los render target, rasterizer state, viewports etc propios del passe */
	};
}
