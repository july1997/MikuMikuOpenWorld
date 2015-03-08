#pragma once

//フレームレート関係
//画面の更新速度を一定にする

class FpsManager
{
private:
	int mStartTime = 0,
		mCount = 0,
		FPS = 60,
		t = 0;
	float mFps=0;

public:
	//コンストラクタ
	FpsManager();
	//デストラクタ
	~FpsManager();

	//フレームレートを制御し、画面をフリップします。
	void controlFps();

	//フレームレート値を表示します
	void displayFps(int x, int y, int color = -1);

	//フレームレート値の設定を変更します
	void setFps(int fps);

	//現在のフレームレート値を取得します
	float getNowFps();

	//フレームレート値の設定を取得します
	int getDefaultFps();

	//フレームレートを制御するだけの関数
	void controlWaitFps();

	//フレームレートを制御し、画面をフリップし、裏画面を初期化します。
	void controlClearDrawScreenFps();

	int getWaitTime();
};