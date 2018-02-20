#include "Core.h"
#include "DxLib.h"


Core::Core()
{

}


Core::~Core()
{

}


size_t Core::setup()
{
	//画面モード
	SetGraphMode(1280, 720, 32);
	ChangeWindowMode(1);
	SetMainWindowText("MikuMikuOpenWorld Updater");//ウィンドウモード

	//　ウインドウモードの時にウインドウのサイズを自由に変更出来るように設定する
	SetWindowSizeChangeEnableFlag(TRUE);

	SetMultiThreadFlag(TRUE);// マルチスレッドに適したモードで起動する

	//ログを出力しない
	SetOutApplicationLogValidFlag(FALSE);

	// DirectX11を使用する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//Zバッファの深度
	SetZBufferBitDepth(24);

	//TCPで送信するデータの先頭に４バイト付けない
	SetUseDXNetWorkProtocol(FALSE);

	SetAlwaysRunFlag(TRUE);//ウインドウがアクティブではない状態でも処理を続行する
	//SetWaitVSyncFlag(FALSE);//垂直同期を待たない

	//多重起動
	SetDoubleStartValidFlag(TRUE);

	//UTF8
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	if (DxLib_Init() == -1)return -1;

	//SetWindowUserCloseEnableFlag(FALSE);//閉じるボタンを押しても閉じない

	SetMouseDispFlag(TRUE);//マウスカーソルを表示

	SetDrawScreen(DX_SCREEN_BACK);//裏画面に描画設定

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// Zバッファを有効にする。
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする。
	SetWriteZBuffer3D(TRUE);


	return 0;
}