#include "Network.h"
#include "Youtube.h"

#include "DxLib.h"

#include "fps.h"
#include "Core.h"

#include "Model.h"
#include "Bullet_physics.h"
#include "Player.h"
#include "Camera.h"
#include "Object.h"
#include "NetworkPlayer.h"
#include "Scene.h"

#include <thread>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Core core;

	core.setupDxLib();

	FpsManager Fps;
	Fps.setFps(30);

	Bullet_physics bullet;
	int world = bullet.createWorld();

	Player player(&bullet, world);
	switch (GetRand(12))
	{
	case 0:
		player.loadModel("model/Tda�~�N���� �L���~�����s/Tda�~�N���� �L���~�����s.mv1");
		break;
	case 1:
		player.loadModel("model/Tda���~�N�����s�[�X/Tda���~�N�����s�[�XRSP.mv1");
		break;
	case 2:
		player.loadModel("model/Appearance Miku/Appearance Miku.mv1");
		break;
	case 3:
		player.loadModel("model/���҃~�N���σT���^��08/���҃~�N�T���^_�e�X�g08.mv1");
		break;
	case 4:
		player.loadModel("model/����1_5/����Ver1_5.mv1");
		break;
	case 5:
		player.loadModel("model/39�Ƃ�͂����݂��Q/39�Ƃ�͂����݂��Q.mv1");
		break;
	case 6:
		player.loadModel("model/Tda���d���e�gTypeS/Tda���d���e�gTypeS.mv1");
		break;
	case 7:
		player.loadModel("model/�I�I�}����/�I�I�}����.mv1");
		break;
	case 8:
		player.loadModel("model/�����@�����@/�����@�����@.mv1");
		break;
	case 9:
		player.loadModel("model/����1.5/����.mv1");
		break;
	case 10:
		player.loadModel("model/�����~�NXS���ϐ��-1.1/mikuXS���~�N.mv1");
		break;
	case 11:
		player.loadModel("model/m_GUMI_V3_201306/GUMI��_V3.mv1");
		break;
	case 12:
		player.loadModel("model/Luka_v3.3/�������J.mv1");
		break;
	}

	NetworkPlayer networkplayer(&bullet, world);

	Network network(&player, &networkplayer);
	Scene scene(&network);
	scene.Login();

	while (ProcessMessage() == 0 && GetASyncLoadNum() != 0);
	player.attachAnime(0);

	Model stage;
	stage.loadModel("stage/��F��1.52/sorairo1.52.x");
	stage.setScale(10.0f);

	bullet.createGroundBodytoMesh(stage.getModelHandle(), world);

	Model sky;
	sky.loadModel("stage/�X�J�C�h�[��/dome303.X");
	sky.setScale(10.0f);

	Object object;
	object.loadModel("stage/�X�N���[���d�w/�X�N���[��EX2.x");
	object.setScale(10.f);
	object.setRot(VGet(0,-DX_PI_F/4,0));
	object.setPos(VGet(-220.11, 2, 1030));

	Camera camera;

	Youtube yotube;
	int movieflag = 0;
	yotube.setScreen(object.getModelHandle(), 3);

	bool ank = 1;

	for (size_t i = 0; i < networkplayer.getModelHandlesSize(); i++)
	{
		if (i != network.myconnection)
		{
			networkplayer.attachAnime(i, 0);
		}
	}

	std::thread t([&network](){ network.update(); });

	while (ProcessMessage() == 0 && scene.exitConfirm() == 0)
	{
		network.candelete = 0;

		if (network.deleting == 1)
		{
			while (network.deleting == 1)
			{
				ProcessMessage();
			}
		}

		SetCameraNearFar(3.0f, 30000.0f);

		if (scene.inputting())player.playerMovementKeyboard();
		else player.setPlayerMovement(0,0,0,0,0,0);

		networkplayer.controlPlayer();

		bullet.stepSimulation(world, 30.f);

		player.update();

		networkplayer.updatePlayerVec();

		MV1PhysicsCalculation(player.getModelHandle(), 1000.0f / 30.0f);

		player.playAnime();

		if (!player.run   &&    player.nowanimeindex == 2 || player.nowanimeindex == 2 && player.brendflag && !player.run)player.switchAnime(1);
		if (player.run   &&    player.nowanimeindex == 1 || player.nowanimeindex == 1 && player.brendflag && player.run == 1)player.switchAnime(2);
		if ( player.moveflag && player.nowanimeindex == 0 || player.nowanimeindex == 0 && player.brendflag)player.switchAnime(1);
		if (!player.moveflag && player.nowanimeindex != 0 || player.nowanimeindex !=0 && player.brendflag && player.run == 0)player.switchAnime(0);


		for (size_t i = 0; i < networkplayer.getModelHandlesSize(); i++)
		{
			if (i != network.myconnection)
			{
				networkplayer.playAnime(i);

				if (!networkplayer.networkGetCommand(i, 5) && networkplayer.nowanimeindex[i] == 2 || networkplayer.nowanimeindex[i] == 2 && networkplayer.brendflag[i] && !networkplayer.networkGetCommand(i, 5))networkplayer.switchAnime(i, 1);
				if (networkplayer.networkGetCommand(i, 5) && networkplayer.nowanimeindex[i] == 1 || networkplayer.nowanimeindex[i] == 1 && networkplayer.brendflag[i] && networkplayer.networkGetCommand(i, 5))networkplayer.switchAnime(i, 2);
				if (networkplayer.networkGetCommand(i,3) && networkplayer.nowanimeindex[i] == 0 || networkplayer.nowanimeindex[i] == 0 && networkplayer.brendflag[i])networkplayer.switchAnime(i, 1);
				if (!networkplayer.networkGetCommand(i, 3) && networkplayer.nowanimeindex[i] != 0 || networkplayer.nowanimeindex[i] != 0 && networkplayer.brendflag[i] && !networkplayer.networkGetCommand(i, 5))networkplayer.switchAnime(i, 0);
			}
		}

		camera.MouseCamera(player.getPos(),VGet(0,13.8f,0));

		object.draw();

		networkplayer.drawPlayers();

		//player.playerMovementMouse(stage.getModelHandle());

		player.draw();

		sky.draw();
		stage.draw();

		scene.displayMyName();
		scene.displayName();

		scene.displayChatToScreen();
		scene.yotubeRequest();

		if (network.YoutubeURL.size() != 0 && movieflag == 0)
		{
			int f = yotube.playYotubeMovie(network.YoutubeURL);

			if (f == 1)
			{
				movieflag = 1;

				network.YoutubeURL = "";
				network.Extension = -1;

				//5���i300*10000000�i�m�b�j�ȏゾ������
				if (yotube.getMovieStopTime() > 3000000000)ank = 1;
				else ank = 0;

				network.YoutubeMessege.push_back("����̏������������܂���");

				std::string hun = std::to_string((double)(yotube.getMovieStopTime() / 60) / 10000000);
				network.YoutubeMessege.push_back("���掞�� : " + hun.substr(0, hun.find('.')+2) + "��");
			}
			else
			{
				if (f == -1)
				{
					network.YoutubeMessege.push_back("����̍Đ��Ɏ��s���܂���");

					network.YoutubeURL = "";
					network.Extension = -1;
					network.PlayStartTime = 0;

					network.sendyotubecommand = 3;
				}

				movieflag = 0;
			}
		}
		if (movieflag == 1)
		{
			time_t timer;
			time(&timer);

			if (timer >= network.PlayStartTime)
			{
				yotube.playMovie();
				movieflag = 2;

				network.YoutubeMessege.push_back("����̍Đ����J�n���܂�");
			}
		}
		if (movieflag == 2)
		{
			yotube.updateYotubeMovie();
			
			int volume = 0;
			float Length = VSize(object.getPos()) - VSize(player.getPos());

			if (Length<0)volume = (int)Length * 10;
			else if (Length>0)volume = -(int)Length * 10;

			if (Length < 100 && Length > -100)volume = -1000;
			if (Length > 1000 && Length < -1000)volume = -10000;

			yotube.setVolume(volume);

			if ( yotube.Playing() == 0 || network.Extension == 0)
			{
				network.YoutubeMessege.push_back("����̍Đ��̍Đ����I�����܂���");

				network.sendyotubecommand = 3 ;

				if (network.Extension == 0)yotube.pauseMovie();

				movieflag = 0;
			}
		}

		scene.yotubeQuestionnaire(ank);

		scene.displayChat();

		network.candelete = 1;

		Fps.displayFps(1280 - 20, 0);

		Fps.controlFps();
	}

	t.detach();

	network.close();

	DxLib_End();

	return 0;
}