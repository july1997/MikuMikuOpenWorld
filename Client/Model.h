#pragma once

//�L�����N�^�[�ANPC�A��蕨�Ȃǂ̊��N���X

#include "DxLib.h"
#include <math.h>

class Model
{
protected:

	int modelhandle;

public:
	//���z�f�X�g���N�^
	virtual ~Model(){};

	virtual void loadModel(const char *FileName)
	{
		modelhandle = MV1LoadModel(FileName);
	}
	virtual void setModel(int &ModelHandel)
	{
		modelhandle = ModelHandel;
	}

	virtual void setPos(VECTOR pos)
	{
		MV1SetPosition(modelhandle, pos);
	}
	virtual void setRot(VECTOR rot)
	{
		MV1SetRotationXYZ(modelhandle, rot);
	}
	virtual void setScale(float scale)
	{
		MV1SetScale(modelhandle, VGet(scale, scale, scale));
	}

	VECTOR getPos()
	{
		return MV1GetPosition(modelhandle);
	}
	VECTOR getRot()
	{
		return MV1GetRotationXYZ(modelhandle);
	}

	virtual void draw()
	{
		MV1DrawModel(modelhandle);
	}

	int getModelHandle()
	{
		return modelhandle;
	}

	float calculation_distance(VECTOR v, VECTOR v2)
	{//�x�N�g���̋����̓�����v�Z����
		float distance_f = pow((v.x - v2.x), 2) + pow((v.y - v2.y), 2) + pow((v.z - v2.z), 2);
		return distance_f;
	}
};

