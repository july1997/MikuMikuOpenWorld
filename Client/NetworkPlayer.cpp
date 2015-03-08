#include "NetworkPlayer.h"

#include <mutex>

NetworkPlayer::NetworkPlayer(Bullet_physics *bullet_, int world_)
{
	bullet = bullet_;

	world = world_;
}
void NetworkPlayer::myConnect()
{
	commnad.resize(commnad.size() + 1);
	for (size_t i = 0; i<commnad.size(); i++){
		commnad[i].resize(6);
	}

	pos.push_back(VGet(0, 0, 0));
	rot.push_back(VGet(0, 0, 0));

	animehandle.push_back(0);
	animetotaltime.push_back(0);
	nowanimeindex.push_back(0);
	animetime.push_back(0);
	blendanimehandle.push_back(0);
	rate.push_back(0);
	brendflag.push_back(0);

	controller.push_back(bullet->pretendCreateCharacter());

	modelhandles.push_back(0);

	playernun++;
}

void NetworkPlayer::loadModel(const char *FileName)
{
	modelhandles.push_back(MV1LoadModel(FileName));
}

void NetworkPlayer::setModel(int &ModelHandel)
{
	modelhandles.push_back(ModelHandel);
}


void NetworkPlayer::networkSetCommand(int playernun, int commandnun, bool _command)
{
	commnad[playernun][commandnun] = _command;
}

void NetworkPlayer::networkSetPosRot(int playernun, VECTOR _pos, btQuaternion &_rot)
{
	bullet->setCharacterPosRot(playernun +1, _pos, _rot);
}

VECTOR NetworkPlayer::networkGetPos(int playernun)
{
	return MV1GetPosition(modelhandles[playernun]);
}

VECTOR NetworkPlayer::networkGetRot(int playernun)
{
	return MV1GetRotationXYZ(modelhandles[playernun]);
}

size_t NetworkPlayer::getModelHandlesSize()
{
	return playernun;
}

void NetworkPlayer::addPlayer(const char *FileName)
{
	if (deleteplayernunber.size() == 0)
	{
		SetUseASyncLoadFlag(TRUE);

		loadModel(FileName);

		SetUseASyncLoadFlag(FALSE);

		controller.push_back(bullet->createCharacter(world, VGet(0, 10, 0), 2.f));
		bullet->controlCharacter(controller[playernun], 0, 0, 0, 0, 0, 0.5f);

		commnad.resize(commnad.size() + 1);		// ()���̐������v�f���ɂȂ�
		for (size_t i = 0; i < commnad.size(); i++){
			commnad[i].resize(6);
		}

		pos.push_back(VGet(0, 0, 0));
		rot.push_back(VGet(0, 0, 0));

		animehandle.push_back(0);
		animetotaltime.push_back(0);
		nowanimeindex.push_back(0);
		animetime.push_back(0);
		blendanimehandle.push_back(0);
		rate.push_back(0);
		brendflag.push_back(0);

		attachAnime(playernun, 0);
		playernun++;
	}
	else
	{
		SetUseASyncLoadFlag(TRUE);

		modelhandles[deleteplayernunber[0]] = MV1LoadModel(FileName);

		SetUseASyncLoadFlag(FALSE);

		for (size_t i = 0; i < commnad[deleteplayernunber[0]].size(); i++){
			commnad[deleteplayernunber[0]][i] = 0;
		}

		pos[deleteplayernunber[0]] = VGet(0, 0, 0);
		rot[deleteplayernunber[0]] = VGet(0, 0, 0);

		attachAnime(playernun, 0);
		playernun++;

		deleteplayernunber.erase(deleteplayernunber.begin());
	}
}

void NetworkPlayer::draw(int playernun)
{
	MV1DrawModel(modelhandles[playernun]);
}

bool NetworkPlayer::networkGetCommand(int playernun, int commandnun)
{
	return commnad[playernun][commandnun];
}

void NetworkPlayer::controlPlayer(int playernun)
{
	if (commnad[playernun][5] == 0)
	{
		bullet->controlCharacter(controller[playernun], commnad[playernun][0], commnad[playernun][1], commnad[playernun][2], commnad[playernun][3], commnad[playernun][4], 0.5f);
	}
	else
	{
		bullet->controlCharacter(controller[playernun], commnad[playernun][0], commnad[playernun][1], commnad[playernun][2], commnad[playernun][3], commnad[playernun][4], 1.4f);
	}
}

void NetworkPlayer::updateVec(int playernun)
{
	MV1SetPosition(modelhandles[playernun], bullet->getCharacterPos(controller[playernun], VGet(0, 10, 0)));

	MV1SetRotationXYZ(modelhandles[playernun], bullet->VGetRot(bullet->getCharacterRot(controller[playernun])));
}

void NetworkPlayer::attachAnime(int playernun,int AnimIndex)
{
	animehandle[playernun] = MV1AttachAnim(modelhandles[playernun], AnimIndex);

	animetotaltime[playernun] = MV1GetAttachAnimTotalTime(modelhandles[playernun], animehandle[playernun]);

	nowanimeindex[playernun] = AnimIndex;
}

void NetworkPlayer::setAnimeTime(int playernun, float time)
{
	animetime[playernun] = time;

	MV1SetAttachAnimTime(modelhandles[playernun], animehandle[playernun], animetime[playernun]);
}

void NetworkPlayer::playAnime(int playernun, float playspeed)
{
	MV1SetAttachAnimTime(modelhandles[playernun], animehandle[playernun], animetime[playernun]);

	animetime[playernun] += playspeed;

	if (animetime[playernun] >= animetotaltime[playernun])
	{
		animetime[playernun] = 0;
	}
}

bool NetworkPlayer::switchAnime(int playernun, int AnimIndex, float uprate)
{
	if (!brendflag[playernun])
	{
		blendanimehandle[playernun] = MV1AttachAnim(modelhandles[playernun], AnimIndex);

		brendflag[playernun] = 1;
	}

	MV1SetAttachAnimBlendRate(modelhandles[playernun], animehandle[playernun], 1.0f - rate[playernun]);

	MV1SetAttachAnimBlendRate(modelhandles[playernun], blendanimehandle[playernun], rate[playernun]);

	rate[playernun] += uprate;

	if (rate[playernun] >= 1.f)
	{
		MV1DetachAnim(modelhandles[playernun], animehandle[playernun]);

		animehandle[playernun] = MV1AttachAnim(modelhandles[playernun], AnimIndex);

		animetotaltime[playernun] = MV1GetAttachAnimTotalTime(modelhandles[playernun], animehandle[playernun]);

		nowanimeindex[playernun] = AnimIndex;

		MV1DetachAnim(modelhandles[playernun], blendanimehandle[playernun]);

		rate[playernun] = 0.f;

		brendflag[playernun] = 0;
	}

	return brendflag[playernun];
}

void NetworkPlayer::drawPlayers()
{
	for (size_t i = 0; i < getModelHandlesSize(); i++)
	{
		if (i != myconnection)
		{
			if (CheckCameraViewClip_Box(networkGetPos(i),VAdd( networkGetPos(i),VGet(0,18,0))) == FALSE)
			{
				draw(i);
			}
		}
	}
}

void NetworkPlayer::controlPlayer()
{
	for (size_t i = 0; i < getModelHandlesSize(); i++)
	{
		if (i != myconnection)
		{
			controlPlayer(i);
		}
	}
}

void NetworkPlayer::updatePlayerVec()
{
	for (size_t i = 0; i < getModelHandlesSize(); i++)
	{
		if (i != myconnection)
		{
			updateVec(i);
		}
	}
}

void NetworkPlayer::deletePlayer(int playernun_)
{
	//���f�����폜
	MV1DeleteModel(modelhandles[playernun_]);

	deleteplayernunber.push_back(playernun_);

	playernun--;

	/*
	std::vector<std::vector<bool>>::iterator commnadi = commnad.begin();
	commnadi += playernun_;
	commnad.erase(commnadi);

	std::vector<VECTOR>::iterator veci = pos.begin();
	veci += playernun_;
	pos.erase(veci);
	veci = rot.begin();
	veci += playernun_;
	rot.erase(veci);

	std::vector<int>::iterator hadlei = animehandle.begin();
	hadlei += playernun_;
	animehandle.erase(hadlei);
	hadlei = controller.begin();
	hadlei += playernun_;
	controller.erase(hadlei);
	hadlei = modelhandles.begin();
	hadlei += playernun_;
	modelhandles.erase(hadlei);
	hadlei = nowanimeindex.begin();
	hadlei += playernun_;
	nowanimeindex.erase(hadlei);
	hadlei = blendanimehandle.begin();
	hadlei += playernun_;
	blendanimehandle.erase(hadlei);

	std::vector<float>::iterator timei = animetotaltime.begin();
	timei += playernun_;
	animetotaltime.erase(timei);

	timei = animetime.begin();
	timei += playernun_;
	animetime.erase(timei);

	timei = rate.begin();
	timei += playernun_;
	rate.erase(timei);

	std::vector<bool>::iterator flagi = brendflag.begin();
	flagi += playernun_;
	brendflag.erase(flagi);

	playernun--;
	*/
}