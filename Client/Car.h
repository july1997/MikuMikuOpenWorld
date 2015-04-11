#pragma once

#include "Model.h"
#include "Bullet_physics.h"

class Car : public Model
{
	Bullet_physics *bullet;

	int carbody, carhandel;

	float gEngineForce = 0.f;
	float gBreakingForce = 0.f;
	float gVehicleSteering = 0.f;

	const float accel = 1000.f;
	const float breaking = 100.f;
	const float back = 500.f;

	const float steeringIncrement = 0.005f;
	const float steeringClamp = 0.35f;

	int move=0;
	size_t movecounter = 0;

public:
	Car(Bullet_physics *bullet_, int world);
	~Car();

	void drive();

	void update();

	void setCarBodyPos(VECTOR position, btQuaternion &rotation);

	void setDrive(float _Engine, float _Breaking, float _Steering);

	btQuaternion getPosQuaternion();

	VECTOR getCommnad();

	float getSpeed();
};

