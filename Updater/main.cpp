#include "DxLib.h"

#include "UI/UI.h"
#include "Core.h"
#include "Common/Fps.h"
#include "WebDownloader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>



//マルチスレッド関係------------------------------------------------------------------------------

HANDLE m_ChildProcess;

DWORD dwD;
HANDLE hHandle;

HANDLE hReadPipe;
HANDLE hWritePipe;

DWORD WINAPI ThreadFunc(PVOID pv) //マルチスレッドで行う処理
{
	SECURITY_ATTRIBUTES PipeAttributes;
	ZeroMemory(&PipeAttributes, sizeof(SECURITY_ATTRIBUTES));

	PipeAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	PipeAttributes.bInheritHandle = TRUE;
	PipeAttributes.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&hReadPipe, &hWritePipe, &PipeAttributes, 0))
	{
		// 失敗
	}

	PROCESS_INFORMATION ProcessInformation;
	ZeroMemory(&ProcessInformation, sizeof(PROCESS_INFORMATION));

	STARTUPINFO StartUpInfo;
	ZeroMemory(&StartUpInfo, sizeof(STARTUPINFO));
	StartUpInfo.cb = sizeof(STARTUPINFO);
	StartUpInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	StartUpInfo.wShowWindow = SW_HIDE; // これで、非表示
	StartUpInfo.hStdOutput = hWritePipe;

	TCHAR pCommandLine[] = _T("Update.exe -y");

	if (!CreateProcess
	(
		NULL,
		pCommandLine,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&StartUpInfo,
		&ProcessInformation
	))
	{
		// 失敗
	}

	// プロセスハンドルを保存
	m_ChildProcess = ProcessInformation.hProcess;

	return 0;
}

void update()
{
	static TCHAR lpszCmd[MAX_PATH + 64];

	//マルチスレッドを作成-------------------------------------
	DWORD dwThread;

	hHandle = CreateThread(NULL, 0, ThreadFunc, (LPVOID)lpszCmd, 0, &dwD);

	//エラーチェック
	if (hHandle == 0)
	{
		MessageBox(GetMainWindowHandle(), "_beginthreadex Failed.", "error", MB_OK);//エラー表示
	}
	//--------------------------------------------------------

	//マルチスレッドを開始する（即座に制御を返す）
	WaitForSingleObject(hHandle, 0);
}

//-----------------------------------------------------------------------------------------------

//フォルダ削除 関係------------------------------------------------------------------------------
//この関数の引数fileNameは\で終わってはいけません。 
bool removeDirectory(std::string fileName)
{
	bool retVal = true;
	std::string nextFileName;

	WIN32_FIND_DATA foundFile;

	HANDLE hFile = FindFirstFile((fileName + "\\*.*").c_str(), &foundFile);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			//If a found file is . or .. then skip
			if (strcmp(foundFile.cFileName, ".") != 0 && strcmp(foundFile.cFileName, "..") != 0)
			{
				//The path should be absolute path
				nextFileName = fileName + "\\" + foundFile.cFileName;

				//If the file is a directory
				if ((foundFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				{
					removeDirectory(nextFileName.c_str());
					RemoveDirectory(nextFileName.c_str());
				}
				//If the file is a file
				else
				{
					DeleteFile(nextFileName.c_str());
				}
			}
		} while (FindNextFile(hFile, &foundFile) != 0);
	}

	FindClose(hFile);

	//Delete starting point itseft
	if (RemoveDirectory(fileName.c_str()) == 0)retVal = false;

	return retVal;
}

//----------------------------------------------------------------------------------------------



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Core core;
	if (core.setup() == -1)return -1;

	FpsManager fps;

	UI ui;
	int font, font2, font3;

	//画像の読み込み
	ui.loadBackgroundImage("System/Picture/background.jpg");
	font3 = ui.loadDxFont("System/Font/Koruri Light4.dft");
	font2 = ui.loadDxFont("System/Font/Koruri Light3.dft");
	font = ui.loadDxFont("System/Font/Koruri Light2.dft");

	int box = ui.makeBoxRoundedBar(400, 40, 440, 290, GetColor(145, 145, 145), GetColor(145, 145, 145), 0);
	int inbox = ui.makeBoxRoundedBar(390, 30, 445, 295, GetColor(195, 216, 37), GetColor(195, 216, 37), 0);
	int e = 0;

	//ファイルの読み込み
	std::ifstream fin("System/Update/releases.txt");

	if (!fin)
		return 1;

	std::stringstream strstream;
	strstream << fin.rdbuf();
	fin.close();

	//ファイルの内容をstringに入れる
	std::string data(strstream.str());

	bool majorupdate = 0;
	size_t p = data.find("tag_name");
	std::string ver = data.substr(p + 11, (data.find(",", p) - 1) - (p + 11));
	if (ver.substr(ver.rfind(".")+1, 1) == "0")majorupdate = 1;

	//ダウンロード----------------------------------------------------------------------------------

	size_t up = data.find("browser_download_url");
	std::string url = data.substr(up + 23, (data.find('?"', up + 23)) - (up + 23));
	
	WebDownloader* dl = new WebDownloader();
	int r = dl->httpsOpen(
		(char *)"github.com",
		(char *)url.substr(19, url.size()).c_str(),
		(char *)"github.com",
		(char *)"./", //.exeと同じディレクトリなら「"./"」
		(char *)"Update.exe"                     //拡張子も必要なので注意
	);
	if (r == 0)dl->StartDownload();

	int mastload = dl->getDownloadSize();

	//ダウンロード
	while (ProcessMessage() == 0)
	{
		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0)return 0;

		ui.drawBackgroundImage();

		ui.drawString(font3, u8" Update to Miku Miku Open World " + ver, 0, 0, 0, GetColor(238, 238, 238));

		if (r == 0)
		{
			ui.drawString(font2, u8"ダウンロードしています", 640, 250, 1);

			ui.drawBox(box, 128);

			ui.changeBoxRoundedBar(inbox, e, 30, GetColor(195, 216, 37), GetColor(195, 216, 37), 0);
			ui.drawBox(inbox, 128);
			if (e < ((double)dl->getReadSize() / (double)mastload) * (double)390)e += 1;

			ui.drawString(font, std::to_string(dl->getReadSize()) + u8" / " + std::to_string(dl->getDownloadSize()) + u8" Byte", 640, 335, 1);

			if (dl->getReadSize() >= dl->getDownloadSize())break;
		}
		else
		{
			ui.drawString(font, u8"ダウンロードに失敗　エラーコード" + std::to_string(r), 640, 270, GetColor(237, 28, 36));
			WaitKey();
			return 0;
		}

		//fps.displayFps(1280 - 20, 0);
		fps.measureFps();
	}
	

	//アップデート----------------------------------------------------------------------------------
	update();
	
	BYTE pbBuffer[4096];
	memset(pbBuffer, '\0', 4096);

	DWORD olddwAvailSize = 0;

	std::string ss;
	e = 0;
	bool installed = 0, remove = 0;

	if (!majorupdate)remove = 1;

	size_t timer = 0;
	while (ProcessMessage() == 0)
	{
		ui.drawBackgroundImage();

		ui.drawString(font3, u8" Update to Miku Miku Open World " + ver, 0, 0, 0, GetColor(238, 238, 238));

		ui.drawString(font2, u8"インストール中です", 640, 250, 1);

		if (!remove) {
			removeDirectory("System");

			remove = 1;
		}
		else {

			DWORD dwAvailSize = 0;

			PeekNamedPipe(hReadPipe, NULL, 0, NULL, &dwAvailSize, NULL);

			memset(pbBuffer, '\0', olddwAvailSize);
			olddwAvailSize = dwAvailSize;

			if (dwAvailSize > 0)
			{
				DWORD dwRead = 0;
				ReadFile(hReadPipe, pbBuffer, sizeof(pbBuffer), &dwRead, NULL);
			}
			else {
				if (WAIT_OBJECT_0 == WaitForSingleObject(m_ChildProcess, 0))
				{
					if (e >= 390) {
						installed = 1;
						break;
					}
				}
			}

			std::string s((char*)pbBuffer);

			if (s == "")timer++; else timer = 0;
			if (timer > 1000) {
				MessageBox(GetMainWindowHandle(), "エラーが発生しました", "error", MB_OK);
				return 0;
			}

			if (s.size() > 0) {
				size_t t = 0;
				while (1) {
					if (s.find('%', t) == std::string::npos)break;
					t = s.find('%', t);
					t += 1;
				}

				if (t != 0) {
					ss = s.substr(t - 3, 3);
				}
			}
		}

		ui.drawBox(box, 128);

		ui.changeBoxRoundedBar(inbox, e, 30, GetColor(195, 216, 37), GetColor(195, 216, 37), 0);
		ui.drawBox(inbox, 128);
		if (e < ((std::atof(ss.substr(0, 2).c_str()) + 10.0) / (double)100) * (double)390)if (e < 390)e += 1;

		//fps.displayFps(1280 - 20, 0);
		fps.measureFps();
	}
	
	//MikuMikuOpenWorldの起動-------------------------------------------------------------------------------------
	if (installed) {
		DeleteFile("Update.exe");
		struct stat buf;

		while (ProcessMessage() == 0)
		{
			ui.drawBackgroundImage();

			ui.drawString(font3, u8" Update to Miku Miku Open World " + ver, 0, 0, 0, GetColor(238, 238, 238));

			ui.drawString(font2, u8"終了しています", 640, 250, 1);

			//消えるまで待つ
			if (stat("Update.exe", &buf) != 0)break;

			//fps.displayFps(1280 - 20, 0);
			fps.measureFps();
		}

		//MikuMikuOpenWorldの起動
		SHELLEXECUTEINFO sei;
		memset(&sei, 0, sizeof(sei));
		sei.cbSize = sizeof(sei);
		sei.hwnd = NULL;
		sei.lpVerb = _T("open");
		sei.lpFile = "MikuMikuOpenWorld.exe";
		sei.nShow = SW_SHOWNORMAL;

		//標準出力を獲得するならCreateProcessを使う
		if (ShellExecuteEx(&sei) == FALSE) {
			//失敗した場合のエラー処理
			MessageBox(GetMainWindowHandle(), "ShellExecuteEx Failed.", "error", MB_OK);//エラー表示
		}

	}

	DxLib_End();

	return 0;
}