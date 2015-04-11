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
	// COM�I��
	CoUninitialize();
}

void Movie::initMovie(const char *MovieName, int &GraphHandle)
{
	// COM��������
	CoInitialize(NULL);

	// FilterGraph�𐶐�
	CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&pGraphBuilder);

	// SampleGrabber(Filter)�𐶐�
	CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter,
		(LPVOID *)&pSampleGrabberFilter);

	// Filter����ISampleGrabber�C���^�[�t�F�[�X���擾���܂�
	pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber,
		(LPVOID *)&pSampleGrabber);
	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&pCapture);

	// SampleGrabber��ڑ�����t�H�[�}�b�g���w��B
	// �������|�C���g�ł��B
	// �����̎w��̎d���ɂ��SampleGrabber�̑}���ӏ���
	// ����ł��܂��B���̃T���v���̂悤�Ȏw��������
	// ��ʏo�͂̐��O�ŃT���v�����擾�ł��܂��B
	ZeroMemory(&am_media_type, sizeof(am_media_type));
	am_media_type.majortype = MEDIATYPE_Video;
	am_media_type.subtype = MEDIASUBTYPE_RGB24;
	am_media_type.formattype = FORMAT_VideoInfo;
	pSampleGrabber->SetMediaType(&am_media_type);

	// Graph��SampleGrabber Filter��ǉ�
	pGraphBuilder->AddFilter(pSampleGrabberFilter,
		L"Sample Grabber");

	// MediaControl�C���^�[�t�F�[�X�擾
	pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&pMediaControl);

	pGraphBuilder->QueryInterface(IID_IMediaSeeking,
		(LPVOID *)&pMediaSeeking);

	pGraphBuilder->QueryInterface(IID_IVideoWindow,
		(LPVOID*)&m_lpVideoWindow);

	pGraphBuilder->QueryInterface(IID_IBasicAudio,
		(LPVOID *)&pBasicAudio);

	// Graph�𐶐��B
	// ������SampleGrabber���܂�Graph�������I�ɍ쐬����܂��B
	//----------------------------------------------------------
	//mp4�̍Đ��ɂ̓R�[�f�b�N���K�v
	//K-Lite_Codec_Pack_1095_Basic���C���X�g�[��
	//
	hr = pGraphBuilder->RenderFile(stringtowidechar(MovieName), NULL);


	//�r�f�I�̃E�B���h�E�����C���E�B���h�E�ɐݒ肷��
	g_hwnd = GetMainWindowHandle();
	m_lpVideoWindow->put_Owner((OAHWND)g_hwnd);

	m_lpVideoWindow->put_AutoShow(OAFALSE);
	//m_lpVideoWindow->SetWindowPosition(2000, 0, 1, 1);
	m_lpVideoWindow->put_WindowState(SW_HIDE);

	m_lpVideoWindow->put_MessageDrain((OAHWND)g_hwnd);
	m_lpVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);


	// �ڑ����擾�B
	// ���̏�����RenderFile�ɂ��Graph���\�����ꂽ���
	// �s���K�v������܂��B
	hr = pSampleGrabber->GetConnectedMediaType(&am_media_type);
	VIDEOINFOHEADER *pVideoInfoHeader =
		(VIDEOINFOHEADER *)am_media_type.pbFormat;

	if (am_media_type.lSampleSize == 0)
	{
		MessageBox(GetMainWindowHandle(), "�R�[�f�b�N���Ή����Ă��܂���Bmp4���Đ��ł���R�[�f�b�N���C���X�g�[�����Ă�������\nK-Lite_Codec_Pack����������", "error", MB_OK);//�G���[�\��

		initmovie = 0;
	}
	else
	{
		// �Đ��J�n
		//hr = pMediaControl->Run();
		pMediaSeeking->GetDuration(&plength);
		pSampleGrabber->SetBufferSamples(TRUE);

		pVideoInfoHeader->bmiHeader.biWidth = 1024;
		pVideoInfoHeader->bmiHeader.biHeight = 512;
		pVideoInfoHeader->AvgTimePerFrame = (REFERENCE_TIME)floor((10000000.0 / 29 + 0.5));
		pSampleGrabber->GetConnectedMediaType(&am_media_type);
		// �r�f�I �w�b�_�[�ւ̃|�C���^���l������B
		pVideoInfoHeader = (VIDEOINFOHEADER*)am_media_type.pbFormat;

		// �r�f�I �w�b�_�[�ɂ́A�r�b�g�}�b�v��񂪊܂܂��B
		// �r�b�g�}�b�v���� BITMAPINFO �\���̂ɃR�s�[����B
		ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));
		CopyMemory(&BitmapInfo.bmiHeader, &(pVideoInfoHeader->bmiHeader), sizeof(BITMAPINFOHEADER));
		BmpHeader = BitmapInfo.bmiHeader;
		BmpBufferSize = BmpHeader.biSizeImage;
		BmpBuffer = malloc(BmpBufferSize);

		// �摜�n���h�����쐬����
		GraphHandle = MakeGraph(abs(BmpHeader.biWidth), abs(BmpHeader.biHeight));

		initmovie = 1;
	}
}

void Movie::updateMovie(int &GraphHandle)
{
	pMediaSeeking->GetCurrentPosition(&length);//�Đ�����
	// �C���[�W�̎擾
	pSampleGrabber->GetCurrentBuffer((long *)&BmpBufferSize, (long *)BmpBuffer);
	// �C���[�W���摜�n���h���ɓ]��
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
	// ���������
	pMediaControl->Stop();

	// ���������
	pMediaControl->Release();
	pSampleGrabber->SetBufferSamples(0);
	pGraphBuilder->Release();
	pCapture->Release();
	pSampleGrabberFilter->Release();
	free(BmpBuffer);
	m_lpVideoWindow->Release();
	pMediaSeeking->Release();

	// COM�I��
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