#pragma once

#include <string>

//これがないとエラーになる
#include <tchar.h>

#include <dshow.h>
#include "qedit.h"

//directshowを使った動画再生クラス
//dxライブラリではダウンロードしながら再生ができなかったので自分で作った後悔はしてない

class Movie
{
public:
	Movie();
	~Movie();

	//動画再生の準備
	void initMovie(const char *MovieName, int &GraphHandle);

	//GraphHandleを更新する
	void updateMovie(int &GraphHandle);

	//再生中か
	bool Playing();

	LONGLONG getMovieStopTime();

	//再生をする
	void playMovie();

	//一時停止する
	void pauseMovie();

	//音量をセットする 最大0　最小-10000
	void setVolume(int volume);

	//動画の再生を終了する
	void stopMovie();

	bool initmovie = 0;
private:

	LPWSTR stringtowidechar(std::string temp);

	ICaptureGraphBuilder2 *pCapture;
	IGraphBuilder *pGraphBuilder;
	IBaseFilter *pSampleGrabberFilter = 0;
	ISampleGrabber *pSampleGrabber = 0;

	HRESULT hr;
	IMediaSeeking *pMediaSeeking = 0;
	IVideoWindow *m_lpVideoWindow = 0;
	AM_MEDIA_TYPE am_media_type;
	IMediaControl *pMediaControl;

	HWND g_hwnd;

	LONGLONG plength = 0;

	BITMAPINFO BitmapInfo;
	BITMAPINFOHEADER BmpHeader;
	void *BmpBuffer;
	int BmpBufferSize;

	LONGLONG length = 0;

	IMediaEventEx *m_lpMediaEventEx;
	BOOL bIsComplete; long lEventCode; LONG_PTR lEvParam1, lEvParam2;

	IBasicAudio *pBasicAudio;
};

