/********************************************************************/
/**
* @LC     10/10/2016
* @file   soAPITexture.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/25/2015
* @brief  Implementacion de la clase soAPITexture. La textura es una
		  abstraccion de informacion de memoria interpretada como
		  colores. Esta abstraccion es de la api grafica, no como
		  recurso.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"

namespace SoulSDK
{
#define UNKNOWN TEXT("UNKNOWN")
#define R32G32B32A32_TYPELESS TEXT("R32G32B32A32_TYPELESS")
#define R32G32B32A32_FLOAT TEXT("R32G32B32A32_FLOAT")
#define R32G32B32A32_UINT TEXT("R32G32B32A32_UINT")
#define R32G32B32A32_SINT TEXT("R32G32B32A32_SINT")
#define R32G32B32_TYPELESS TEXT("R32G32B32_TYPELESS")
#define R32G32B32_FLOAT TEXT("R32G32B32_FLOAT")
#define R32G32B32_UINT TEXT("R32G32B32_UINT")
#define R32G32B32_SINT TEXT("R32G32B32_SINT")
#define R16G16B16A16_TYPELESS TEXT("R16G16B16A16_TYPELESS")
#define R16G16B16A16_FLOAT TEXT("R16G16B16A16_FLOAT")
#define R16G16B16A16_UNORM TEXT("R16G16B16A16_UNORM")
#define R16G16B16A16_UINT TEXT("R16G16B16A16_UINT")
#define R16G16B16A16_SNORM TEXT("R16G16B16A16_SNORM")
#define R16G16B16A16_SINT TEXT("R16G16B16A16_SINT")
#define R32G32_TYPELESS TEXT("R32G32_TYPELESS")
#define R32G32_FLOAT TEXT("R32G32_FLOAT")
#define R32G32_UINT TEXT("R32G32_UINT")
#define R32G32_SINT TEXT("R32G32_SINT")
#define R32G8X24_TYPELESS TEXT("R32G8X24_TYPELESS")
#define D32_FLOAT_S8X24_UINT TEXT("D32_FLOAT_S8X24_UINT")
#define R32_FLOAT_X8X24_TYPELESS TEXT("R32_FLOAT_X8X24_TYPELESS")
#define X32_TYPELESS_G8X24_UINT TEXT("X32_TYPELESS_G8X24_UINT")
#define R10G10B10A2_TYPELESS TEXT("R10G10B10A2_TYPELESS")
#define R10G10B10A2_UNORM TEXT("R10G10B10A2_UNORM")
#define R10G10B10A2_UINT TEXT("R10G10B10A2_UINT")
#define R11G11B10_FLOAT TEXT("R11G11B10_FLOAT")
#define R8G8B8A8_TYPELESS TEXT("R8G8B8A8_TYPELESS")
#define R8G8B8A8_UNORM TEXT("R8G8B8A8_UNORM")
#define R8G8B8A8_UNORM_SRGB TEXT("R8G8B8A8_UNORM_SRGB")
#define R8G8B8A8_UINT TEXT("R8G8B8A8_UINT")
#define R8G8B8A8_SNORM TEXT("R8G8B8A8_SNORM")
#define R8G8B8A8_SINT TEXT("R8G8B8A8_SINT")
#define R16G16_TYPELESS TEXT("R16G16_TYPELESS")
#define R16G16_FLOAT TEXT("R16G16_FLOAT")
#define R16G16_UNORM TEXT("R16G16_UNORM")
#define R16G16_UINT TEXT("R16G16_UINT")
#define R16G16_SNORM TEXT("R16G16_SNORM")
#define R16G16_SINT TEXT("R16G16_SINT")
#define R32_TYPELESS TEXT("R32_TYPELESS")
#define D32_FLOAT TEXT("D32_FLOAT")
#define R32_FLOAT TEXT("R32_FLOAT")
#define R32_UINT TEXT("R32_UINT")
#define R32_SINT TEXT("R32_SINT")
#define R24G8_TYPELESS TEXT("R24G8_TYPELESS")
#define D24_UNORM_S8_UINT TEXT("D24_UNORM_S8_UINT")
#define R24_UNORM_X8_TYPELESS TEXT("R24_UNORM_X8_TYPELESS")
#define X24_TYPELESS_G8_UINT TEXT("X24_TYPELESS_G8_UINT")
#define R8G8_TYPELESS TEXT("R8G8_TYPELESS")
#define R8G8_UNORM TEXT("R8G8_UNORM")
#define R8G8_UINT TEXT("R8G8_UINT")
#define R8G8_SNORM TEXT("R8G8_SNORM")
#define R8G8_SINT TEXT("R8G8_SINT")
#define R16_TYPELESS TEXT("R16_TYPELESS")
#define R16_FLOAT TEXT("R16_FLOAT")
#define D16_UNORM TEXT("D16_UNORM")
#define R16_UNORM TEXT("R16_UNORM")
#define R16_UINT TEXT("R16_UINT")
#define R16_SNORM TEXT("R16_SNORM")
#define R16_SINT TEXT("R16_SINT")
#define R8_TYPELESS TEXT("R8_TYPELESS")
#define R8_UNORM TEXT("R8_UNORM")
#define R8_UINT TEXT("R8_UINT")
#define R8_SNORM TEXT("R8_SNORM")
#define R8_SINT TEXT("R8_SINT")
#define A8_UNORM TEXT("A8_UNORM")
#define R1_UNORM TEXT("R1_UNORM")
#define R9G9B9E5_SHAREDEXP TEXT("R9G9B9E5_SHAREDEXP")
#define R8G8_B8G8_UNORM   TEXT("R8G8_B8G8_UNORM")
#define G8R8_G8B8_UNORM   TEXT("G8R8_G8B8_UNORM")
#define BC1_TYPELESS   TEXT("BC1_TYPELESS")
#define BC1_UNORM   TEXT("BC1_UNORM")
#define BC1_UNORM_SRGB   TEXT("BC1_UNORM_SRGB")
#define BC2_TYPELESS   TEXT("BC2_TYPELESS")
#define BC2_UNORM   TEXT("BC2_UNORM")
#define BC2_UNORM_SRGB   TEXT("BC2_UNORM_SRGB")
#define BC3_TYPELESS   TEXT("BC3_TYPELESS")
#define BC3_UNORM   TEXT("BC3_UNORM")
#define BC3_UNORM_SRGB   TEXT("BC3_UNORM_SRGB")
#define BC4_TYPELESS   TEXT("BC4_TYPELESS")
#define BC4_UNORM   TEXT("BC4_UNORM")
#define BC4_SNORM   TEXT("BC4_SNORM")
#define BC5_TYPELESS   TEXT("BC5_TYPELESS")
#define BC5_UNORM   TEXT("BC5_UNORM")
#define BC5_SNORM   TEXT("BC5_SNORM")
#define B5G6R5_UNORM   TEXT("DXGI_FORMAT_B5G6R5_UNORM")
#define B5G5R5A1_UNORM   TEXT("B5G5R5A1_UNORM")
#define B8G8R8A8_UNORM   TEXT("B8G8R8A8_UNORM")
#define B8G8R8X8_UNORM   TEXT("B8G8R8X8_UNORM")
#define R10G10B10_XR_BIAS_A2_UNORM   TEXT("R10G10B10_XR_BIAS_A2_UNORM")
#define B8G8R8A8_TYPELESS   TEXT("B8G8R8A8_TYPELESS")
#define B8G8R8A8_UNORM_SRGB   TEXT("B8G8R8A8_UNORM_SRGB")
#define B8G8R8X8_TYPELESS   TEXT("B8G8R8X8_TYPELESS")
#define B8G8R8X8_UNORM_SRGB   TEXT("B8G8R8X8_UNORM_SRGB")
#define BC6H_TYPELESS   TEXT("BC6H_TYPELESS")
#define BC6H_UF16   TEXT("BC6H_UF16")
#define BC6H_SF16   TEXT("BC6H_SF16")
#define BC7_TYPELESS   TEXT("BC7_TYPELESS")
#define BC7_UNORM   TEXT("BC7_UNORM")
#define BC7_UNORM_SRGB   TEXT("BC7_UNORM_SRGB")	
	
	/************************************************************************/
	/* Forward Class declaration                                            */
	/************************************************************************/
	class soSampler;

	/************************************************************************/
	/* Declaracion de la clase soAPITexture									*/
	/************************************************************************/
	class soAPITexture : public soStandardClass<>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soAPITexture();															/*!< Constructor Standard */
		~soAPITexture();														/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		ID3D11ShaderResourceView* m_ptrSRTexture;								/*!< Puntero al recurso de textura listo a ingresar como recurso al shader*/
		ID3D11Texture2D* m_ptrTexture2D;										/*!< Puntero al recurso de textura sin tratar como recurso */
		ID3D11Texture1D* m_ptrTexture1D;										/*!< Puntero al recurso de textura sin tratar como recurso */
		soRect m_TextureSize;													/*!< Guarda la informacion del tamaño de la textura para futuras referencias */
		soString m_Format;														/*!< Formato de la imagen */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();														/*!< inicializacion de la clase */
		virtual void OnShutDown() override;										/*!< Libera los recursos solicitados por la clase para su iniciacion */
		void AssignFormat(uint32 _Format);										/*!< Asigna el formato al string de formato */

	public:
		RESULT Load(const soString& _FilePath);									/*!< Carga desde una ruta especifica */
		RESULT Create(const TextureData& _TextureData);							/*!< Crea una nueva textura segun las especificaciones dadas */
		RESULT Lock1D(uint32& _depthPitch, void ** _Data, uint32& _rowPitch);	/*!< Funcion para obterner acceso a los datos de un recurso situado en el GPU y negarle el acceso a este */
		RESULT Unlock1D();														/*!< Funcion para invalidar el acceso al recurso y habilitarselo a la GPU */
		RESULT Lock2D(uint32& _depthPitch, void ** _pixels, uint32& _rowPitch);	/*!< Funcion para obterner acceso a los datos de un recurso situado en el GPU y negarle el acceso a este */
		RESULT Unlock2D();														/*!< Funcion para invalidar el acceso al recurso y habilitarselo a la GPU */
	};
}