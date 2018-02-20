#include "Updater.h"

Updater::Updater()
{
}


Updater::~Updater()
{
}

int Updater::downloadRelese()
{
	dl = new WebDownloader();

	//https://api.github.com/repos/july1997/MikuMikuOpenWorld/releases
	int r = dl->httpsOpen(
		(char *)"api.github.com",
		(char *)"repos/july1997/MikuMikuOpenWorld/releases",
		(char *)"api.github.com",
		(char *)"./", //.exeと同じディレクトリなら「"./"」
		(char *)"System/Update/releases.txt"                     //拡張子も必要なので注意
	);
	if (r == 0)dl->StartDownload();

	return r;
}

int Updater::getDownloadSize()
{
	return dl->getDownloadSize();
}

int Updater::getReadSize()
{
	return dl->getReadSize();
}

bool Updater::checkUpdate()
{
	//ファイルの読み込み
	std::ifstream fin("System/Update/releases.txt");

	if (!fin)
		return 1;

	std::stringstream strstream;
	strstream << fin.rdbuf();
	fin.close();

	//ファイルの内容をstringに入れる
	std::string data(strstream.str());

	size_t p = data.find("tag_name");
	std::string ver = data.substr(p + 11, (data.find(",", p) - 1) - (p + 11));

	std::string vers(version);
	vers = vers.substr(21, vers.size() - 21);

	if (ver != vers) {
		return 1;
	}

	return 0;
}

void Updater::lunchUpdater()
{
	SHELLEXECUTEINFO sei;
	memset(&sei, 0, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.hwnd = NULL;
	sei.lpVerb = "runas";
	sei.lpFile = "Updater.exe";
	sei.nShow = SW_SHOWNORMAL;

	//標準出力を獲得するならCreateProcessを使う
	if (ShellExecuteEx(&sei) == FALSE) {
		//失敗した場合のエラー処理
		MessageBox(GetMainWindowHandle(), "ShellExecuteEx Failed.", "error", MB_OK);//エラー表示
	}
}