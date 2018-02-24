#include "DxLib.h"

#include "Core.h"
#include "Common/Fps.h"
#include "Common/Model.h"
#include "Common/Camera.h"

#include "Bullet_physics.h"
#include "_dxdebugdraw.h"
#include "Youtube.h"
#include "Effekseer.h"
#include "NetworkManager.h"
#include "Scene.h"
#include "Character.h"
#include "Object.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Core core;
	if (core.setup() == -1)return -1;

	FpsManager fps;
	Camera camera;
	VECTOR vp = { 0,10,0 };

	std::shared_ptr<Bullet_physics> bullet(new Bullet_physics);
	int world = bullet->createWorld(VGet(-0, -0, -0), VGet(10000.f, 10000.f, 10000.f));

	//非同期読み込み設定に変更
	SetUseASyncLoadFlag(TRUE);

	Model stage;
	stage.loadModel(u8"System/Stage/唄川町 ver0.10/models/唄川町Light【軽量版】ver0.10.mv1");

	std::shared_ptr<Character> chara(new Character(bullet, world));
	chara->loadModel(u8"System/Model/Tda式初音ミクV4X_Ver1.00/Tda式初音ミクV4X_Ver1.00/Tda式初音ミクV4X.mv1");

	Model sky;
	sky.loadModel(u8"System/Skybox/skydome/skybox.mqo");
	
	//設定を戻す
	SetUseASyncLoadFlag(FALSE);

	//int specCubeTex = LoadGraph(u8"Skybox/GrandCanyon/skybox_32.dds");
	//SetUseTextureToShader(2, specCubeTex);

	Youtube yotube;
	yotube.upgrade();
	//while(yotube.downloadmovie("https://www.youtube.com/watch?v=wDVX2bEcJtk"))ProcessMessage();
	//yotube.playMovie();

	//Effekseer_DX effect;


	std::shared_ptr<NetworkManager> network(new NetworkManager(bullet, world, chara));

	Scene scene(network);
	if (!scene.update())
	{
		if (scene.login())
		{
			scene.loading();

			stage.setScale(10.f);
			bullet->createGroundBodytoMesh(stage.getModelHandle(), world);

			chara->setup();

			sky.setScale(40.f);

			while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				camera.MouseCamera(stage.getModelHandle(), chara->getPos(), vp);
				//camera.CameraAdditionally( chara.getPos(), chara.getRot(), vp);
				//camera.MouseCamera(stage.getModelHandle(), MV1GetFramePosition(chara->getModelHandle(), MV1SearchFrame(chara->getModelHandle(), u8"頭")));

				//yotube.update();

				chara->playAnime();

				chara->animeControl();

				bullet->stepSimulation(world);

				chara->update();

				network->multiplayerUpdate();

				sky.setPos(chara->getPos());

				// ライティングの計算をしないように設定を変更
				SetUseLighting(FALSE);

				sky.draw();

				SetUseLighting(TRUE);

				chara->draw();

				network->multiplayerDraw();

				stage.draw();

				//chara->debug();

				scene.drawName();

				scene.tutorial();

				if (!scene.chat()) {
					chara->playerMovementKeyboard();
				}
				else {
					chara->noMovement();
				}

				fps.displayFps(1280 - 20, 0);
				fps.measureFps();
			}
		}
	}

	DxLib_End();

	return 0;
}