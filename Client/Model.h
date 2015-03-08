#pragma once

//キャラクター、NPC、乗り物などの基底クラス

#include "DxLib.h"

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
};

