#include "Font.h"

#include "DxLib.h"

Font::Font()
{
}

int Font::makeFont(const char* font_path ,char *font_name, int font_size, int Thick)
{
	// ********** �t�H���g�̃��[�h **********
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
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);

		return-1;
	}
}

int Font::loadDxFont(const char *FontPath,int EdgeSize)
{
	if (deletedFont.size() > 0)
	{
		int dfont = deletedFont[0];
		deletedFont.erase(deletedFont.begin());

		// �c�w�t�H���g�f�[�^�t�@�C����ǂݍ��݁A�t�H���g�n���h����ϐ� FontHandle �ɕۑ����� 
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
