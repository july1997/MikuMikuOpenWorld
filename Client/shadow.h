#pragma once
#include "DxLib.h"

class shadow
{
	// 影用の深度値を保存する描画可能グラフィック
	int DepthBufferGraphHandle;

	// 影用の深度記録画像を作成した際のカメラのビュー行列と射影行列
	MATRIX LightCamera_ViewMatrix;
	MATRIX LightCamera_ProjectionMatrix;

	// 深度記録画像への描画用の剛体メッシュ用頂点シェーダーと
	// スキニングメッシュ用の頂点シェーダー
	int Skin4_DepthShadow_Step1_VertexShader;
	int Normal_DepthShadow_Step1_VertexShader;

	// 深度記録画像への描画用のピクセルシェーダー
	int DepthShadow_Step1_PixelShader;

	// 深度記録画像を使ったディレクショナルライト一つ付きの描画用の剛体メッシュ用頂点シェーダーと
	// スキニングメッシュ用の頂点シェーダー
	int Skin4_DirLight_DepthShadow_Step2_VertexShader;
	int Normal_DirLight_DepthShadow_Step2_VertexShader;

	// 深度記録画像を使ったディレクショナルライト一つ付きの描画用のピクセルシェーダー
	int DirLight_DepthShadow_Step2_PixelShader;


	VECTOR defright, defdirection;

public:

	shadow();

	void SetupDepthImage(VECTOR playerpos)
	{
		// 描画先を影用深度記録画像にする
		SetDrawScreen(DepthBufferGraphHandle);

		// 影用深度記録画像を真っ白にクリア
		SetBackgroundColor(255, 255, 255);
		ClearDrawScreen();
		SetBackgroundColor(0, 0, 0);

		SetupCamera_Ortho(500.f);

		SetCameraNearFar(3.f, 2000.f);

		SetCameraPositionAndTargetAndUpVec(VAdd(playerpos, VGet(2.0f, 1000.0f, -35.0f)), VAdd(playerpos, VGet(0.0f, 12.0f, 0.0f)), VGet(0.0f, 0.7f, 0.6f));

		// 設定したカメラのビュー行列と射影行列を取得しておく
		LightCamera_ViewMatrix = GetCameraViewMatrix();
		LightCamera_ProjectionMatrix = GetCameraProjectionMatrix();

		// モデルの描画にオリジナルのシェーダーを使用するように設定する
		MV1SetUseOrigShader(TRUE);

		// 深度記録画像への描画用のピクセルシェーダーをセット
		SetUsePixelShader(DepthShadow_Step1_PixelShader);

		// 深度記録画像への剛体メッシュ描画用の頂点シェーダーをセット
		SetUseVertexShader(Normal_DepthShadow_Step1_VertexShader);
	}
	void SetupDepthImage2()
	{
		// 深度記録画像へのスキニングメッシュ描画用の頂点シェーダーをセット
		SetUseVertexShader(Skin4_DepthShadow_Step1_VertexShader);
	}
	void SetupDepthImage3()
	{
		// モデルの描画にオリジナルのシェーダーを使用するようにした設定を解除
		MV1SetUseOrigShader(FALSE);

		SetDrawScreen(DX_SCREEN_BACK);//裏画面に描画設定

		SetCameraNearFar(3.0f, 2000.0f);
	}

	void DrawModelWithDepthShadow(void)
	{
		// モデルの描画にオリジナルのシェーダーを使用するように設定する
		MV1SetUseOrigShader(TRUE);

		// 深度記録画像を使った影＋ディレクショナルライト一つ描画用のピクセルシェーダーをセット
		SetUsePixelShader(DirLight_DepthShadow_Step2_PixelShader);

		// 影用深度記録画像を描画したときのカメラのビュー行列と射影行列を定数に設定する
		SetVSConstFMtx(43, LightCamera_ViewMatrix);
		SetVSConstFMtx(47, LightCamera_ProjectionMatrix);

		// 影用深度記録画像をテクスSetUseVertexShaderチャ１にセット
		SetUseTextureToShader(1, DepthBufferGraphHandle);

		// 深度記録画像を使った影＋ディレクショナルライト一つの剛体メッシュ描画用の頂点シェーダーをセット
		SetUseVertexShader(Normal_DirLight_DepthShadow_Step2_VertexShader);
	}
	void DrawModelWithDepthShadow2(void)
	{
		// 深度記録画像を使った影＋ディレクショナルライト一つのスキニングメッシュ描画用の頂点シェーダーをセット
		SetUseVertexShader(Skin4_DirLight_DepthShadow_Step2_VertexShader);
	}
	void DrawModelWithDepthShadow3(void)
	{
		// 使用テクスチャの設定を解除
		SetUseTextureToShader(1, -1);

		// 設定した定数を解除
		ResetVSConstF(43, 16);
		ResetVSConstF(47, 16);

		// モデルの描画にオリジナルのシェーダーを使用するようにした設定を解除
		MV1SetUseOrigShader(FALSE);
	}

};

