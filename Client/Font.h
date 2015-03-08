#pragma once

#include <vector>
#include <string>

class Font
{
	const char* font_path = "font/Koruri-20141224/Koruri-Light.ttf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	const char* font_name = "Koruri Light"; // �ǂݍ��ރt�H���g�̖��O
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

