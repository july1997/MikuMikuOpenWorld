#pragma once

#include "DxLib.h"

#include <string>
#include <fstream>

#include "fps.h"

using std::string;

#include <memory>
typedef std::shared_ptr<FpsManager> Fps_ptr;

class Core
{
private:

public:
	//�R���X�g���N�^
	Core();
	//�f�X�g���N�^
	~Core();

	//�A�v���P�[�V�����̋N��
	//int Run();

	int setupDxLib();

	int readFileSearchToWord_int(char *file, char *word);

	void loadloop(Fps_ptr& Fps);

};

