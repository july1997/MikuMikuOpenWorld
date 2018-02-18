#include "MultiPlayers.h"



MultiPlayers::MultiPlayers(std::shared_ptr<Bullet_physics> bullet_, int world_)
{
	bullet = bullet_;

	world = world_;
}

MultiPlayers::~MultiPlayers()
{
}

unsigned int MultiPlayers::findPlayerID(unsigned int PlayerID)
{
	auto iter = std::find(playerID.begin(), playerID.end(), PlayerID);

	unsigned int index = std::distance(playerID.begin(), iter);
	if (index == playerID.size())
	{
		return -1;
	}

	return index;
}

int MultiPlayers::addPlayer(unsigned int PlayerID, std::string name)
{
	playerID.push_back(PlayerID);

	names.push_back(name);

	players.push_back(std::shared_ptr<Character>(new Character(bullet, world)));

	ts.push_back(0);
	oldts.push_back(0);
	k.push_back(0);
	d.push_back(VGet(0,0,0));
	oldd.push_back(VGet(0, 0, 0));
	kd.push_back(VGet(0, 0, 0));

	return 0;
}

int MultiPlayers::loadModel(unsigned int PlayerID, char *ModelPath)
{
	players[findPlayerID(PlayerID)]->loadModel(ModelPath);

	return 0;
}

int MultiPlayers::setModel(unsigned int PlayerID, int ModelHandle, std::string ModelPath)
{
	players[findPlayerID(PlayerID)]->setModel(ModelHandle);

	return 0;
}

int MultiPlayers::copyModel(unsigned int PlayerID, int ModelHandle)
{
	// 次に読み込むモデルの物理演算モードをリアルタイム物理演算にする
	MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);

	players[findPlayerID(PlayerID)]->copyModel(ModelHandle);

	//設定をもどす
	MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);

	return 0;
}

int MultiPlayers::deletePlayer(unsigned int PlayerID)
{
	playersSize--;

	unsigned int pos = findPlayerID(PlayerID);

	players[pos]->~Character();

	playerID.erase(playerID.begin() + pos);

	players.erase(players.begin() + pos);

	names.erase(names.begin() + pos);

	return 0;
}

int MultiPlayers::setup(unsigned int PlayerID)
{
	players[findPlayerID(PlayerID)]->setup();

	return 0;
}

void MultiPlayers::playAnimeControl()
{
	for (int i = 0; i < playersSize; i++)
	{
		players[i]->playAnime();

		players[i]->animeControl();
	}
}

void MultiPlayers::update()
{
	for (int i = 0; i < playersSize; i++)
	{
		players[i]->update();
	}
}

void MultiPlayers::draw()
{
	for (int i = 0; i < playersSize; i++)
	{
		oldts[i] += k[i];
		players[i]->setTurningSpeed(oldts[i]);

		oldd[i] = VAdd(d[i], kd[i]);
		players[i]->setDirection(oldd[i]);

		players[i]->draw();
	}
}

int MultiPlayers::setPosBullet(unsigned int &PlayerID, btVector3 &pos, btQuaternion &q)
{
	unsigned char id = findPlayerID(PlayerID);
	if (id != -1)
	{
		players[id]->setPosRotBullet(pos, q);

		return 0;
	}
	else
	{
		return -1;
	}
}

void MultiPlayers::setState(unsigned int PlayerID,std::string &state)
{
	if(state != players[findPlayerID(PlayerID)]->getState())
	players[findPlayerID(PlayerID)]->setState(state);
}

void MultiPlayers::setDirection(unsigned int PlayerID, VECTOR dir)
{
	oldd[findPlayerID(PlayerID)] = d[findPlayerID(PlayerID)];
	d[findPlayerID(PlayerID)] = dir;
	kd[findPlayerID(PlayerID)] = VScale(VSub(d[findPlayerID(PlayerID)], oldd[findPlayerID(PlayerID)]), 1.f / 30.f);
}

void MultiPlayers::setTurningSpeed(unsigned int PlayerID, float TurningSpeed)
{
	oldts[findPlayerID(PlayerID)] = ts[findPlayerID(PlayerID)];
	ts[findPlayerID(PlayerID)] = TurningSpeed;
	k[findPlayerID(PlayerID)] = (ts[findPlayerID(PlayerID)] - oldts[findPlayerID(PlayerID)]) / 30.f;
}

std::string MultiPlayers::getName(unsigned int PlayerID)
{
	return names[findPlayerID(PlayerID)];
}

unsigned int MultiPlayers::getPlayerID(size_t PlayerNunber)
{
	return playerID[PlayerNunber];
}

size_t MultiPlayers::getMultiplayerSize()
{
	return playersSize;
}

void MultiPlayers::addPlayerSize()
{
	playersSize++;
}

VECTOR MultiPlayers::getPos(unsigned int PlayerID)
{
	return players[findPlayerID(PlayerID)]->getPos();
}