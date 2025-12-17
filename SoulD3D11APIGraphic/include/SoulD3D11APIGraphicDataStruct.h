/********************************************************************
LC			10/14/2016
Created:	9/5/2016
Filename:	API_DataStruct.h
Author:		Lehi Muñoz

Purpose:	Cabecera que incluye las estructuras de datos para la
			inicializacion de componentes abstaidos de la api
*********************************************************************/
#pragma once

#include "soApiDefines.h"

namespace SoulSDK
{
	/************************************************************************/
	/* soSampler                                                            */
	/************************************************************************/
	struct SamplerData											/*!< Informacion para la inicializacion del sampler */
	{
		API_TEXTURE_ADDRESS_MODE D3D11_TEXTURE_ADDRESS_MODE_U;	/*!< Direccion U */
		API_TEXTURE_ADDRESS_MODE D3D11_TEXTURE_ADDRESS_MODE_V;	/*!< Direccion V */
		API_TEXTURE_ADDRESS_MODE D3D11_TEXTURE_ADDRESS_MODE_W;	/*!< Direccion W */
		API_FILTER D3D11_FILTER;								/*!< Filtro para tomar las muestras de la textura */
		API_COMPARISON_FUNC D3D11_COMPARISON_FUNC;				/*!< Comparacion de la muestra */
		float MinLOD;											/*!< Investigar */
		float MaxLOD;											/*!< Investigar */
		float MipLODBias;										/*!< Investigar */
		uint32 MaxAnisotropy;									/*!< Anisotropia maxima */
		bool DefaultSampler;									/*!< Retorna el Sampler default del dxgi Manager */

		SamplerData()
		{
			D3D11_TEXTURE_ADDRESS_MODE_U = API_TEXTURE_ADDRESS_WRAP;
			D3D11_TEXTURE_ADDRESS_MODE_V = API_TEXTURE_ADDRESS_WRAP;
			D3D11_TEXTURE_ADDRESS_MODE_W = API_TEXTURE_ADDRESS_WRAP;
			D3D11_FILTER = API_FILTER_MIN_MAG_MIP_LINEAR;
			D3D11_COMPARISON_FUNC = API_COMPARISON_ALWAYS;
			MinLOD = 0;
			MaxLOD = 0;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			DefaultSampler = true;
		}
	};

	/************************************************************************/
	/* soApiTexture                                                         */
	/************************************************************************/
	struct Texture1D
	{
		uint32 Width;											/*!< Tamaño de la textura */
		uint32 MipLevels;										/*!< Verciones escaladas de la textura hacia abajo, por default 1 */
		uint32 ArraySize;										/*!< Bloques de informacion, por default 1 */
		APIGI_FORMAT DXGI_FORMAT;								/*!< Formato de salida del buffer, por default S8 */
		API_USAGE D3D11_USAGE;									/*!< Uso del bloque de memoria, por default con uso dinamico */
		uint32 D3D11_BIND_FLAG;									/*!< Bandera de anclaje, por default la anclamos al back buffer */
		uint32 CPUAccesFlag;									/*!< Nivel de acceso del CPU, por default 0, ningun acceso */
		uint32 MiscFlags;										/*!< Banderas foraneas, por default 0 */

		Texture1D()
		{
			Width = 16;
			MipLevels = 1;
			ArraySize = 1;
			DXGI_FORMAT = APIGI_FORMAT_R32G32B32A32_FLOAT;
			D3D11_USAGE = API_USAGE_DYNAMIC;
			D3D11_BIND_FLAG = API_BIND_SHADER_RESOURCE;			
			CPUAccesFlag = API_CPU_ACCESS_WRITE;
			MiscFlags = 0;
		}
	};

	struct Texture2D
	{
		APIGI_FORMAT DXGI_FORMAT;								/*!< Formato de salida del buffer, por default S8 */
		API_USAGE D3D11_USAGE;									/*!< Uso del bloque de memoria, por default con uso dinamico */
		uint32 D3D11_BIND_FLAG;									/*!< Bandera de anclaje, por default la anclamos al back buffer */
		soRect TextureSize;										
		uint32 MipLevels;										/*!< Verciones escaladas de la textura hacia abajo, por default 1 */
		uint32 ArraySize;										/*!< Bloques de informacion, por default 1 */
		uint32 SampleDescCount;									/*!< Antialiasing, por default 1 */
		uint32 SampleDescQuality;								/*!< Antialiasing, por default 0*/
		uint32 CPUAccesFlag;									/*!< Nivel de acceso del CPU, por default 0, ningun acceso */
		uint32 MiscFlags;										/*!< Banderas foraneas, por default 0 */
		float ScalarViewPortWidth;								/*!< Indica si el tamaño ingresado para la textura debe reescalarse */
		float ScalarViewPortHeigth;								/*!< Indica si el tamaño ingresado para la textura debe reescalarse */

		Texture2D()
		{
			DXGI_FORMAT = APIGI_FORMAT_D24_UNORM_S8_UINT;
			D3D11_USAGE = API_USAGE_DEFAULT;
			D3D11_BIND_FLAG = API_BIND_SHADER_RESOURCE;
			TextureSize = soRect(0, 0, 16, 16);
			MipLevels = 1;
			ArraySize = 1;
			SampleDescCount = 1;
			SampleDescQuality = 0;
			CPUAccesFlag = 0;
			MiscFlags = 0;
			ScalarViewPortWidth = 1.0f;
			ScalarViewPortHeigth = 1.0f;
		}
	};

	struct TextureData 
	{
		API_TextureType TextureType;							/*!< Define el tipo de textura que se genera como shader resource view en el CB */
		Texture2D T2DData;										/*!< Informacion de inicio para una textura 2D */
		Texture1D T1DData;										/*!< Informacion de inicio para una textura 1D */

		TextureData()
		{
			TextureType = API_Texture2D;
		}
	};

	/************************************************************************/
	/* soStructuredBuffer                                                   */
	/************************************************************************/
	struct StructuredBufferData
	{
		uint32 Elements;				/*!< Numero de elementos */
		uint32 D3D11_BIND_FLAG;			/*!< Banderas de descriocion del buffer */
		bool Dynamic;					/*!< Indica si es dinamico o no */

		StructuredBufferData()
		{
			Elements = 1;
			D3D11_BIND_FLAG = API_BIND_CONSTANT_BUFFER;
			Dynamic = false;
		}
	};

	/************************************************************************/
	/* soBuffer                                                             */
	/************************************************************************/
	struct BufferData
	{
		API_USAGE D3D11_USAGE;									/*!< Uso del bloque de memoria, por default con uso dinamico */
		uint32 D3D11_BIND_FLAG;									/*!< Tipo de buffer, Por default Vertexbufer*/
		uint32 ByteWidth;										/*!< Tamaño en la memoria */
		uint32 CPUAccesFlag;									/*!< Nivel de acceso del CPU, por default 0, ningun acceso */
		API_SUBRESOURCE_DATA D3D11_SUBRESOURCE_DATA;			/*!< Indica si es algun subrecurso, esto mapea sectores de memoria dentro de una locacion */

		BufferData()
		{
			D3D11_USAGE = API_USAGE_DEFAULT;
			D3D11_BIND_FLAG = API_BIND_CONSTANT_BUFFER;
			ByteWidth = 0;
			CPUAccesFlag = 0;
			ZeroMemory(&D3D11_SUBRESOURCE_DATA, sizeof(API_SUBRESOURCE_DATA));
		}
	};

	/************************************************************************/
	/* soConstantBuffer                                                     */
	/************************************************************************/
	struct ConstantBufferData
	{		
		uint32 D3D11_BIND_FLAG;									/*!< Tipo de buffer, Por default Vertexbufer*/
		uint32 ByteWidth;										/*!< Tamaño en la memoria */		
		API_SUBRESOURCE_DATA D3D11_SUBRESOURCE_DATA;			/*!< Indica si es algun subrecurso, esto mapea sectores de memoria dentro de una locacion */
		
		ConstantBufferData()
		{
			D3D11_BIND_FLAG = API_BIND_CONSTANT_BUFFER;
			ByteWidth = 0;
			ZeroMemory(&D3D11_SUBRESOURCE_DATA, sizeof(API_SUBRESOURCE_DATA));
		}
	};

	/************************************************************************/
	/* soDepthStencil														*/
	/************************************************************************/
	struct DepthStencilData
	{
		API_DEPTH_WRITE_MASK D3D11_DEPTH_WRITE_MASK;			/*!< Indica donde se aplica */
		API_COMPARISON_FUNC D3D11_COMPARISON_FUNC;				/*!< Indica el tipo de comparacion, alto, medio o bajo */
		TextureData _Texture2DData;								/*!< Informacion del BackBuffer, ya que queda ligado al back buffer */
		bool DepthEnable;										/*!< Indica si el estado del Depth Stencil es prendido o apagado */
		bool StencilEnable;										/*!< Indica si el Stencil esta prendido o apagado */
		uint8 StencilReadMask;									/*!< Indica la profundidad de bits para la lectura del depth stencil */
		uint8 StencilWriteMask;									/*!< Indica la profundidad de bits para la escritura del depth stencil */
		API_STENCIL_OP FrontFaceStencilFailOp;					/*!< Indica la operacion del Stencil en un triangulo hacia la camara */
		API_STENCIL_OP FrontFaceStencilDepthFailOp;				/*!< Indica la operacion del Stencil en un triangulo hacia la camara */
		API_STENCIL_OP FrontFaceStencilPassOp;					/*!< Indica la operacion del Stencil en un triangulo hacia la camara */
		API_COMPARISON_FUNC FrontFaceStencilFunc;				/*!< Indica la operacion del Stencil en un triangulo hacia la camara */
		API_STENCIL_OP BackFaceStencilFailOp;					/*!< Indica la operacion del Stencil en un triangulo hacia el contrario de la camara */
		API_STENCIL_OP BackFaceStencilDepthFailOp;				/*!< Indica la operacion del Stencil en un triangulo hacia el contrario de la camara */
		API_STENCIL_OP BackFaceStencilPassOp;					/*!< Indica la operacion del Stencil en un triangulo hacia el contrario de la camara */
		API_COMPARISON_FUNC BackFaceStencilFunc;				/*!< Indica la operacion del Stencil en un triangulo hacia el contrario de la camara */

		DepthStencilData()
		{
			D3D11_DEPTH_WRITE_MASK = API_DEPTH_WRITE_MASK_ALL;
			D3D11_COMPARISON_FUNC = API_COMPARISON_LESS;
			DepthEnable = true;
			StencilEnable = true;
			StencilReadMask = API_DEFAULT_STENCIL_READ_MASK;
			StencilWriteMask = API_DEFAULT_STENCIL_WRITE_MASK;
			FrontFaceStencilFailOp = API_STENCIL_OP_KEEP;
			FrontFaceStencilDepthFailOp = API_STENCIL_OP_KEEP;
			FrontFaceStencilPassOp = API_STENCIL_OP_REPLACE;
			FrontFaceStencilFunc = API_COMPARISON_ALWAYS;
			BackFaceStencilFailOp = API_STENCIL_OP_KEEP;
			BackFaceStencilDepthFailOp = API_STENCIL_OP_KEEP;
			BackFaceStencilPassOp = API_STENCIL_OP_REPLACE;
			BackFaceStencilFunc = API_COMPARISON_ALWAYS;
			
		}
	};

	/************************************************************************/
	/* soDeviceSwapChain											        */
	/************************************************************************/
	struct SwapChainData
	{
		APIGI_FORMAT DXGI_FORMAT;								/*!< Formato de salida del buffer, por default 32 bits */
		APIGI_USAGE DXGI_USAGE;									/*!< Uso que se asignara el bufer, por default de salida */
		APIGI_SWAP_EFFECT DXGI_SWAP_EFFECT;						/*!< Indica si se requiere efectos de swap */
		uint32 BufferCount;										/*!< Numero de Buffers, por default 1*/
		soRect ScreenDimentions;								/*!< Dimenciones del area cliente */
		HWND OutputHWND;										/*!< Handle a la ventana de salida */
		uint32 SampleDescCount;									/*!< Antialiasing, por default 1 */
		uint32 SampleDescQuality;								/*!< Antialiasing, por default 0*/
		bool Windowed;											/*!< Output en modo exclusivo o ventana */
		bool vSync;												/*!< indica si se tiene sincronizacion vertical */

		SwapChainData()
		{
			DXGI_FORMAT = APIGI_FORMAT_R8G8B8A8_UNORM;
			DXGI_USAGE = APIGI_USAGE_RENDER_TARGET_OUTPUT;
			DXGI_SWAP_EFFECT = APIGI_SWAP_EFFECT_DISCARD;
			BufferCount = 1;
			ScreenDimentions = soRect(0, 0, 800, 600);
			SampleDescCount = 1;
			SampleDescQuality = 0;
			Windowed = true;
			vSync = true;
		}
	};
		
	/************************************************************************/
	/* soRasterizerState											        */
	/************************************************************************/
	struct RasterizerStateData
	{
		API_FILL_MODE D3D11_FILL_MODE;							/*!< Define el metodo de rellenado de los triangulos */
		API_CULL_MODE D3D11_CULL_MODE;							/*!< Define el metodo de eliminacion de triangulos respecto a su normal y la camara */
		bool FrontCounterClockwise;								/*!< Define el metodo de generacion de triangulos a partir de los puntos ingresados en el buffer */
		int32 DepthBias;										/*!< Valor de profundidad dado a un pixel */
		float DepthBiasClamp;									/*!< Valor maximo de DepthBias */
		float SlopeScaledDepthBias;								/*!< Scalar dado al slope de un pixel */
		bool DepthClipEnable;									/*!< Permite o restringe el clipping en la coordenada Z por hardware */
		bool ScissorEnable;										/*!< Permite el corte rectangular. elimina los pixeles fuera de un triangulo */
		bool MultisampleEnable;									/*!< Permite sampleo cuadrupe */
		bool AntialiasedLineEnable;								/*!< Activa el antialiassing */

		RasterizerStateData()
		{
			D3D11_FILL_MODE = API_FILL_SOLID;
			D3D11_CULL_MODE = API_CULL_NONE;
			FrontCounterClockwise = false;
			DepthBias = 0;
			DepthBiasClamp = 0.0f;
			SlopeScaledDepthBias = 0.0f;
			DepthClipEnable = true;
			ScissorEnable = false;
			MultisampleEnable = true;
			AntialiasedLineEnable = true;
		}
	};

	/************************************************************************/
	/* soRenderTarget										                */
	/************************************************************************/
	struct RenderTargetData
	{
		TextureData _TextureData;							/*!< Informacion de la textura de salida del render target */
		soVector4D _CleanColor;								/*!< Color de limpiesa */
		soString Name;										/*!< Nombre del render Target */

		RenderTargetData()
		{
			_CleanColor = soVector4D(0, 0, 0, 1);
		}
	};
	
	/************************************************************************/
	/* soVertexBuffer								                        */
	/************************************************************************/
	struct Vertex											/*!< Informacion representativa de un solo Vertice */
	{
		soVector4D Position;								/*!< Posicion del vertice en espacio 3D */
		soVector2D Texture;									/*!< Coordenadas de UV */
		soVector3D Normals;									/*!< Norma del Vertice */
		soVector3D BiNormals;								/*!< Binormal del vertice, se puede crear a partir del Cross product de la normal y tangente */
		soVector3D Tangent;									/*!< Tangente del Vertice */
		uint32 BoneIndex[4];								/*!< Arreglo de indices de los huesos que afectan al vertice */
		float BoneWeight[4];								/*!< Peso de los huesos sobre el mesh */
		soString *BoneName[4];								/*!< Nombre del hueso asociado al mesh */
		
		Vertex() 
		{
			for (uint8 i = 0; i < 4; i++)
			{
				BoneName[i] = NULL;
				BoneIndex[i] = 0;
				BoneWeight[i] = 0.0f;
			}			
		}

		~Vertex()
		{
			for (uint8 i = 0; i < 4; i++)
			{				
				SAFE_DELETE(BoneName[i]);
			}
		}
	};

	/************************************************************************/
	/* soShader_                                                            */
	/************************************************************************/
	struct ShaderData
	{
		soString FilePath;									/*!< Ruta de acceso al archivo */
		soString S_Profile;									/*!< Vercion del shader */
		soString S_EntryPoint;								/*!< Nombre de la funcion de entrada del shader */
		uint32 ShaderFlags;

		ShaderData()
		{
			ShaderFlags = 0;
			ShaderFlags |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
			ShaderFlags |= D3DCOMPILE_ENABLE_STRICTNESS;
		}
	};

	/************************************************************************/
	/* soBlendState                                                         */
	/************************************************************************/
	struct BlendData
	{
		bool                           AlphaToCoverageEnable;
		bool                           IndependentBlendEnable;
		API11_RENDER_TARGET_BLEND_DESC RenderTarget[8];

		BlendData()
		{
			AlphaToCoverageEnable = false;
			IndependentBlendEnable = false;

			const API11_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
			{
				FALSE,
				API11_BLEND_ONE, API11_BLEND_ZERO, API11_BLEND_OP_ADD,
				API11_BLEND_ONE, API11_BLEND_ZERO, API11_BLEND_OP_ADD,
				API11_COLOR_WRITE_ENABLE_ALL,
			};
			for (uint8 i = 0; i < 8; ++i)
			{
				RenderTarget[i] = defaultRenderTargetBlendDesc;
			}
		}
	};
	
	/************************************************************************/
	/* soViewPort                                                           */
	/************************************************************************/
	struct ViewPortData
	{
		float Width;										/*!< Ancho del area cliente, NECESARIO ASIGNAR */
		float Height;										/*!< Alto del area cliente, NECESARIO ASIGNAR */
		float MinDepth;										/*!< Minimo en espacio unitario, por default 0 */
		float MaxDepth;										/*!< Maximo en el espacio unitario, por default 1 */
		float TopLeftX;										/*!< Coordenada de la esquina superior izquierda en X, por default 0 para ponerlo en toda el area cliente */
		float TopLeftY;										/*!< Coordenada de la esquina superior izquierda en Y, por default 0 para ponerlo en toda el area cliente */

		ViewPortData()
		{
			Width = 800;
			Height = 600;
			MinDepth = 0.0f;
			MaxDepth = 1.0f;
			TopLeftX = 0;
			TopLeftY = 0;
		}
	};

	/************************************************************************/
	/* soDXGIManager										                */
	/************************************************************************/
	struct InitGraphicInfo
	{
		SwapChainData SCData;								/*!< Informacion para la inicializacion del SwapChain */
		DepthStencilData DSData;							/*!< Informacion para la inicializacion del depth stencil */
		ViewPortData VPData;								/*!< Informacion de inicializacion para el view port */

		InitGraphicInfo(const soRect& _ClientSize, const HWND& _OutputHwnd)
		{
			SCData.ScreenDimentions.m_Width = _ClientSize.m_Width;
			SCData.ScreenDimentions.m_Height = _ClientSize.m_Height;
			SCData.OutputHWND = _OutputHwnd;
			TextureData TD;
			DSData._Texture2DData.T2DData.TextureSize = _ClientSize;
			DSData._Texture2DData.T2DData.D3D11_BIND_FLAG = API_BIND_DEPTH_STENCIL;
			DSData._Texture2DData.T2DData.DXGI_FORMAT = APIGI_FORMAT_D24_UNORM_S8_UINT;
			VPData.Width = static_cast<float>(_ClientSize.m_Width);
			VPData.Height = static_cast<float>(_ClientSize.m_Height);
		}
	};
}