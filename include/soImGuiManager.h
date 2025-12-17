/************************************************************************/
/**
* @LC	  11/23/2016
* @file   soImGuiManager.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/23/2016
* @brief  Declaration of class soImGuiManager. Esta clase gestiona la 
		  funcionalidad de la interfas de IMGUI. Adapta toda la informacion
		  a nuestro pipeline para su render e inputs
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para compilar                      */
/************************************************************************/
#include <imgui.h>
#include <imgui_internal.h>
#include "SoulEngineDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soEditorGUIManager                           */
	/************************************************************************/
	class soImGuiManager : public soModule<soImGuiManager, const ImGuiData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soImGuiManager();																/*!< Constructor Standard */
		~soImGuiManager();																/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		HWND m_hwnd;																		/*!< Identificador local a la ventana */
		ID3D11ShaderResourceView* m_pFontTextureView;										/*!< textura fuente */

	public:
		int32 m_VertexBufferSize;															/*!< Numero de vertices. Ya que conforme se agreguen interfaces se incrementa el numero de vertices, no se usara un pool por ahora */
		int32 m_IndexBufferSize;															/*!< Numero de indices. Ya que conforme se agreguen interfaces se incrementa el numero de indices, no se usara un pool por ahora */
		soConstantBuffer m_VertexBuffer;													/*!< Buffer de vertices. Por ahora se construye uno al ser variable y diferente a la construccion standard */
		soConstantBuffer m_IndexBuffer;														/*!< Buffer de indices */
		soMatrix4x4 m_OtographicProyection;													/*!< Proyeccion ortografica para el render de las interfaces */
		soViewPort m_GUIViewPort;															/*!< Viewport relacionado con la interfaz */
		soBlendState m_BlendState;															/*!< Estado de blend para la GUI */
		ImDrawData* m_DrawData;																/*!< Informacion para render */
		
		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const ImGuiData& _EditorGUIData);								/*!< Inicializacion */
		virtual void OnShutDown()override;													/*!< Libera los recursos solicitados */
		void CreateFontsTexture();															/*!< Genera la textura para la fuente */

	public:
		void Update();																		/*!< Actualizacion */
		void Render();																		/*!< Asigna todos los recursos para su render */
		IMGUI_API LRESULT InputHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	/*!< Despacha las entradas que involucren la gui */
	};
}