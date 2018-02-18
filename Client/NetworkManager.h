#pragma once

#include "NetWork.h"

#include "Common/Fps.h"

#include "Character.h"
#include "MultiPlayers.h"

class NetworkManager
{
public:
	NetworkManager(std::shared_ptr<Bullet_physics> bullet_, int world_, std::shared_ptr<Character> cahara_);
	~NetworkManager();

	bool connect();
	bool login(std::string mail, std::string pass);
	bool signUp(std::string mail, std::string pass, std::string nane);

	bool CorrectVersion = 1;

	int getid();
	string getname();

	void startSend();

	string oldState = "";
	bool sendDir = 0;

	size_t getChatMassegeSize();
	std::string getChatMassege();

	void multiplayerDraw();
	void multiplayerUpdate();

	bool startCommunication();

	size_t getMultiplayerSize();

	std::shared_ptr<Character> cahara;
	MultiPlayers *players;

	int sendChat(string str);
private:
	std::string coneection = std::string("118.27.12.173");

	bool cpt = 0;
	vector<string> chatmessege;

	Network network;

	std::thread t;
	std::thread t2;

	void setCorrect(bool c);

	void update();

	void sendLoop();

	std::vector<std::string> chat;
};

