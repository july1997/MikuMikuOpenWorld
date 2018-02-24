#pragma once

#include "DxLib.h"

#include "WebDownloader.h"
#include "Version.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Updater
{
	WebDownloader* dl;

	DWORD dwD;
	HANDLE hHandle;

public:
	Updater();
	~Updater();

	int downloadRelese();
	int checkUpdate();

	int getDownloadSize();
	int getReadSize();

	void lunchUpdater();
};

