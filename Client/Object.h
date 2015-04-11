#pragma once

#include "Model.h"
#include "Bullet_physics.h"
#include "Car.h"

#include <vector>
#include <memory> 

//現在のclass Objectの利点：複製が容易

//bulletとの連携
//配置するときの便利ビュー
class Object : public Model
{
protected:

	std::vector<int> modelhandles;

	float parentmodelscale = 1;

public:
	Object();

	void copyModel(int copyNum);

	void copiedsetPos(int copiedhandle, VECTOR pos);

	void copiedsetRot(int copiedhandle, VECTOR rot);

	void copiedsetScale(int copiedhandle,float scale);

	int getCopyModel(int copiedhandle);

	virtual void setScale(float scale);

	virtual void draw();
};

//オブジェクトの管理
class ObjectManager
{
	Bullet_physics *bullet;
	int world;

	std::vector<std::shared_ptr<Object>> object;
	size_t objects = 0;

	std::vector<int> bodyhandel;

	size_t cars = 0;
	std::vector<std::shared_ptr<Car>> car;

	std::vector<size_t> Ownership;

public:

	ObjectManager(Bullet_physics *_bullet,int _world);

	void addObject(size_t type,VECTOR Form)
	{
		if (type == 0)
		{
			object.push_back(std::shared_ptr<Object>(new Object));

			object[objects]->loadModel("dice.mv1");
			object[objects]->setScale(10.f);

			bodyhandel.push_back(bullet->createBoxBody(world, Form));

			Ownership.push_back(0);

			objects++;
		}
		else if (type == 1)
		{
			car.push_back(std::shared_ptr<Car>(new Car(bullet,world)));

			car[cars]->loadModel("model/日産・スカイライン GT-R VスペックII Nur BNR34後期型/日産・スカイライン GT-R VスペックII Nur BNR34後期型.mv1");

			bodyhandel.push_back(-1);

			Ownership.push_back(0);

			cars++;
		}
	}

	void update()
	{
		for (size_t i = 0; i < object.size(); i++)
		{
			object[i]->setPos(bullet->getBodyPos(bodyhandel[i]));
			object[i]->setRot(bullet->getBodyRot(bodyhandel[i]));
		}

		for (size_t i = 0; i < car.size(); i++)
		{
			car[i]->update();
		}
	}

	void draw()
	{
		for (size_t i = 0; i < object.size(); i++)
		{
			object[i]->draw();
		}

		for (size_t i = 0; i < car.size(); i++)
		{
			car[i]->draw();
		}
	}

	void setPosition(size_t objects, VECTOR position, btQuaternion &rotation)
	{
		if (bodyhandel[objects] == -1)
		{
			car[objects]->setCarBodyPos(position, rotation);
		}
		else
		{
			bullet->getbtRigidBody(bodyhandel[objects])->setWorldTransform(btTransform(rotation, btVector3(position.x, position.y, position.z)));
		}
	}

	void drive(size_t cars, float _Engine, float _Breaking, float _Steering)
	{
		car[cars]->setDrive(_Engine, _Breaking, _Steering);
	}

	VECTOR getCarPos(size_t cars)
	{
		return car[cars]->getPos();
	}

	btQuaternion getCarRot(size_t cars)
	{
		return car[cars]->getPosQuaternion();
	}

	VECTOR getCarCommand(size_t cars)
	{
		return car[cars]->getCommnad();
	}

	bool getOwnership(size_t objects)
	{
		return Ownership[objects];
	}

	void setOwnership(size_t objects, size_t flag)
	{
		Ownership[objects] = flag;
	}

	size_t carSize()
	{
		return car.size();
	}

	void driveCar(size_t cars)
	{
		car[cars]->drive();
	}

	float getSpeed(size_t cars)
	{
		return car[cars]->getSpeed();
	}

	std::shared_ptr<Car> getCar(size_t cars)
	{
		return car[cars];
	}
};