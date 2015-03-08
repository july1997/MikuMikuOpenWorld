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
	//コンストラクタ
	Core();
	//デストラクタ
	~Core();

	//アプリケーションの起動
	//int Run();

	int setupDxLib();

	int readFileSearchToWord_int(char *file, char *word);

	void loadloop(Fps_ptr& Fps);

};

