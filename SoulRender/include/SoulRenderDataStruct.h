/********************************************************************
LC			10/14/2016
Created:	9/5/2016
Filename:	SoulRenderDataStruct.h 
Author:		Lehi Muñoz (lehi.munoz@gmail.com)

Purpose:	Cabecera que incluye las estructuras de datos para la
			inicializacion de componentes abstaidos del render manager
*********************************************************************/
#pragma once

#include "Soul_Engine.h"

namespace SoulSDK
{
	/************************************************************************/
	/* soEffect																*/
	/************************************************************************/
	struct EffectData
	{
		soString _Name;								/* Nombre del efecto */

		EffectData()
		{
		};
	};

	/************************************************************************/
	/* soTechnique	                                                        */
	/************************************************************************/
	struct TechniqueData
	{
		soString _Name;								/* Nombre de la tecnica */

		TechniqueData()
		{
		};
	};

	/************************************************************************/
	/* soRenderPass                                                         */
	/************************************************************************/
	struct RenderPassData
	{
		soString _Name;								/* Nombre del pase */
		RasterizerStateData _RasterizerStateInfo;	/* Rasterizador */
		ShaderData _FragmentShaderData;				/* Informacion de Fragment shader */
		ShaderData _VertexShaderData;				/* Informacion de Vertex Shader */
		ShaderData _ComputeShader;					/* Informacion de Compute shader */
		bool SetStencilView;						/* Identifica si el depth stencil actua o no sobre el pase */
		uint32 RenderTargetNumber;					/* Numero de render targets asociados con el pase */

		RenderPassData()
		{
			SetStencilView = true;
		};
	};

	/************************************************************************/
	/* Estructuras de Constant buffer de alimentacion del shader	        */
	/************************************************************************/
	struct FrameBuffer
	{
		soMatrix4x4 mCameraViewProj;
		soMatrix4x4 mCameraView;
		soMatrix4x4 mCameraProj;
		soVector4D viewPos;
		soVector4D mCameraNearFar;
		uint32 ViewportDimensions[4];

		FrameBuffer() {};
	};

	struct  DirectionalLightCB
	{
		soVector4D LigthPosition;
		soVector4D LigthColor;

		DirectionalLightCB()
		{
			LigthPosition = soVector4D(0, 500, 0, 1);
			LigthColor = soVector4D(1, 1, 1, 1);
		};
	};
}