#pragma once

//キャラクター、NPC、乗り物などの基底クラス

#include "DxLib.h"
#include <math.h>

class Model
{
protected:

	int modelhandle;

public:
	//仮想デストラクタ
	virtual ~Model(){};

	virtual void loadModel(const char *FileName)
	{
		modelhandle = MV1LoadModel(FileName);
	}

	virtual int setPos(VECTOR pos)
	{
		return MV1SetPosition(modelhandle, pos);
	}
	virtual int setRot(VECTOR rot)
	{
		return MV1SetRotationXYZ(modelhandle, rot);
	}
	virtual int setScale(float scale)
	{
		return MV1SetScale(modelhandle, VGet(scale, scale, scale));
	}

	virtual VECTOR getPos()
	{
		return MV1GetPosition(modelhandle);
	}
	virtual VECTOR getRot()
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

	void setModel(int &ModelHandel)
	{
		modelhandle = ModelHandel;
	}

	void copyModel(int &ModelHandel)
	{
		modelhandle = MV1DuplicateModel(ModelHandel);
	}
};

