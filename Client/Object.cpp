#include "Object.h"

Object::Object(Bullet_physics *_bullet, int _world)
{
	bullet = _bullet;

	world = _world;
}

size_t Object::loadModel(const char *FileName, float scale)
{
	models.push_back(std::shared_ptr<Model>(new Model));

	models[objects]->loadModel(FileName);

	models[objects]->setScale(scale);

	bodyhandel.push_back(bullet->createBodytoMesh2(models[objects]->getModelHandle(), world));

	forms.push_back(VGet(0,0,0));
	objects++;

	return objects - 1;
}

size_t Object::loadModel(const char *FileName, VECTOR Form, float scale)
{
	models.push_back(std::shared_ptr<Model>(new Model));

	models[objects]->loadModel(FileName);

	models[objects]->setScale(scale);

	bodyhandel.push_back(bullet->createBoxBody(world, Form));
	forms.push_back(Form);

	objects++;

	return objects - 1;
}

size_t Object::deleteModel(int object)
{
	models[objects]->~Model();

	models.erase(models.begin() + object);

	forms.erase(forms.begin() + object);

	bullet->removeRigidBody( world, bodyhandel[object]);

	bodyhandel.erase(bodyhandel.begin() + object);

	objects--;

	return 0;
}

size_t Object::copyModel(int object, float scale)
{
	models.push_back(std::shared_ptr<Model>(new Model));

	int cm = MV1DuplicateModel(models[object]->getModelHandle());

	models[objects]->setModel(cm);
	
	if(scale != 1.0f)models[objects]->setScale(scale);

	if (forms[object].x != 0 && forms[object].y != 0 && forms[object].z != 0)
	{
		bodyhandel.push_back(bullet->createBoxBody(world, forms[object]));

		forms.push_back(forms[object]);
	}
	else
	{
		bodyhandel.push_back(bullet->createBodytoMesh2(models[objects]->getModelHandle(), world));

		forms.push_back(VGet(0, 0, 0));
	}

	objects++;

	return objects - 1;
}

int Object::setPos(int object, VECTOR pos)
{
	models[object]->setPos(pos);

	btTransform bt = bullet->getbtRigidBody(bodyhandel[object])->getWorldTransform();

	bt.setOrigin(bullet->btvDxv(pos));

	bullet->getbtRigidBody(bodyhandel[object])->setWorldTransform(bt);

	return 0;
}
int Object::setRot(int object, VECTOR rot)
{
	models[object]->setRot(rot);

	btTransform bt = bullet->getbtRigidBody(bodyhandel[object])->getWorldTransform();

	bt.setRotation(btQuaternion(rot.x, rot.y, rot.z, 1));

	bullet->getbtRigidBody(bodyhandel[object])->setWorldTransform(bt);

	return 0;
}
int Object::setScale(int object, float scale,float mass)
{
	if (object < models.size())
	{
		if (forms[object].x != 0 && forms[object].y != 0 && forms[object].z != 0)
		{
			models[object]->setScale(scale);

			btCollisionShape* boxshape = new btBoxShape(bullet->btvDxv(VScale(forms[object], scale)));
			btVector3 test = bullet->btvDxv(VGet(0.f, 0.f, 0.f));
			boxshape->calculateLocalInertia(btScalar(mass), test);//inertia@Šµ«
			bullet->getbtRigidBody(bodyhandel[object])->setCollisionShape(boxshape);

			return 0;
		}
		else
		{
			models[objects]->setScale(scale);

			return 0;
		}
	}
	else
	{
		return - 1;
	}
}

int Object::getModelHandle(int object)
{
	return models[object]->getModelHandle();
}

VECTOR Object::getPos(int object)
{
	return models[object]->getPos();
}

VECTOR Object::getRot(int object)
{
	return models[object]->getRot();
}

void Object::update()
{
	for (size_t i = 0; i < models.size(); i++)
	{
		models[i]->setPos(bullet->getBodyPos(bodyhandel[i]));
		models[i]->setRot(bullet->getBodyRot(bodyhandel[i]));
	}
}

void Object::draw()
{
	for (size_t i = 0; i < models.size(); i++)
	{
		models[i]->draw();
	}
}

void Object::testingPositioning(int object)
{
	if (CheckHitKey(KEY_INPUT_LCONTROL) == 0)
	{
		if (CheckHitKey(KEY_INPUT_UP) != 0)
		{
			if (CheckHitKey(KEY_INPUT_LSHIFT) != 0)
			{
				testingpos.y += 1.f;
			}
			else
			{
				testingpos.z += 1.f;
			}
		}
		if (CheckHitKey(KEY_INPUT_DOWN) != 0)
		{
			if (CheckHitKey(KEY_INPUT_LSHIFT) != 0)
			{
				testingpos.y -= 1.f;
			}
			else
			{
				testingpos.z -= 1.f;
			}
		}
		if (CheckHitKey(KEY_INPUT_RIGHT) != 0)
		{
			testingpos.x += 1.f;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) != 0)
		{
			testingpos.x -= 1.f;
		}
	}
	else
	{
		if (CheckHitKey(KEY_INPUT_UP) != 0)
		{
			testingrot.x += 1.f;
		}
		if (CheckHitKey(KEY_INPUT_DOWN) != 0)
		{
			testingrot.x -= 1.f;
			
		}
		if (CheckHitKey(KEY_INPUT_RIGHT) != 0)
		{
			if (CheckHitKey(KEY_INPUT_LSHIFT) != 0)
			{
				testingrot.z -= 1.f;
			}
			else
			{
				testingrot.y += 1.f;
			}
		}
		if (CheckHitKey(KEY_INPUT_LEFT) != 0)
		{
			if (CheckHitKey(KEY_INPUT_LSHIFT) != 0)
			{
				testingrot.z += 1.f;
			}
			else
			{
				testingrot.y -= 1.f;
			}
		}
	}
	setPos(object, testingpos);
	setRot(object, VGet(testingrot.x * DX_PI_F / 180.0f, testingrot.y * DX_PI_F / 180.0f, testingrot.z * DX_PI_F / 180.0f));

	DrawFormatString(0, 0, GetColor(255, 255, 255), "object = %d Position = x=%f y=%f z=%f  Rotation = x=%f y=%f z=%f", 
		object,testingpos.x, testingpos.y, testingpos.z, testingrot.x, testingrot.y, testingrot.z);
}