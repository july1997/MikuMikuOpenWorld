#include "Movie.h"

#include <math.h>
#include <string>

#include "DxLib.h"

#pragma comment( lib, "Strmiids.lib" )

Movie::Movie()
{
}


Movie::~Movie()
{
	// COM終了
	CoUninitialize();
}

void Movie::initMovie(const char *MovieName, int &GraphHandle)
{
	// COMを初期化
	CoInitialize(NULL);

	// FilterGraphを生成
	CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&pGraphBuilder);

	// SampleGrabber(Filter)を生成
	CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter,
		(LPVOID *)&pSampleGrabberFilter);

	// FilterからISampleGrabberインターフェースを取得します
	pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber,
		(LPVOID *)&pSampleGrabber);
	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&pCapture);

	// SampleGrabberを接続するフォーマットを指定。
	// ここがポイントです。
	// ここの指定の仕方によりSampleGrabberの挿入箇所を
	// 決定できます。このサンプルのような指定をすると
	// 画面出力の寸前でサンプルを取得できます。
	ZeroMemory(&am_media_type, sizeof(am_media_type));
	am_media_type.majortype = MEDIATYPE_Video;
	am_media_type.subtype = MEDIASUBTYPE_RGB24;
	am_media_type.formattype = FORMAT_VideoInfo;
	pSampleGrabber->SetMediaType(&am_media_type);

	// GraphにSampleGrabber Filterを追加
	pGraphBuilder->AddFilter(pSampleGrabberFilter,
		L"Sample Grabber");

	// MediaControlインターフェース取得
	pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&pMediaControl);

	pGraphBuilder->QueryInterface(IID_IMediaSeeking,
		(LPVOID *)&pMediaSeeking);

	pGraphBuilder->QueryInterface(IID_IVideoWindow,
		(LPVOID*)&m_lpVideoWindow);

	pGraphBuilder->QueryInterface(IID_IBasicAudio,
		(LPVOID *)&pBasicAudio);

	// Graphを生成。
	// ここでSampleGrabberを含んだGraphが自動的に作成されます。
	//----------------------------------------------------------
	//mp4の再生にはコーデックが必要
	//K-Lite_Codec_Pack_1095_Basicをインストール
	//
	hr = pGraphBuilder->RenderFile(stringtowidechar(MovieName), NULL);


	//ビデオのウィンドウをメインウィンドウに設定する
	g_hwnd = GetMainWindowHandle();
	m_lpVideoWindow->put_Owner((OAHWND)g_hwnd);

	m_lpVideoWindow->put_AutoShow(OAFALSE);
	//m_lpVideoWindow->SetWindowPosition(2000, 0, 1, 1);
	m_lpVideoWindow->put_WindowState(SW_HIDE);

	m_lpVideoWindow->put_MessageDrain((OAHWND)g_hwnd);
	m_lpVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);


	// 接続情報取得。
	// この処理はRenderFileによりGraphが構成された後に
	// 行う必要があります。
	hr = pSampleGrabber->GetConnectedMediaType(&am_media_type);
	VIDEOINFOHEADER *pVideoInfoHeader =
		(VIDEOINFOHEADER *)am_media_type.pbFormat;

	if (am_media_type.lSampleSize == 0)
	{
		MessageBox(GetMainWindowHandle(), "コーデックが対応していません。mp4を再生できるコーデックをインストールしてください\nK-Lite_Codec_Packがおすすめ", "error", MB_OK);//エラー表示

		initmovie = 0;
	}
	else
	{
		// 再生開始
		//hr = pMediaControl->Run();
		pMediaSeeking->GetDuration(&plength);
		pSampleGrabber->SetBufferSamples(TRUE);

		pVideoInfoHeader->bmiHeader.biWidth = 1024;
		pVideoInfoHeader->bmiHeader.biHeight = 512;
		pVideoInfoHeader->AvgTimePerFrame = (REFERENCE_TIME)floor((10000000.0 / 29 + 0.5));
		pSampleGrabber->GetConnectedMediaType(&am_media_type);
		// ビデオ ヘッダーへのポインタを獲得する。
		pVideoInfoHeader = (VIDEOINFOHEADER*)am_media_type.pbFormat;

		// ビデオ ヘッダーには、ビットマップ情報が含まれる。
		// ビットマップ情報を BITMAPINFO 構造体にコピーする。
		ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));
		CopyMemory(&BitmapInfo.bmiHeader, &(pVideoInfoHeader->bmiHeader), sizeof(BITMAPINFOHEADER));
		BmpHeader = BitmapInfo.bmiHeader;
		BmpBufferSize = BmpHeader.biSizeImage;
		BmpBuffer = malloc(BmpBufferSize);

		// 画像ハンドルを作成する
		GraphHandle = MakeGraph(abs(BmpHeader.biWidth), abs(BmpHeader.biHeight));

		initmovie = 1;
	}
}

void Movie::updateMovie(int &GraphHandle)
{
	pMediaSeeking->GetCurrentPosition(&length);//再生時間
	// イメージの取得
	pSampleGrabber->GetCurrentBuffer((long *)&BmpBufferSize, (long *)BmpBuffer);
	// イメージを画像ハンドルに転送
	ReCreateGraphFromBmp((BITMAPINFO *)&BmpHeader, BmpBuffer, GraphHandle);
}

bool Movie::Playing()
{
	LONGLONG pos, stop;
	pMediaSeeking->GetPositions(&pos, &stop);

	if (pos != stop)
	{
		return 1;
	}

	return 0;
}

LONGLONG Movie::getMovieStopTime()
{
	LONGLONG stop;
	pMediaSeeking->GetPositions(NULL, &stop);

	return stop;
}

void Movie::playMovie()
{
	pMediaControl->Run();
}

void Movie::pauseMovie()
{
	pMediaControl->Pause();
}

void Movie::setVolume(int volume)
{
	pBasicAudio->put_Volume(volume);
}

void Movie::stopMovie()
{
	// 資源を解放
	pMediaControl->Stop();

	// 資源を解放
	pMediaControl->Release();
	pSampleGrabber->SetBufferSamples(0);
	pGraphBuilder->Release();
	pCapture->Release();
	pSampleGrabberFilter->Release();
	free(BmpBuffer);
	m_lpVideoWindow->Release();
	pMediaSeeking->Release();

	// COM終了
	CoUninitialize();
}

LPWSTR Movie::stringtowidechar(std::string temp)
{
	int n;
	n = MultiByteToWideChar(CP_ACP, 0, temp.c_str(), temp.size(), NULL, 0);
	LPWSTR p = new WCHAR[n + 1];
	n = MultiByteToWideChar(CP_ACP, 0, temp.c_str(), temp.size(), p, n);
	*(p + n) = '\0';
	return p;
}