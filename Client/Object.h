#pragma once

#include "Common/Model.h"
#include "Bullet_physics.h"


#include <vector>
#include <memory> 

//物理演算しないためにはModel.hをお使いください
class Object : public Model
{
protected:

	Bullet_physics *bullet;
	int world;
	std::vector<VECTOR> forms;
	std::vector<int> bodyhandel;

	size_t objects = 0;

	std::vector<std::shared_ptr<Model>> models;

	VECTOR testingpos = VGet(0,0,0), testingrot = VGet(0,0,0);

public:
	Object(Bullet_physics *_bullet, int _world);

	//モデルのメッシュから剛体を作成
	size_t loadModel(const char *FileName, float scale);

	size_t loadModel(const char *FileName, VECTOR Form, float scale);

	size_t deleteModel(int object);

	size_t copyModel(int object, float scale = 1.0f);

	int setPos(int object, VECTOR pos);

	int setRot(int object, VECTOR rot);

	int setScale(int object, float scale, float mass = 1);

	virtual int getModelHandle(int object);

	VECTOR getPos(int object);

	VECTOR getRot(int object);
	
	//オブジェクトの位置をキーボードで移動できるようにします
	void testingPositioning(int object);

	//全モデルの位置を更新
	void update();

	//モデルを全て描画
	virtual void draw();
};

/*
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

	ObjectManager::ObjectManager(Bullet_physics *_bullet, int _world)
	{
		bullet = _bullet;

		world = _world;
	}

	void addObject(size_t type,VECTOR Form)
	{
		if (type == 0)
		{
			object.push_back(std::shared_ptr<Object>(new Object(bullet, world)));

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
*/