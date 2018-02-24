#include "Camera.h"

void Camera::MouseCamera(VECTOR camerapos, VECTOR viewpoint,VECTOR addVce)
{
	SetCameraNearFar(3.f, 8000.f);

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
	
	RotV = MGetRotX(-CamV);
	RotH = MGetRotY(CamH);
	CamPos = VTransform(VTransform(VGet(viewpoint.x, viewpoint.y, viewpoint.z+CamR), RotV), RotH);
	SetCameraPositionAndTargetAndUpVec(VAdd(camerapos, CamPos), VAdd(camerapos, addVce), VGet(0, 1, 0));

	oldcmerapos = CamH;
}

void Camera::MouseCamera(int stagemodel,VECTOR camerapos, VECTOR addVce)
{
	SetCameraNearFar(10.f, 15000.f);

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

	RotV = MGetRotX(-CamV);
	RotH = MGetRotY(CamH);
	CamPos = VTransform(VTransform(VGet(0.0f, 17.75f, CamR), RotV), RotH);

	VECTOR campp = VAdd(camerapos, CamPos);

	if(!cameraflag)MV1SetupCollInfo(stagemodel, -1, 8, 8, 8), cameraflag = 1;

	// モデルと線分との当たり判定
	MV1_COLL_RESULT_POLY HitPoly = MV1CollCheck_Line(stagemodel, -1, VAdd(camerapos, addVce), campp);

	// 当たったかどうかで処理を分岐
	if (HitPoly.HitFlag == 1)
	{
		campp = HitPoly.HitPosition;
	}

	SetCameraPositionAndTargetAndUpVec(campp, VAdd(camerapos, addVce), VGet(0, 1, 0));

	oldcmerapos = CamH;
}


void Camera::CameraAdditionally(VECTOR pos,VECTOR rot, VECTOR addVce)
{
	SetCameraNearFar(3.f, 4000.f);

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
	SetCameraPositionAndTargetAndUpVec(VAdd(pos, CamPos), VAdd(pos, addVce), VGet(0, 1, 0));
}

void Camera::setCamera(VECTOR camerapos, VECTOR addVce)
{
	SetCameraPositionAndTargetAndUpVec(VAdd(camerapos, CamPos), VAdd(camerapos, addVce), VGet(0, 1, 0));
}

float Camera::getOldCameraPos()
{
	return oldcmerapos;
}

VECTOR Camera::getPos()
{
	return CamPos;
}