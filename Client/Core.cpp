#include "Core.h"

//Core.h���Q�Ƃ��Ă�������
Core::Core()
{
}


Core::~Core()
{
}

int Core::setupDxLib()
{
	SetGraphMode(readFileSearchToWord_int("Setting.ini", "SCREEN_SIZE_X "),
		readFileSearchToWord_int("Setting.ini", "SCREEN_SIZE_Y "), 32);

	ChangeWindowMode(readFileSearchToWord_int("Setting.ini", "FULL_SCREEN "));
	SetMainWindowText("Project Agg");//�E�B���h�E���[�h

	SetMultiThreadFlag(TRUE);// �}���`�X���b�h�ɓK�������[�h�ŋN������

	SetAlwaysRunFlag(TRUE);//����������҂��Ȃ�
	if (DxLib_Init() == -1)return -1;

	SetWindowUserCloseEnableFlag(FALSE);//����{�^���������Ă����Ȃ�

	SetMouseDispFlag(TRUE);//�}�E�X�J�[�\����\��

	SetDrawScreen(DX_SCREEN_BACK);//����ʂɕ`��ݒ�

	return 0;
}

int Core::readFileSearchToWord_int(char *file, char *word)
{
	std::ifstream ifs(file);
	std::string str;
	if (ifs.fail())
	{
		return -1;
	}
	while (getline(ifs, str))//��s�ǂݏo��
	{
		unsigned int loc = str.find(word, 0);

		if (loc != string::npos)
		{
			string Search = string(word);
			string scw = str.substr(Search.size(), str.size());

			return std::stoi(scw);
		}
	}
	return -1;
}

void Core::loadloop(Fps_ptr& Fps)
{
	int font = CreateFontToHandle("�l�r �S�V�b�N", 15, 4, DX_FONTTYPE_ANTIALIASING_8X8);
	int loadinig = LoadGraph("Picture/load.png");
	int looptime = 0, looptime2 = 0;

	while (ProcessMessage() == 0 && GetASyncLoadNum() != 0)
	{
		//�O���f�o�b�J�΍�@���J����Ƃ��ɃR�����g���O��
		//if(IsDebuggerPresent()!=0)return 0;

		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 145);
		DrawRotaGraph2(640, 380, 40, 40, 1.0, DX_PI / 180 * 30 * looptime, loadinig, FALSE);
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		looptime2++;
		if (looptime2 == 6)looptime++, looptime2 = 0;
		if (looptime == 12)looptime = 0;

		DrawStringToHandle(640 - GetDrawFormatStringWidthToHandle(font, "�Ȃ����[�f�B���O") / 2, 440, "�Ȃ����[�f�B���O", GetColor(255, 255, 255), font);

		DrawFormatStringToHandle(640 - GetDrawFormatStringWidthToHandle(font, "2/2") / 2, 460, -1, font, "%d/3", GetASyncLoadNum());

		Fps->displayFps(1280 - 20, 0);

		Fps->controlFps();
	}
}
