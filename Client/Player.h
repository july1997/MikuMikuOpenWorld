#pragma once

#include "Model.h"
#include "Bullet_physics.h"

#include <string>
#include <vector>

//プレーヤーの操作やアニメーション

class Player : public Model
{
protected:

	int MouseX , MouseY ;
	bool moving = 0, movementmouseflag = 0;
	VECTOR targetpos;
	Model pointermodel;
	int counter = 0;

	int animehandle;
	float animetotaltime;

	float animetime=0.f;

	std::string modelname;

	int blendanimehandle;
	float rate = 0.f;

	Bullet_physics *bullet;
	int caharacter;

	bool w = 0;
	int runcounter = 0;

public:
	
	bool right, left, backward, forward, jump, run;

	Player(Bullet_physics *bullet_, int world);

	bool brendflag = 0;
	int nowanimeindex;

	int movecounter = 0;
	bool moveflag = 0, movestart = 0;

	virtual void loadModel(char *FileName);
	
	//使用できません
	virtual void setModel(){}

	void attachAnime(int AnimIndex);

	void setAnimeTime(float time = 0.f);

	void playAnime(float playspeed = 1.f);

	bool switchAnime(int AnimIndex, float uprate = 0.125f);

	void playerMovementKeyboard();

	void setPlayerMovement(bool left_, bool right_, bool backward_, bool forward_, bool jump_, bool run_);

	void update();

	VECTOR getPosBullet();
	btQuaternion getRotBullet();

	//マウスでクリックされたところに移動する
	bool playerMovementMouse(int stagemodel);

	std::string getModelFileName();
};

