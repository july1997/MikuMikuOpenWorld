#pragma once

#include "Model.h"

#include <vector>

//bulletとの連携
//配置するときの便利ビュー
class Object : public Model
{
protected:

	std::vector<int> modelhandles;

	float parentmodelscale = 1;

public:

	void copyModel(int copyNum);

	void copiedsetPos(int copiedhandle, VECTOR pos);

	void copiedsetRot(int copiedhandle, VECTOR rot);

	void copiedsetScale(int copiedhandle,float scale);

	int getCopyModel(int copiedhandle);

	virtual void setScale(float scale);

	virtual void draw();
};


