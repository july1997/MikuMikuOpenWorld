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
	int NowActiveKey, Flash = 0, SelectClauseX;

	std::vector<std::string> ButtonString;
public:

	void drawBackground(char *Picture);


	int makeBoxRoundedBar(int XLength, int YLength, int EarlyX = 0, int EarlyY = 0, int Color = -1, int BarColor = 13882323, int Barhigh = 15, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	int makeBoxRounded(int XLength, int YLength, int Color = -1, int EarlyX = 0, int EarlyY = 0, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	//�����ŉ摜�f�[�^�����Ȃ�
	void directDrawBoxRounded(int x, int y, int XLength, int YLength, int BlendRate = 255, int Color = -1, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	void changeBoxRoundedBar(int BoxHandel, int XLength, int YLength, int Color = -1, int BarColo = 13882323, int Barhigh = 15, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	void changeBoxRounded(int BoxHandel, int XLength, int YLength, int Color = -1, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	void drawBox(int x, int y, int BoxHandel, int BlendRate = 255);

	void drawBox(int BoxHandel, int BlendRate = 255);

	//Box�Ƀ}�E�X�����^����(bar�t���̂�)
	void updateBox(int BoxHandel);

	//�}�E�X�����Box�T�C�Y��ύX����
	void updateSize(int BoxHandel, int MinimumValueX=0, int MinimumValueY=0, int Color = -1, int BarColor = 13882323, int Barhigh = 15, int EdgeSize = 8, int AntiAliasing = 2, int Margin = 3);

	//�}�E�X�ł̃X�N���[�������^����
	int updateScroll(int BoxHandel, int downlimit=0, int ScrollBarColor = 13882323);

	void setScroll(int BoxHandel, int scroll);

	//���̗ނ̍���
	int getBoxHeight(int BoxHandel);

	//���̗ނ̉���
	int getBoxWidth(int BoxHandel);

	void deleteBox(int BoxHandel);

	//���ɕ������`�悷��
	void drawStringToBox(int BoxHandel, std::string string, int InBoxX, int InBoxY, int FontHandel, bool Center = 0, int Color = -1, int WidthLimit = 10);

	//���͗�
	int makeInputBox(int XLength, int YLength, int maxlengh = 36, char *initString = "", bool cancancel = 0);
	//InputBox���������ǂꂩ�ЂƂ͕K���L���ɂ��Ă��������@�\�����Ȃ����삪�N����ꍇ������܂�
	void activateInputBox(int BoxHandel);
	void drawInputBoxToBox(int BoxHandel, int InputBoxHandel, int InBoxX, int InBoxY, int &FontHandel, int BlendRate = 255, bool drawString = 1, bool Relative = 0);
	bool updateInputBox(int InputBoxHandel, std::string &data);
	int getNowActiveKey();
	std::string getInputString(int InputBoxHandel);
	void setInputString(int InputBoxHandel, char *setString);
	
	//�{�^��
	int makeButton(const char *drawString, int &FontHandel);
	void drawButtonToBox(int BoxHandel, int ButtonHandel, int InBoxX, int InBoxY, int BlendRate = 255, bool Relative=0);
	bool updateButton(int BoxHandel);
	void drawButton(int ButtonHandel, int X, int Y, int BlendRate = 255);
	std::string getButtonString(int ButtonHandel);
};

