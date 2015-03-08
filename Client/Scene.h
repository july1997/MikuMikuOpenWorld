#pragma once

#include <vector>
#include "Network.h"
#include "UI.h"

class Scene
{
	UI *ui;
	Network *network;
	
	int font, bigfont;

	bool EndFlag = 0;

	std::string messe;
	int chatbox, chatinputbox, sousin;
	bool showchat = 0, inp = 0,inputi=0;
	std::vector<std::string> chatmessege;
	size_t chatmessegesize = 0; bool follow = 0;


	bool yotubeboxinit = 0,yinp=0; std::string youtubeinm;
	int yotubebox,yotubeinput,yotubebottun;
	std::vector<std::string> youtubemessege;
	bool yfollow = 0; bool Question = 0;
	int Questionyes = 0, Questionno=0;
	bool questionnaire = 0, yetq=0;
	int qtime = 1;
	time_t RequestTime = 0;
	//アンケートの収集を開始する時間（秒)(t_startenquettime*回数 -t_substartenquettime)
	const int t_startenquettime = 300;
	const int t_substartenquettime = 60;
	//アンケートを収集する時間（秒）
	const int t_limitenquetegettime = 300;

	size_t displaytedsize = 0;
	std::vector<int> hukidasi, playernunbermessege, sowtime;
	int showmessegenun = 0;

public:
	Scene(Network *network_);

	bool Login();

	//終了確認する
	bool exitConfirm();

	void displayMyName();
	void displayName();

	void displayChat();
	bool inputting();

	void displayChatToScreen();

	void yotubeRequest();
	void yotubeQuestionnaire(bool Enquete = 1);

};

