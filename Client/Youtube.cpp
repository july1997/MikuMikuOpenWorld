#include "Youtube.h"
#include "Movie.h"

#include <sys/stat.h>
#include <shellapi.h>

#include <process.h>

#include <string>

#include "Fps.h"

#include "DxLib.h"

HANDLE m_ChildProcess;

Youtube::Youtube()
{
	screen_handel = MakeScreen(1024, 512, FALSE);
}


Youtube::~Youtube()
{
	CloseHandle(hHandle);//�X���b�h�����
}

int Youtube::playYotubeMovie(std::string &VideoURL)
{
	switch (step)
	{
	case 0:
		deleteProcessflag = 0, deleteCacheflag = 0, file = 0, downloadmovieflag = 0, moviefile = 0;
		timer = 0;
		step = 1;

		if (movieflag==1)stopMovie();
		movieflag = 0;

		loadinig = LoadGraph("Picture/load.png");
		looptime = 0, looptime2 = 0;

		break;

	case 1:
		if (!deleteProcess())
		{
			step = 2;
		}
		break;

	case 2:
		if (!deleteCache())
		{
			step = 3;
		}
		break;

	case 3:
		if (file)
		{

			if (!downloadmovie(VideoURL))
			{
				step = 4;

				timer = 0;
			}
		}
		else
		{
			GraHandle = LoadGraph("cache/b.png");

			step = 0;

			movieflag= - 1;
		}
		break;

	case 4:
		if (moviefile)
		{
			timer++;
			if (timer == 30 * 10)
			{
				initMovie("cache/cache.mp4", GraHandle);

				movieflag = 1;

				step = 0;
			}
		}
		else
		{
			GraHandle = LoadGraph("cache/b.png");

			step = 0;

			movieflag= - 1;
		}
		break;
	}

	SetDrawScreen(screen_handel);

	ClearDrawScreen();

	looptime2++;
	if (looptime2 == 6)looptime++, looptime2 = 0;
	if (looptime == 12)looptime = 0;

	DrawRotaGraph2(512, 256, 40, 40, 1.0, DX_PI / 180 * 30 * looptime, loadinig, FALSE);

	SetDrawScreen(DX_SCREEN_BACK);

	if (movieflag == -1)
	{
		for (size_t i = 0; i < model_handel.size(); i++)
		{
			MV1SetTextureGraphHandle(model_handel[i], screen_tex[i], GraHandle, FALSE);
		}
	}
	else
	{
		for (size_t i = 0; i < model_handel.size(); i++)
		{
			MV1SetTextureGraphHandle(model_handel[i], screen_tex[i], screen_handel, FALSE);
		}
	}

	return movieflag;
}

bool Youtube::deleteProcess()
{
	//�O��Đ����Ă���
	if (movieflag && !deleteProcessflag)
	{
		//�Đ����I������
		stopMovie();

		//�v���Z�X�������I��
		::TerminateProcess(m_ChildProcess, 0);

		CloseHandle(hHandle);

		deleteProcessflag = 1;

		timer = 0;
	}

	if (deleteProcessflag)
	{
		timer++;

		//������Ƒ҂�
		if (timer == 15)deleteProcessflag = 0;
	}

	return deleteProcessflag;
}

bool Youtube::deleteCache()
{
	if (!deleteCacheflag)
	{
		//�O�̃L���b�V���������������
		DeleteFile("cache/cache.mp4");

		deleteCacheflag = 1;

		timer = 0;
	}

	//������܂ő҂�
	if (stat("cache/cache.mp4", &buf) != 0)deleteCacheflag = 0, file = 1;

	timer++;
	if (timer == 30 * 10)deleteCacheflag = 0;

	return deleteCacheflag;
}

bool Youtube::downloadmovie(std::string &VideoURL)
{
	if (!downloadmovieflag)
	{
		std::string exe;
		std::string path;

		if (VideoURL.find("https://www.youtube.com/watch?") != std::string::npos)
		{
			exe = std::string("youtube-dl.exe");
			std::string pa("-f best[height=360] --no-part -o cache/cache.mp4 ");
			std::string th(VideoURL);
			path = pa + th;
		}
		else
		{
			exe = std::string("youtube-dl.exe");
			std::string pa("--no-part -o cache/cache.mp4 ");
			std::string th(VideoURL);
			path = pa + th;
		}

		//yotube-dl���g���ē�����_�E�����[�h
		//yotube-dl�Ń_�E�����[�hURL���擾�ł��܂������ڂɂ͍Đ��ł��܂���
		yotube_dl(exe.c_str(), path.c_str());

		downloadmovieflag = 1;

		timer = 0;
	}

	//test.mp4���ł���܂Ń��[�v
	if (stat("cache/cache.mp4", &buf) == 0)downloadmovieflag = 0, moviefile = 1;

	timer++;
	if (timer == 30 * 30)downloadmovieflag = 0;

	return downloadmovieflag;
}



void Youtube::setScreen(const int &ModelHandel, int ScreenTexture)
{
	model_handel.push_back(ModelHandel);
	screen_tex.push_back(MV1GetMaterialDifMapTexture(ModelHandel, ScreenTexture));

	MV1SetTextureGraphHandle(ModelHandel, ScreenTexture, screen_handel, FALSE);
}

void Youtube::updateYotubeMovie()
{
	bool in = 0;
	for (size_t i = 0; i < model_handel.size(); i++)
	{
		if (!CheckCameraViewClip(MV1GetPosition(model_handel[i])))
		{
			in = 1;
			break;
		}
	}

	if (in)
	{
		//�Đ���
		if (Playing() == 1)
		{
			//�`���ύX�@���J�����̐ݒ肪����������܂�
			SetDrawScreen(screen_handel);

			//�e�N�X�`���̃T�C�Y�ɍ����悤�Ɋg��
			DrawExtendGraph(0, 0, 1024, 512, GraHandle, FALSE);

			SetDrawScreen(DX_SCREEN_BACK);

			updateMovie(GraHandle);

			for (size_t i = 0; i < model_handel.size(); i++)
			{
				if (!CheckCameraViewClip(MV1GetPosition(model_handel[i])))
				{
					MV1SetTextureGraphHandle(model_handel[i], screen_tex[i], screen_handel, FALSE);
				}
			}
		}
	}
}

void Youtube::pauseYotubeMovie()
{
	pauseMovie();
}

void Youtube::playYotubeMovie()
{
	playMovie();
}


//�}���`�X���b�h�֌W------------------------------------------------------------------------------
//�ۑ����Ȃ���Đ����邽�߂�_beginthreadex���g���Ă܂�(std::thread�ł̓G���[)

//�}���`�X���b�h�ɓn������
char Url[256];
char Exe[256];

unsigned int __stdcall ThreadFunc(PVOID pv) //�}���`�X���b�h�ōs������
{
	DWORD dwThread = GetCurrentThreadId();

	SHELLEXECUTEINFO sei;
	memset(&sei, 0, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.hwnd = GetForegroundWindow();
	sei.lpVerb = _T("open");
	sei.lpFile = Exe;
	sei.lpParameters = Url;
	sei.nShow = SW_HIDE;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;

	//�W���o�͂��l������Ȃ�CreateProcess���g��
	if (ShellExecuteEx(&sei) == FALSE) {
		//���s�����ꍇ�̃G���[����
		MessageBox(GetMainWindowHandle(), "ShellExecuteEx Failed.", "error", MB_OK);//�G���[�\��
	}

	// �v���Z�X�n���h����ۑ�
	m_ChildProcess = sei.hProcess;

	return 0;
}

void Youtube::yotube_dl(const char *exe, const char *word)
{
	strcpy_s(Url, word);
	strcpy_s(Exe, exe);

	//�}���`�X���b�h���쐬-------------------------------------
	DWORD dwThread;

	hHandle = (HANDLE)_beginthreadex(
		NULL,          // security descripter 
		0,               // stack size
		ThreadFunc, //start address 
		NULL,          // arglist
		0,               // initial state. 0 - running
		(unsigned int*)&dwThread);

	//�G���[�`�F�b�N
	if (hHandle == 0)
	{
		MessageBox(GetMainWindowHandle(), "_beginthreadex Failed.", "error", MB_OK);//�G���[�\��
	}
	//--------------------------------------------------------

	//�}���`�X���b�h���J�n����i�����ɐ����Ԃ��j
	WaitForSingleObject(hHandle, 0);
}

//-----------------------------------------------------------------------------------------------