#include "Player.h"

#include <math.h>

Player::Player(Bullet_physics *bullet_, int world)
{
	bullet = bullet_;

	caharacter = bullet->createCharacter(world, VGet(0, 10, 0), 2.f);
}

void Player::loadModel(char *FileName)
{
	modelhandle = MV1LoadModel(FileName);

	modelname = std::string(FileName);
}

void Player::attachAnime(int AnimIndex)
{
	animehandle = MV1AttachAnim(modelhandle, AnimIndex);

	animetotaltime = MV1GetAttachAnimTotalTime(modelhandle, animehandle);

	nowanimeindex = AnimIndex;
}

void Player::setAnimeTime(float time)
{
	animetime = time;

	MV1SetAttachAnimTime(modelhandle, animehandle, animetime);
}

void Player::playAnime(float playspeed)
{
	MV1SetAttachAnimTime(modelhandle, animehandle, animetime);

	animetime += playspeed;

	if (animetime >= animetotaltime)
	{
		animetime = 0;
	}
}

bool Player::switchAnime(int AnimIndex , float uprate)
{
	if (!brendflag)
	{
		blendanimehandle = MV1AttachAnim(modelhandle, AnimIndex);

		brendflag = 1;
	}

	MV1SetAttachAnimBlendRate(modelhandle, animehandle, 1.0f - rate);

	MV1SetAttachAnimBlendRate(modelhandle, blendanimehandle, rate);

	rate += uprate;

	if (rate >= 1.f)
	{
		MV1DetachAnim(modelhandle, animehandle);

		animehandle = MV1AttachAnim(modelhandle, AnimIndex);

		animetotaltime = MV1GetAttachAnimTotalTime(modelhandle, animehandle);

		nowanimeindex = AnimIndex;

		MV1DetachAnim(modelhandle, blendanimehandle);

		rate = 0.f;

		brendflag = 0;
	}

return brendflag;
}

void Player::playerMovementKeyboard()
{
	if (!movestart)
	{
		if (!moving)
		{
			if (CheckHitKey(KEY_INPUT_D) == 1)
			{
				right = 1;

				movestart = 1;
			}
			else
			{
				right = 0;
			}
			if (CheckHitKey(KEY_INPUT_A) == 1)
			{
				left = 1;

				movestart = 1;
			}
			else
			{
				left = 0;
			}
			if (CheckHitKey(KEY_INPUT_W) == 1)
			{
				forward = 1;

				movestart = 1;

				moveflag = 1;

				if (CheckHitKey(KEY_INPUT_LSHIFT) == 1)
				{
					run = 1;
				}
				else
				{
					run = 0;
				}
			}
			else
			{
				forward = 0;

				moveflag = 0;

				run = 0;
			}
			if (CheckHitKey(KEY_INPUT_S) == 1)
			{
				backward = 1;

				movestart = 1;
			}
			else
			{
				backward = 0;
			}
			if (CheckHitKey(KEY_INPUT_SPACE) == 1)
			{
				if (bullet->getCharacterController(caharacter) && bullet->getCharacterController(caharacter)->canJump())
				{
					jump = 1;

					movestart = 1;
				}
				else
				{
					jump = 0;
				}
			}
			else
			{
				jump = 0;
			}
		}
		else
		{
			//マウスでの移動中
			if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_A) ||
				CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_S) ||
				CheckHitKey(KEY_INPUT_SPACE))
			{
				moving = 0;
			}
		}
	}
	else
	{
		if (movecounter == 15)
		{
			movestart = 0;

			movecounter = 0;
		}
		else
		{
			//15フレーム数える
			movecounter++;
		}
	}

	/*
	if (forward == 1)
	{
		if (runcounter != 15)
		{
			runcounter++;

			if (CheckHitKey(KEY_INPUT_W) == 0 && w == 0)w = 1;

			if (w == 1 && CheckHitKey(KEY_INPUT_W) == 1)run = 1, w = 0;
		}
	}
	else
	{
		run = 0;

		runcounter = 0;

		w = 0;
	}
	*/

	if (run == 0)bullet->controlCharacter(caharacter, left, right, backward, forward, jump, 0.5f);
	else bullet->controlCharacter(caharacter, left, right, backward, forward, jump, 1.4);
}

void Player::setPlayerMovement(bool left_, bool right_, bool backward_, bool forward_, bool jump_, bool run_)
{
	right = right_;
	left = left_;
	backward = backward_;
	forward = forward_;
	jump = jump_;
	run = run_;

	if (!forward)moveflag = 0;

	if (!run_)bullet->controlCharacter(caharacter, left_, right_, backward_, forward_, jump_, 0.5);
	else bullet->controlCharacter(caharacter, left_, right_, backward_, forward_, jump_, 1.4);
}

void Player::update()
{
	setPos(bullet->getCharacterPos(caharacter, VGet(0, 10, 0)));
	setRot(bullet->VGetRot(bullet->getCharacterRot(caharacter)));
}

VECTOR Player::getPosBullet()
{
	return bullet->getCharacterPos(caharacter);
}

btQuaternion Player::getRotBullet()
{
	return bullet->getCharacterRotRaw(caharacter);
}

bool Player::playerMovementMouse(int stagemodel)
{
	if (!moving)
	{
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			if (!movementmouseflag)
			{
				MV1SetupCollInfo(stagemodel, -1, 8, 8, 8);

				pointermodel.loadModel("model/消臭力/消臭力.pmx");

				pointermodel.setScale(2.f);

				movementmouseflag = 1;
			}

			GetMousePoint(&MouseX, &MouseY);

			// マウスポインタがある画面上の座標に該当する３Ｄ空間上の Near 面の座標を取得
			VECTOR StartPos = ConvScreenPosToWorldPos(VGet((float)MouseX, (float)MouseY, 0.0f));

			// マウスポインタがある画面上の座標に該当する３Ｄ空間上の Far 面の座標を取得
			VECTOR EndPos = ConvScreenPosToWorldPos(VGet((float)MouseX, (float)MouseY, 1.0f));

			// モデルと線分との当たり判定
			MV1_COLL_RESULT_POLY HitPoly = MV1CollCheck_Line(stagemodel, -1, StartPos, EndPos);

			//スクリーン座標からワールド座標に変換
			ConvScreenPosToWorldPos(VGet((float)MouseX, (float)MouseY, 0.0f));

			// 当たったかどうかで処理を分岐
			if (HitPoly.HitFlag == 1)
			{
				targetpos = HitPoly.HitPosition;

				moving = 1;
			}
		}
	}
	else
	{

		VECTOR nowtargetpos = VNorm(VSub(targetpos, getPos()));

		// atan2 を使用して角度を取得
		float Angle = atan2(nowtargetpos.x, nowtargetpos.z);

		pointermodel.setRot(VGet(0, Angle - DX_PI_F, 0));

		//ポインター
		pointermodel.setPos(VGet(targetpos.x, targetpos.y + 5 + 3 * sin(DX_PI_F / 90 * counter), targetpos.z));

		counter++;
		if (counter == 180)counter = 0;

		pointermodel.draw();
		//

		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)moving = 0;
	}

	return moving;
}

std::string Player::getModelFileName()
{
	return modelname;
}