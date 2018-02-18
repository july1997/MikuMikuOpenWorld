#pragma once

#include "DxLib.h"

#include "Common/Model.h"
#include "Bullet_physics.h"

#include <string>
#include <memory>

class Character : public Model
{
	std::shared_ptr<Bullet_physics> bullet;
	int caharacter;
	std::string modelname;

	bool _right = 0, _left = 0, _backward = 0, _forward = 0, _jump = 0, _run = 0;
	bool right = 0, left = 0, backward = 0, forward = 0, jump = 0, run = 0;
	float speed = 0.f;

	VECTOR sub = VGet(0,2.3,0);

	//アニメーション
	int attachAnime(int AnimIndex, int* time, float blend_ = 1.f);
	void blend(int blendAnime, float rate, float time);


	int MoveAnimFrameIndex;
	VECTOR Position;
	bool changeAnime,changing = 0;
	bool fin = 0;

	std::vector<int> anime;
	int totaltime, totaltime2;
	float time = 0,timer2 = 0, timer3 = 0;
	float Rate = 0;
	int nowAni = 0;
	int deley = 0;
	int fintime = 0;
	std::string state = "stand";
	std::string inputstate = "stand";

	float TurningSpeed;

	bool bk = 0;

	bool switchAnime(int before, int after, float afteranimetimer = 0.0f, float rateSpeed = 0.01f, bool fin_ = 1);
	bool atta = 0;
	int shifttimer = 0;
	int dance = 0;

	btVector3 direction;
	void stateManager();

	bool fadein = 0;
	float fadeinRate = 0.f;
public:
	Character(std::shared_ptr<Bullet_physics> bullet_, int world);
	~Character();


	virtual void loadModel(const char *FileName);

	void setup();

	void update();

	void playerMovementKeyboard();
	void noMovement();

	void playAnime();

	void animeControl();

	void debug();

	VECTOR gertDirection();
	void setDirection(VECTOR dir);

	std::string getState();
	void setState(std::string state_);

	int setPosRotBullet(btVector3 &pos, btQuaternion &q);
	VECTOR getPosBullet();
	btQuaternion getRotBullet();

	float getTurningSpeed();
	void setTurningSpeed(float TurningSpeed);
};

