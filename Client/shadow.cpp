#include "shadow.h"


shadow::shadow()
{
	// 作成する画像のフォーマットを不動小数点型で１チャンネル、１６ビットにする
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);
	SetCreateDrawValidGraphChannelNum(1);
	SetCreateGraphColorBitDepth(16);

	// 影用深度記録画像の作成
	DepthBufferGraphHandle = MakeScreen(2048, 2048, FALSE);

	// 設定を元に戻す
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);

	// 深度記録画像描画用の頂点シェーダーを読み込む
	Skin4_DepthShadow_Step1_VertexShader = LoadVertexShader("Shader/shadow/Vso/SkinMesh4_DepthShadow_Step1VS.vso");
	Normal_DepthShadow_Step1_VertexShader = LoadVertexShader("Shader/shadow/Vso/NormalMesh_DepthShadow_Step1VS.vso");

	// 深度記録画像描画用のピクセルシェーダーを読み込む
	DepthShadow_Step1_PixelShader = LoadPixelShader("Shader/shadow/Pso/DepthShadow_Step1PS.pso");

	// 深度記録画像を使ったディレクショナルライト一つの描画用頂点シェーダーを読み込む
	Normal_DirLight_DepthShadow_Step2_VertexShader = LoadVertexShader("Shader/shadow/Vso/NormalMesh_DirLight_DepthShadow_Step2VS.vso");

	// 深度記録画像を使ったディレクショナルライト一つの描画用ピクセルシェーダーを読み込む
	DirLight_DepthShadow_Step2_PixelShader = LoadPixelShader("Shader/shadow/Pso/DirLight_DepthShadow_Step2PS.pso");
}


