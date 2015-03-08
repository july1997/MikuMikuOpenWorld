#include "Core.h"

//Core.hを参照してください
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
	SetMainWindowText("Project Agg");//ウィンドウモード

	SetMultiThreadFlag(TRUE);// マルチスレッドに適したモードで起動する

	SetAlwaysRunFlag(TRUE);//垂直同期を待たない
	if (DxLib_Init() == -1)return -1;

	SetWindowUserCloseEnableFlag(FALSE);//閉じるボタンを押しても閉じない

	SetMouseDispFlag(TRUE);//マウスカーソルを表示

	SetDrawScreen(DX_SCREEN_BACK);//裏画面に描画設定

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
	while (getline(ifs, str))//一行読み出す
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
	int font = CreateFontToHandle("ＭＳ ゴシック", 15, 4, DX_FONTTYPE_ANTIALIASING_8X8);
	int loadinig = LoadGraph("Picture/load.png");
	int looptime = 0, looptime2 = 0;

	while (ProcessMessage() == 0 && GetASyncLoadNum() != 0)
	{
		//外部デバッカ対策　公開するときにコメントを外す
		//if(IsDebuggerPresent()!=0)return 0;

		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 145);
		DrawRotaGraph2(640, 380, 40, 40, 1.0, DX_PI / 180 * 30 * looptime, loadinig, FALSE);
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		looptime2++;
		if (looptime2 == 6)looptime++, looptime2 = 0;
		if (looptime == 12)looptime = 0;

		DrawStringToHandle(640 - GetDrawFormatStringWidthToHandle(font, "なうローディング") / 2, 440, "なうローディング", GetColor(255, 255, 255), font);

		DrawFormatStringToHandle(640 - GetDrawFormatStringWidthToHandle(font, "2/2") / 2, 460, -1, font, "%d/3", GetASyncLoadNum());

		Fps->displayFps(1280 - 20, 0);

		Fps->controlFps();
	}
}
