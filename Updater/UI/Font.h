#pragma once

#include <vector>
#include <string>

class Font
{
	std::vector<int> FontHandle;
	int FontNun = -1;
	std::vector<int> deletedFont;
	int rightside = 0, leftside = 0;

public:

	Font();
	~Font();

	//フォントファイル(ttf)を読み込む
	int makeFont(const char* font_path, char *font_name,int font_size = 18, int Thick = 3);

	//ＤＸフォントデータファイルを読み込む
	int loadDxFont(const char *FontPath, int EdgeSize = 0);


	//フォントで文字を書く
	void drawString(int FontHandel_, std::string string, int x = 0, int y = 0, bool Center = 0, int Color = -1);

	//標準フォントで文字を書く
	void drawString( std::string string, int x = 0, int y = 0, bool Center = 0, int Color = -1);

	//フォントで右端から被らないように文字を書く
	void drawStringRightSide(int FontHandel_, std::string string, bool end = 0, int Color = -1);

	//標準フォントで右端から被らないように文字を書く
	void drawStringRightSide(std::string string, bool end = 0, int Color = -1);

	//フォントで左端から被らないように文字を書く
	void drawStringLeftSide(int FontHandel_, std::string string, bool end = 0, int Color = -1);

	//標準フォント左端から被らないように文字を書く
	void drawStringLeftSide(std::string string, bool end = 0, int Color = -1);


	//フォントハンドルを得る
	int getFont(int FontHandel_);

	//フォントの削除
	void deleteFont(int FontHandel_);
};

