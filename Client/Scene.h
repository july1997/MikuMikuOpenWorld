#pragma once

#include "NetworkManager.h"

#include "DxLib.h"

#include "UI/UI.h"
#include "Common/Fps.h"
#include "Updater.h"

#include <regex>

class Scene
{
public:
	Scene(std::shared_ptr<NetworkManager> network_);
	~Scene();

	bool login();

	bool chat();

	void drawName();

	void tutorial();

	void loading();

	bool update();

private:

	bool connect();

	int regist(std::string mail, std::string pass);

	bool illegalChara(std::string str);
	bool Specialsymbol(std::string str);
	bool Invalidname(std::string str);

	UI ui;
	int font, font2, font3;

	FpsManager fps;

	std::shared_ptr<NetworkManager> network;

	std::vector<std::string> chatmessege;
	int chatbox, chatinputbox, sousin;
	bool load = 0, inp = 0,  follow = 0;
	string messe;

	bool tutorialf = 0, tutoriall = 0;
	int tutorialbox;

	bool step = 0;

	Updater updater;
};

