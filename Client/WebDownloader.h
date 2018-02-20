#pragma once
#include "DxLib.h"

#include <fstream>
#include <thread>

#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")

//https://the-united-front.blogspot.jp/2014/05/wininetinternetreadfilehtml.html �l�Q�l
//-106�̃G���[�̓Z�L�����e�B�\�t�g�ɒe����Ă�\������
//
//�Ԃ�l�ꗗ
//
//-106:ERR_INTERNET_DISCONNECTED        �C���^�[�l�b�g�ɐڑ�����Ă��Ȃ��B
//-403:"403 Forbidden"                                �T�[�o�[�Ƀ��N�G�X�g�����ۂ��ꂽ�B
//-404:"404 Not Found"                                �w�肵��URL�Ƀh�L�������g�����݂��Ȃ��B
//-1       

class WebDownloader
{
private:
	HINTERNET hInternet;
	HINTERNET hHttpSession;
	HINTERNET hHttpRequest;
	DWORD RequiredBufSize, RequiredBufSize_Size;
	DWORD ReadSize;        //���̑���œǂ݂����ꂽ�T�C�Y
	DWORD ReadSize_sum;        //�ǂݏo���T�C�Y�̍��v
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

	//�񓯗p
	int httpOpen(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName);
	int httpsOpen(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName);
	int StartDownload();

	//�����p
	int httpDownload(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName);

	//�擾�n
	int getDownloadSize();
	int getReadSize();
	bool getIsOpen();
};

