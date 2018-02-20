#include "WebDownloader.h"



WebDownloader::WebDownloader()
{
}


WebDownloader::~WebDownloader()
{
}

int WebDownloader::httpOpen(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName)
{
	// WININET初期化
	hInternet = InternetOpen(
		"Web_Downloader",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);

	// サーバへ接続
	hHttpSession = InternetConnect(
		hInternet,
		Server_Host_Address,
		INTERNET_DEFAULT_HTTP_PORT,//HTTP_PORT
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,//SERVICE_HTTP
		0,
		0);

	// HTTP要求の作成
	hHttpRequest = HttpOpenRequest(
		hHttpSession,
		"GET",
		FileLocation,
		NULL,
		Referer,
		NULL,
		0,
		0);

	// 作成したHTTP要求の発行
	BOOL bHttpSendRequest = HttpSendRequest(
		hHttpRequest,
		NULL,
		0,
		NULL,
		0);
	if (bHttpSendRequest == 0) {//bHttpSendRequestが0の場合はインターネットに接続されいないので、エラーを返す。
		return -106;//106:ERR_INTERNET_DISCONNECTED
	}

	//404や403エラーが発生していないか、を確認する。
	//char BufSizeText[1000];
	//DWORD BufSizeTextSize = 1000;
	DWORD StatusCode, StatusCode_Size = sizeof(DWORD);
	BOOL bHttpQueryInfo_STATUS = HttpQueryInfo(                        //エラーコードを数値として返してくれる。
		hHttpRequest,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,        //「HTTP_QUERY_FLAG_NUMBER」を指定しなかった場合、文字列として返されるので、char型の配列をバッファに用意すればいい。
		&StatusCode,                //BufSizeText                                        //「HTTP_QUERY_FLAG_NUMBER」を指定しない場合(&の有無に注意)
		&StatusCode_Size,        //&BufSizeTextSize                                //「HTTP_QUERY_FLAG_NUMBER」を指定しない場合
		NULL);

	if (StatusCode != HTTP_STATUS_OK) {
		//エラー処理

		//リソースの解放
		InternetCloseHandle(hHttpRequest);//「HttpQueryInfo()」および「InternetReadFile()」は「InternetCloseHandle()」の必要は無い。
		InternetCloseHandle(hHttpSession);
		InternetCloseHandle(hInternet);

		//一回試しただけだが、106エラーは検出できなかった。
		//(おそらくサーバーから応答の無いエラーは検出できないと考えられる。)
		//(WinInet.hを見れば分かるが、106エラーは定義すらされていない。検出できる訳が無かった。)
		if (StatusCode == 403) {
			return -403;                        //404エラー
		}
		else if (StatusCode == 404) {
			return -404;                        //403エラー
		}
		else {
			return -1;                                //その他のエラーについては、今回扱わないので全て-1とする。「return StatusCode;」あるいは「「return (-1*StatusCode);」」としてもよかったのだが、今回は他の関数との都合でこのようにした。
		}
	}
	else {

		Directory = SaveDirectory;
		Name = SaveName;

		//正常終了
		isOpen = 1;
		return 0;
	}
}

int WebDownloader::httpsOpen(char *Server_Host_Address, char *FileLocation, char *Referer, char *SaveDirectory, char *SaveName)
{
	// WININET初期化
	hInternet = InternetOpen(
		"Web_Downloader",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);

	// サーバへ接続
	hHttpSession = InternetConnect(
		hInternet,
		Server_Host_Address,
		INTERNET_DEFAULT_HTTPS_PORT,//HTTPS_PORT
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,//SERVICE_HTTP
		0,
		0);

	// HTTP要求の作成
	hHttpRequest = HttpOpenRequest(
		hHttpSession,
		"GET",
		FileLocation,
		HTTP_VERSION,
		Referer,
		NULL,
		INTERNET_FLAG_SECURE,
		0);

	// 作成したHTTP要求の発行
	BOOL bHttpSendRequest = HttpSendRequest(
		hHttpRequest,
		NULL,
		0,
		NULL,
		0);
	if (bHttpSendRequest == 0) {//bHttpSendRequestが0の場合はインターネットに接続されいないので、エラーを返す。
		return -106;//106:ERR_INTERNET_DISCONNECTED
	}

	//404や403エラーが発生していないか、を確認する。
	//char BufSizeText[1000];
	//DWORD BufSizeTextSize = 1000;
	DWORD StatusCode, StatusCode_Size = sizeof(DWORD);
	BOOL bHttpQueryInfo_STATUS = HttpQueryInfo(                        //エラーコードを数値として返してくれる。
		hHttpRequest,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,        //「HTTP_QUERY_FLAG_NUMBER」を指定しなかった場合、文字列として返されるので、char型の配列をバッファに用意すればいい。
		&StatusCode,                //BufSizeText                                        //「HTTP_QUERY_FLAG_NUMBER」を指定しない場合(&の有無に注意)
		&StatusCode_Size,        //&BufSizeTextSize                                //「HTTP_QUERY_FLAG_NUMBER」を指定しない場合
		NULL);

	if (StatusCode != HTTP_STATUS_OK) {
		//エラー処理

		//リソースの解放
		InternetCloseHandle(hHttpRequest);//「HttpQueryInfo()」および「InternetReadFile()」は「InternetCloseHandle()」の必要は無い。
		InternetCloseHandle(hHttpSession);
		InternetCloseHandle(hInternet);

		//一回試しただけだが、106エラーは検出できなかった。
		//(おそらくサーバーから応答の無いエラーは検出できないと考えられる。)
		//(WinInet.hを見れば分かるが、106エラーは定義すらされていない。検出できる訳が無かった。)
		if (StatusCode == 403) {
			return -403;                        //404エラー
		}
		else if (StatusCode == 404) {
			return -404;                        //403エラー
		}
		else {
			return -1;                                //その他のエラーについては、今回扱わないので全て-1とする。「return StatusCode;」あるいは「「return (-1*StatusCode);」」としてもよかったのだが、今回は他の関数との都合でこのようにした。
		}
	}
	else {

		Directory = SaveDirectory;
		Name = SaveName;

		//正常終了
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
		//ダウンロード処理。

		//ファイルのダウンロードに必要とされるバッファのサイズを確認する。
		RequiredBufSize_Size = sizeof(DWORD);

		if (HttpQueryInfo(                        //エラーコードを数値として返してくれる。
			hHttpRequest,
			HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
			(LPVOID)&RequiredBufSize,
			&RequiredBufSize_Size,
			NULL))
		{
			//必要なサイズだけメモリを確保する。
			Buf_main = (char *)malloc(RequiredBufSize*sizeof(char));
			dwReadSize = RequiredBufSize / 100;   // 1 回の読み出しでデータの 1% を読み出します

			char Buf_SaveName[200];
			sprintf_s(Buf_SaveName, sizeof(Buf_SaveName), "%s\\%s", Directory, Name);//保存するファイル名の生成
			fopen_s(&file, Buf_SaveName, "wb");

			ReadSize_sum = 0;        //読み出しサイズの合計

			DWORD cReadCount;
			char *pCopyPtr = Buf_main;
			for (cReadCount = 0; cReadCount < 100; cReadCount++)
			{
				InternetReadFile(hHttpRequest, pCopyPtr, dwReadSize, &ReadSize);
				pCopyPtr = pCopyPtr + ReadSize;
				ReadSize_sum += ReadSize;
			}

			// 整数除算の丸めのため追加の読み出しを行います。
			InternetReadFile(hHttpRequest,
				pCopyPtr,
				RequiredBufSize - (pCopyPtr - Buf_main),
				&ReadSize);

			// データの終りは Null になります。
			Buf_main[RequiredBufSize-1] = 0;

			ReadSize_sum += ReadSize;

			//ファイルへの書き出し。
			fwrite(Buf_main, ReadSize_sum, 1, file);//fwrite(&Buf_main,ReadSize,1,file);としても誤って動いてしまうし、普通に再生もできるが、Chromeでダウンロードしたファイルとバイナリエディタで比較すると、ファイルの先頭と末尾の辺りにゴミが混じるので注意。


			fclose(file);
			free(Buf_main);

			isOpen = 0;

			InternetCloseHandle(hHttpRequest);//「HttpQueryInfo()」および「InternetReadFile()」については「InternetCloseHandle()」する必要は無い。
			InternetCloseHandle(hHttpSession);
			InternetCloseHandle(hInternet);

			if (ReadSize_sum != RequiredBufSize) {
				//「HttpQueryInfo()」で取得したファイルサイズと、実際に読みだしたファイルサイズが違う場合はエラーを返す。
				//もし、サーバーからの応答が遅い???などの理由で上手く行かない場合は、上記の「do{}while();」文に、
				//(必要であれば「Sleep();」を挟みながら、)サイズが一致するまでループを回すのもありだと思う。
				//(当然だが、試行回数に制限を入れておかないと、無限ループに陥る可能性があるので注意。)
				//ただ、自分の環境では、そのような事例はまだ無いので、実装はしない。
				return -1;
			}
		}
		else
		{
			DWORD err = GetLastError();
			// コンテント長がないので完了した割合 (%) を計算できません。
			// 完了するまで読み出しのみを行います。
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
	// WININET初期化
	hInternet = InternetOpen(
		"Web_Downloader",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);

	// サーバへ接続
	hHttpSession = InternetConnect(
		hInternet,
		Server_Host_Address,
		INTERNET_DEFAULT_HTTP_PORT,//HTTP_PORT
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,//SERVICE_HTTP
		0,
		0);

	// HTTP要求の作成
	hHttpRequest = HttpOpenRequest(
		hHttpSession,
		"GET",
		FileLocation,
		NULL,
		Referer,
		NULL,
		0,
		0);

	// 作成したHTTP要求の発行
	BOOL bHttpSendRequest = HttpSendRequest(
		hHttpRequest,
		NULL,
		0,
		NULL,
		0);
	if (bHttpSendRequest == 0) {//bHttpSendRequestが0の場合はインターネットに接続されいないので、エラーを返す。
		return -106;//106:ERR_INTERNET_DISCONNECTED
	}

	//404や403エラーが発生していないか、を確認する。
	//char BufSizeText[1000];
	//DWORD BufSizeTextSize = 1000;
	DWORD StatusCode, StatusCode_Size = sizeof(DWORD);
	BOOL bHttpQueryInfo_STATUS = HttpQueryInfo(                        //エラーコードを数値として返してくれる。
		hHttpRequest,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,        //「HTTP_QUERY_FLAG_NUMBER」を指定しなかった場合、文字列として返されるので、char型の配列をバッファに用意すればいい。
		&StatusCode,                //BufSizeText                                        //「HTTP_QUERY_FLAG_NUMBER」を指定しない場合(&の有無に注意)
		&StatusCode_Size,        //&BufSizeTextSize                                //「HTTP_QUERY_FLAG_NUMBER」を指定しない場合
		NULL);

	if (StatusCode != HTTP_STATUS_OK) {
		//エラー処理

		//リソースの解放
		InternetCloseHandle(hHttpRequest);//「HttpQueryInfo()」および「InternetReadFile()」は「InternetCloseHandle()」の必要は無い。
		InternetCloseHandle(hHttpSession);
		InternetCloseHandle(hInternet);

		//一回試しただけだが、106エラーは検出できなかった。
		//(おそらくサーバーから応答の無いエラーは検出できないと考えられる。)
		//(WinInet.hを見れば分かるが、106エラーは定義すらされていない。検出できる訳が無かった。)
		if (StatusCode == 403) {
			return -403;                        //404エラー
		}
		else if (StatusCode == 404) {
			return -404;                        //403エラー
		}
		else {
			return -1;                                //その他のエラーについては、今回扱わないので全て-1とする。「return StatusCode;」あるいは「「return (-1*StatusCode);」」としてもよかったのだが、今回は他の関数との都合でこのようにした。
		}
	}
	else {
		//ダウンロード処理。

		//ファイルのダウンロードに必要とされるバッファのサイズを確認する。
		RequiredBufSize, RequiredBufSize_Size = sizeof(DWORD);
		BOOL bHttpQueryInfo_ContentLength = HttpQueryInfo(                        //エラーコードを数値として返してくれる。
			hHttpRequest,
			HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
			&RequiredBufSize,
			&RequiredBufSize_Size,
			NULL);

		//必要なサイズだけメモリを確保する。
		char *Buf_main = (char *)malloc(RequiredBufSize*sizeof(char));

		char Buf_SaveName[200];
		sprintf_s(Buf_SaveName, sizeof(Buf_SaveName), "%s\\%s", SaveDirectory, SaveName);//保存するファイル名の生成
		FILE *file;
		fopen_s(&file, Buf_SaveName, "wb");

		ReadSize_sum = 0;        //読み出しサイズの合計

									   // コンテンツの内容を取得
		do {
			BOOL bInternetReadFile = InternetReadFile(
				hHttpRequest,
				Buf_main,
				RequiredBufSize,//        BufSize,
				&ReadSize);

			//ブレークポイント掛けて、物理的にネットケーブルを抜き差しして、HttpSendRequestはエラー無しで通し、InternetReadFileでエラーを出そうとしたけど、出ないから、
			//こっちのエラー処理は不要。(HttpSendRequestだけでokと言う事)
			//実際にファイルの保存までやってみたけど、mp3が正しく保存されている。
			//恐らくだが、HttpSendRequestが成功した時点でダウンロードは全て完了している。
			//ただし、
			//バッファ一回のループでダウンロードできなかったり、サーバー側が分割してファイルを送信してくる場合には、どうなるか分からないので、一応確認だけしておく。
			if (bInternetReadFile == 0) {//bInternetReadFileが0の場合はインターネットに接続されいないので、エラーを返す。
				free(Buf_main);
				return -106;//106:ERR_INTERNET_DISCONNECTED
			}

			ReadSize_sum += ReadSize;

			//ファイルへの書き出し。
			fwrite(Buf_main, ReadSize, 1, file);//fwrite(&Buf_main,ReadSize,1,file);としても誤って動いてしまうし、普通に再生もできるが、Chromeでダウンロードしたファイルとバイナリエディタで比較すると、ファイルの先頭と末尾の辺りにゴミが混じるので注意。

		} while (ReadSize != 0);//一回で読みだせるとは限らないので、ループを回す。

		fclose(file);
		free(Buf_main);

		if (ReadSize_sum != RequiredBufSize) {
			//「HttpQueryInfo()」で取得したファイルサイズと、実際に読みだしたファイルサイズが違う場合はエラーを返す。
			//もし、サーバーからの応答が遅い???などの理由で上手く行かない場合は、上記の「do{}while();」文に、
			//(必要であれば「Sleep();」を挟みながら、)サイズが一致するまでループを回すのもありだと思う。
			//(当然だが、試行回数に制限を入れておかないと、無限ループに陥る可能性があるので注意。)
			//ただ、自分の環境では、そのような事例はまだ無いので、実装はしない。
			return -1;
		}
	}

	InternetCloseHandle(hHttpRequest);//「HttpQueryInfo()」および「InternetReadFile()」については「InternetCloseHandle()」する必要は無い。
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