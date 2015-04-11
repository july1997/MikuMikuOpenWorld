#pragma once

#include "Model.h"
#include "Bullet_physics.h"

#include <vector>
#include <mutex>

class NetworkPlayer : public Model
{
	std::vector<std::vector<bool>> commnad;
	std::vector<VECTOR> pos;
	std::vector<VECTOR> rot;

	std::vector<int> modelhandles;

	std::vector<int> controller;
	Bullet_physics *bullet;
	int world;

	std::vector<int> animehandle;
	std::vector<float> animetotaltime;
	std::vector<float> animetime;
	std::vector<int> blendanimehandle;
	std::vector<float> rate;

	int playernun = 0;

	std::vector<int> deleteplayernunber;

	std::mutex mtx;

public:
	std::vector<bool> brendflag;
	std::vector<int> nowanimeindex;

	int myconnection = 0;

	std::vector<bool> Ownership;

	NetworkPlayer(Bullet_physics *bullet_, int world_);

	virtual void loadModel(const char *FileName);
	virtual void setModel(int &ModelHandel);


	void networkSetCommand(size_t playernun, int commandnun, bool _command);

	void networkSetPosRot(int playernun, VECTOR _pos, btQuaternion &_rot);

	VECTOR networkGetPos(int playernun);

	VECTOR networkGetRot(int playernun);

	size_t getModelHandlesSize();


	bool networkGetCommand(int playernun, int commandnun);

	bool networkGetpos(int playernun, int commandnun);

	bool networkGetRot(int playernun, int commandnun);

	
	void myConnect();

	void addPlayer(const char *FileNam);

	virtual void draw(int playernun);

	void controlPlayer(int playernun);

	void updateVec(int playernun);


	void attachAnime(int playernun, int AnimIndex);

	void setAnimeTime(int playernun, float time = 0.f);

	void playAnime(int playernun, float playspeed = 1.f);

	bool switchAnime(int playernun, int AnimIndex, float uprate = 0.125f);


	void drawPlayers();

	void controlPlayer();

	void updatePlayerVec();


	void deletePlayer(int playernun);

};

