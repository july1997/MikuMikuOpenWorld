#pragma once

#include "Font.h"

class UI : public Font
{
	int Background;
	bool loadBackground = 0;

	int BoxNun = -1;
	std::vector<int> Box;
	std::vector<int> BoxX;
	std::vector<int> BoxY;
	std::vector<int> BoxXLength;
	std::vector<int> BoxYLength;
	std::vector<int> BarHigh;
	std::vector<int> CrickX;
	std::vector<int> CrickY;
	std::vector<int> OperationType;
	std::vector<int> ScrollValue;
	std::vector<int> DeletedBox;

	std::vector<int> InputHandle;
	std::vector<int> CloseInput;
	int NowActiveKey, NowActiveKeyI, Flash = 0, SelectClauseX;

	std::vector<std::string> ButtonString;

public:

	void loadBackgroundImage(const char *Picture);
	void drawBackgroundImage();


	int makeBoxRoundedBar(int XLength, int YLength, int EarlyX = 0, int EarlyY = 0, int Color = -1, int BarColor = 13882323, int Barhigh = 15, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	int makeBoxRounded(int XLength, int YLength, int Color = -1, int EarlyX = 0, int EarlyY = 0, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	//内部で画像データを作らない
	void directDrawBoxRounded(int x, int y, int XLength, int YLength, int BlendRate = 255, int Color = -1, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	void changeBoxRoundedBar(int BoxHandel, int XLength, int YLength, int Color = -1, int BarColo = 13882323, int Barhigh = 15, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	void changeBoxRounded(int BoxHandel, int XLength, int YLength, int Color = -1, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	void drawBox(int x, int y, int BoxHandel, int BlendRate = 255);

	void drawBox(int BoxHandel, int BlendRate = 255);

	//Boxにマウス操作を与える(bar付きのみ)
	void updateBox(int BoxHandel);

	//マウス操作でBoxサイズを変更する
	void updateSize(int BoxHandel, int MinimumValueX=0, int MinimumValueY=0, int Color = -1, int BarColor = 13882323, int Barhigh = 15, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	//マウスでのスクロール操作を与える
	int updateScroll(int BoxHandel, int downlimit=0, int ScrollBarColor = 13882323);

	void setScroll(int BoxHandel, int scroll);

	//箱の類の高さ
	int getBoxHeight(int BoxHandel);

	//箱の類の横幅
	int getBoxWidth(int BoxHandel);

	void deleteBox(int BoxHandel);

	//箱に文字列を描画する
	void drawStringToBox(int BoxHandel, std::string string, int InBoxX, int InBoxY, int FontHandel, bool Center = 0, int Color = -1, int WidthLimit = 10);

	//入力欄
	int makeInputBox(int XLength, int YLength, int color = -1 ,int maxlengh = 36, const char *initString = 0, bool cancancel = 0, bool SingleOnly = 0, bool NumOnly = 0);
	//InputBoxを作ったらどれかひとつは必ず有効にしてください　予期しない動作が起こる場合があります
	void activateInputBox(int BoxHandel);
	void drawInputBoxToBox(int BoxHandel, int InputBoxHandel, int InBoxX, int InBoxY, int &FontHandel, int BlendRate = 255, bool drawString = 1, bool Relative = 0,bool Asterisk = 0);
	bool updateInputBox(int InputBoxHandel, std::string data = "");
	int getNowActiveKey();
	std::string getInputString(int InputBoxHandel);
	void setInputString(int InputBoxHandel, const char *setString);
	int getNowActiveKeyI();

	//ボタン
	int makeButton(const char *drawString, int &FontHandel, int color = -1);
	void drawButtonToBox(int BoxHandel, int ButtonHandel, int InBoxX, int InBoxY, int BlendRate = 255, bool Relative=0);
	bool updateButton(int BoxHandel);
	void drawButton(int ButtonHandel, int X, int Y, int BlendRate = 255);
	std::string getButtonString(int ButtonHandel);

	//便利系
	int clearScreen();
	int show();
	void drawMausePoint();
};

