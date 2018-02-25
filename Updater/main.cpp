#include "DxLib.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "UI/UI.h"
#include "Core.h"
#include "Common/Fps.h"
#include "WebDownloader.h"


//ZIP展開 関係------------------------------------------------------------------------------
#include "unzip.h"
using namespace std;

#pragma comment(lib, "zlib.lib")

#define for if (0) ; else for
#define IsShiftJIS(x) ((BYTE)((x ^ 0x20) - 0xA1) <= 0x3B)

//---------------------------------------------------------------------------
// ファイルが存在するかどうか
bool IsFileExist(const string &strFilename) {
	return GetFileAttributes(strFilename.c_str()) != 0xffffffff;
}

//---------------------------------------------------------------------------
// 再帰的にディレクトリを作成（strPathの末尾には必ず\をつけること）
bool CreateDirectoryReflex(const string &strPath) {
	const char *p = strPath.c_str();
	for (; *p; p += IsShiftJIS(*p) ? 2 : 1)
		if (*p == '\\') {
			string strSubPath(strPath.c_str(), p);
			if (!IsFileExist(strSubPath.c_str()))
				if (!CreateDirectory(strSubPath.c_str(), NULL))
					return false;
		}
	return true;
}

//---------------------------------------------------------------------------
/* unz_file_info_interntal contain internal info about a file in zipfile*/
typedef struct unz_file_info64_internal_s
{
	ZPOS64_T offset_curfile;/* relative offset of local header 8 bytes */
} unz_file_info64_internal;
/* file_in_zip_read_info_s contain internal information about a file in zipfile,
when reading and decompress it */
typedef struct
{
	char  *read_buffer;         /* internal buffer for compressed data */
	z_stream stream;            /* zLib stream structure for inflate */

#ifdef HAVE_BZIP2
	bz_stream bstream;          /* bzLib stream structure for bziped */
#endif

	ZPOS64_T pos_in_zipfile;       /* position in byte on the zipfile, for fseek*/
	uLong stream_initialised;   /* flag set if stream structure is initialised*/

	ZPOS64_T offset_local_extrafield;/* offset of the local extra field */
	uInt  size_local_extrafield;/* size of the local extra field */
	ZPOS64_T pos_local_extrafield;   /* position in the local extra field in read*/
	ZPOS64_T total_out_64;

	uLong crc32;                /* crc32 of all data uncompressed */
	uLong crc32_wait;           /* crc32 we must obtain after decompress all */
	ZPOS64_T rest_read_compressed; /* number of byte to be decompressed */
	ZPOS64_T rest_read_uncompressed;/*number of byte to be obtained after decomp*/
	zlib_filefunc64_32_def z_filefunc;
	voidpf filestream;        /* io structore of the zipfile */
	uLong compression_method;   /* compression method (0==store) */
	ZPOS64_T byte_before_the_zipfile;/* byte before the zipfile, (>0 for sfx)*/
	int   raw;
} file_in_zip64_read_info_s;
typedef struct
{
	zlib_filefunc64_32_def z_filefunc;
	int is64bitOpenFunction;
	voidpf filestream;        /* io structore of the zipfile */
	unz_global_info64 gi;       /* public global information */
	ZPOS64_T byte_before_the_zipfile;/* byte before the zipfile, (>0 for sfx)*/
	ZPOS64_T num_file;             /* number of the current file in the zipfile*/
	ZPOS64_T pos_in_central_dir;   /* pos of the current file in the central dir*/
	ZPOS64_T current_file_ok;      /* flag about the usability of the current file*/
	ZPOS64_T central_pos;          /* position of the beginning of the central dir*/

	ZPOS64_T size_central_dir;     /* size of the central directory  */
	ZPOS64_T offset_central_dir;   /* offset of start of central directory with
								   respect to the starting disk number */

	unz_file_info64 cur_file_info; /* public info about the current file in zip*/
	unz_file_info64_internal cur_file_info_internal; /* private info about it*/
	file_in_zip64_read_info_s* pfile_in_zip_read; /* structure about the current
												  file if we are decompressing it */
	int encrypted;

	int isZip64;

#    ifndef NOUNCRYPT
	unsigned long keys[3];     /* keys defining the pseudo-random sequence */
	const z_crc_t* pcrc_32_tab;
#    endif
} unz64_s;
//----------------------------------------------------------------------------------------------

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
	if (ver.substr(ver.rfind(".") + 1, 1) == "0")majorupdate = 1;

	//ダウンロード----------------------------------------------------------------------------------
	size_t up = data.find("browser_download_url");
	std::string url = data.substr(up + 23, (data.find('?"', up + 23)) - (up + 23));

	WebDownloader* dl = new WebDownloader();
	int r = dl->httpsOpen(
		(char *)"github.com",
		(char *)url.substr(19, url.size()).c_str(),
		(char *)"github.com",
		(char *)"./", //.exeと同じディレクトリなら「"./"」
		(char *)"Update.zip"                     //拡張子も必要なので注意
	);
	if (r == 0)dl->StartDownload();

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

			if (e < ((double)dl->getReadSize() / (double)dl->getDownloadSize()) * (double)390)e += 2;

			ui.drawString(font, std::to_string(dl->getReadSize()) + u8" / " + std::to_string(dl->getDownloadSize()) + u8" Byte", 640, 335, 1);

			if (dl->getReadSize() >= dl->getDownloadSize()) {
				WaitTimer(3000);
				break;
			}
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
	e = 0;
	bool installed = 0, remove = 0;
	if (!majorupdate)remove = 1;

	string strZipFilename = "Update.zip", strTargetPath = "";
	unzFile hUnzip = unzOpen(strZipFilename.c_str());
	if (!hUnzip) {
		MessageBox(GetMainWindowHandle(), "ファイルを開けません", "error", MB_OK);
		return 0;
	}
	unz64_s* s;
	s = (unz64_s*)hUnzip;
	size_t zipfilesize = s->offset_central_dir,nowfilesize = 0.0;

	while (ProcessMessage() == 0)
	{
		ui.drawBackgroundImage();

		ui.drawString(font3, u8" Update to Miku Miku Open World " + ver, 0, 0, 0, GetColor(238, 238, 238));

		ui.drawString(font2, u8"インストール中です", 640, 250, 1);

		ui.drawBox(box, 128);

		ui.changeBoxRoundedBar(inbox, e, 30, GetColor(195, 216, 37), GetColor(195, 216, 37), 0);
		ui.drawBox(inbox, 128);

		if (!remove) {
			removeDirectory("System");

			remove = 1;
		}
		else {

			char szConFilename[512];
			unz_file_info fileInfo;
			if (unzGetCurrentFileInfo(hUnzip, &fileInfo, szConFilename, sizeof szConFilename, NULL, 0, NULL, 0) != UNZ_OK)
			break;

			int nLen = strlen(szConFilename);
			for (int i = 0; i < nLen; ++i)
				if (szConFilename[i] == '/')
					szConFilename[i] = '\\';

			// ディレクトリの場合
			//if (nLen >= 2 && !IsShiftJIS(szConFilename[nLen - 2]) && szConFilename[nLen - 1] == '\\') {
			//	CreateDirectoryReflex(strTargetPath + szConFilename);
			//	continue;
			//}

			// ファイルの場合
			if (unzOpenCurrentFile(hUnzip) != UNZ_OK)break;
			CreateDirectoryReflex(strTargetPath + szConFilename);
			HANDLE hFile = CreateFile((strTargetPath + szConFilename).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			BYTE szBuffer[8192];
			DWORD dwSizeRead;
			while ((dwSizeRead = unzReadCurrentFile(hUnzip, szBuffer, sizeof szBuffer)) > 0)
				WriteFile(hFile, szBuffer, dwSizeRead, &dwSizeRead, NULL);
			FlushFileBuffers(hFile);
			CloseHandle(hFile);
			unzCloseCurrentFile(hUnzip);

			if (unzGoToNextFile(hUnzip) == UNZ_END_OF_LIST_OF_FILE) {

				unzClose(hUnzip);
				installed = 1;
				break;
			}

			nowfilesize += fileInfo.compressed_size;

			ui.drawString(font, std::to_string(nowfilesize) + u8" / " + std::to_string(zipfilesize) + u8" Byte", 640, 335, 1);

			if (e < ((double)nowfilesize / (double)zipfilesize) * (double)390)e += 2;
		}

		//fps.displayFps(1280 - 20, 0);
		fps.measureFps();
	}

	//MikuMikuOpenWorldの起動-------------------------------------------------------------------------------------
	if (installed) {
		DeleteFile("Update.zip");
		struct stat buf;

		while (ProcessMessage() == 0)
		{
			ui.drawBackgroundImage();

			ui.drawString(font3, u8" Update to Miku Miku Open World " + ver, 0, 0, 0, GetColor(238, 238, 238));

			ui.drawString(font2, u8"終了しています", 640, 250, 1);

			//消えるまで待つ
			if (stat("Update.zip", &buf) != 0)break;

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
	else {
		MessageBox(GetMainWindowHandle(), "エラーが発生しました", "error", MB_OK);
	}

	DxLib_End();

	return 0;
}