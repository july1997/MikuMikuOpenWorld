#include "NetworkManager.h"



NetworkManager::NetworkManager(std::shared_ptr<Bullet_physics> bullet_, int world_, std::shared_ptr<Character> cahara_) : players(new MultiPlayers(bullet_, world_))
{
	cahara = cahara_;
}


NetworkManager::~NetworkManager()
{
}


bool NetworkManager::connect()
{
	if (network.connect(coneection) != -1 && network.makeUDPSocket(coneection) != -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool NetworkManager::startCommunication()
{
	network.setid(0);

	if (network.send(1, "GET", "", 0, 0) != -1) {

		network.receive_start();
		network.UDP_receive_start();

		t = std::thread(&NetworkManager::update, this);
		t.detach();

		return 1;
	}

	return 0;
}

bool NetworkManager::login(std::string mail, std::string pass)
{
	network.setid(0);

	std::string s(mail + " " + pass);

	network.send(3, "LOGIN", s, 0, 1, 0, 1);

	return 1;
}

bool NetworkManager::signUp(std::string mail, std::string pass, std::string nane)
{
	network.setid(0);

	std::string s(mail + " " + pass + " " + nane);

	network.send(3, "REGST", s, 0, 1, 0, 1);
	network.setname(nane);

	return 1;
}

void NetworkManager::update()
{
	FpsManager fps;

	//別スレッドで立ち上げ 60fps
	while (!ProcessMessage())
	{

		if (network.getMassegeSize() > 0)//tcp
		{
			string str = network.getMassege();

			const unsigned char command = str[0];

			if (command == (unsigned char)1) {//Plane

				string mess = str.substr(1, str.find(" "));
				str.erase(0, mess.length() + 1);

				if (mess == "KEY ") {
					if (!cpt) {
						//rsa公開鍵を受け取る
						network.getRsaPublicKey(str);

						//aes共通鍵を暗号化して送る
						network.makeAESKey();

						network.setid(1);
						cpt = 1;
					}
					else
					{
						MessageBox(GetMainWindowHandle(), "不明なエラー", "error", MB_OK);
						break;
					}
				}

			}
			else if (command == (unsigned char)2) {//RSA

				string mess = str.substr(1, str.find(" "));
				str.erase(0, mess.length() + 1);

				if (mess == "SIGN ") {
					if (!network.signature(str)) {
						setCorrect(0);
						break;
					}
				}
			}
			else if (command == (unsigned char)3) {//AES

				string mess = str.substr(1, str.find(" "));
				str.erase(0, mess.length() + 1);

				//暗号を解く
				string dstr(str);
				network.decryptionByAes(dstr);


				if (mess == "LOGIN ") {
					string result = dstr.substr(0, dstr.find(" "));
					dstr.erase(0, result.length() + 1);

					if (result == "SUCCESS") {
						string id = dstr.substr(0, dstr.find(" "));
						dstr.erase(0, id.length() + 1);
						string name = dstr.substr(0, dstr.find(" "));
						dstr.erase(0, name.length() + 1);

						network.setid(stoi(id));
						network.setname(name);
					}
					else if (result == "FAILURE") network.setid(-1);
					else if (result == "MISTAKE_TOO_MUCH")network.setid(-2);
				}
				else if (mess == "REGST ") {
					string result = dstr.substr(0, dstr.find(" "));
					dstr.erase(0, result.length() + 1);
					string id = dstr.substr(0, dstr.find(" "));
					dstr.erase(0, id.length() + 1);
					if (result == "SUCCESS") {

						network.setid(stoi(id));
					}
					else if (result == "FAILURE") network.setid(-1);
					else if (result == "MISTAKE_TOO_MUCH")network.setid(-2);
				}
				else if (mess == "SETPOS ") {
					float t[7];
					if (dstr.find(" ") != string::npos) {
						for (int i = 0; i < 7; i++) {
							t[i] = stod(dstr.substr(0, dstr.find(" ")));
							dstr.erase(0, dstr.find(" ") + 1);
						}
					}

					btVector3 bt = btVector3(t[0], t[1], t[2]);
					btQuaternion bq = btQuaternion(t[3], t[4], t[5], t[7]);
					cahara->setPosRotBullet(bt, bq);

				}
				else if (mess == "LIST ") {
					try {
						std::vector<unsigned int> usrid;
						while (1) {

							string userid = dstr.substr(0, dstr.find(" "));
							dstr.erase(0, userid.length() + 1);
							unsigned int uid = stoi(userid);
							usrid.push_back(uid);

							if (uid == 0) {//誰もいない
								for (int j = 0; j < players->getMultiplayerSize(); j++) {
									players->deletePlayer(players->getPlayerID(j));
								}
							}
							else{
								if (players->findPlayerID(uid) == -1)
								{
									string name = dstr.substr(0, dstr.find(" "));
									dstr.erase(0, name.length() + 1);
									players->addPlayer(uid, name);
									players->copyModel(uid, cahara->getModelHandle());
									players->setup(uid);
									players->addPlayerSize();

									float t[7];
									for (int i = 0; i < 7; i++) {
										t[i] = stod(dstr.substr(0, dstr.find(" ")));
										dstr.erase(0, dstr.find(" ") + 1);
									}
									btVector3 bt = btVector3(t[0], t[1], t[2]);
									btQuaternion bq = btQuaternion(t[3], t[4], t[5], t[6]);

									players->setPosBullet(uid, bt, bq);

									break;
								}
								else {
									dstr.erase(0, dstr.find(" ") + 1);
									dstr.erase(0, dstr.find(" ") + 1);
									dstr.erase(0, dstr.find(" ") + 1);
									dstr.erase(0, dstr.find(" ") + 1);
									dstr.erase(0, dstr.find(" ") + 1);
									dstr.erase(0, dstr.find(" ") + 1);
									dstr.erase(0, dstr.find(" ") + 1);
									dstr.erase(0, dstr.find(" ") + 1);
								}
							}
							if (dstr == " ")break;

						}

						if (usrid.size() != 0) {
							for (int i = 0; i < players->getMultiplayerSize(); i++) {
								bool find = 0;
								size_t findpos = 0;
								for (int j = 0; j < usrid.size(); j++) {
									if (players->getPlayerID(i) == usrid[j])find = 1;
								}

								if (!find) {
									players->deletePlayer(usrid[i]);
								}
							}
						}
					}
					catch (...) {
					}
				}
				else if (mess == "POS ") {
					try {
						string userid = dstr.substr(0, dstr.find(" "));
						dstr.erase(0, userid.length() + 1);

						unsigned int uid = stoi(userid);

						if (players->findPlayerID(uid) != -1)
						{
							float t[7];
							for (int i = 0; i < 7; i++) {
								t[i] = atof(dstr.substr(0, dstr.find(" ")).c_str());
								dstr.erase(0, dstr.find(" ") + 1);
							}

							btVector3 bt = btVector3(t[0], t[1], t[2]);
							btQuaternion bq = btQuaternion(t[3], t[4], t[5], t[6]);
							players->setPosBullet(uid, bt, bq);
						}
					}
					catch (...) {
					}
				}
				else if (mess == "RES ") {
					string userid = dstr.substr(0, dstr.find(" "));
					dstr.erase(0, userid.length() + 1);

					unsigned int uid = stoi(userid);

					if (players->findPlayerID(uid) != -1)
					{
						chat.push_back(players->getName(uid) + " : " + dstr);
					}
					else if (uid == 0) {
						chat.push_back(u8"システム : " + dstr);
					}
				}
				else if (mess == "STATE ") {
					try {

						string userid = dstr.substr(0, dstr.find(" "));
						dstr.erase(0, userid.length() + 1);
						unsigned int uid = stoi(userid);

						if (players->findPlayerID(uid) != -1)
						{
							players->setState(uid, dstr);
						}
					}
					catch (...) {
					}
				}
				else if (mess == "DIR ") {
					try {
						string userid = dstr.substr(0, dstr.find(" "));
						dstr.erase(0, userid.length() + 1);
						unsigned int uid = stoi(userid);

						if (players->findPlayerID(uid) != -1)
						{
							string dir1 = dstr.substr(0, dstr.find(" "));
							dstr.erase(0, dir1.length() + 1);
							string dir2 = dstr.substr(0, dstr.find(" "));
							dstr.erase(0, dir2.length() + 1);
							string dir3 = dstr.substr(0, dstr.find(" "));
							dstr.erase(0, dir3.length() + 1);
							VECTOR D = VGet(stod(dir1), stod(dir2), stod(dir3));

							players->setTurningSpeed(uid, stod(dstr));
							players->setDirection(uid, D);
						}
					}
					catch (...) {
					}
				}
			}
			else if (str.substr(0, 6) == "SYSTEM") {

				str.erase(0, str.find(" ") + 1);

				chat.push_back(u8"システム : " + network.convertUTF8(str));
			}

		}
		fps.controlWaitFps();
	}
}

void NetworkManager::setCorrect(bool c) {
	CorrectVersion = c;
}

int NetworkManager::getid()
{
	return network.getid();
}
string NetworkManager::getname()
{
	return network.getname();
}

void NetworkManager::sendLoop()
{
	FpsManager fps;
	size_t counter = 0;

	//別スレッドで立ち上げ 60fps
	while (!ProcessMessage())
	{

		if (counter % 30 == 0)
		{
			//direction
			VECTOR d = cahara->gertDirection();
			float ts = cahara->getTurningSpeed();
			if (VSize(d) != 0) {
				string dir1 = to_string(d.x);
				string dir2 = to_string(d.y);
				string die3 = to_string(d.z);
				string sts = to_string(ts);

				string s = (dir1.substr(0, dir1.size() - 1) + " " + dir2.substr(0, dir2.size() - 1) + " " + die3.substr(0, die3.size() - 1) + " " + sts.substr(0, sts.size() - 1));
				network.send(3, "DIR", s, 0, 1, 0, 1);
				sendDir = 0;
			}
			else {
				if (!sendDir) {
					string s = (to_string(d.x) + " " + to_string(d.y) + " " + to_string(d.z) + " " + to_string(ts));
					network.send(3, "DIR", s, 0, 1, 0, 1);
					sendDir = 1;
				}
			}
		}

		string state = cahara->getState();
		if (state != oldState)
		{
			//state
			oldState = state;
			network.send(3, "STATE", state, 0, 1, 0, 1);
		}

		if (counter >= 300)
		{
			//position
			string posi1 = to_string((int)cahara->getPosBullet().x);
			string posi2 = to_string((int)cahara->getPosBullet().y);
			string posi3 = to_string((int)cahara->getPosBullet().z);
			string posi4 = to_string(cahara->getRotBullet().getX());
			posi4 = posi4.substr(0, posi4.find('.') + 4);
			string posi5 = to_string(cahara->getRotBullet().getY());
			posi5 = posi5.substr(0, posi5.find('.') + 4);
			string posi6 = to_string(cahara->getRotBullet().getZ());
			posi6 = posi6.substr(0, posi6.find('.') + 4);
			string posi7 = to_string(cahara->getRotBullet().getW());
			posi7 = posi7.substr(0, posi7.find('.') + 4);

			std::string s(posi1 + " " + posi2 + " " + posi3 + " " + posi4 + " " + posi5 + " " + posi6 + " " + posi7 + " ");
			network.send(3, "POS", s, 0, 1, 0, 1);

			counter = 0;
		}

		counter++;


		fps.controlWaitFps();
	}
}

void NetworkManager::startSend()
{
	t2 = std::thread(&NetworkManager::sendLoop, this);
	t2.detach();
}

size_t NetworkManager::getChatMassegeSize()
{
	return chat.size();
}

std::string NetworkManager::getChatMassege()
{
	string m = chat[0];
	chat.erase(chat.begin());

	return m;
}

void NetworkManager::multiplayerDraw()
{
	players->draw();
}

void NetworkManager::multiplayerUpdate()
{
	players->playAnimeControl();

	players->update();
}

size_t NetworkManager::getMultiplayerSize()
{
	return players->getMultiplayerSize();
}

int NetworkManager::sendChat(string str)
{
	return network.send(3, "RES", str, 0, 1, 0, 1);
}