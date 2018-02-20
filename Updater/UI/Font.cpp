#include "Font.h"

#include "DxLib.h"

Font::Font()
{
}

int Font::makeFont(const char* font_path ,char *font_name, int font_size, int Thick)
{
	// ********** フォントのロード **********
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {

		if (deletedFont.size() > 0)
		{
			int dfont = deletedFont[0];
			deletedFont.erase(deletedFont.begin());

			FontHandle[dfont] = CreateFontToHandle(font_name, font_size, Thick, DX_FONTTYPE_ANTIALIASING_8X8);

			return dfont;
		}
		else
		{
			FontNun++;

			FontHandle.push_back(CreateFontToHandle(font_name, font_size, Thick, DX_FONTTYPE_ANTIALIASING_8X8));

			return FontNun;
		}
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);

		return-1;
	}
}

int Font::loadDxFont(const char *FontPath,int EdgeSize)
{
	if (deletedFont.size() > 0)
	{
		int dfont = deletedFont[0];
		deletedFont.erase(deletedFont.begin());

		// ＤＸフォントデータファイルを読み込み、フォントハンドルを変数 FontHandle に保存する 
		FontHandle[dfont] = LoadFontDataToHandle(FontPath, EdgeSize);

		return dfont;
	}
	else
	{
		FontNun++;

		FontHandle.push_back(LoadFontDataToHandle(FontPath, EdgeSize));

		return FontNun;
	}
}

void Font::drawString(int FontHandel_, std::string string, int x, int y, bool Center, int Color)
{
	if (FontHandle.size() != 0)
	{
		/*
		if (!bds)
		{
		// フォントハンドル生成
		FontHandle = CreateFontToHandle(font_name, font_size, 3, DX_FONTTYPE_ANTIALIASING_8X8);

		for (size_t i = 0; i < string.size(); i++)
		{
		// ソフトイメージ生成
		const int SoftImage = MakeARGB8ColorSoftImage(font_size, font_size);

		// フォントハンドルを指定してソフトイメージに文字列を描画
		BltStringSoftImageToHandle(0, 0, &string[i], SoftImage, -1, FontHandle);

		// ソフトイメージからグラフィックハンドルを取得
		FontGRHandle.push_back(CreateGraphFromSoftImage(SoftImage));

		// ソフトイメージとフォントハンドルは削除して
		// グラフィックハンドルのみが残る
		DeleteSoftImage(SoftImage);
		}

		DeleteFontToHandle(FontHandle);

		bds = 1;
		}


		for (size_t i = 0; i < string.size(); i++)
		{
		// 後はグラフィックハンドルを使って描画するだけ
		DrawGraph(x + font_size*i, y, FontGRHandle[i], TRUE);
		}
		*/

		if (Center)
		{
			DrawStringToHandle(x - GetDrawFormatStringWidthToHandle(FontHandle[FontHandel_], string.c_str()) / 2, y, string.c_str(), Color, FontHandle[FontHandel_]);
		}
		else
		{
			DrawStringToHandle(x, y, string.c_str(), Color, FontHandle[FontHandel_]);
		}
	}
}

void Font::drawString(std::string string, int x, int y, bool Center, int Color)
{
	if (Center)
	{
		int StrLen = strlen(string.c_str());
		DrawString( x - GetDrawStringWidth(string.c_str(),StrLen) / 2, y, string.c_str(), Color);
	}
	else
	{
		DrawString(x, y, string.c_str(), Color);
	}
}

void Font::drawStringRightSide(int FontHandel_, std::string string,bool end, int Color)
{
	int FontSize;
	GetFontStateToHandle(NULL, &FontSize, NULL, FontHandel_);

	DrawStringToHandle( 0, FontSize*rightside, string.c_str(), Color, FontHandle[FontHandel_]);

	rightside++;
	if (end)rightside = 0;
}

void Font::drawStringRightSide(std::string string, bool end, int Color)
{
	DrawString( 0, 15 * rightside, string.c_str(), Color);

	rightside++;
	if (end)rightside = 0;
}

void Font::drawStringLeftSide(int FontHandel_, std::string string, bool end, int Color)
{
	int Sx;
	GetScreenState(&Sx, NULL, NULL);

	int FontSize;
	GetFontStateToHandle(NULL, &FontSize, NULL, FontHandel_);

	DrawStringToHandle(Sx - GetDrawFormatStringWidthToHandle(FontHandle[FontHandel_], string.c_str()), FontSize*leftside, string.c_str(), Color, FontHandle[FontHandel_]);

	leftside++;
	if (end)leftside = 0;
}

void Font::drawStringLeftSide(std::string string, bool end, int Color)
{
	int Sx;
	GetScreenState(&Sx, NULL, NULL);

	int StrLen = strlen(string.c_str());
	DrawString(Sx - GetDrawStringWidth(string.c_str(), StrLen), 15 * leftside, string.c_str(), Color);

	leftside++;
	if (end)leftside = 0;
}


int Font::getFont(int FontHandel_)
{
	return FontHandle[FontHandel_];
}

void Font::deleteFont(int FontHandel_)
{
	DeleteFontToHandle(FontHandel_);

	deletedFont.push_back(FontHandel_);

}

Font::~Font()
{
}
