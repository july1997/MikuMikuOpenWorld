#pragma once

#include <vector>
#include <string>

class Font
{
	const char* font_path = "font/Koruri-20141224/Koruri-Light.ttf"; // 読み込むフォントファイルのパス
	const char* font_name = "Koruri Light"; // 読み込むフォントの名前
	const int font_size = 18;

	std::vector<int> FontHandle;
	int FontNun = -1;
	std::vector<int> deletedFont;

public:

	Font();
	~Font();

	int makeFont(int font_size = 18, int Thick = 3);

	void drawString(int x, int y, std::string string, int FontHandel_, bool Center = 0, int Color = -1);

	int getFont(int FontHandel_);

	void deleteFont(int FontHandel_);
};

