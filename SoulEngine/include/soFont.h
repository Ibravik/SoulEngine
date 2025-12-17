/********************************************************************/
/**
* @file   soProfiler.h
* @author Rubén Rodríguez Serrano (rubn2113@gmail.com)
* @date   2017/08/01
* @brief
*
*
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

#include <Soul_StandardTools.h>
#include <Soul_Graphics.h>
#include "SoulEngineDataStruct.h"

namespace SoulSDK
{

#define MAX_CHARACTERS_NUM 800
#define FONT_RECT_HEIGHT 50
#define SPACE_SIZE 10

	struct FontInfo
	{
		//	Character
		char Character;

		//Longitude
		int longitudeX;
		int longitudeY;

		//	Texcoords
		float Tex_u0;
		float Tex_v0;
		float Tex_u1;
		float Tex_v1;
	};

	struct FontRenderInfo
	{
		FontInfo binData;
		soRect rect;
	};

	class soFont : public soModule<soFont, const FontData&>
	{
	public:
		soFont();
		virtual ~soFont();

	private:
		soString m_textBuffer;
		soVector2D m_position;
		std::vector<FontInfo> m_vecFontBinInfo;
		std::vector<FontRenderInfo> m_vecFontInfo;
		soVector2D m_screenSize;
		int m_numOfCharacters;
		int m_numOfRows;

	private:
		RESULT OnStartUp(const FontData& _fontData);								/*!< Inicializacion */
		virtual void OnShutDown()override;											/*!< Libera los recursos solicitados */
		bool ReadBinFontFileInfo(soString _path);
		
	
	public:
		void DrawFontText(int _posX, int _posY, soString& _text);
		void Render(ID3D11Buffer* _dynamicBuffer);
		void SetScreenSize(float width, float height);
	};
}
