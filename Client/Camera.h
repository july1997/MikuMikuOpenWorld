#pragma once

#include "DxLib.h"

class Camera
{
	const float MinV = -DX_PI_F / 180.0f * 40.0f, MaxV = DX_PI_F / 180.0f * 38.0f;
	const float MinH = 0.0f, MaxH = DX_PI_F * 2.0f;
	const float MinR = 15.0f, MaxR = 100.0f;
	const float AngleRateX = DX_PI_F / 360.0f, AngleRateY = DX_PI_F / 180.0f, ZoomRate = 5.0f;

	float CamV = 0.0f, CamH = 0.0f, CamR = 30.0f;

	int NewX = 0, NewY = 0, NewBtn = 0, OldX = 0, OldY = 0, OldBtn = 0;
	int MoveX = 0, MoveY = 0, MoveZ = 0;

	bool putmouse = 0;

	MATRIX RotV, RotH;
	VECTOR CamPos = VGet(0.0f, 17.75f, CamR), FocusPos = VGet(0.0f, 17.5f, 0.0f);

public:
	void MouseCamera(VECTOR camerapos,VECTOR addVce=VGet(0,0,0));

	void CameraAdditionally(VECTOR pos, VECTOR rot);

	void setCamera(VECTOR camerapos, VECTOR addVce = VGet(0, 0, 0));
};

