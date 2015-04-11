#include "Scene.h"

#include "Youtube.h"

#include "DxLib.h"

Scene::Scene(Network *network_)
{
	network = network_;
}

bool Scene::Login()
{
	ui = new UI;
	font = ui->makeFont(18, 3);

	ui->drawBackground("design/Image Background.png");

	bigfont = ui->makeFont(24, 6);
	int boxfont = ui->makeFont(18, 3);
	int box = ui->makeBoxRoundedBar(400, 400, 440, 200);
	int input = ui->makeInputBox(360, 30, 36, "mikumikuopenworld.ddns.net");//42-146-229-131.rev.home.ne.jp
	int input2 = ui->makeInputBox(360, 30,36,"192.168.11.2");
	ui->activateInputBox(input);

	int ok = ui->makeButton("OK", font);
	int offline = ui->makeButton("オフライン", font);
	bool setNetwork = 0;

	FpsManager fps;
	int sc = 0, e = 0; bool inputend = 0; std::string inp; int error = 0;
	while (ProcessMessage() == 0 && exitConfirm() == 0)
	{
		ui->drawBackground("design/Image Background.png");

		ui->drawString(640 - 880 * sin(DX_PI_F / 180 * e) * 2, 100, "ログイン", bigfont, 1);

		if (sc != 90)
		{
			sc++;
			ui->changeBoxRoundedBar(box, 440, 300 * sin(DX_PI_F / 180 * sc));
		}

		ui->updateBox(box);

		if (inputend == 1)
		{
			ui->drawBox(440 - 880 * sin(DX_PI_F / 180 * e) * 2, 200, box, 128);
		}
		else
		{
			ui->drawBox(box, 128);
		}

		ui->drawStringToBox(box, "ホスト名から接続", 20, 20, boxfont);
		ui->drawInputBoxToBox(box, input, 20, 40, boxfont, 128);

		ui->drawStringToBox(box, "IPアドレスから接続", 20, 110, boxfont);
		ui->drawInputBoxToBox(box, input2, 20, 130, boxfont, 128);

		ui->drawButtonToBox(box, ok, 200, 230, 72);
		ui->drawButtonToBox(box, offline, 310, 260, 72);

		if (ui->updateInputBox(input, inp) && inputend == 0)
		{
			if (!network->connectHostName(inp))inputend = 1, error = 0;
			else ui->setInputString(input, ""), error=1;
		}

		if (ui->updateInputBox(input2, inp) && inputend == 0)
		{
			if (!network->connectIP(inp))inputend = 1, error = 0;
			else ui->setInputString(input2, ""), error=2;
		}

		if (ui->updateButton(ok) == 1 && inputend == 0)
		{
			if (ui->getNowActiveKey() == input)
			{
				inp = std::string(ui->getInputString(ui->getNowActiveKey()));
				if (!network->connectHostName(inp))inputend = 1, error = 0;
				else ui->setInputString(input, ""), error=1;
			}
			else
			{
				inp = std::string(ui->getInputString(ui->getNowActiveKey()));
				if (!network->connectIP(inp))inputend = 1, error = 0;
				else ui->setInputString(input2, ""), error=2;
			}
		}

		if (ui->updateButton(offline) == 1)
		{
			network->useNetwork = 0;
			setNetwork = 1;
		}
		if (setNetwork == 1)break;


		if (error == 1)ui->drawStringToBox(box, "ホスト名が間違っています", 40, 20+ui->getBoxHeight(input)+20, font, 0, GetColor(237, 28, 36));
		if (error == 2)ui->drawStringToBox(box, "IPアドレスが間違っています", 40, 110 + ui->getBoxHeight(input2)+20, font, 0, GetColor(237, 28, 36));

		if (inputend == 1)
		{
			e++;
			if (e == 90)break;
		}

		fps.displayFps(1260, 0);

		fps.controlFps();
	}

	if (setNetwork != 1)
	{
		ui->deleteBox(input);
		ui->deleteBox(input2);

		network->login();

		int input3 = ui->makeInputBox(360, 30, 16);
		ui->activateInputBox(input3);

		e = -90; int b = LoadBlendGraph("Picture/b.png"); std::string n; inputend = 0;
		while (ProcessMessage() == 0 && exitConfirm() == 0)
		{
			ui->drawBackground("design/Image Background.png");

			ui->drawString(640 - 640 * sin(DX_PI_F / 180 * e) * 2, 100, "名前の入力", bigfont, 1);

			ui->drawBox(440 - 640 * sin(DX_PI_F / 180 * e) * 2, 200, box, 128);

			ui->drawStringToBox(box, "名前（全角8文字まで,入力無しも可能)", 20, 30, boxfont);
			ui->drawInputBoxToBox(box, input3, 20, 50, boxfont, 128);

			ui->drawButtonToBox(box, ok, 200, 250, 72);

			if (e < 0)
			{
				e++;
			}

			if (ui->updateInputBox(input3, n) == 1 && inputend == 0)
			{
				inputend = 1;
			}

			if (ui->updateButton(ok) == 1 && inputend == 0)
			{
				n = std::string(ui->getInputString(ui->getNowActiveKey()));
				inputend = 1;
			}

			if (inputend)
			{
				e++;
				if (e == 90)break;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, e * 3);
				DrawGraph(0, 0, b, FALSE);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			}

			fps.displayFps(1260, 0);

			fps.controlFps();
		}

		ui->deleteBox(ok);
		ui->deleteBox(input3);
		ui->deleteBox(box);

		ClearDrawScreen();
		ui->drawString(640, 300, "Now Loading...", boxfont, 1);
		ScreenFlip();

		network->setup(n);
	}
	else
	{
		ClearDrawScreen();
		ui->drawString(640, 300, "Now Loading...", boxfont, 1);
		ScreenFlip();
	}

	return 0;
}

bool Scene::exitConfirm()
{
	//ウィンドウが閉じられようとしている か ESCAPEキーが押されている
	if (GetWindowUserCloseFlag(1) == TRUE || CheckHitKey(KEY_INPUT_ESCAPE) == 1)
	{
		FpsManager fps;

		int scr = MakeGraph(1280, 720);
		GetDrawScreenGraph(0, 0, 1280, 720, scr);

		GraphFilter(scr, DX_GRAPH_FILTER_GAUSS, 8, 1000);

		int box = ui->makeBoxRoundedBar(600, 210, 340, 230);
		int yes = ui->makeButton("はい",font);
		int no = ui->makeButton("いいえ", font);

		while (ProcessMessage() == 0)
		{
			DrawGraph(0, 0, scr, FALSE);

			ui->updateBox(box);
			ui->drawBox(box,128);

			ui->drawButtonToBox(box,yes,150,150, 72);
			ui->drawButtonToBox(box, no, 400, 150, 72);

			if (ui->updateButton(yes) == 1){
				EndFlag = 1; 
				break; 
			}

			if (ui->updateButton(no) == 1){
				GetWindowUserCloseFlag(EndFlag); 
				break;
			}

			ui->drawStringToBox(box, "終了しますか？", 300,40 ,bigfont ,1);

			fps.controlFps();
		}

		ui->deleteBox(box);
		ui->deleteBox(yes);
		ui->deleteBox(no);

		if (EndFlag == 1)
		{
			DrawGraph(0, 0, scr, FALSE);
			ui->drawStringToBox(box, "終了しています・・・", 300, 40, bigfont, 1);
			ScreenFlip();
		}
	}

	ClearDrawScreen();

	return EndFlag;
}

void Scene::displayMyName()
{
	std::string name;

	VECTOR pos = network->getMyChara(name);

	ui->drawString((int)pos.x, (int)pos.y, name, font, 1);
}

void Scene::displayName()
{
	for (int i = 0; i < network->getPlayerNun(); i++)
	{
		if (i != network->getMycone())
		{
			std::string name;

			VECTOR pos = network->getChara(name, i);

			ui->drawString((int)pos.x, (int)pos.y, name, font, 1);
		}
	}
}

void Scene::displayChatToScreen()
{
	if (network->messege_Size() != displaytedsize)
	{
		for (size_t i = displaytedsize; i < network->messege_Size(); i++)
		{
			if (network->MessegeOrigin[i] != -1)
			{
				sowtime.push_back(0);

				playernunbermessege.push_back(network->MessegeOrigin[i]);

				std::string ten(" : ");
				std::string header = network->HandelName[network->MessegeOrigin[i]] + ten;

				std::string tes(network->getMessege(i).substr(header.size(), network->getMessege(i).size() - 1));

				hukidasi.push_back(ui->makeButton(network->getMessege(i).substr(header.size(), network->getMessege(i).size() - 1).c_str(), font));
			}
			else
			{
				sowtime.push_back(0);
				playernunbermessege.push_back(0);
				hukidasi.push_back(0);
			}
		}

		displaytedsize = network->messege_Size();
	}

	if (playernunbermessege.size() > 0)
	{
		if (showmessegenun != displaytedsize)
		{
			for (size_t i = showmessegenun; i < hukidasi.size(); i++)
			{
				if (network->MessegeOrigin[i] != -1)
				{
					if (sowtime[i] != 150)
					{
						if (playernunbermessege[i] == network->getMycone())
						{
							std::string name;
							std::string mess = ui->getButtonString(hukidasi[i]);

							VECTOR pos = network->getMyChara(name);

							ui->drawButton(hukidasi[i], pos.x - GetDrawFormatStringWidthToHandle(ui->getFont(font), name.c_str()) / 2
								- GetDrawFormatStringWidthToHandle(ui->getFont(font), mess.c_str()) / 2, pos.y - 25, 128);
						}
						else
						{
							std::string name;
							std::string mess = ui->getButtonString(hukidasi[i]);

							VECTOR pos = network->getChara(name, playernunbermessege[i]);

							ui->drawButton(hukidasi[i], pos.x - GetDrawFormatStringWidthToHandle(ui->getFont(font), name.c_str()) / 2
								- GetDrawFormatStringWidthToHandle(ui->getFont(font), mess.c_str()) / 2, pos.y - 25, 128);
							
						}

						if (sowtime[i] != 150)sowtime[i]++;
					}
				}
			}
		}
	}
}

void Scene::displayChat()
{
	if (!showchat)
	{
		chatbox = ui->makeBoxRoundedBar(600, 200, 650, 490, GetColor(175,175,175),13882323,36);

		chatinputbox = ui->makeInputBox(300, 30, 60,"",1);

		sousin = ui->makeButton("送信", font);

		showchat = 1;
	}

	ui->updateBox(chatbox);
	ui->updateSize(chatbox, 200, 200, GetColor(175, 175, 175), 13882323, 36);

	ui->drawBox(chatbox, 186);

	ui->drawStringToBox(chatbox, "コメント", ui->getBoxWidth(chatbox) / 2, 2, font, 1);

	if (ui->getBoxWidth(chatbox) - 100 > 100)ui->changeBoxRounded(chatinputbox, ui->getBoxWidth(chatbox) - 100, 30);

	if (inp == 0)
	{
		ui->drawInputBoxToBox(chatbox, chatinputbox, 30, ui->getBoxHeight(chatbox) - 50, font, 128);
	}
	else
	{
		ui->drawInputBoxToBox(chatbox, chatinputbox, 30, ui->getBoxHeight(chatbox) - 50, font, 128, 0);
	}

	ui->drawButtonToBox(chatbox, sousin, 70, 50, 72,1);

	if (ui->updateInputBox(chatinputbox, messe) == 1 && inp == 0 || ui->updateButton(sousin) == 1 && inp == 0)
	{
		if (ui->updateButton(sousin) == 1)messe = std::string(ui->getInputString(chatinputbox));

		network->inputChat(messe);

		ui->setInputString(chatinputbox,"");

		inputi = 1;

		inp = 1;
	}
	
	if (inp == 1 && ui->updateInputBox(chatinputbox, messe) == 0)
	{
		inp = 0;
	}

	int scroll = ui->updateScroll(chatbox,30);

	if (network->messege_Size() > 0)
	{
		if (network->messege_Size() != chatmessegesize)
		{
			for (size_t i = chatmessegesize; i < network->messege_Size(); i++)
			{
				chatmessege.push_back(network->getMessege(i));
			}

			chatmessegesize = network->messege_Size();
		}

		int FontSize;
		GetFontStateToHandle(NULL, &FontSize, NULL, ui->getFont(font));

		int max = ui->getBoxHeight(chatbox) / FontSize - 4;
		if (max < 0)
		{
			max = chatmessege.size();

			scroll = 0;
		}
		else
		{
			if (max + scroll >= chatmessege.size())scroll = chatmessege.size()-max;
			if (scroll < 0)max = chatmessege.size(), scroll=0;
			if (scroll == 0 && follow == 0)if (max <= chatmessege.size())scroll = chatmessege.size() - max, follow = 1;
		}

		for (int i = 0; i < max; i++)
		{
			ui->drawStringToBox(chatbox, chatmessege[i + scroll], 20, 24 + 18 * (i), font,0,-1,75);
		}
	}
}

bool Scene::inputting()
{
	if (!inputi)return 1;
	else return inp;
}

void Scene::yotubeRequest()
{
	if (!yotubeboxinit)
	{
		yotubebox = ui->makeBoxRoundedBar(400, 300, 850, 160, GetColor(175, 175, 175), 13882323, 36);

		yotubeinput = ui->makeInputBox(230, 30, 60, "", 1);

		yotubebottun = ui->makeButton("リクエスト", font);

		yotubeboxinit = 1;
	}

	ui->updateBox(yotubebox);
	ui->updateSize(yotubebox, 300, 200, GetColor(175, 175, 175), 13882323, 36);
	ui->drawBox(yotubebox, 186);

	ui->drawStringToBox(yotubebox, "YouTube", ui->getBoxWidth(yotubebox) / 2, 2, font, 1);

	if (ui->getBoxWidth(yotubebox) - 150 > 100)ui->changeBoxRounded(yotubeinput, ui->getBoxWidth(yotubebox) - 170, 30);
	if (yinp == 0)
	{
		ui->drawInputBoxToBox(yotubebox, yotubeinput, 30, ui->getBoxHeight(yotubebox) - 50, font, 128);
	}
	else
	{
		ui->drawInputBoxToBox(yotubebox, yotubeinput, 30, ui->getBoxHeight(yotubebox) - 50, font, 128, 0);
	}

	if (ui->updateInputBox(yotubeinput, youtubeinm) == 1 && yinp == 0 
		|| ui->updateButton(yotubebottun) == 1 && yinp == 0)
	{
		if (ui->updateButton(yotubebottun) == 1)youtubeinm = std::string(ui->getInputString(yotubeinput));

		if (youtubeinm.find("https://www.youtube.com/watch?") != std::string::npos 
			|| youtubeinm.find("http://www.nicovideo.jp/watch/sm") != std::string::npos)
		{
			time_t timer;
			time(&timer);

			if (RequestTime == 0 || RequestTime + 300 < timer)
			{
				network->sendYoutubeRequest(youtubeinm);

				RequestTime = timer;
			}
			else
			{
				network->YoutubeMessege.push_back("連続でリクエスト出来ません");
			}
			ui->setInputString(yotubeinput, "");
		}
		else
		{
			ui->setInputString(yotubeinput, "");

			network->YoutubeMessege.push_back("URLが間違っています");
		}

		yinp = 1;
	}
	if (yinp == 1 && ui->updateInputBox(yotubeinput, youtubeinm) == 0 && ui->updateButton(yotubebottun) == 0)
	{
		yinp = 0;
	}

	ui->drawButtonToBox(yotubebox, yotubebottun, 130, 50, 72, 1);

	int scroll = ui->updateScroll(yotubebox, 30);

	if (network->YoutubeMessege.size() > 0)
	{
		if (network->YoutubeMessege.size() != youtubemessege.size())
		{
			for (size_t i = youtubemessege.size(); i < network->YoutubeMessege.size(); i++)
			{
				youtubemessege.push_back(network->YoutubeMessege[i]);
			}
		}

		int FontSize;
		GetFontStateToHandle(NULL, &FontSize, NULL, ui->getFont(font));

		int max = 4;
		if (max < 0)
		{
			max = youtubemessege.size();

			scroll = 0;
		}
		else
		{
			if (max + scroll >= youtubemessege.size())scroll = youtubemessege.size() - max;
			if (scroll < 0)max = youtubemessege.size(), scroll = 0;
			if (scroll == 0 && yfollow == 0)if (max <= youtubemessege.size())scroll = youtubemessege.size() - max, yfollow = 1;
		}

		for (int i = 0; i < max; i++)
		{
			ui->drawStringToBox(yotubebox, youtubemessege[i + scroll], 20, 18 * 2 + 18 * (i), font,0,-1,75);
		}
	}

	ui->drawStringToBox(yotubebox, "ログ", 5, 18, font);

}

void Scene::yotubeQuestionnaire(bool Enquete)
{
	time_t timer;
	time(&timer);

	if (network->PlayStartTime != 0)
	{
		if (timer < network->PlayStartTime)
		{
			//再生開始前

			//日本時間に変換
			struct tm gmt;
			errno_t err = localtime_s(&gmt, &timer);

			ui->drawStringToBox(yotubebox, "現在の時刻 : " + std::to_string(gmt.tm_hour) + " : " + std::to_string(gmt.tm_min) + " . " + std::to_string(gmt.tm_sec), 20, 90 + 18, font);
		}
		else
		{
			ui->drawStringToBox(yotubebox, "再生中", 20, 90 + 18, font);

			if (Enquete)
			{
				if (timer > network->PlayStartTime + (t_limitenquetegettime * qtime) - t_substartenquettime && questionnaire == 0)
				{
					questionnaire = 1;

					network->YoutubeMessege.push_back("アンケートを開始します");
				}

				if (questionnaire == 1)
				{
					if (!Question)
					{
						Questionyes = ui->makeButton("はい", font);
						Questionno = ui->makeButton("いいえ", font);
						Question = 1;
					}
					ui->drawStringToBox(yotubebox, "引き続きこの動画を視聴しますか", 20, 90 + 18 + 18, font);

					ui->drawButtonToBox(yotubebox, Questionyes, 20, 144 + 18, 72);
					ui->drawButtonToBox(yotubebox, Questionno, 80, 144 + 18, 72);

					if (ui->updateButton(Questionyes) == 1 && yetq == 0)
					{
						yetq = 1;

						network->YoutubeMessege.push_back("アンケートに賛成しました");
					}
					if (ui->updateButton(Questionno) == 1 && yetq == 0)
					{
						yetq = 1;

						network->sendyotubecommand = 1;

						network->YoutubeMessege.push_back("アンケートに反対しました");
					}

					//集計時間を過ぎた
					if (timer > network->PlayStartTime + (t_limitenquetegettime * qtime))
					{
						if (yetq != 1)yetq = 1;

						//延長しない
						if (network->Extension == 0)
						{
							yetq = 0;

							network->PlayStartTime = 0;

							questionnaire = 0;
							qtime = 1;
						}
						if (network->Extension == 1)
						{
							yetq = 0;
							questionnaire = 0;

							qtime++;
						}

						if (network->Extension ==-1 && network->sendyotubecommand == 0)
						{
							network->sendyotubecommand = 2;
						}
					}
					else
					{
						if (yetq == 0)ui->drawStringToBox(yotubebox, "アンケート終了まであと : " + std::to_string((network->PlayStartTime + (t_limitenquetegettime * qtime)) - timer), 20, 114 + 90, font);
						else ui->drawStringToBox(yotubebox, "アンケート集計まであと : " + std::to_string((network->PlayStartTime + (t_limitenquetegettime * qtime)) - timer), 20, 114 + 90, font);

						if (network->Extension != -1)network->Extension = -1;
					}
				}
			}
		}
	}
}
