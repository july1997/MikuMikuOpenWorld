#include "WebDownloader.h"



WebDownloader::WebDownloader()
{
}


WebDownloader::~WebDownloader()
{
}

int WebDownloader::httpOpen(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName)
{
	// WININET������
	hInternet = InternetOpen(
		"Web_Downloader",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);

	// �T�[�o�֐ڑ�
	hHttpSession = InternetConnect(
		hInternet,
		Server_Host_Address,
		INTERNET_DEFAULT_HTTP_PORT,//HTTP_PORT
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,//SERVICE_HTTP
		0,
		0);

	// HTTP�v���̍쐬
	hHttpRequest = HttpOpenRequest(
		hHttpSession,
		"GET",
		FileLocation,
		NULL,
		Referer,
		NULL,
		0,
		0);

	// �쐬����HTTP�v���̔��s
	BOOL bHttpSendRequest = HttpSendRequest(
		hHttpRequest,
		NULL,
		0,
		NULL,
		0);
	if (bHttpSendRequest == 0) {//bHttpSendRequest��0�̏ꍇ�̓C���^�[�l�b�g�ɐڑ����ꂢ�Ȃ��̂ŁA�G���[��Ԃ��B
		return -106;//106:ERR_INTERNET_DISCONNECTED
	}

	//404��403�G���[���������Ă��Ȃ����A���m�F����B
	//char BufSizeText[1000];
	//DWORD BufSizeTextSize = 1000;
	DWORD StatusCode, StatusCode_Size = sizeof(DWORD);
	BOOL bHttpQueryInfo_STATUS = HttpQueryInfo(                        //�G���[�R�[�h�𐔒l�Ƃ��ĕԂ��Ă����B
		hHttpRequest,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,        //�uHTTP_QUERY_FLAG_NUMBER�v���w�肵�Ȃ������ꍇ�A������Ƃ��ĕԂ����̂ŁAchar�^�̔z����o�b�t�@�ɗp�ӂ���΂����B
		&StatusCode,                //BufSizeText                                        //�uHTTP_QUERY_FLAG_NUMBER�v���w�肵�Ȃ��ꍇ(&�̗L���ɒ���)
		&StatusCode_Size,        //&BufSizeTextSize                                //�uHTTP_QUERY_FLAG_NUMBER�v���w�肵�Ȃ��ꍇ
		NULL);

	if (StatusCode != HTTP_STATUS_OK) {
		//�G���[����

		//���\�[�X�̉��
		InternetCloseHandle(hHttpRequest);//�uHttpQueryInfo()�v����сuInternetReadFile()�v�́uInternetCloseHandle()�v�̕K�v�͖����B
		InternetCloseHandle(hHttpSession);
		InternetCloseHandle(hInternet);

		//��񎎂������������A106�G���[�͌��o�ł��Ȃ������B
		//(�����炭�T�[�o�[���牞���̖����G���[�͌��o�ł��Ȃ��ƍl������B)
		//(WinInet.h������Ε����邪�A106�G���[�͒�`���炳��Ă��Ȃ��B���o�ł���󂪖��������B)
		if (StatusCode == 403) {
			return -403;                        //404�G���[
		}
		else if (StatusCode == 404) {
			return -404;                        //403�G���[
		}
		else {
			return -1;                                //���̑��̃G���[�ɂ��ẮA���񈵂�Ȃ��̂őS��-1�Ƃ���B�ureturn StatusCode;�v���邢�́u�ureturn (-1*StatusCode);�v�v�Ƃ��Ă��悩�����̂����A����͑��̊֐��Ƃ̓s���ł��̂悤�ɂ����B
		}
	}
	else {

		Directory = SaveDirectory;
		Name = SaveName;

		//����I��
		isOpen = 1;
		return 0;
	}
}

int WebDownloader::httpsOpen(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName)
{
	// WININET������
	hInternet = InternetOpen(
		"Web_Downloader",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);

	// �T�[�o�֐ڑ�
	hHttpSession = InternetConnect(
		hInternet,
		Server_Host_Address,
		INTERNET_DEFAULT_HTTPS_PORT,//HTTPS_PORT
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,//SERVICE_HTTP
		0,
		0);

	// HTTP�v���̍쐬
	hHttpRequest = HttpOpenRequest(
		hHttpSession,
		"GET",
		FileLocation,
		HTTP_VERSION,
		Referer,
		NULL,
		INTERNET_FLAG_SECURE,
		0);

	// �쐬����HTTP�v���̔��s
	BOOL bHttpSendRequest = HttpSendRequest(
		hHttpRequest,
		NULL,
		0,
		NULL,
		0);
	if (bHttpSendRequest == 0) {//bHttpSendRequest��0�̏ꍇ�̓C���^�[�l�b�g�ɐڑ����ꂢ�Ȃ��̂ŁA�G���[��Ԃ��B
		return -106;//106:ERR_INTERNET_DISCONNECTED
	}

	//404��403�G���[���������Ă��Ȃ����A���m�F����B
	//char BufSizeText[1000];
	//DWORD BufSizeTextSize = 1000;
	DWORD StatusCode, StatusCode_Size = sizeof(DWORD);
	BOOL bHttpQueryInfo_STATUS = HttpQueryInfo(                        //�G���[�R�[�h�𐔒l�Ƃ��ĕԂ��Ă����B
		hHttpRequest,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,        //�uHTTP_QUERY_FLAG_NUMBER�v���w�肵�Ȃ������ꍇ�A������Ƃ��ĕԂ����̂ŁAchar�^�̔z����o�b�t�@�ɗp�ӂ���΂����B
		&StatusCode,                //BufSizeText                                        //�uHTTP_QUERY_FLAG_NUMBER�v���w�肵�Ȃ��ꍇ(&�̗L���ɒ���)
		&StatusCode_Size,        //&BufSizeTextSize                                //�uHTTP_QUERY_FLAG_NUMBER�v���w�肵�Ȃ��ꍇ
		NULL);

	if (StatusCode != HTTP_STATUS_OK) {
		//�G���[����

		//���\�[�X�̉��
		InternetCloseHandle(hHttpRequest);//�uHttpQueryInfo()�v����сuInternetReadFile()�v�́uInternetCloseHandle()�v�̕K�v�͖����B
		InternetCloseHandle(hHttpSession);
		InternetCloseHandle(hInternet);

		//��񎎂������������A106�G���[�͌��o�ł��Ȃ������B
		//(�����炭�T�[�o�[���牞���̖����G���[�͌��o�ł��Ȃ��ƍl������B)
		//(WinInet.h������Ε����邪�A106�G���[�͒�`���炳��Ă��Ȃ��B���o�ł���󂪖��������B)
		if (StatusCode == 403) {
			return -403;                        //404�G���[
		}
		else if (StatusCode == 404) {
			return -404;                        //403�G���[
		}
		else {
			return -1;                                //���̑��̃G���[�ɂ��ẮA���񈵂�Ȃ��̂őS��-1�Ƃ���B�ureturn StatusCode;�v���邢�́u�ureturn (-1*StatusCode);�v�v�Ƃ��Ă��悩�����̂����A����͑��̊֐��Ƃ̓s���ł��̂悤�ɂ����B
		}
	}
	else {

		Directory = SaveDirectory;
		Name = SaveName;

		//����I��
		isOpen = 1;
		return 0;
	}
}


int WebDownloader::StartDownload()
{
	std::thread th(&WebDownloader::WebDownloading, this);

	// thread start
	th.detach();

	return 0;
}

int WebDownloader::WebDownloading()
{
	if (isOpen)
	{
		//�_�E�����[�h�����B

		//�t�@�C���̃_�E�����[�h�ɕK�v�Ƃ����o�b�t�@�̃T�C�Y���m�F����B
		RequiredBufSize_Size = sizeof(DWORD);

		if (HttpQueryInfo(                        //�G���[�R�[�h�𐔒l�Ƃ��ĕԂ��Ă����B
			hHttpRequest,
			HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
			(LPVOID)&RequiredBufSize,
			&RequiredBufSize_Size,
			NULL))
		{
			//�K�v�ȃT�C�Y�������������m�ۂ���B
			Buf_main = (char *)malloc(RequiredBufSize*sizeof(char));
			dwReadSize = RequiredBufSize / 100;   // 1 ��̓ǂݏo���Ńf�[�^�� 1% ��ǂݏo���܂�

			char Buf_SaveName[200];
			sprintf_s(Buf_SaveName, sizeof(Buf_SaveName), "%s\\%s", Directory, Name);//�ۑ�����t�@�C�����̐���
			fopen_s(&file, Buf_SaveName, "wb");

			ReadSize_sum = 0;        //�ǂݏo���T�C�Y�̍��v

			DWORD cReadCount;
			char *pCopyPtr = Buf_main;
			for (cReadCount = 0; cReadCount < 100; cReadCount++)
			{
				InternetReadFile(hHttpRequest, pCopyPtr, dwReadSize, &ReadSize);
				pCopyPtr = pCopyPtr + ReadSize;
				ReadSize_sum += ReadSize;
			}

			// �������Z�̊ۂ߂̂��ߒǉ��̓ǂݏo�����s���܂��B
			InternetReadFile(hHttpRequest,
				pCopyPtr,
				RequiredBufSize - (pCopyPtr - Buf_main),
				&ReadSize);

			// �f�[�^�̏I��� Null �ɂȂ�܂��B
			Buf_main[RequiredBufSize-1] = 0;

			ReadSize_sum += ReadSize;

			//�t�@�C���ւ̏����o���B
			fwrite(Buf_main, ReadSize_sum, 1, file);//fwrite(&Buf_main,ReadSize,1,file);�Ƃ��Ă�����ē����Ă��܂����A���ʂɍĐ����ł��邪�AChrome�Ń_�E�����[�h�����t�@�C���ƃo�C�i���G�f�B�^�Ŕ�r����ƁA�t�@�C���̐擪�Ɩ����̕ӂ�ɃS�~��������̂Œ��ӁB


			fclose(file);
			free(Buf_main);

			isOpen = 0;

			InternetCloseHandle(hHttpRequest);//�uHttpQueryInfo()�v����сuInternetReadFile()�v�ɂ��ẮuInternetCloseHandle()�v����K�v�͖����B
			InternetCloseHandle(hHttpSession);
			InternetCloseHandle(hInternet);

			if (ReadSize_sum != RequiredBufSize) {
				//�uHttpQueryInfo()�v�Ŏ擾�����t�@�C���T�C�Y�ƁA���ۂɓǂ݂������t�@�C���T�C�Y���Ⴄ�ꍇ�̓G���[��Ԃ��B
				//�����A�T�[�o�[����̉������x��???�Ȃǂ̗��R�ŏ�肭�s���Ȃ��ꍇ�́A��L�́udo{}while();�v���ɁA
				//(�K�v�ł���΁uSleep();�v�����݂Ȃ���A)�T�C�Y����v����܂Ń��[�v���񂷂̂����肾�Ǝv���B
				//(���R�����A���s�񐔂ɐ��������Ă����Ȃ��ƁA�������[�v�Ɋׂ�\��������̂Œ��ӁB)
				//�����A�����̊��ł́A���̂悤�Ȏ���͂܂������̂ŁA�����͂��Ȃ��B
				return -1;
			}
		}
		else
		{
			DWORD err = GetLastError();
			// �R���e���g�����Ȃ��̂Ŋ����������� (%) ���v�Z�ł��܂���B
			// ��������܂œǂݏo���݂̂��s���܂��B
			char pData[100];
			DWORD dwBytesRead = 1;
			while (dwBytesRead)
			{
				InternetReadFile(hHttpRequest, pData, 99, &dwBytesRead);
				pData[dwBytesRead] = 0;
			}
			fwrite(pData, RequiredBufSize, 1, file);
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int WebDownloader::httpDownload(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName)
{
	// WININET������
	hInternet = InternetOpen(
		"Web_Downloader",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);

	// �T�[�o�֐ڑ�
	hHttpSession = InternetConnect(
		hInternet,
		Server_Host_Address,
		INTERNET_DEFAULT_HTTP_PORT,//HTTP_PORT
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,//SERVICE_HTTP
		0,
		0);

	// HTTP�v���̍쐬
	hHttpRequest = HttpOpenRequest(
		hHttpSession,
		"GET",
		FileLocation,
		NULL,
		Referer,
		NULL,
		0,
		0);

	// �쐬����HTTP�v���̔��s
	BOOL bHttpSendRequest = HttpSendRequest(
		hHttpRequest,
		NULL,
		0,
		NULL,
		0);
	if (bHttpSendRequest == 0) {//bHttpSendRequest��0�̏ꍇ�̓C���^�[�l�b�g�ɐڑ����ꂢ�Ȃ��̂ŁA�G���[��Ԃ��B
		return -106;//106:ERR_INTERNET_DISCONNECTED
	}

	//404��403�G���[���������Ă��Ȃ����A���m�F����B
	//char BufSizeText[1000];
	//DWORD BufSizeTextSize = 1000;
	DWORD StatusCode, StatusCode_Size = sizeof(DWORD);
	BOOL bHttpQueryInfo_STATUS = HttpQueryInfo(                        //�G���[�R�[�h�𐔒l�Ƃ��ĕԂ��Ă����B
		hHttpRequest,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,        //�uHTTP_QUERY_FLAG_NUMBER�v���w�肵�Ȃ������ꍇ�A������Ƃ��ĕԂ����̂ŁAchar�^�̔z����o�b�t�@�ɗp�ӂ���΂����B
		&StatusCode,                //BufSizeText                                        //�uHTTP_QUERY_FLAG_NUMBER�v���w�肵�Ȃ��ꍇ(&�̗L���ɒ���)
		&StatusCode_Size,        //&BufSizeTextSize                                //�uHTTP_QUERY_FLAG_NUMBER�v���w�肵�Ȃ��ꍇ
		NULL);

	if (StatusCode != HTTP_STATUS_OK) {
		//�G���[����

		//���\�[�X�̉��
		InternetCloseHandle(hHttpRequest);//�uHttpQueryInfo()�v����сuInternetReadFile()�v�́uInternetCloseHandle()�v�̕K�v�͖����B
		InternetCloseHandle(hHttpSession);
		InternetCloseHandle(hInternet);

		//��񎎂������������A106�G���[�͌��o�ł��Ȃ������B
		//(�����炭�T�[�o�[���牞���̖����G���[�͌��o�ł��Ȃ��ƍl������B)
		//(WinInet.h������Ε����邪�A106�G���[�͒�`���炳��Ă��Ȃ��B���o�ł���󂪖��������B)
		if (StatusCode == 403) {
			return -403;                        //404�G���[
		}
		else if (StatusCode == 404) {
			return -404;                        //403�G���[
		}
		else {
			return -1;                                //���̑��̃G���[�ɂ��ẮA���񈵂�Ȃ��̂őS��-1�Ƃ���B�ureturn StatusCode;�v���邢�́u�ureturn (-1*StatusCode);�v�v�Ƃ��Ă��悩�����̂����A����͑��̊֐��Ƃ̓s���ł��̂悤�ɂ����B
		}
	}
	else {
		//�_�E�����[�h�����B

		//�t�@�C���̃_�E�����[�h�ɕK�v�Ƃ����o�b�t�@�̃T�C�Y���m�F����B
		RequiredBufSize, RequiredBufSize_Size = sizeof(DWORD);
		BOOL bHttpQueryInfo_ContentLength = HttpQueryInfo(                        //�G���[�R�[�h�𐔒l�Ƃ��ĕԂ��Ă����B
			hHttpRequest,
			HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
			&RequiredBufSize,
			&RequiredBufSize_Size,
			NULL);

		//�K�v�ȃT�C�Y�������������m�ۂ���B
		char *Buf_main = (char *)malloc(RequiredBufSize*sizeof(char));

		char Buf_SaveName[200];
		sprintf_s(Buf_SaveName, sizeof(Buf_SaveName), "%s\\%s", SaveDirectory, SaveName);//�ۑ�����t�@�C�����̐���
		FILE *file;
		fopen_s(&file, Buf_SaveName, "wb");

		ReadSize_sum = 0;        //�ǂݏo���T�C�Y�̍��v

									   // �R���e���c�̓��e���擾
		do {
			BOOL bInternetReadFile = InternetReadFile(
				hHttpRequest,
				Buf_main,
				RequiredBufSize,//        BufSize,
				&ReadSize);

			//�u���[�N�|�C���g�|���āA�����I�Ƀl�b�g�P�[�u���𔲂��������āAHttpSendRequest�̓G���[�����Œʂ��AInternetReadFile�ŃG���[���o�����Ƃ������ǁA�o�Ȃ�����A
			//�������̃G���[�����͕s�v�B(HttpSendRequest������ok�ƌ�����)
			//���ۂɃt�@�C���̕ۑ��܂ł���Ă݂����ǁAmp3���������ۑ�����Ă���B
			//���炭�����AHttpSendRequest�������������_�Ń_�E�����[�h�͑S�Ċ������Ă���B
			//�������A
			//�o�b�t�@���̃��[�v�Ń_�E�����[�h�ł��Ȃ�������A�T�[�o�[�����������ăt�@�C���𑗐M���Ă���ꍇ�ɂ́A�ǂ��Ȃ邩������Ȃ��̂ŁA�ꉞ�m�F�������Ă����B
			if (bInternetReadFile == 0) {//bInternetReadFile��0�̏ꍇ�̓C���^�[�l�b�g�ɐڑ����ꂢ�Ȃ��̂ŁA�G���[��Ԃ��B
				free(Buf_main);
				return -106;//106:ERR_INTERNET_DISCONNECTED
			}

			ReadSize_sum += ReadSize;

			//�t�@�C���ւ̏����o���B
			fwrite(Buf_main, ReadSize, 1, file);//fwrite(&Buf_main,ReadSize,1,file);�Ƃ��Ă�����ē����Ă��܂����A���ʂɍĐ����ł��邪�AChrome�Ń_�E�����[�h�����t�@�C���ƃo�C�i���G�f�B�^�Ŕ�r����ƁA�t�@�C���̐擪�Ɩ����̕ӂ�ɃS�~��������̂Œ��ӁB

		} while (ReadSize != 0);//���œǂ݂�����Ƃ͌���Ȃ��̂ŁA���[�v���񂷁B

		fclose(file);
		free(Buf_main);

		if (ReadSize_sum != RequiredBufSize) {
			//�uHttpQueryInfo()�v�Ŏ擾�����t�@�C���T�C�Y�ƁA���ۂɓǂ݂������t�@�C���T�C�Y���Ⴄ�ꍇ�̓G���[��Ԃ��B
			//�����A�T�[�o�[����̉������x��???�Ȃǂ̗��R�ŏ�肭�s���Ȃ��ꍇ�́A��L�́udo{}while();�v���ɁA
			//(�K�v�ł���΁uSleep();�v�����݂Ȃ���A)�T�C�Y����v����܂Ń��[�v���񂷂̂����肾�Ǝv���B
			//(���R�����A���s�񐔂ɐ��������Ă����Ȃ��ƁA�������[�v�Ɋׂ�\��������̂Œ��ӁB)
			//�����A�����̊��ł́A���̂悤�Ȏ���͂܂������̂ŁA�����͂��Ȃ��B
			return -1;
		}
	}

	InternetCloseHandle(hHttpRequest);//�uHttpQueryInfo()�v����сuInternetReadFile()�v�ɂ��ẮuInternetCloseHandle()�v����K�v�͖����B
	InternetCloseHandle(hHttpSession);
	InternetCloseHandle(hInternet);

	return 0;
}

int WebDownloader::getDownloadSize()
{
	return (int)RequiredBufSize;
}

int WebDownloader::getReadSize()
{
	return ReadSize_sum;
}

bool WebDownloader::getIsOpen()
{
	return isOpen;
}