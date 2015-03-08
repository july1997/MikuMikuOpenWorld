#pragma once

#include <string>

//���ꂪ�Ȃ��ƃG���[�ɂȂ�
#include <tchar.h>

#include <dshow.h>
#include "qedit.h"

//directshow���g��������Đ��N���X
//dx���C�u�����ł̓_�E�����[�h���Ȃ���Đ����ł��Ȃ������̂Ŏ����ō��������͂��ĂȂ�

class Movie
{
public:
	Movie();
	~Movie();

	//����Đ��̏���
	void initMovie(const char *MovieName, int &GraphHandle);

	//GraphHandle���X�V����
	void updateMovie(int &GraphHandle);

	//�Đ�����
	bool Playing();

	LONGLONG getMovieStopTime();

	//�Đ�������
	void playMovie();

	//�ꎞ��~����
	void pauseMovie();

	//���ʂ��Z�b�g���� �ő�0�@�ŏ�-10000
	void setVolume(int volume);

	//����̍Đ����I������
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

