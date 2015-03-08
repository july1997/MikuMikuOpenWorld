#include "Camera.h"

void Camera::MouseCamera(VECTOR camerapos,VECTOR addVce)
{
	// �}�E�X���͂̎擾
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

	// �}�E�X�̏�񂩂�J�����̏��ɕϊ�
	CamV += MoveY * AngleRateX;
	CamH += MoveX * AngleRateY;
	CamR += MoveZ * ZoomRate;

	// �␳
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