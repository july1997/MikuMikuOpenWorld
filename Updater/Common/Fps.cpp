#include "fps.h"
#include "DxLib.h"


FpsManager::FpsManager()
{
}


FpsManager::~FpsManager()
{
}

void FpsManager::controlFps()
{
	if (mCount == 0){
		mStartTime = GetNowCount();
	}
	if (mCount == FPS){
		mFps = 1000.f / ((GetNowCount() - mStartTime) / (float)FPS);
		mStartTime = GetNowCount();
		mCount = 0;
	}
	mCount++;

	ScreenFlip();

	t = mCount * 1000 / FPS - (GetNowCount() - mStartTime);

	WaitTimer(t);
}

void FpsManager::controlWaitFps()
{
	if (mCount == 0){
		mStartTime = GetNowCount();
	}
	if (mCount == FPS){
		mFps = 1000.f / ((GetNowCount() - mStartTime) / (float)FPS);
		mStartTime = GetNowCount();
		mCount = 0;
	}
	mCount++;

	t = mCount * 1000 / FPS - (GetNowCount() - mStartTime);

	WaitTimer(t);
}

void FpsManager::controlClearDrawScreenFps()
{
	if (mCount == 0){
		mStartTime = GetNowCount();
	}
	if (mCount == FPS){
		mFps = 1000.f / ((GetNowCount() - mStartTime) / (float)FPS);
		mStartTime = GetNowCount();
		mCount = 0;
	}
	mCount++;

	ScreenFlip();

	ClearDrawScreen();

	t = mCount * 1000 / FPS - (GetNowCount() - mStartTime);

	WaitTimer(t);
}

void FpsManager::measureFps()
{
	if (mCount == 0) {
		mStartTime = GetNowCount();
	}
	if (mCount == FPS) {
		mFps = 1000.f / ((GetNowCount() - mStartTime) / (float)FPS);
		mStartTime = GetNowCount();
		mCount = 0;
	}
	mCount++;

	ScreenFlip();

	ClearDrawScreen();
}

int FpsManager::getWaitTime()
{
	return t;
}

void FpsManager::displayFps(int x, int y, int color)
{
	DrawFormatString(x, y, color, "%.f", mFps);
}

void FpsManager::setFps(int fps)
{
	FPS = fps;
}

float FpsManager::getNowFps()
{
	return mFps;
}

int FpsManager::getDefaultFps()
{
	return FPS;
}