#pragma once
#include "Movie.h"

#include <vector>

class Youtube : public Movie
{
public:
	Youtube();
	~Youtube();

	//URL���瓮����_�E�����[�h���čĐ�����@�߂�l:1 ����
	int playYotubeMovie(std::string &VideoURL);

	//���J�����̐ݒ肪����������܂�
	void updateYotubeMovie();

	//ModelHandel��ScreenTexture(�e�N�X�`���ԍ�)�ɓ�����Đ����邽�߂̏���
	void setScreen(const int &ModelHandel, const int ScreenTexture);

	//�Đ����ꎞ��~����
	void pauseYotubeMovie();

	//�Đ����ĊJ����
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

