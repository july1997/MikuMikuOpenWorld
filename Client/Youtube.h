#pragma once
#include "Movie.h"

#include <vector>

class Youtube : public Movie
{
public:
	Youtube();
	~Youtube();

	//URLから動画をダウンロードして再生する　戻り値:1 成功
	int playYotubeMovie(std::string &VideoURL);

	//＊カメラの設定が初期化されます
	void updateYotubeMovie();

	//ModelHandelのScreenTexture(テクスチャ番号)に動画を再生するための準備
	void setScreen(const int &ModelHandel, const int ScreenTexture);

	//再生を一時停止する
	void pauseYotubeMovie();

	//再生を再開する
	void playYotubeMovie();

private:

	size_t timer = 0;
	bool deleteProcessflag = 0, deleteCacheflag = 0, file = 0, downloadmovieflag = 0, moviefile = 0;
	struct stat buf;
	bool deleteProcess();
	bool deleteCache();
	bool downloadmovie(std::string &VideoURL);
	int step = 0;
	int loadinig;
	int looptime = 0, looptime2 = 0;

	int movieflag = 0;

	int screen_handel;

	HANDLE hHandle;

	void yotube_dl(const char *exe, const char *word);

	std::vector<int> screen_tex;
	std::vector<int> model_handel;

	int GraHandle;

};

