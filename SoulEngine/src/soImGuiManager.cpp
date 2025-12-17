/********************************************************************/
/**
* @LC	  11/23/2016
* @file   soImGuiManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/23/2016
* @brief  Implementacion de la clase soImGuiManager
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soImGuiManager.h"

/************************************************************************/
/* Definicion de la clase soEditorGUIManager						    */
/************************************************************************/
namespace SoulSDK
{
	soImGuiManager::soImGuiManager()
	{
		Connect(std::bind(&soImGuiManager::OnStartUp, this, std::placeholders::_1));

		m_VertexBufferSize = 5000;
		m_IndexBufferSize = 10000;
	}

	soImGuiManager::~soImGuiManager()
	{
	}

	void UpdateRenderInfo(ImDrawData* draw_data)	
	{
		soImGuiManager* _GUI = soImGuiManager::InstancePtr();
		// Create and grow vertex/index buffers if needed
		if (!_GUI->m_VertexBuffer.IsStarted() || _GUI->m_VertexBufferSize < draw_data->TotalVtxCount)
		{
			if (_GUI->m_VertexBuffer.IsStarted())
			{ 
				_GUI->m_VertexBuffer.ShutDown();
			}

			_GUI->m_VertexBufferSize = draw_data->TotalVtxCount + 5000;

			ConstantBufferData CBD;

			CBD.ByteWidth = _GUI->m_VertexBufferSize * sizeof(ImDrawVert);
			CBD.D3D11_BIND_FLAG = API_BIND_VERTEX_BUFFER;

			_GUI->m_VertexBuffer.StartUp(CBD);
		}
		if (!_GUI->m_IndexBuffer.IsStarted() || _GUI->m_IndexBufferSize < draw_data->TotalIdxCount)
		{
			if (_GUI->m_IndexBuffer.IsStarted())
			{
				_GUI->m_IndexBuffer.ShutDown();
			}

			_GUI->m_IndexBufferSize = draw_data->TotalIdxCount + 10000;

			ConstantBufferData CBD;

			CBD.ByteWidth = _GUI->m_IndexBufferSize * sizeof(ImDrawIdx);
			CBD.D3D11_BIND_FLAG = D3D11_BIND_INDEX_BUFFER;

			_GUI->m_IndexBuffer.StartUp(CBD);
		}

		// Copy and convert all vertices into a single contiguous buffer
		ImDrawVert* vtx_dst = (ImDrawVert*)_GUI->m_VertexBuffer.MapDiscard();
		ImDrawIdx* idx_dst = (ImDrawIdx*)_GUI->m_IndexBuffer.MapDiscard();

		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
			vtx_dst += cmd_list->VtxBuffer.Size;
			idx_dst += cmd_list->IdxBuffer.Size;
		}
		_GUI->m_VertexBuffer.Unmap();
		_GUI->m_IndexBuffer.Unmap();

		// Setup orthographic projection matrix
		{
			float L = 0.0f;
			float R = ImGui::GetIO().DisplaySize.x;
			float B = ImGui::GetIO().DisplaySize.y;
			float T = 0.0f;
			
			_GUI->m_OtographicProyection = soMatrix4x4(
				2.0f / (R - L),		0.0f,				0.0f, 0.0f,
				0.0f,				2.0f / (T - B),		0.0f, 0.0f,
				0.0f,				0.0f,				0.5f, 0.0f,
				(R + L) / (L - R),	(T + B) / (B - T),	0.5f, 1.0f
			);
		}
				
		// Setup viewport
		_GUI->m_GUIViewPort.ShutDown();

		ViewPortData VD;
		VD.Height = ImGui::GetIO().DisplaySize.y;
		VD.Width = ImGui::GetIO().DisplaySize.x;

		_GUI->m_GUIViewPort.StartUp(VD);

		_GUI->m_DrawData = draw_data;
	}

	RESULT soImGuiManager::OnStartUp(const ImGuiData& _EditorGUIData)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeyMap[ImGuiKey_Tab] = VK_TAB;          // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
		io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
		io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
		io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
		io.KeyMap[ImGuiKey_Home] = VK_HOME;
		io.KeyMap[ImGuiKey_End] = VK_END;
		io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
		io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
		io.KeyMap[ImGuiKey_A] = 'A';
		io.KeyMap[ImGuiKey_C] = 'C';
		io.KeyMap[ImGuiKey_V] = 'V';
		io.KeyMap[ImGuiKey_X] = 'X';
		io.KeyMap[ImGuiKey_Y] = 'Y';
		io.KeyMap[ImGuiKey_Z] = 'Z';

		io.RenderDrawListsFn = UpdateRenderInfo;  // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
		m_hwnd = _EditorGUIData.hwnd;
		io.ImeWindowHandle = m_hwnd;
		io.DisplaySize.x = _EditorGUIData.Width;
		io.DisplaySize.y = _EditorGUIData.Height;
				
		// Create the blending setup
		{
			BlendData BD;

			BD.AlphaToCoverageEnable = false;
			BD.IndependentBlendEnable = false;
			BD.RenderTarget[0].BlendEnable = true;
			BD.RenderTarget[0].SrcBlend = API11_BLEND_SRC_ALPHA;
			BD.RenderTarget[0].DestBlend = API11_BLEND_INV_SRC_ALPHA;
			BD.RenderTarget[0].BlendOp = API11_BLEND_OP_ADD;
			BD.RenderTarget[0].SrcBlendAlpha = API11_BLEND_INV_SRC_ALPHA;
			BD.RenderTarget[0].DestBlendAlpha = API11_BLEND_ZERO;
			BD.RenderTarget[0].BlendOpAlpha = API11_BLEND_OP_ADD;
			BD.RenderTarget[0].RenderTargetWriteMask = API11_COLOR_WRITE_ENABLE_ALL;
			
			m_BlendState.StartUp(BD);
		}

		CreateFontsTexture();

		//ImGui::GetCurrentContext()->Initialized = false;
		//ImGui::NewFrame();
		
		return EC_OK;
	}

	void soImGuiManager::OnShutDown()
	{
	}

	void soImGuiManager::CreateFontsTexture()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		// Upload texture to graphics system
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;

			ID3D11Texture2D *pTexture = NULL;
			D3D11_SUBRESOURCE_DATA subResource;
			subResource.pSysMem = pixels;
			subResource.SysMemPitch = desc.Width * 4;
			subResource.SysMemSlicePitch = 0;
			AppiManager.m_Device.m_ptrD3DDevice->CreateTexture2D(&desc, &subResource, &pTexture);

			// Create texture view
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			AppiManager.m_Device.m_ptrD3DDevice->CreateShaderResourceView(pTexture, &srvDesc, &m_pFontTextureView);
			pTexture->Release();
		}

		// Store our identifier
		io.Fonts->TexID = (void *)m_pFontTextureView;
	}

	void soImGuiManager::Update()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Setup display size (every frame to accommodate for window resizing)
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

		// Read keyboard modifiers inputs
		io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
		io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
		io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
		io.KeySuper = false;
		// io.KeysDown : filled by WM_KEYDOWN/WM_KEYUP events
		// io.MousePos : filled by WM_MOUSEMOVE events
		// io.MouseDown : filled by WM_*BUTTON* events
		// io.MouseWheel : filled by WM_MOUSEWHEEL events

		// Hide OS mouse cursor if ImGui is drawing it
		if (io.MouseDrawCursor)
			SetCursor(NULL);

		// Start the frame
		//ImGui::NewFrame(); //Ya no sirve aqui
	}

	void soImGuiManager::Render()
	{
		soDXGIManager& AppiManager = soDXGIManager::Instance();
		
		//Prepare the info to render
		ImGui::Render();

		//Set Viewport
		AppiManager.SetViewPort(m_GUIViewPort);

		uint32 stride = sizeof(ImDrawVert);
		uint32 offset = 0;
		APIGI_FORMAT AF = APIGI_FORMAT_R16_UINT;
		API_PRIMITIVE_TOPOLOGY APT = API_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		soVertexBuffer VertexBuffer;
		soIndexBuffer IndexBuffer;

		IndexBuffer.m_ptrIndexBuffer = m_IndexBuffer.m_ptrConstantBuffer;
		VertexBuffer.m_ptrVertexBuffer = m_VertexBuffer.m_ptrConstantBuffer;

		if (IndexBuffer.m_ptrIndexBuffer == NULL || VertexBuffer.m_ptrVertexBuffer == NULL)
		{
			return;
		}

		//Seteamos el VertexBuffer del mesh
		soDXGIManager::Instance().SetVertexBuffer(0, 1, VertexBuffer, stride, offset);

		//Seteamos el index buffer del mesh
		soDXGIManager::Instance().SetIndexBuffer(IndexBuffer, AF, offset);

		//Seteamos la topologia primitiva
		soDXGIManager::Instance().SetPrimitiveTopology(APT);
				
		//Set Render State		
		AppiManager.SetBlendState(m_BlendState, 0, 0xffffffff);
		
		// Render command lists
		int vtx_offset = 0;
		int idx_offset = 0;
		for (int n = 0; n < m_DrawData->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = m_DrawData->CmdLists[n];
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					const D3D11_RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
					AppiManager.m_Device.m_ptrD3DContext->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&pcmd->TextureId);
					AppiManager.m_Device.m_ptrD3DContext->RSSetScissorRects(1, &r);

					//Imprimimos el mesh
					AppiManager.m_Device.m_ptrD3DContext->DrawIndexed(pcmd->ElemCount, idx_offset, vtx_offset);
				}
				idx_offset += pcmd->ElemCount;
			}
			vtx_offset += cmd_list->VtxBuffer.Size;
		}
				
		IndexBuffer.m_ptrIndexBuffer = NULL;
		VertexBuffer.m_ptrVertexBuffer = NULL;
	}

	LRESULT soImGuiManager::InputHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		(void)hWnd;
		ImGuiIO& io = ImGui::GetIO();
		switch (msg)
		{
		case WM_LBUTTONDOWN:
			io.MouseDown[0] = true;
			return true;
		case WM_LBUTTONUP:
			io.MouseDown[0] = false;
			return true;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = true;
			return true;
		case WM_RBUTTONUP:
			io.MouseDown[1] = false;
			return true;
		case WM_MBUTTONDOWN:
			io.MouseDown[2] = true;
			return true;
		case WM_MBUTTONUP:
			io.MouseDown[2] = false;
			return true;
		case WM_MOUSEWHEEL:
			io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
			return true;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			return true;
		case WM_KEYDOWN:
			if (wParam < 256)
				io.KeysDown[wParam] = 1;
			return true;
		case WM_KEYUP:
			if (wParam < 256)
				io.KeysDown[wParam] = 0;
			return true;
		case WM_CHAR:
			// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
			if (wParam > 0 && wParam < 0x10000)
				io.AddInputCharacter((unsigned short)wParam);
			return true;
		}
		return 0;
	}

}