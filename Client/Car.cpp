#include "Car.h"

Car::Car(Bullet_physics *bullet_, int world)
{
	bullet = bullet_;

	carhandel = bullet->createCar(&carbody, world, VGet(10.0f, 7.5f, 30.0f), VGet(60, 10, 0), 100.f);
}

Car::~Car()
{
}

void Car::drive()
{ 
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		gVehicleSteering += steeringIncrement;
		if (gVehicleSteering > steeringClamp)gVehicleSteering = steeringClamp;
	}
	else
	{
		if (CheckHitKey(KEY_INPUT_LEFT) == 1)
		{
			gVehicleSteering -= steeringIncrement;
			if (gVehicleSteering < -steeringClamp)gVehicleSteering = -steeringClamp;
		}
		else
		{
			if (gVehicleSteering != 0)
			{
				if (gVehicleSteering < 0)
				{
					gVehicleSteering += steeringIncrement*5;
				}
				else
				{
					gVehicleSteering -= steeringIncrement*5;
				}
				if (gVehicleSteering < 0.1&&gVehicleSteering > -0.1)gVehicleSteering = 0.f;
			}
		}
	}

	if (move == 0 && CheckHitKey(KEY_INPUT_UP) == 1)
	{
		move = 1;
	}

	if (move == 0 && CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		move = 2;
	}

	if (move == 0 && CheckHitKey(KEY_INPUT_B) == 1)
	{
		move = 3;
	}

	if (move != 0)
	{
		switch (move)
		{
		case 1:
			gEngineForce = accel;
			gBreakingForce = 0.f;
			break;
		case 2:
			gBreakingForce = breaking;
			gEngineForce = 0.f;
			break;
		case 3:
			gEngineForce = -back;
			gBreakingForce = 0.f;
			break;
		}

		movecounter++;
		if (movecounter >= 30)move = 0, movecounter = 0;
	}
	else
	{
		gEngineForce = 0.f;
		gBreakingForce = 0.f;
	}

	bullet->controlCar(carhandel, gEngineForce, gBreakingForce, gVehicleSteering);

	bullet->controlWheel(carhandel, modelhandle, 62, 56, 74, 69);
}

void Car::update()
{
	setPos(bullet->getBodyPos(carbody, VGet(0, 10, 0)));

	setRot(bullet->getBodyRot(carbody));
}

void Car::setCarBodyPos(VECTOR position, btQuaternion &rotation)
{
	bullet->getbtRigidBody(carbody)->setWorldTransform(btTransform(rotation, btVector3(position.x, position.y, position.z)));
}

void Car::setDrive(float _Engine, float _Breaking, float _Steering)
{
	bullet->controlCar(carhandel, _Engine, _Breaking, _Steering);

	bullet->controlWheel(carhandel, modelhandle, 62, 56, 74, 69);
}

btQuaternion Car::getPosQuaternion()
{
	return bullet->getbtRigidBody(carbody)->getWorldTransform().getRotation();
}

VECTOR Car::getCommnad()
{
	return VGet(gEngineForce, gBreakingForce, gVehicleSteering);
}

float Car::getSpeed()
{
	return  bullet->getbtRaycastVehicle(carhandel)->getCurrentSpeedKmHour();
}