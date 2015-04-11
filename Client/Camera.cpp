#include "Camera.h"

void Camera::MouseCamera(VECTOR camerapos,VECTOR addVce)
{
	// マウス入力の取得
	MoveX = MoveY = 0;
	MoveZ = -GetMouseWheelRotVol();
	if ((NewBtn = GetMouseInput()) & MOUSE_INPUT_RIGHT) {
		putmouse = 1;
		GetMousePoint(&NewX, &NewY);
		if (OldBtn & MOUSE_INPUT_RIGHT) {
			MoveX = NewX - OldX;
			MoveY = NewY - OldY;
		}
		OldX = NewX; OldY = NewY;
	}
	else
	{
		putmouse = 0;
	}
	OldBtn = NewBtn;

	// マウスの情報からカメラの情報に変換
	CamV += MoveY * AngleRateX;
	CamH += MoveX * AngleRateY;
	CamR += MoveZ * ZoomRate;

	// 補正
	if (CamV > MaxV)CamV = MaxV;
	if (CamV < MinV)CamV = MinV;
	if (CamH > MaxH)CamH -= MaxH;
	if (CamH < MinH)CamH += MaxH;
	if (CamR > MaxR)CamR = MaxR;
	if (CamR < MinR)CamR = MinR;
	
	RotV = MGetRotX(CamV);
	RotH = MGetRotY(CamH);
	CamPos = VTransform(VTransform(VGet(0.0f, 17.75f, CamR), RotV), RotH);
	SetCameraPositionAndTargetAndUpVec(VAdd(camerapos, CamPos), VAdd(camerapos, addVce), VGet(0, 1, 0));
}

void Camera::CameraAdditionally(VECTOR pos,VECTOR rot)
{
	// マウス入力の取得
	MoveZ = -GetMouseWheelRotVol();

	// マウスの情報からカメラの情報に変換
	CamR += MoveZ * ZoomRate;

	// 補正
	if (CamR > MaxR)CamR = MaxR;
	if (CamR < MinR)CamR = MinR;

	RotV = MGetRotX(rot.x);
	RotH = MGetRotY(rot.y);
	MATRIX RotZ = MGetRotZ(rot.z);
	CamPos = VTransform(VTransform(VTransform(VGet(0.0f, 30.0f, CamR), RotV), RotH), RotZ);
	SetCameraPositionAndTargetAndUpVec(VAdd(pos, CamPos), pos, VGet(0, 1, 0));
}

void Camera::setCamera(VECTOR camerapos, VECTOR addVce)
{
	SetCameraPositionAndTargetAndUpVec(VAdd(camerapos, CamPos), VAdd(camerapos, addVce), VGet(0, 1, 0));
}