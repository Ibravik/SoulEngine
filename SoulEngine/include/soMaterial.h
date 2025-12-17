/************************************************************************/
/**
* @LC	  10/9/2016
* @file   soMaterial.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Declaration of base class soMaterial. El material define el
		  resultado final dentro del pipeline del render.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/********************************************************************/
/* Inclusión de cabeceras requeridas                				*/
/********************************************************************/	
#include "Soul_Graphics.h"
#include "soTexture.h"
#include "soPhysicsManager.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Enum que identifica los tipos de texturas que el modelo puede tener  */
	/************************************************************************/
	enum eTextureType
	{
		Texture_Slot_0 = 0,
		Texture_Slot_1,
		Texture_Slot_2,
		Texture_Slot_3,
		Texture_Slot_4,
		Texture_Slot_5,
		Texture_Slot_6,
		Texture_Slot_7,
		Texture_Slot_8,
		Texture_Slot_9,
		Texture_Slot_10,
		Texture_Slot_11,
		Texture_Slot_12,
		Texture_Slot_TotalSlots,
	};

	/************************************************************************/
	/* Enum que define los diferentes tipos de propiedades disponibles      */
	/************************************************************************/
	enum MaterialProperty
	{
		MAT_PROP_NOPROP = 0x00,
		MAT_PROP_ALBEDO = 0x01,
		MAT_PROP_METALLIC = 0x02,
		MAT_PROP_NORMAL = 0x04,
		MAT_PROP_ROUGHNESS = 0x08,
		MAT_PROP_IRRADIANCE = 0x10,
		MAT_PROP_ENVIRONMENT = 0x20,
	};

	/************************************************************************/
	/* Enum que define los diferentes tipos de Slots disponibles para shader*/
	/************************************************************************/
	enum MaterialPropertySlot
	{
		Shader_Slot_0 = MAT_PROP_ALBEDO,
		Shader_Slot_1 = MAT_PROP_METALLIC,
		Shader_Slot_2 = MAT_PROP_NORMAL,
		Shader_Slot_3 = MAT_PROP_ROUGHNESS,
		Shader_Slot_4 = MAT_PROP_IRRADIANCE,
		Shader_Slot_5 = MAT_PROP_ENVIRONMENT,
		Shader_Slot_6 = 0x40,
		Shader_Slot_8 = 0x80,
		Shader_Slot_7 = 0x100,
		Shader_Slot_9 = 0x200,
		Shader_Slot_10 = 0x400,
		Shader_Slot_11 = 0x800,
		Shader_Slot_12 = 0x1000,
		Shader_ALL_Slot = ((((((((((((Shader_Slot_0 | Shader_Slot_1) | Shader_Slot_2) | Shader_Slot_3) | Shader_Slot_4) | Shader_Slot_5) | Shader_Slot_6) | Shader_Slot_7) | Shader_Slot_8) | Shader_Slot_9) | Shader_Slot_10) | Shader_Slot_11) | Shader_Slot_12),
		Shader_TotalSlot = 0x2000,
	};
			
	/************************************************************************/
	/* Declaración de la clase soMaterial				    	         	*/
	/************************************************************************/
	class soMaterial : public soStandardClass<const MaterialData&>
	{
		/********************************************************************/
		/* Constructores y destructor							    		*/
		/********************************************************************/
	public:
		soMaterial();																				/*!< Constructor standard */
		~soMaterial();																				/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		uint8 m_MaterialFlags;																		/*!< Contiene la informacion de las propiedades con las que cuenta el material */
		soTexture* m_Textures[Texture_Slot_TotalSlots];												/*!< Arreglo que contiene las texturas asociadas al material */
		uint32 m_ID;																				/*!< ID del material */
		soString m_MaterialName;																	/*!< Nombre del material*/
		int32 m_References;																			/*!< Numero de referencias del material */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const MaterialData& _MaterialData);										/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;															/*!< Libera los recursos solicitados por la clase para su iniciacion */
		void LoadTexture(const MaterialData& _MaterialData, uint8 _MatProp, soString& _DefaultPath);/*!< Carga una textura a partir del canal indicado(pensado para proporcionar compatibilidad con diferentes pipeline de carga de modelos)*/
		soString GetChanelInfo(uint8 _MatProp);														/*!< Retorna la terminacion segun la propiedad del material */

	public:
		void AssingTexture(soTexture* _Texture, uint8 _TextureSlot);								/*!< Asigna un recurso valido a la textura emparentada */
		void SetShaderTexture(uint32 _Material);													/*!< Retorna la textura , si no existe un NULL */
		bool CheckMaterialProperty(uint8& _MatProp);												/*!< Retorna positivo si el material cuenta con las propiedades solicitadas */
		void Render(uint32 _MatProp);																/*!< Renderea el mesh */
		void EndRender();																			/*!< desliga las texturas como recurso de shader */
	};
}
