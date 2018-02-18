#pragma once

#include "DxLib.h"

#include <string>

class Youtube
{
public:
	Youtube();
	~Youtube();

	//URLから動画をダウンロードして再生する　戻り値:0　終了
	size_t downloadmovie(std::string VideoURL);

	//再生を開始
	void playMovie();

	//画面の更新
	void update();

	//再生を一時停止、再開する
	void pause();

	//再生位置を変更する
	void seek(int time);
	
	//再生位置を得る
	int getSeek();

	//yotude-dlの更新
	bool upgrade();

private:

	int screen_handel;

	size_t downloadmovieflag = 3;
	size_t timer = 0;
	bool deleteCacheflag = 0, upgradeflag = 0;
	struct stat buf;

	void deleteProcess();
	bool deleteCache();

	std::string exe;
	std::string path;

	void yotube_dl(const char *exe, const char *word);

	DWORD dwD;
	HANDLE hHandle;
};

