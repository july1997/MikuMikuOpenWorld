#include "Object.h"


void Object::copyModel(int copyNum)
{
	for (int i = 0; i < copyNum; i++)
	{
		modelhandles.push_back(MV1DuplicateModel(modelhandle));

		MV1SetScale(modelhandles[i], VGet(parentmodelscale, parentmodelscale, parentmodelscale));
	}
}

void Object::copiedsetPos(int copiedhandle,VECTOR pos)
{
	MV1SetPosition(modelhandles[copiedhandle], pos);
}

void Object::copiedsetRot(int copiedhandle,VECTOR rot)
{
	MV1SetRotationXYZ(modelhandles[copiedhandle], rot);
}

void Object::copiedsetScale(int copiedhandle, float scale)
{
	MV1SetScale(modelhandles[copiedhandle], VGet(scale, scale, scale));
}

void Object::setScale(float scale)
{
	parentmodelscale = scale;

	MV1SetScale(modelhandle, VGet(scale, scale, scale));
}

int Object::getCopyModel(int copiedhandle)
{
	return modelhandles[copiedhandle];
}

void Object::draw()
{
	MV1DrawModel(modelhandle);

	for (size_t i = 0; i < modelhandles.size(); i++)
	{
		MV1DrawModel(modelhandles[i]);
	}
}