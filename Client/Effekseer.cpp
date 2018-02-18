#include "Effekseer.h"



Effekseer_DX::Effekseer_DX()
{
	// 描画用インスタンスの生成
	g_device = (ID3D11Device*)GetUseDirect3D11Device();
	g_context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();

	g_renderer = ::EffekseerRendererDX11::Renderer::Create(g_device, g_context, MAX_INSTANCE_NUM);

	// エフェクト管理用インスタンスの生成
	g_manager = ::Effekseer::Manager::Create(2000);

	// 描画用インスタンスから描画機能を設定
	g_manager->SetSpriteRenderer(g_renderer->CreateSpriteRenderer());
	g_manager->SetRibbonRenderer(g_renderer->CreateRibbonRenderer());
	g_manager->SetRingRenderer(g_renderer->CreateRingRenderer());
	g_manager->SetTrackRenderer(g_renderer->CreateTrackRenderer());
	g_manager->SetModelRenderer(g_renderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	g_manager->SetTextureLoader(g_renderer->CreateTextureLoader());
	g_manager->SetModelLoader(g_renderer->CreateModelLoader());

	// DxLibに合わせて左手座標系に変更
	g_manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// 何でもいいから画像読込
	grHandle = LoadGraph("Effect/Texture/black.png");

	// エフェクトの読込
	g_effect = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*)L"Effect/laser.efk");

	// エフェクトの再生
	g_handle = g_manager->Play(g_effect, 0, 0, 0);
}


Effekseer_DX::~Effekseer_DX()
{
	// エフェクトの停止
	g_manager->StopEffect(g_handle);

	// エフェクトの破棄
	ES_SAFE_RELEASE(g_effect);

	// 先にエフェクト管理用インスタンスを破棄
	//g_manager->Destroy();

	// 次に描画用インスタンスを破棄
	g_renderer->Destroy();
}


void Effekseer_DX::update()
{

	time++;
	// 定期的にエフェクトを再生する
	if (time % 60 == 0)
	{
		// エフェクトの再生
		g_handle = g_manager->Play(g_effect, 0, 0, 0);
	}

	// 3. 透過フラグを立てて画像を描画
	// (黒一色(RGB = (0, 0, 0))の画像を用意！)
	DrawGraph(0, 0, grHandle, TRUE);

	// 頂点バッファに溜まった頂点データを吐き出す
	RenderVertex();

	// エフェクトの移動処理を行う
	g_manager->AddLocation(g_handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	// エフェクトの更新処理
	g_manager->Update();

	// DXライブラリ側のカメラ行列・射影行列と
	// Effekseer側のカメラ行列・射影行列の同期
	// 投影行列を設定
	MATRIX m = GetCameraProjectionMatrix();
	::Effekseer::Matrix44 f;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j<4; j++)
		{
			f.Values[i][j] = m.m[i][j];
		}
	}
	g_renderer->SetProjectionMatrix(f);

	// カメラ行列を設定
	m = GetCameraViewMatrix();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j<4; j++)
		{
			f.Values[i][j] = m.m[i][j];
		}
	}
	g_renderer->SetCameraMatrix(f);
	ClearDrawScreen();
	// 4. エフェクトの描画開始処理
	g_renderer->BeginRendering();

	// 5. Effekseer側のエフェクト表示
	g_manager->Draw();

	// 6. エフェクトの描画終了処理
	g_renderer->EndRendering();

	// 7. DXライブラリの設定を戻す
	RefreshDxLibDirect3DSetting();
}