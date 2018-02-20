#pragma once
#include "DxLib.h"

#include <fstream>
#include <thread>

#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")

//https://the-united-front.blogspot.jp/2014/05/wininetinternetreadfilehtml.html 様参考
//-106のエラーはセキュリティソフトに弾かれてる可能性あり
//
//返り値一覧
//
//-106:ERR_INTERNET_DISCONNECTED        インターネットに接続されていない。
//-403:"403 Forbidden"                                サーバーにリクエストが拒否された。
//-404:"404 Not Found"                                指定したURLにドキュメントが存在しない。
//-1       

class WebDownloader
{
private:
	HINTERNET hInternet;
	HINTERNET hHttpSession;
	HINTERNET hHttpRequest;
	DWORD RequiredBufSize, RequiredBufSize_Size;
	DWORD ReadSize;        //一回の操作で読みだされたサイズ
	DWORD ReadSize_sum;        //読み出しサイズの合計
	DWORD dwReadSize;
	FILE *file;
	char *Buf_main;
	bool isOpen = 0;
	char *Directory;
	char *Name;

	int WebDownloading();

public:
	WebDownloader();
	~WebDownloader();

	//非同用
	int httpOpen(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName);
	int httpsOpen(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName);
	int StartDownload();

	//同期用
	int httpDownload(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName);

	//取得系
	int getDownloadSize();
	int getReadSize();
	bool getIsOpen();
};

