#pragma once

#include "Character.h"

#include <vector>
#include <memory>

class MultiPlayers
{
	std::shared_ptr<Bullet_physics> bullet;
	int world;

	std::vector<std::shared_ptr<Character>> players;
	std::vector<unsigned int> playerID;
	std::vector<std::string> names;

	std::vector<float> ts;
	std::vector<VECTOR> d;
	std::vector<float> oldts;
	std::vector<VECTOR> oldd;
	std::vector<float> k;
	std::vector<VECTOR> kd;

	size_t playersSize = 0;
public:
	MultiPlayers(std::shared_ptr<Bullet_physics> bullet_, int world_);
	~MultiPlayers();

	unsigned int findPlayerID(unsigned int PlayerID);

	int addPlayer(unsigned int PlayerID, std::string name);

	int loadModel(unsigned int PlayerID, char *ModelPath);
	int  setModel(unsigned int PlayerID, int ModelHandle, std::string ModelPath);
	int copyModel(unsigned int PlayerID, int ModelHandle);

	int deletePlayer(unsigned int PlayerID);

	int setup(unsigned int PlayerID);

	//全員分のアニメを再生
	void playAnimeControl();

	//全員分の位置を更新
	void update();

	//全員分の描画
	void draw();

	//座標を設定
	int setPosBullet(unsigned int &PlayerID, btVector3 &pos, btQuaternion &q);

	//アニメーションの状態を設定
	void setState(unsigned int PlayerID,std::string &state);

	//方向を設定
	void setDirection(unsigned int PlayerID, VECTOR dir);

	void setTurningSpeed(unsigned int PlayerID, float TurningSpeed);

	VECTOR getPos(unsigned int PlayerID);

	std::string getName(unsigned int PlayerID);
	unsigned int getPlayerID(size_t PlayerNunber);
	size_t getMultiplayerSize();
	void addPlayerSize();
};

