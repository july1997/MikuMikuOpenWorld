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

	//�t�H���g�t�@�C��(ttf)��ǂݍ���
	int makeFont(const char* font_path, char *font_name,int font_size = 18, int Thick = 3);

	//�c�w�t�H���g�f�[�^�t�@�C����ǂݍ���
	int loadDxFont(const char *FontPath, int EdgeSize = 0);


	//�t�H���g�ŕ���������
	void drawString(int FontHandel_, std::string string, int x = 0, int y = 0, bool Center = 0, int Color = -1);

	//�W���t�H���g�ŕ���������
	void drawString( std::string string, int x = 0, int y = 0, bool Center = 0, int Color = -1);

	//�t�H���g�ŉE�[������Ȃ��悤�ɕ���������
	void drawStringRightSide(int FontHandel_, std::string string, bool end = 0, int Color = -1);

	//�W���t�H���g�ŉE�[������Ȃ��悤�ɕ���������
	void drawStringRightSide(std::string string, bool end = 0, int Color = -1);

	//�t�H���g�ō��[������Ȃ��悤�ɕ���������
	void drawStringLeftSide(int FontHandel_, std::string string, bool end = 0, int Color = -1);

	//�W���t�H���g���[������Ȃ��悤�ɕ���������
	void drawStringLeftSide(std::string string, bool end = 0, int Color = -1);


	//�t�H���g�n���h���𓾂�
	int getFont(int FontHandel_);

	//�t�H���g�̍폜
	void deleteFont(int FontHandel_);
};

