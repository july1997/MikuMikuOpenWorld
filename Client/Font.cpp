#include "Font.h"

#include "DxLib.h"

Font::Font()
{
	// ********** フォントのロード **********
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}
}

int Font::makeFont(int font_size, int Thick)
{
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

void Font::drawString(int x, int y, std::string string, int FontHandel_,bool Center, int Color)
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
	if (FontHandle.size() != 0)
	{
		// ********** フォントのアンロード **********
		if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
		}
		else {
			MessageBox(NULL, "remove failure", "", MB_OK);
		}
	}
}
