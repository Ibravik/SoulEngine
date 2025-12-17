/********************************************************************
LC			1/18/2016
Created:	2015/11/11
Filename:	Soul_Graphics.h
Author:		Lehi Muñoz

Purpose:	Cabecera que incluye las librerias linkeadas asi como los
			archivos propios de la clase
*********************************************************************/
#pragma once

#include "Soul_Core.h"
/************************************************************************/
/* Inclucion de cabeceras que incluye la libreria                       */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"
#include "soDXGIManager.h"
#include "soBuffer.h"
#include "soFragmentShader.h"
#include "soIndexBuffer.h"
#include "soInputLayout.h"
#include "soRasterizerState.h"
#include "soRenderTarget.h"
#include "soSampler.h"
#include "soAPITexture.h"
#include "soVertexBuffer.h"
#include "soVertexShader.h"
#include "soConstantBuffer.h"
#include "soComputeShader.h"
#include "soBlendState.h"
#include "soStructuredBuffer.h"

/************************************************************************/
/* Inclucion de librerias necesarias para el funcionamiento de el modulo*/
/************************************************************************/
#pragma comment(lib, "SoulCore")

//Librerias de directX
#pragma comment (lib, "d3d11")
#pragma comment (lib, "D3dcompiler")
#pragma comment (lib, "dxerr")
#pragma comment (lib, "dxguid")
#pragma comment (lib, "winmm")
#pragma comment (lib, "comctl32")
#pragma comment (lib, "DXGI")
#pragma comment (lib, "assimp-vc140-mt")

//Librerias de assimp
#if defined(_DEBUG)
#	pragma comment (lib, "d3dx11d")
#	pragma comment (lib, "d3dx9d")
#else
#	pragma comment (lib, "d3dx11")
#	pragma comment (lib, "d3dx9")
#endif
