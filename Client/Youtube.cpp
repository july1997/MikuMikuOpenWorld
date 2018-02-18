#include "Youtube.h"

HANDLE m_ChildProcess;

Youtube::Youtube()
{
	screen_handel = MakeScreen(640,360, FALSE);
}


Youtube::~Youtube()
{
	deleteProcess();

	deleteCache();
}

size_t Youtube::downloadmovie(std::string VideoURL)
{
	if (!downloadmovieflag)downloadmovieflag = 4;

	switch (downloadmovieflag) {
	case 4:
		deleteProcess();
		downloadmovieflag = 3;

		break;
	case 3:
		if (!deleteCache())downloadmovieflag = 2;

		break;
	case 2:
		if (VideoURL.find("https://www.youtube.com/watch?") != std::string::npos)
		{
			exe = std::string("cache/youtube-dl.exe");
			std::string pa("-f best[height=360] --no-part -o cache/cache.mp4 ");
			std::string th(VideoURL);
			path = pa + th;
		}
		else
		{
			exe = std::string("cache/youtube-dl.exe");
			std::string pa("--no-part -o cache/cache.mp4 ");
			std::string th(VideoURL);
			path = pa + th;
		}

		//yotube-dlを使って動画をダウンロード
		//yotube-dlでダウンロードURLを取得できますが直接には再生できません
		yotube_dl(exe.c_str(), path.c_str());

		downloadmovieflag = 1;

		timer = 0;

		break;
	case 1:

		GetExitCodeThread(hHandle, &dwD);
		if (dwD != STILL_ACTIVE)downloadmovieflag = 0;

		//test.mp4ができるまでループ
		//if (stat("cache/cache.mp4", &buf) == 0)downloadmovieflag = 0;

		//timer++;
		//if (timer == 30 * 30)downloadmovieflag = 0;

		break;
	}

	return downloadmovieflag;
}

void Youtube::playMovie()
{
	screen_handel = LoadGraph("cache/cache.mp4");
	PlayMovieToGraph(screen_handel);
}

void Youtube::update()
{
	DrawGraph(0, 0, screen_handel, FALSE);
}

void Youtube::pause()
{
	PauseMovieToGraph(screen_handel);
}

void Youtube::seek(int time)
{
	SeekMovieToGraph(screen_handel, time);
}

int Youtube::getSeek()
{
	return TellMovieToGraph(screen_handel);
}

void Youtube::deleteProcess()
{
	GetExitCodeThread(hHandle, &dwD);
	if (dwD == STILL_ACTIVE)//プロセスが起動中
	{
		//プロセスを強制終了
		::TerminateProcess(m_ChildProcess, 0);

		CloseHandle(hHandle);
	}
}

bool Youtube::deleteCache()
{
	if (!deleteCacheflag)
	{
		//前のキャッシュがあったら消す
		DeleteFile("cache/cache.mp4");

		deleteCacheflag = 1;

		timer = 0;
	}

	//消えるまで待つ
	if (stat("cache/cache.mp4", &buf) != 0)deleteCacheflag = 0;

	timer++;
	if (timer == 30 * 10)deleteCacheflag = 0;

	return deleteCacheflag;
}

bool Youtube::upgrade()
{
	if (!upgradeflag) {
		exe = std::string("cache/youtube-dl.exe");
		std::string c("-U ");

		yotube_dl(exe.c_str(), c.c_str());

		upgradeflag = 1;
	}
	GetExitCodeThread(hHandle, &dwD);
	if (dwD != STILL_ACTIVE)upgradeflag = 0;

	return upgradeflag;
}

//マルチスレッド関係------------------------------------------------------------------------------
//保存しながら再生するために_beginthreadexを使ってます(std::threadではエラー)

//マルチスレッドに渡すため
char Url[256];
char Exe[256];

DWORD WINAPI ThreadFunc(PVOID pv) //マルチスレッドで行う処理
{
	SHELLEXECUTEINFO sei;
	memset(&sei, 0, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.hwnd = GetForegroundWindow();
	sei.lpVerb = _T("open");
	sei.lpFile = Exe;
	sei.lpParameters = Url;
	sei.nShow = SW_HIDE;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;

	//標準出力を獲得するならCreateProcessを使う
	if (ShellExecuteEx(&sei) == FALSE) {
		//失敗した場合のエラー処理
		MessageBox(GetMainWindowHandle(), "ShellExecuteEx Failed.", "error", MB_OK);//エラー表示
	}
	// プロセスハンドルを保存
	m_ChildProcess = sei.hProcess;

	//終了するまで待つ
	WaitForSingleObject(sei.hProcess, INFINITE);

	return 0;
}

void Youtube::yotube_dl(const char *exe, const char *word)
{
	static TCHAR lpszCmd[MAX_PATH + 64];

	strcpy_s(Url, word);
	strcpy_s(Exe, exe);

	//マルチスレッドを作成-------------------------------------
	DWORD dwThread;

	hHandle = CreateThread(NULL, 0, ThreadFunc, (LPVOID)lpszCmd, 0, &dwD);

	//エラーチェック
	if (hHandle == 0)
	{
		MessageBox(GetMainWindowHandle(), "_beginthreadex Failed.", "error", MB_OK);//エラー表示
	}
	//--------------------------------------------------------
}

//-----------------------------------------------------------------------------------------------