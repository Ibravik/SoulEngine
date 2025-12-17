#include "soFont.h"


namespace SoulSDK
{
	soFont::soFont()
	{
		Connect(std::bind(&soFont::OnStartUp, this, std::placeholders::_1));
		m_numOfCharacters = 0;
		m_numOfRows = 0;
	}


	soFont::~soFont()
	{
	}

	RESULT soFont::OnStartUp(const FontData& _fontData)
	{
		if (!ReadBinFontFileInfo(_fontData.fontBinFilePath))
		{
			return EC_FALSE;
		}

		if (m_vecFontBinInfo.empty())
		{
			return EC_FALSE;
		}

		for (size_t i = 0; i < m_vecFontBinInfo.size(); ++i)
		{
			FontRenderInfo tempInfo;
			tempInfo.binData = m_vecFontBinInfo[i];
			tempInfo.rect.m_Height = FONT_RECT_HEIGHT;
			tempInfo.rect.m_Width = m_vecFontBinInfo[i].longitudeX;
	
			m_vecFontInfo.push_back(tempInfo);
		}

		m_screenSize = _fontData.screenSize;

		return EC_OK;
	}

	void soFont::OnShutDown()
	{
		m_vecFontBinInfo.clear();
		m_vecFontInfo.clear();
	}

	void soFont::DrawFontText(int _posX, int _posY, soString& _text)
	{
		if (_text.size() == 0)
			return;

		m_textBuffer = _text;
		m_position = soVector2D(static_cast<float>(_posX), static_cast<float>(_posY));

		//Transform position to screen space
		m_position.X = m_position.X / m_screenSize.X;
		m_position.X = (m_position.X * 2) - 1;

		m_position.Y = m_position.Y / m_screenSize.Y;
		m_position.Y = (m_position.Y * 2) - 1;
	}

	void soFont::Render(ID3D11Buffer* _dynamicBuffer)
	{
		if (m_textBuffer.size() == 0)
			return;

		if (m_textBuffer.size() > static_cast<size_t>(MAX_CHARACTERS_NUM))
		{
			OutputDebugString(L"There are more characters than the maximum permitted");
			return;
		}
		
		soDXGIManager& AppiManager = soDXGIManager::Instance();

		D3D11_MAPPED_SUBRESOURCE mapResource;
		RESULT hr = AppiManager.m_Device.m_ptrD3DContext->Map(_dynamicBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
		if (FAIL(hr))
			OutputDebugString(L"Failed Mapping Dynamic Buffer");

		// Point to our vertex buffer’s internal data. 
		FontVertex* ptrData = (FontVertex*)mapResource.pData;


		soVector2D nextPosition = m_position;
	
		for (size_t i = 0; i < m_textBuffer.size(); ++i)
		{
			for (size_t j = 0; j < m_vecFontInfo.size(); ++j)
			{
				//Enter
				if (m_textBuffer[i] == '\n')
				{
					m_numOfRows++;
					nextPosition.X = m_position.X;

					float enterSize;
					enterSize = (m_vecFontInfo[j].rect.m_Height) / m_screenSize.Y;
					enterSize = abs((2 * enterSize) - 1);
					enterSize = 1 - enterSize;

					nextPosition.Y = m_position.Y - (enterSize * m_numOfRows);
					break;
				}
				
				//Space
				if (m_textBuffer[i] == ' ')
				{
					FontVertex arrayVertex[6];

					float spaceSizeX = SPACE_SIZE / m_screenSize.X;
					spaceSizeX = abs((2 * spaceSizeX) - 1);
					spaceSizeX = 1 - spaceSizeX;

					float spaceSizeY;
					spaceSizeY = (m_vecFontInfo[j].rect.m_Height * 0.5f) / m_screenSize.Y;
					spaceSizeY = abs((2 * spaceSizeY) - 1);
					spaceSizeY = 1 - spaceSizeY;

					//Vertex
					arrayVertex[0].Position = soVector4D(nextPosition.X + spaceSizeX, nextPosition.Y + spaceSizeY, 0.0f, 1.0f);
					arrayVertex[1].Position = soVector4D(nextPosition.X + spaceSizeX, nextPosition.Y, 0.0f, 1.0f);
					arrayVertex[2].Position = soVector4D(nextPosition.X, nextPosition.Y, 0.0f, 1.0f);

					arrayVertex[3].Position = soVector4D(nextPosition.X, nextPosition.Y, 0.0f, 1.0f);
					arrayVertex[4].Position = soVector4D(nextPosition.X, nextPosition.Y + spaceSizeY, 0.0f, 1.0f);
					arrayVertex[5].Position = soVector4D(nextPosition.X + spaceSizeX, nextPosition.Y + spaceSizeY, 0.0f, 1.0f);

					//Texcoords
					arrayVertex[0].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u1, m_vecFontInfo[j].binData.Tex_v0);
					arrayVertex[1].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u1, m_vecFontInfo[j].binData.Tex_v1);
					arrayVertex[2].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u0, m_vecFontInfo[j].binData.Tex_v1);

					arrayVertex[3].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u0, m_vecFontInfo[j].binData.Tex_v1);
					arrayVertex[4].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u0, m_vecFontInfo[j].binData.Tex_v0);
					arrayVertex[5].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u1, m_vecFontInfo[j].binData.Tex_v0);

					memcpy(ptrData, &arrayVertex, sizeof(arrayVertex));

					ptrData += 6;
					nextPosition.X = nextPosition.X + spaceSizeX;
					break;
				}


				//Charcters
				if (m_textBuffer[i] == m_vecFontInfo[j].binData.Character)
				{
					FontVertex arrayVertex[6];

					// X and Y longitude of each character
					float letterSizeX;
					letterSizeX = static_cast<float>(m_vecFontInfo[j].binData.longitudeX) / m_screenSize.X;
					letterSizeX = abs((2 * letterSizeX) - 1);
					letterSizeX = 1 - letterSizeX;

					float letterSizeY;
					letterSizeY = static_cast<float>(m_vecFontInfo[j].binData.longitudeY) / m_screenSize.Y;
					letterSizeY = abs((2 * letterSizeY) - 1);
					letterSizeY = 1 - letterSizeY;

					//Vertex
					arrayVertex[0].Position = soVector4D(nextPosition.X + letterSizeX, nextPosition.Y + letterSizeY, 0.0f, 1.0f);
					arrayVertex[1].Position = soVector4D(nextPosition.X + letterSizeX, nextPosition.Y, 0.0f, 1.0f);
					arrayVertex[2].Position = soVector4D(nextPosition.X, nextPosition.Y, 0.0f, 1.0f);

					arrayVertex[3].Position = soVector4D(nextPosition.X, nextPosition.Y, 0.0f, 1.0f);
					arrayVertex[4].Position = soVector4D(nextPosition.X, nextPosition.Y + letterSizeY, 0.0f, 1.0f);
					arrayVertex[5].Position = soVector4D(nextPosition.X + letterSizeX, nextPosition.Y + letterSizeY, 0.0f, 1.0f);

					//Texcoords
					arrayVertex[0].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u1, m_vecFontInfo[j].binData.Tex_v0);
					arrayVertex[1].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u1, m_vecFontInfo[j].binData.Tex_v1);
					arrayVertex[2].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u0, m_vecFontInfo[j].binData.Tex_v1);

					arrayVertex[3].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u0, m_vecFontInfo[j].binData.Tex_v1);
					arrayVertex[4].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u0, m_vecFontInfo[j].binData.Tex_v0);
					arrayVertex[5].Texture = soVector2D(m_vecFontInfo[j].binData.Tex_u1, m_vecFontInfo[j].binData.Tex_v0);

					memcpy(ptrData, &arrayVertex, sizeof(arrayVertex));
					
					ptrData += 6;
					nextPosition.X = nextPosition.X + letterSizeX;
					m_numOfCharacters++;

					break;
				}
			}
		}

		AppiManager.m_Device.m_ptrD3DContext->Unmap(_dynamicBuffer, 0);

		uint32 vertexCount = 6 * m_textBuffer.size();;
		int32 startVertex = 0;
		
		AppiManager.Draw(vertexCount, startVertex);
	
		m_numOfRows = 0;
		m_numOfCharacters = 0;
	}

	void soFont::SetScreenSize(float _width, float _height)
	{
		m_screenSize.X = _width;
		m_screenSize.Y = _height;
	}

	bool soFont::ReadBinFontFileInfo(soString _path)
	{
		int numOfFonts;

		// Obtain file to read
		std::fstream BinaryFile(_path, std::ios::in | std::ios::binary); // input operations / binary mode

		if (BinaryFile.is_open())
		{
			// Obtain num of characters
			BinaryFile.read((char*)&numOfFonts, sizeof(int));

			// Resize vector
			m_vecFontBinInfo.resize(numOfFonts);

			// Read data
			BinaryFile.read((char*)m_vecFontBinInfo.data(), numOfFonts * sizeof(FontInfo));

			// Close File		
			BinaryFile.close();
		}
		else
		{
			OutputDebugString(L"Failed open bin file");
			return false;
		}
		return true;
	}	
}
