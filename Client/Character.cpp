#include "Character.h"


Character::Character(std::shared_ptr<Bullet_physics> bullet_, int world)
{
	bullet = std::shared_ptr<Bullet_physics>(bullet_);

	caharacter = bullet->createCharacter(world, VGet(0, 10, 0), 2.f);
}

Character::~Character()
{
}

void Character::loadModel(const char *FileName)
{
	// 次に読み込むモデルの物理演算モードをリアルタイム物理演算にする
	MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);

	modelhandle = MV1LoadModel(FileName);

	//設定をもどす
	MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);

	modelname = std::string(FileName);
}

void Character::setup() 
{
	MV1SetOpacityRate(modelhandle, 0.0f);

	anime.push_back(attachAnime(0, &totaltime));
	nowAni = 0;

	// アニメーションで移動をしているフレームの番号を検索する
	MoveAnimFrameIndex = MV1SearchFrame(modelhandle, u8"センター");
}

int Character::attachAnime(int AnimIndex, int* time, float blend_)
{
	int a = MV1AttachAnim(modelhandle, AnimIndex);

	*time = MV1GetAttachAnimTotalTime(modelhandle, a);

	blend(a, blend_, 0.f);

	return a;
}

void Character::blend(int blendAnime, float rate, float time)
{
	MV1SetAttachAnimBlendRate(modelhandle, blendAnime, rate);

	MV1SetAttachAnimTime(modelhandle, blendAnime, time);
}

bool Character::switchAnime(int before, int after, float afteranimetimer, float rateSpeed, bool fin_) {

	if (!atta)anime.push_back(attachAnime(after, &totaltime2, 0.f)), atta = 1;
	if (Rate < 1.f) {
		Rate += rateSpeed;
		blend(anime[1], Rate, afteranimetimer);
		blend(anime[0], 1.f - Rate, time);
	}
	else {
		nowAni = after;
		Rate = 0;
		totaltime = totaltime2;
		atta = 0;
		if (fin_)fin = 1;

		MV1DetachAnim(modelhandle, anime[0]);
		anime.erase(anime.begin());
		return 1;
	}

	return 0;
}

void Character::playAnime()
{
	if (!dance) {
		//---センターの移動成分を消す---
		//設定を元に戻す
		MV1ResetFrameUserLocalMatrix(modelhandle, MoveAnimFrameIndex);
		// ボーンのローカル→ワールド行列を取得
		MATRIX Local = MV1GetFrameLocalMatrix(modelhandle, MoveAnimFrameIndex);
		// Ｚだけ 1.0f を代入したベクトルを行列を使って変換
		Local.m[3][0] = 0.0f;
		Local.m[3][1] = 0.0f;
		Local.m[3][2] = 0.0f;
		// アニメーションで移動をしているフレームを無効にする
		MV1SetFrameUserLocalMatrix(modelhandle, MoveAnimFrameIndex, Local);
		//------------------------------


		// アニメーション時間を進める前の『アニメーションで移動をしているフレームの座標』を取得しておく
		VECTOR PrevPosition = MV1GetAttachAnimFrameLocalPosition(modelhandle, anime[0], MoveAnimFrameIndex);

		time += 0.5f;
		MV1SetAttachAnimTime(modelhandle, anime[0], time);

		// 『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」』と、
		// 『アニメーション再生時間を進める前の「アニメーションで移動をしているフレームの座標」』との差分をモデルの座標に加算する
		VECTOR NowPosition = MV1GetAttachAnimFrameLocalPosition(modelhandle, anime[0], MoveAnimFrameIndex);
		Position = VSub(NowPosition, PrevPosition);
		setPos(VAdd(getPos(), VGet(0, Position.y, 0)));
	}
	else {

		//設定を元に戻す
		MV1ResetFrameUserLocalMatrix(modelhandle, MoveAnimFrameIndex);

		time += 0.5f;
		MV1SetAttachAnimTime(modelhandle, anime[0], time);
	}
}

void Character::playerMovementKeyboard()
{
	speed = VSize(VGet(Position.x, 0, Position.z));

	if (CheckHitKey(KEY_INPUT_D) == 1)
	{
		_right = 1;
	}
	else
	{
		_right = 0;
	}
	if (CheckHitKey(KEY_INPUT_A) == 1)
	{
		_left = 1;
	}
	else
	{
		_left = 0;
	}
	if (CheckHitKey(KEY_INPUT_W) == 1)
	{
		if (CheckHitKey(KEY_INPUT_LSHIFT) == 1)
		{
			_run = 1;
			shifttimer++;
		}
		else
		{
			_run = 0;
			shifttimer = 0;
		}

		_forward = 1;
	}
	else
	{
		_forward = 0;
	}
	if (CheckHitKey(KEY_INPUT_S) == 1)
	{
		_backward = 1;
	}
	else
	{
		_backward = 0;
	}
	if (!_right && !_left && !_forward && !_run && !_backward) {
		if (dance == 0) {
			if (CheckHitKey(KEY_INPUT_F1) == 1)
			{
				state = "Happy＆Pride";
				blend(anime[0], 0.f, 0.f);
				anime[0] = attachAnime(11, &totaltime);
				time = 0;
				dance = 1;
				inputstate = state;
			}
		}
	}
	else if (dance)dance = 0;

	stateManager();

	if (state == "walk_e") {
		//ゆっくり停止
		speed = speed - speed * sin(180.0 / DX_PI * timer3 / 5000.f);
		timer3++;
	}
	else if (state == "run_s") {
		//ゆっくり発進
		if (timer3 > 50.f)speed = 0.25 * sin(180.0 / DX_PI * timer3 / 3000.f);
		timer3++;
	}
	else if(state == "run_e") {
		//ゆっくり停止
		speed = speed - speed * sin(180.0 / DX_PI * timer3 / 10000.f);
		timer3++;
	}
	else if (state == "back_s") {
		//ゆっくり停止
		speed = speed + 0.25 *sin(180.0 / DX_PI * timer3 / 10000.f);
		timer3++;
	}
	else if (state == "back_e") {
		//ゆっくり停止
		speed = speed - speed * sin(180.0 / DX_PI * timer3 / 10000.f);
		timer3++;
	}

	bool __forward = _forward, __backward = _backward,__run = _run;
	if (state.find("run") != std::string::npos || state.find("walk") != std::string::npos || state.find("dash") != std::string::npos)__forward = 1, __backward = 0;
	if (state == "back_s" || state == "back_e")__backward = 1;
	if (state.find("dash") != std::string::npos || state.find("run") != std::string::npos)__run = 1;
	if (!fin) {
		if (!__run)
			TurningSpeed = speed / 5.0f;
		else
			TurningSpeed = speed / 15.0f;
		direction = bullet->controlCharacter(caharacter, _left, _right, __backward, __forward, _jump, speed * 2,TurningSpeed);
	}
	else {
		if (fintime == 2)fin = 0, fintime = 0;
		fintime++;
	}

}

void Character::noMovement()
{
	bool __forward = _forward, __backward = _backward;
	if(state.find("run") != std::string::npos || state.find("walk") != std::string::npos || state.find("dash") != std::string::npos)__forward = 1, __backward = 0;
	if (state == "back_s" || state == "back_e")__backward = 1;

	TurningSpeed = speed / 15.0f;

	direction = bullet->controlCharacter(caharacter, 0, 0, __backward, __forward, 0, speed * 2, TurningSpeed);
}

void Character::stateManager()
{
	//状態管理
	if (state.find("run") == std::string::npos && state.find("walk") == std::string::npos && state.find("dash") == std::string::npos) {
		if (_forward) {
			if (_run) {
				state = "run_s";
				timer2 = 10.f;
				inputstate = state;
			}
			else {
				state = "walk_s";
				timer2 = 0;
				inputstate = state;
			}
		}
		if (_backward && !bk) {
			bk = 1;
			state = "back_s";
			inputstate = state;
		}
		if (bk && !_backward) {
			if (state == "back")
			{
				state = "back_e";
				inputstate = state;
				bk = 0;
			}
		}
	}
	else {
		if (!_forward && state == "walk") {
			state = "walk_e";
			inputstate = state;
		}
		else
		{
			if (state == "walk" && _run) {
				state = "walk_to_run";
				timer2 = 101.f;
				inputstate = state;
			}
		}
		if ((!_forward) && state == "run") {
			state = "run_e";
			timer2 = 101.f;
			inputstate = state;
		}
		else
		{
			//if (state == "run" && !_run) {
			//	state = "run_to_walk";
			//	timer2 = 122.f;
			//}
		}
		if (state == "run" && state.find("dash") == std::string::npos && shifttimer > 60) {
			state = "dash_s";
			inputstate = state;
		}
		if (state == "dash" && !_forward)
		{
			state = "dash_e";
			inputstate = state;
		}
	}
}

void Character::animeControl()
{
	//Walk
	if (state == "walk_s") {
		if (switchAnime(nowAni, 1, timer2, 0.05)) {
			state = "walk_l";
			time = timer2;
			timer2 = 0;
		}
		timer2 += 0.5;
	}
	if (state == "walk_l") {
		if (switchAnime(nowAni, 2, time + 5, 0.05)) {
			state = "walk";
			time += 5;
		}
	}
	if (state == "walk") {
		if (time >= totaltime - 50.f) {

			if (switchAnime(nowAni, 2, timer2)) {
				time = timer2;
				timer2 = 0;
			}
			timer2 += 0.5;
		}
	}
	if (state == "walk_e") {
		if (switchAnime(nowAni, 0, timer2)) {
			state = "stand";
			time = timer2;
			timer2 = 0;
			timer3 = 0;
		}
		timer2 += 0.5;
	}

	//Run
	if (state == "run_s") {
		if (switchAnime(nowAni, 4, timer2)) {
			state = "run_l";
			time = timer2;
			timer2 = 0;
			timer3 = 0;
		}
		timer2 += 0.5;
	}
	if (state == "run_l") {
		if (switchAnime(nowAni, 5, time + 5)) {
			state = "run";
			time += 5;
		}
	}
	if (state == "run") {
		if (time >= totaltime - 50.f) {
			if (switchAnime(nowAni, 5, timer2)) {
				time = timer2;
				timer2 = 0;
			}
			timer2 += 0.5;
		}
	}
	if (state == "run_e") {
		if (deley == 0) {
			if (switchAnime(nowAni, 6, timer2)) {
				deley = 1;
				time = timer2;
				timer2 = 0;
			}
			timer2 += 0.5;
		}
		else {
			deley++;
			if (deley == 30)state = "run_f", deley = 0, timer3 = 0;
		}
	}
	if (state == "run_f") {
		if (switchAnime(nowAni, 0, timer2)) {
			state = "stand";
			time = timer2;
			timer2 = 0;
		}
		timer2 += 0.5;
	}

	//oher
	if (state == "walk_to_run") {
		if (switchAnime(nowAni, 7, timer2)) {
			state = "run_walk";
			time = timer2;
			timer2 = 0;
		}
		timer2 += 0.5;
	}
	if (state == "run_walk") {
		if (switchAnime(nowAni, 5, timer2)) {
			state = "run";
			time = timer2;
			timer2 = 0;
		}
		timer2 += 0.5;
	}
	if (state == "run_to_walk") {
		if (switchAnime(nowAni, 8, timer2)) {
			state = "walk_run";
			time = timer2;
			timer2 = 0;
		}
		timer2 += 0.5;
	}
	if (state == "walk_run") {
		if (switchAnime(nowAni, 2, timer2)) {
			state = "walk";
			time = timer2;
			timer2 = 0;
		}
		timer2 += 0.5;
	}
	if (state == "back_s") {
		if (switchAnime(nowAni, 10, timer2)) {
			state = "back";
			time = timer2;
			timer2 = 0;
			timer3 = 0;
		}
		timer2 += 0.5;
	}
	if (state == "back") {
		if (time >= totaltime - 50.f) {
			if (switchAnime(nowAni, 10, timer2)) {
				time = timer2;
				timer2 = 0;
			}
			timer2 += 0.5;
		}
	}
	if (state == "back_e") {
		if (switchAnime(nowAni, 0, timer2)) {
			state = "stand";
			time = timer2;
			timer2 = 0;
			timer3 = 0;
		}
		timer2 += 0.5;
	}
	if (state == "dash_s") {
		if (switchAnime(nowAni, 9, timer2)) {
			state = "dash";
			time = timer2;
			timer2 = 0;
		}
		timer2 += 0.5;
	}
	if (state == "dash") {
		if (time >= totaltime - 50.f) {
			if (switchAnime(nowAni, 9, timer2)) {
				time = timer2;
				timer2 = 0;
			}
			timer2 += 0.5;
		}
	}
	if (state == "dash_e") {
		if (switchAnime(nowAni, 5, timer2)) {
			state = "run";
			time = timer2;
			timer2 = 0;
		}
		timer2 += 0.5;
	}

	if (time > totaltime)time = 0;
}

void Character::debug() {
	DrawFormatString(0, 0, -1, "state %s\n nowAni %d  time %f \n timer2 %f Rate %f\n xyz %f %f %f", state.c_str(), nowAni, time, timer2, Rate, getPos().x, getPos().y, getPos().z);
}

void Character::update()
{
	if (!dance) {
		setPos(bullet->getCharacterPos(caharacter, sub));
	}
	else {
		setPos(bullet->getCharacterPos(caharacter, VGet(0,9.3f,0)));
	}
	setRot(bullet->VGetRot(bullet->getCharacterRot(caharacter)));

	MV1PhysicsCalculation(modelhandle, 1000.0f / 60.0f);

	if (!fadein) {
		fadeinRate += 0.01f;
		MV1SetOpacityRate(modelhandle, fadeinRate);
		if (fadeinRate >= 1.0f)fadein = 1;
	}
}

VECTOR Character::gertDirection()
{
	return VGet(direction.getX(), direction.getY(), direction.getZ());
}

void Character::setDirection(VECTOR dir)
{
	bullet->setCharacterDirection(caharacter,btVector3(dir.x, dir.y, dir.z));
}

std::string Character::getState()
{
	return inputstate;
}

void Character::setState(std::string state_)
{
	state = state_;
	inputstate = state;
}

int Character::setPosRotBullet(btVector3 &pos, btQuaternion &q)
{
	bullet->setCharacterPosRot(caharacter, bullet->btVGet(pos),q);

	return 0;
}

VECTOR Character::getPosBullet()
{
	return bullet->getCharacterPos(caharacter, VGet(0,0,0));
}

btQuaternion Character::getRotBullet()
{
	return bullet->getCharacterRotRaw(caharacter);
}

float Character::getTurningSpeed()
{
	if (_right) {
		return -TurningSpeed;
	}
	if (_left) {
		return TurningSpeed;
	}
	return 0.f;
}

void Character::setTurningSpeed(float TurningSpeed)
{
	bullet->setCharacterTurningSpeed(caharacter, TurningSpeed);
}