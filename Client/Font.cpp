#include "Font.h"

#include "DxLib.h"

Font::Font()
{
	// ********** �t�H���g�̃��[�h **********
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
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
		// �t�H���g�n���h������
		FontHandle = CreateFontToHandle(font_name, font_size, 3, DX_FONTTYPE_ANTIALIASING_8X8);

		for (size_t i = 0; i < string.size(); i++)
		{
		// �\�t�g�C���[�W����
		const int SoftImage = MakeARGB8ColorSoftImage(font_size, font_size);

		// �t�H���g�n���h�����w�肵�ă\�t�g�C���[�W�ɕ������`��
		BltStringSoftImageToHandle(0, 0, &string[i], SoftImage, -1, FontHandle);

		// �\�t�g�C���[�W����O���t�B�b�N�n���h�����擾
		FontGRHandle.push_back(CreateGraphFromSoftImage(SoftImage));

		// �\�t�g�C���[�W�ƃt�H���g�n���h���͍폜����
		// �O���t�B�b�N�n���h���݂̂��c��
		DeleteSoftImage(SoftImage);
		}

		DeleteFontToHandle(FontHandle);

		bds = 1;
		}


		for (size_t i = 0; i < string.size(); i++)
		{
		// ��̓O���t�B�b�N�n���h�����g���ĕ`�悷�邾��
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
		// ********** �t�H���g�̃A�����[�h **********
		if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
		}
		else {
			MessageBox(NULL, "remove failure", "", MB_OK);
		}
	}
}
