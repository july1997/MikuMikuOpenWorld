#include "UI.h"

#include "DxLib.h"


void UI::loadBackgroundImage(const char *Picture)
{
	if (!loadBackground)
	{
		Background = LoadGraph(Picture);

		loadBackground = 1;
	}
	else
	{
		DeleteGraph(Background);

		Background = LoadGraph(Picture);
	}
}

void UI::drawBackgroundImage()
{
	DrawExtendGraph(0, 0, 1280, 720, Background, FALSE);
}

int UI::makeBoxRoundedBar(int XLength, int YLength, int EarlyX, int EarlyY, int Color, int BarColor, int Barhigh, int EdgeSize, int AntiAliasing, int Margin)
{
	int dboxnun = 0; bool del = 0;
	if (DeletedBox.size() > 0)
	{
		dboxnun = DeletedBox[DeletedBox.size() - 1];
		//削除
		DeletedBox.pop_back();

		del = 1;

		ScrollValue[dboxnun] = 0;

		OperationType[dboxnun] = 0;

		ButtonString[dboxnun] = "";

		InputHandle[dboxnun] = 0;
		CloseInput[dboxnun] = -1;

		CrickX[dboxnun] = 0;
		CrickY[dboxnun] = 0;

		BarHigh[dboxnun] = Barhigh;

		BoxX[dboxnun] = EarlyX;
		BoxY[dboxnun] = EarlyY;

		BoxXLength[dboxnun] = XLength;
		BoxYLength[dboxnun] = YLength;
	}
	else
	{
		BoxNun++;

		ScrollValue.push_back(0);

		OperationType.push_back(0);

		ButtonString.push_back("");

		InputHandle.push_back(0);
		CloseInput.push_back(-1);

		CrickX.push_back(0);
		CrickY.push_back(0);

		BarHigh.push_back(Barhigh);

		BoxX.push_back(EarlyX);
		BoxY.push_back(EarlyY);

		BoxXLength.push_back(XLength);
		BoxYLength.push_back(YLength);
	}

	int aXLength = XLength * AntiAliasing, aYLength = YLength * AntiAliasing;

	int box = MakeScreen(Margin * 2 + aXLength + 2, Margin * 2 + aYLength + 2, TRUE);

	SetDrawScreen(box);

	DrawBox(Margin + EdgeSize, Margin, Margin + aXLength - EdgeSize, Margin + aYLength + 1, Color, TRUE);

	DrawBox(Margin, Margin + EdgeSize, Margin + aXLength + 2, Margin + aYLength - EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_MULA, 255);

	DrawBox(Margin, Margin, Margin + aXLength + 2, Margin + Barhigh + 2, BarColor, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	if (del)
	{
		DeleteGraph(Box[dboxnun]);

		Box[dboxnun] = MakeScreen(XLength, YLength, TRUE);

		SetDrawScreen(Box[dboxnun]);
	}
	else
	{
		Box.push_back(MakeScreen(XLength, YLength, TRUE));

		SetDrawScreen(Box[BoxNun]);
	}

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	DrawExtendGraph(0, 0, XLength, YLength, box, TRUE);

	SetDrawMode(DX_DRAWMODE_NEAREST);

	DeleteGraph(box);

	SetDrawScreen(DX_SCREEN_BACK);

	GraphFilter(Box[dboxnun], DX_GRAPH_FILTER_GAUSS, 8, 20);

	if (del)
	{
		return dboxnun;
	}
	else
	{
		return BoxNun;
	}
}

int UI::makeBoxRounded(int XLength, int YLength, int Color, int EarlyX, int EarlyY, int EdgeSize, int AntiAliasing, int Margin)
{
	int dboxnun = 0; bool del = 0;
	if (DeletedBox.size() > 0)
	{
		dboxnun = DeletedBox[DeletedBox.size() - 1];
		//削除
		DeletedBox.pop_back();

		del = 1;

		ScrollValue[dboxnun] = 0;

		OperationType[dboxnun] = 0;

		ButtonString[dboxnun] = "";

		InputHandle[dboxnun] = 0;
		CloseInput[dboxnun] = -1;

		CrickX[dboxnun] = 0;
		CrickY[dboxnun] = 0;

		BarHigh[dboxnun] = 0;

		BoxX[dboxnun] = EarlyX;
		BoxY[dboxnun] = EarlyY;

		BoxXLength[dboxnun] = XLength;
		BoxYLength[dboxnun] = YLength;
	}
	else
	{
		BoxNun++;

		ScrollValue.push_back(0);

		OperationType.push_back(0);

		ButtonString.push_back("");

		InputHandle.push_back(0);
		CloseInput.push_back(-1);

		CrickX.push_back(0);
		CrickY.push_back(0);

		BoxX.push_back(EarlyX);
		BoxY.push_back(EarlyY);

		BarHigh.push_back(0);

		BoxXLength.push_back(XLength);
		BoxYLength.push_back(YLength);
	}

	int aXLength = XLength * AntiAliasing, aYLength = YLength * AntiAliasing;

	int box = MakeScreen(Margin * 2 + aXLength + 2, Margin * 2 + aYLength + 2, TRUE);

	SetDrawScreen(box);

	DrawBox(Margin + EdgeSize, Margin, Margin + aXLength - EdgeSize, Margin + aYLength + 1, Color, TRUE);

	DrawBox(Margin, Margin + EdgeSize, Margin + aXLength + 2, Margin + aYLength - EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	if (del)
	{
		DeleteGraph(Box[dboxnun]);

		Box[dboxnun] = MakeScreen(XLength, YLength, TRUE);

		SetDrawScreen(Box[dboxnun]);
	}
	else
	{
		Box.push_back(MakeScreen(XLength, YLength, TRUE));

		SetDrawScreen(Box[BoxNun]);
	}

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	DrawExtendGraph(0, 0, XLength, YLength, box, TRUE);

	SetDrawMode(DX_DRAWMODE_NEAREST);

	DeleteGraph(box);

	SetDrawScreen(DX_SCREEN_BACK);

	GraphFilter(Box[BoxNun], DX_GRAPH_FILTER_GAUSS, 8, 20);

	if (del)
	{
		return dboxnun;
	}
	else
	{
		return BoxNun;
	}
}

void UI::directDrawBoxRounded(int x, int y, int XLength, int YLength, int BlendRate, int Color, int EdgeSize, int AntiAliasing, int Margin)
{
	int aXLength = XLength * AntiAliasing, aYLength = YLength * AntiAliasing;

	int Box = MakeScreen(Margin * 2 + aXLength + 2, Margin * 2 + aYLength + 2, TRUE);

	SetDrawScreen(Box);

	DrawBox(Margin + EdgeSize, Margin, Margin + aXLength - EdgeSize, Margin + aYLength + 1, Color, TRUE);

	DrawBox(Margin, Margin + EdgeSize, Margin + aXLength + 2, Margin + aYLength - EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	SetDrawScreen(DX_SCREEN_BACK);

	if (BlendRate != 255)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, BlendRate);
	}

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	GraphFilter(Box, DX_GRAPH_FILTER_GAUSS, 8, 20);

	DrawExtendGraph(x, y, x + XLength, y + YLength, Box, TRUE);

	SetDrawMode(DX_DRAWMODE_NEAREST);

	DeleteGraph(Box);

	if (BlendRate != 255)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}

void UI::changeBoxRoundedBar(int BoxHandel, int XLength, int YLength, int Color, int BarColor, int Barhigh, int EdgeSize, int AntiAliasing, int Margin)
{
	BarHigh[BoxHandel] = Barhigh;

	BoxXLength[BoxHandel] = XLength;
	BoxYLength[BoxHandel] = YLength;

	int aXLength = XLength * AntiAliasing, aYLength = YLength * AntiAliasing;

	DeleteGraph(Box[BoxHandel]);

	int box = MakeScreen(Margin * 2 + aXLength + 2, Margin * 2 + aYLength + 2, TRUE);

	SetDrawScreen(box);

	DrawBox(Margin + EdgeSize, Margin, Margin + aXLength - EdgeSize, Margin + aYLength + 1, Color, TRUE);

	DrawBox(Margin, Margin + EdgeSize, Margin + aXLength + 2, Margin + aYLength - EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_MULA, 255);

	DrawBox(Margin, Margin, Margin + aXLength + 2, Margin + Barhigh + 2, BarColor, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	Box[BoxHandel] = MakeScreen(XLength, YLength, TRUE);

	SetDrawScreen(Box[BoxHandel]);

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	DrawExtendGraph(0, 0, XLength, YLength, box, TRUE);

	SetDrawMode(DX_DRAWMODE_NEAREST);

	DeleteGraph(box);

	SetDrawScreen(DX_SCREEN_BACK);

	GraphFilter(Box[BoxHandel], DX_GRAPH_FILTER_GAUSS, 8, 20);
}

void UI::changeBoxRounded(int BoxHandel, int XLength, int YLength, int Color, int EdgeSize, int AntiAliasing, int Margin)
{
	BoxXLength[BoxHandel] = XLength;
	BoxYLength[BoxHandel] = YLength;

	int aXLength = XLength * AntiAliasing, aYLength = YLength * AntiAliasing;

	DeleteGraph(Box[BoxHandel]);

	int box = MakeScreen(Margin * 2 + aXLength + 2, Margin * 2 + aYLength + 2, TRUE);

	SetDrawScreen(box);

	DrawBox(Margin + EdgeSize, Margin, Margin + aXLength - EdgeSize, Margin + aYLength + 1, Color, TRUE);

	DrawBox(Margin, Margin + EdgeSize, Margin + aXLength + 2, Margin + aYLength - EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	DrawCircle(Margin + aXLength - EdgeSize, Margin + aYLength - EdgeSize, EdgeSize, Color, TRUE);

	Box[BoxHandel] = MakeScreen(XLength, YLength, TRUE);

	SetDrawScreen(Box[BoxHandel]);

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	DrawExtendGraph(0, 0, XLength, YLength, box, TRUE);

	SetDrawMode(DX_DRAWMODE_NEAREST);

	DeleteGraph(box);

	SetDrawScreen(DX_SCREEN_BACK);

	GraphFilter(Box[BoxHandel], DX_GRAPH_FILTER_GAUSS, 8, 20);
}

void UI::drawBox(int x, int y, int BoxHandel, int BlendRate)
{
	if (BlendRate != 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, BlendRate);
	}

	DrawGraph(x, y, Box[BoxHandel], TRUE);

	if (BlendRate != 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	BoxX[BoxHandel] = x;
	BoxY[BoxHandel] = y;
}

void UI::drawBox(int BoxHandel, int BlendRate)
{
	if (BlendRate != 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, BlendRate);
	}

	DrawGraph(BoxX[BoxHandel], BoxY[BoxHandel], Box[BoxHandel], TRUE);

	if (BlendRate != 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

void UI::updateBox(int BoxHandel)
{
	if (BarHigh[BoxHandel] != 0)
	{
		int MouseX, MouseY;
		// マウスの位置を取得
		GetMousePoint(&MouseX, &MouseY);

		if (MouseX >= BoxX[BoxHandel] && MouseX <= BoxX[BoxHandel] + BoxXLength[BoxHandel])
		{
			if (MouseY >= BoxY[BoxHandel] && MouseY <= BoxY[BoxHandel] + BarHigh[BoxHandel]/2)
			{
				//カーソルを変更
				HCURSOR cur = SetCursor(LoadCursor(NULL, IDC_SIZEALL));

				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					if (CrickX[BoxHandel] == 0 && CrickY[BoxHandel] == 0)
					{
						CrickX[BoxHandel] = MouseX;
						CrickY[BoxHandel] = MouseY;

						OperationType[BoxHandel] = 1;
					}
				}
			}
		}

		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			if (OperationType[BoxHandel] == 1)
			{
				BoxX[BoxHandel] -= CrickX[BoxHandel] - MouseX;
				BoxY[BoxHandel] -= CrickY[BoxHandel] - MouseY;

				CrickX[BoxHandel] = MouseX;
				CrickY[BoxHandel] = MouseY;
			}

			if (BoxX[BoxHandel] < 0)BoxX[BoxHandel] = 0;
			if (BoxX[BoxHandel] + BoxXLength[BoxHandel] > 1280)BoxX[BoxHandel] = 1280 - BoxXLength[BoxHandel];
			if (BoxY[BoxHandel] < 0)BoxY[BoxHandel] = 0;
			if (BoxY[BoxHandel] + BoxYLength[BoxHandel] > 720)BoxY[BoxHandel] = 720 - BoxYLength[BoxHandel];

		}
		else
		{
			if (OperationType[BoxHandel] == 1)
			{
				CrickX[BoxHandel] = 0;
				CrickY[BoxHandel] = 0;

				OperationType[BoxHandel] = 0;
			}
		}
	}
}

void UI::updateSize(int BoxHandel, int MinimumValueX, int MinimumValueY, int Color, int BarColor, int Barhigh, int EdgeSize, int AntiAliasing, int Margin)
{
	if (BarHigh[BoxHandel] != 0)
	{

		int MouseX, MouseY;
		// マウスの位置を取得
		GetMousePoint(&MouseX, &MouseY);

		//右下
		if (MouseX >= BoxX[BoxHandel] + BoxXLength[BoxHandel] - BarHigh[BoxHandel] / 3 && MouseX <= BoxX[BoxHandel] + BoxXLength[BoxHandel])
		{
			if (MouseY >= BoxY[BoxHandel] + BoxYLength[BoxHandel] - BarHigh[BoxHandel] / 3 && MouseY <= BoxY[BoxHandel] + BoxYLength[BoxHandel])
			{
				//カーソルを変更
				HCURSOR cur = SetCursor(LoadCursor(NULL, IDC_SIZENWSE));

				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
				{
					if (CrickX[BoxHandel] == 0 && CrickY[BoxHandel] == 0)
					{
						CrickX[BoxHandel] = MouseX;
						CrickY[BoxHandel] = MouseY;

						OperationType[BoxHandel] = 4;
					}
				}
			}
			else if (MouseY >= BoxY[BoxHandel] && MouseY <= BoxY[BoxHandel] + BoxYLength[BoxHandel])
			{
				//カーソルを変更
				HCURSOR cur = SetCursor(LoadCursor(NULL, IDC_SIZEWE));

				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
				{
					if (CrickX[BoxHandel] == 0 && CrickY[BoxHandel] == 0)
					{
						CrickX[BoxHandel] = MouseX;
						CrickY[BoxHandel] = MouseY;

						OperationType[BoxHandel] = 2;
					}
				}
			}
		}
		else
		{
			//下
			if (MouseX >= BoxX[BoxHandel] && MouseX <= BoxX[BoxHandel] + BoxXLength[BoxHandel])
			{
				if (MouseY >= BoxY[BoxHandel] + BoxYLength[BoxHandel] - BarHigh[BoxHandel]/ 3 && MouseY <= BoxY[BoxHandel] + BoxYLength[BoxHandel])
				{
					if (CrickX[BoxHandel] == 0 && CrickY[BoxHandel] == 0)
					{
						//カーソルを変更
						HCURSOR cur = SetCursor(LoadCursor(NULL, IDC_SIZENS));

						if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
						{
							CrickX[BoxHandel] = MouseX;
							CrickY[BoxHandel] = MouseY;

							OperationType[BoxHandel] = 3;
						}
					}
				}
			}
		}

		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			switch (OperationType[BoxHandel])
			{
			case 2:
				if (BoxXLength[BoxHandel] - (+CrickX[BoxHandel] - MouseX) > 0 && BoxXLength[BoxHandel] - (+CrickX[BoxHandel] - MouseX) < 1280)
				{
					if (CrickX[BoxHandel] != MouseX && CrickY[BoxHandel] != MouseY)
					{
						if (BoxXLength[BoxHandel] - (+CrickX[BoxHandel] - MouseX) > MinimumValueX)
						{
							changeBoxRoundedBar(BoxHandel, BoxXLength[BoxHandel] - (+CrickX[BoxHandel] - MouseX), BoxYLength[BoxHandel], Color, BarColor, Barhigh, EdgeSize, AntiAliasing, Margin);

							CrickX[BoxHandel] = MouseX;
							CrickY[BoxHandel] = MouseY;
						}
					}
				}
				break;

			case 3:
				if (BoxYLength[BoxHandel] - (+CrickY[BoxHandel] - MouseY) > 0 && BoxYLength[BoxHandel] - (+CrickY[BoxHandel] - MouseY) < 720)
				{
					if (CrickX[BoxHandel] != MouseX && CrickY[BoxHandel] != MouseY)
					{
						if (BoxYLength[BoxHandel] - (+CrickY[BoxHandel] - MouseY) > MinimumValueY)
						{
							changeBoxRoundedBar(BoxHandel, BoxXLength[BoxHandel], BoxYLength[BoxHandel] - (+CrickY[BoxHandel] - MouseY), Color, BarColor, Barhigh, EdgeSize, AntiAliasing, Margin);

							CrickX[BoxHandel] = MouseX;
							CrickY[BoxHandel] = MouseY;
						}
					}
				}
				break;

			case 4:
				if (BoxXLength[BoxHandel] - (+CrickX[BoxHandel] - MouseX) > 0 && BoxXLength[BoxHandel] - (+CrickX[BoxHandel] - MouseX) < 1280)
				{
					if (BoxYLength[BoxHandel] - (+CrickY[BoxHandel] - MouseY) > 0 && BoxYLength[BoxHandel] - (+CrickY[BoxHandel] - MouseY) < 720)
					{
						if (BoxXLength[BoxHandel] - (+CrickX[BoxHandel] - MouseX) > MinimumValueX && BoxYLength[BoxHandel] - (+CrickY[BoxHandel] - MouseY) > MinimumValueY)
						{
							if (CrickX[BoxHandel] != MouseX && CrickY[BoxHandel] != MouseY)
							{
								changeBoxRoundedBar(BoxHandel, BoxXLength[BoxHandel] - (+CrickX[BoxHandel] - MouseX), BoxYLength[BoxHandel] - (+CrickY[BoxHandel] - MouseY), Color, BarColor, Barhigh, EdgeSize, AntiAliasing, Margin);

								CrickX[BoxHandel] = MouseX;
								CrickY[BoxHandel] = MouseY;
							}
						}
					}
				}
				break;
			}

			if (BoxX[BoxHandel] < 0)BoxX[BoxHandel] = 0;
			if (BoxX[BoxHandel] + BoxXLength[BoxHandel] > 1280)BoxX[BoxHandel] = 1280 - BoxXLength[BoxHandel];
			if (BoxY[BoxHandel] < 0)BoxY[BoxHandel] = 0;
			if (BoxY[BoxHandel] + BoxYLength[BoxHandel] > 720)BoxY[BoxHandel] = 720 - BoxYLength[BoxHandel];

		}
		else
		{
			if (OperationType[BoxHandel] != 1 && OperationType[BoxHandel] != 0)
			{
				CrickX[BoxHandel] = 0;
				CrickY[BoxHandel] = 0;

				OperationType[BoxHandel] = 0;
			}
		}
	}
}

int UI::updateScroll(int BoxHandel, int downlimit, int ScrollBarColor)
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		int MouseX, MouseY;
		// マウスの位置を取得
		GetMousePoint(&MouseX, &MouseY);

		if (MouseX >= BoxX[BoxHandel] + BoxXLength[BoxHandel] - BarHigh[BoxHandel] * 2 && MouseX < BoxX[BoxHandel] + BoxXLength[BoxHandel] - BarHigh[BoxHandel])
		{
			if (MouseY >= BoxY[BoxHandel] + BarHigh[BoxHandel] + 10 + ScrollValue[BoxHandel] && MouseY < BoxY[BoxHandel] + BarHigh[BoxHandel] * 2 + 15 + ScrollValue[BoxHandel])
			{
				if (CrickX[BoxHandel] == 0 && CrickY[BoxHandel] == 0)
				{
					CrickX[BoxHandel] = MouseX;
					CrickY[BoxHandel] = MouseY;

					OperationType[BoxHandel] = 5;
				}
			}
		}

		if (OperationType[BoxHandel] == 5)
		{
			if ((CrickY[BoxHandel] - MouseY) == 0)
			{
				CrickX[BoxHandel] = MouseX;
				CrickY[BoxHandel] = BoxY[BoxHandel] + BarHigh[BoxHandel] + 20;
			}
			else
			{
				ScrollValue[BoxHandel] = -(+CrickY[BoxHandel] - MouseY);
			}
		}

		if (ScrollValue[BoxHandel] < 0)ScrollValue[BoxHandel] = 0;
		if (ScrollValue[BoxHandel] > BoxYLength[BoxHandel] - BarHigh[BoxHandel] * 3 - 25 - downlimit)ScrollValue[BoxHandel] = BoxYLength[BoxHandel] - BarHigh[BoxHandel] * 3 - 25 - downlimit;
	}
	else
	{
		if (OperationType[BoxHandel] == 5)
		{
			CrickX[BoxHandel] = 0;
			CrickY[BoxHandel] = 0;

			OperationType[BoxHandel] = 0;
		}
	}

	if (BoxXLength[BoxHandel] > BoxXLength[BoxHandel] - BarHigh[BoxHandel] * 2 && BarHigh[BoxHandel] * 2 + 15 + ScrollValue[BoxHandel] < BoxYLength[BoxHandel])
		DrawBox(BoxX[BoxHandel] + BoxXLength[BoxHandel] - BarHigh[BoxHandel] - 15, BoxY[BoxHandel] + BarHigh[BoxHandel] + 10 + ScrollValue[BoxHandel],
			BoxX[BoxHandel] + BoxXLength[BoxHandel] - BarHigh[BoxHandel], BoxY[BoxHandel] + BarHigh[BoxHandel] * 2 + 15 + ScrollValue[BoxHandel], ScrollBarColor, TRUE);

	return ScrollValue[BoxHandel];
}

void UI::setScroll(int BoxHandel, int scroll)
{
	ScrollValue[BoxHandel] = scroll;
}

int UI::getBoxHeight(int BoxHandel)
{
	return BoxYLength[BoxHandel];
}

int UI::getBoxWidth(int BoxHandel)
{
	return BoxXLength[BoxHandel];
}

void UI::deleteBox(int BoxHandel)
{
	//画像を削除
	DeleteGraph(Box[BoxHandel]);

	//削除したハンドル番号を記憶
	DeletedBox.push_back(BoxHandel);
}

void UI::drawStringToBox(int BoxHandel, std::string string, int InBoxX, int InBoxY, int FontHandel, bool Center, int Color, int WidthLimit)
{
	if (InBoxX < BoxXLength[BoxHandel] && InBoxY + 18 < BoxYLength[BoxHandel])
	{
		if (GetDrawFormatStringWidthToHandle(getFont(FontHandel), string.c_str()) < BoxXLength[BoxHandel] - WidthLimit)
		{
			//画面内
			drawString(FontHandel, string, BoxX[BoxHandel] + InBoxX, BoxY[BoxHandel] + InBoxY, Center, Color);
		}
		else
		{
			//画面外
			int cs = 0;

			//はみ出した文字を数える
			for (size_t i = 0; i < string.size(); i++)
			{
				if (i % 3 == 0)
				{
					if (GetDrawFormatStringWidthToHandle(getFont(FontHandel), string.substr(i, string.size() - 1).c_str()) <= BoxXLength[BoxHandel] - WidthLimit)
					{
						cs = i;
						break;
					}
				}
			}

			drawString(FontHandel, string.substr(cs, string.size() - 1).c_str(), BoxX[BoxHandel] + InBoxX, BoxY[BoxHandel] + InBoxY, Center, Color);
		}
	}
}


int UI::makeInputBox(int XLength, int YLength, int color, int maxlengh, const char *initString, bool cancancel, bool SingleOnly, bool NumOnly)
{
	int box = makeBoxRounded(XLength, YLength, color);

	InputHandle[box] = MakeKeyInput(maxlengh, cancancel, SingleOnly, NumOnly);
	CloseInput[box] = 0;

	SetKeyInputString(initString, InputHandle[box]);

	return box;
}

void UI::activateInputBox(int InputBoxHandel)
{
	if(InputBoxHandel != -1)
	SetActiveKeyInput(InputHandle[InputBoxHandel]);
	else SetActiveKeyInput(InputBoxHandel);

	NowActiveKey = InputBoxHandel;
	NowActiveKeyI = InputBoxHandel;
}

void UI::setInputString(int InputBoxHandel, const char *setString)
{
	SetKeyInputString(setString, InputHandle[InputBoxHandel]);
}

void UI::drawInputBoxToBox(int BoxHandel, int InputBoxHandel, int InBoxX, int InBoxY, int &FontHandel, int BlendRate, bool drawString, bool Relative,bool Asterisk)
{
	bool draw = 0;

	/*
	int MouseX, MouseY;
	// マウスの位置を取得
	GetMousePoint(&MouseX, &MouseY);

	if (MouseX >= BoxX[BoxHandel] + InBoxX && MouseX <= BoxX[BoxHandel] + InBoxX + BoxXLength[InputBoxHandel])
	{
		if (MouseY >= BoxY[BoxHandel] + InBoxY && MouseY <= BoxY[BoxHandel] + InBoxY + BoxYLength[InputBoxHandel])
		{
			//カーソルを変更
			HCURSOR cur = SetCursor(LoadCursor(NULL, IDC_IBEAM));
		}
	}
	*/

	if (!Relative)
	{
		if (InBoxX < BoxXLength[BoxHandel] && InBoxY + BoxYLength[InputBoxHandel] < BoxYLength[BoxHandel])
		{
			drawBox(BoxX[BoxHandel] + InBoxX, BoxY[BoxHandel] + InBoxY-6, InputBoxHandel, BlendRate);

			draw = 1;
		}
	}
	else
	{
		if (BoxXLength[BoxHandel] - InBoxX < BoxXLength[BoxHandel] && BoxYLength[BoxHandel] - InBoxY < BoxYLength[BoxHandel])
		{
			drawBox(BoxX[BoxHandel] + BoxXLength[BoxHandel] - InBoxX, BoxY[BoxHandel] + BoxYLength[BoxHandel] - InBoxY-6, InputBoxHandel, BlendRate);

			draw = 1;
		}
	}

	if (draw)
	{
		if (drawString)
		{
			char String[256];
			std::string s;

			// 現時点での確定入力文字列を取得する
			GetKeyInputString(String, InputHandle[InputBoxHandel]);

			if (!Asterisk) {
				drawStringToBox(InputBoxHandel, String, 10, 6, FontHandel);
			}
			else {
				std::string s(String),ss;
				for (int i = 0; i < s.length(); i++)ss.append(u8"●");

				drawStringToBox(InputBoxHandel, ss.c_str(), 10, 6, FontHandel);
			}
			

			if (InputBoxHandel == NowActiveKey)
			{
				Flash++;

				//カーソルの位置
				int CursorPos = GetKeyInputCursorPosition(InputHandle[InputBoxHandel]);

				// カーソルのドット単位の位置を取得する
				int CursorDotPos;
				GetDrawStringSizeToHandle(&CursorDotPos, NULL, NULL, String, CursorPos, getFont(FontHandel));
				if (Asterisk)CursorDotPos = CursorPos * 18;

				// ＩＭＥ入力情報を取得する
				const IMEINPUTDATA *ImeData = GetIMEInputData();

				// ＩＭＥ入力情報があるかどうか(ＩＭＥ入力をしているかどうか)で処理を分岐
				if (ImeData)
				{
					drawStringToBox(InputBoxHandel, ImeData->InputString, 10 + CursorDotPos, 6, FontHandel);

					if (BlendRate != 0)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, BlendRate);
					}

					// 文節の数だけ下線を描画する
					for (int i = 0; i < ImeData->ClauseNum; i++)
					{
						// 下線の開始座標を計算
						int LineStartX;
						GetDrawStringSizeToHandle(&LineStartX, NULL, NULL, ImeData->InputString, ImeData->ClauseData[i].Position, getFont(FontHandel));

						// 下線の長さを計算
						int LineLength;
						GetDrawStringSizeToHandle(&LineLength, NULL, NULL, ImeData->InputString + ImeData->ClauseData[i].Position, ImeData->ClauseData[i].Length, getFont(FontHandel));

						// 下線の描画
						if (CursorDotPos + LineStartX + LineLength + 10 + InBoxX > InBoxX + BoxXLength[InputBoxHandel])
						{
							DrawLine(BoxX[BoxHandel] + 10 + InBoxX, BoxY[BoxHandel] + InBoxY + 22,
								BoxX[BoxHandel] + BoxXLength[InputBoxHandel]+ InBoxX, BoxY[BoxHandel] + InBoxY + 22,
								GetColor(255, 255, 255));
						}
						else
						{
							DrawLine(CursorDotPos + LineStartX + 10 + BoxX[BoxHandel] + InBoxX, BoxY[BoxHandel] + InBoxY + 22,
								CursorDotPos + LineStartX + LineLength + 10 + BoxX[BoxHandel] + InBoxX, BoxY[BoxHandel] + InBoxY + 22,
								GetColor(255, 255, 255));
						}

						// もし選択している文節だった場合は文節の開始座標を保存しておく
						if (i == ImeData->SelectClause)
						{
							SelectClauseX = CursorDotPos + LineStartX + BoxX[BoxHandel] + InBoxX + 10;
						}
					}

					// 変換候補リストが存在する場合は描画処理を行う
					if (ImeData->CandidateNum)
					{
						// 変換候補リストの開始ナンバーを計算
						int StartNumber = ImeData->SelectCandidate / 5 * 5;

						// 変換候補リストの同時表示数に達するか、変換候補リストの終端に来るまでループ
						for (int i = 0; StartNumber + i < ImeData->CandidateNum && i < 5; i++)
						{
							// 変換候補の描画
							DrawString(SelectClauseX, i * 20 + BoxY[BoxHandel] + InBoxY + 30, ImeData->CandidateList[StartNumber + i], GetColor(255, 255, 255));

							if (i == 4)DrawFormatString(SelectClauseX + 30, i * 20 + BoxY[BoxHandel] + InBoxY + 30, -1, "%d ", ImeData->SelectCandidate);
						}

						// 選択している変換候補を囲う枠の描画
						int LineStartX;
						GetDrawStringSizeToHandle(&LineStartX, NULL, NULL, ImeData->CandidateList[ImeData->SelectCandidate - StartNumber], strlen(ImeData->CandidateList[ImeData->SelectCandidate - StartNumber]), getFont(FontHandel));

						DrawBox(
							SelectClauseX - 2,
							(ImeData->SelectCandidate - StartNumber) * 20 + BoxY[BoxHandel] + InBoxY + 28,
							SelectClauseX + 2 + LineStartX,
							(ImeData->SelectCandidate - StartNumber) * 20 + BoxY[BoxHandel] + InBoxY + 48,
							GetColor(0, 255, 128), FALSE);
					}

					if (Flash > 30)
					{
						// カーソルの描画
						int LineStartX;
						GetDrawStringSizeToHandle(&LineStartX,NULL, NULL,ImeData->InputString, ImeData->CursorPosition, getFont(FontHandel));

						if (InBoxX + LineStartX + CursorDotPos > InBoxX + BoxXLength[InputBoxHandel])
						{
							DrawBox(BoxX[BoxHandel] + BoxXLength[InputBoxHandel] + InBoxX, BoxY[BoxHandel] + InBoxY, BoxX[BoxHandel] + BoxXLength[InputBoxHandel] -1 + InBoxX, BoxY[BoxHandel] + InBoxY + 19, GetColor(255, 255, 255), TRUE);
						}
						else
						{
							DrawBox(BoxX[BoxHandel] + InBoxX + 14 + CursorDotPos + LineStartX, BoxY[BoxHandel] + InBoxY, BoxX[BoxHandel] + InBoxX + 15 + LineStartX + CursorDotPos, BoxY[BoxHandel] + InBoxY + 19, GetColor(255, 255, 255), TRUE);
						}
					}


					if (BlendRate != 0)
					{
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					}
				}
				else
				{
					if (BlendRate != 0)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, BlendRate);
					}

					// ＩＭＥ入力をしていない場合
					// カーソルを描画
					if (Flash > 30)
					{
						if (InBoxX + CursorDotPos > InBoxX + BoxXLength[InputBoxHandel])
						{
							DrawBox(BoxX[BoxHandel] + BoxXLength[InputBoxHandel] + InBoxX, BoxY[BoxHandel] + InBoxY, BoxX[BoxHandel] + BoxXLength[InputBoxHandel] -1 + InBoxX, BoxY[BoxHandel] + InBoxY + 19, GetColor(255, 255, 255), TRUE);
						}
						else
						{
							DrawBox(BoxX[BoxHandel] + InBoxX + 14 + CursorDotPos, BoxY[BoxHandel] + InBoxY, BoxX[BoxHandel] + InBoxX + 15 + CursorDotPos, BoxY[BoxHandel] + InBoxY + 19, GetColor(255, 255, 255), TRUE);
						}
					}
					if (BlendRate != 0)
					{
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					}
				}

				if (Flash == 60)Flash = 0;
			}
		}
	}
}

bool UI::updateInputBox(int BoxHandel, std::string data)
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		int MouseX, MouseY;
		// マウスの位置を取得
		GetMousePoint(&MouseX, &MouseY);

		if (MouseX > BoxX[BoxHandel] && MouseX < BoxX[BoxHandel] + BoxXLength[BoxHandel])
		{
			if (MouseY > BoxY[BoxHandel] && MouseY < BoxY[BoxHandel] + BoxYLength[BoxHandel])
			{
				activateInputBox(BoxHandel);
			}
		}
	}

	if (CheckKeyInput(InputHandle[BoxHandel]) != 0)
	{
		char String[256];
		GetKeyInputString(String, InputHandle[BoxHandel]);

		data = std::string(String);

		NowActiveKeyI = -1;

		return 1;
	}
	else
	{
		if(CheckHitKey(KEY_INPUT_TAB) != 0) {
			for (int i = BoxHandel; i<CloseInput.size(); i++)
			{
				if (CloseInput[i] == 0)activateInputBox(i);
			}
		}
	}
	return 0;
}

int UI::getNowActiveKey()
{
	return NowActiveKey;
}

int UI::getNowActiveKeyI()
{
	return NowActiveKeyI;
}

std::string UI::getInputString(int InputBoxHandel)
{
	char String[256];

	GetKeyInputString(String, InputHandle[InputBoxHandel]);

	return std::string(String);
}


int UI::makeButton(const char *drawString, int &FontHandel, int color)
{
	int FontSize;
	GetFontStateToHandle(NULL, &FontSize, NULL, getFont(FontHandel));

	int XLength = GetDrawFormatStringWidthToHandle(getFont(FontHandel), drawString) + 11;
	int YLength = FontSize + 10;

	int box = makeBoxRounded(XLength, YLength, color);

	ButtonString[box] = drawString;
	InputHandle[box] = FontHandel;

	return box;
}

void UI::drawButtonToBox(int BoxHandel, int ButtonHandel, int InBoxX, int InBoxY, int BlendRate, bool Relative)
{
	if (!Relative)
	{
		if (InBoxX < BoxXLength[BoxHandel] && InBoxY + BoxYLength[ButtonHandel] < BoxYLength[BoxHandel])
		{
			drawBox(BoxX[BoxHandel] + InBoxX, BoxY[BoxHandel] + InBoxY, ButtonHandel, BlendRate);

			drawStringToBox(ButtonHandel, ButtonString[ButtonHandel], 5, 4, InputHandle[ButtonHandel]);
		}
	}
	else
	{
		if (BoxXLength[BoxHandel] - InBoxX < BoxXLength[BoxHandel] && BoxYLength[BoxHandel] - InBoxY < BoxYLength[BoxHandel])
		{
			drawBox(BoxX[BoxHandel] + BoxXLength[BoxHandel] - InBoxX, BoxY[BoxHandel] + BoxYLength[BoxHandel] - InBoxY, ButtonHandel, BlendRate);

			drawStringToBox(ButtonHandel, ButtonString[ButtonHandel], 5, 5, InputHandle[ButtonHandel]);
		}
	}
}

bool UI::updateButton(int BoxHandel)
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		int MouseX, MouseY;
		// マウスの位置を取得
		GetMousePoint(&MouseX, &MouseY);

		if (MouseX > BoxX[BoxHandel] && MouseX < BoxX[BoxHandel] + BoxXLength[BoxHandel])
		{
			if (MouseY > BoxY[BoxHandel] && MouseY < BoxY[BoxHandel] + BoxYLength[BoxHandel])
			{
				return 1;
			}
		}
	}

	return 0;
}

void UI::drawButton(int ButtonHandel, int X, int Y, int BlendRate)
{
	drawBox(X, Y, ButtonHandel, BlendRate);

	drawStringToBox(ButtonHandel, ButtonString[ButtonHandel], 5, 5, InputHandle[ButtonHandel]);
}

std::string UI::getButtonString(int ButtonHandel)
{
	return ButtonString[ButtonHandel];
}

int UI::clearScreen()
{
	ClearDrawScreen();
	return 0;
}

int UI::show()
{
	ScreenFlip();
	return 0;
}

void UI::drawMausePoint()
{
	int MouseX, MouseY;
	// マウスの位置を取得
	GetMousePoint(&MouseX, &MouseY);

	DrawFormatString(0, 0, -1, "X: %d  Y: %d ", MouseX, MouseY);
}