#pragma once


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <stdio.h>
#include <windows.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <d3d11.h>
#include <XAudio2.h>
#pragma comment(lib, "d3d11.lib" )

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>

#if _DEBUG
#pragma comment(lib, "Effekseer.Debug.lib" )
#pragma comment(lib, "EffekseerRendererDX11.Debug.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.Debug.lib" )
#else
#pragma comment(lib, "Effekseer.Release.lib" )
#pragma comment(lib, "EffekseerRendererDX11.Release.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.Release.lib" )
#endif

#include "DxLib.h"


class Effekseer_DX
{
public:
	Effekseer_DX();
	~Effekseer_DX();

	void update();

private:
	size_t time=0;
	int grHandle;

	const int MAX_INSTANCE_NUM = 2000;

	ID3D11Device *g_device;
	ID3D11DeviceContext *g_context;

	Effekseer::Manager*			g_manager;
	EffekseerRenderer::Renderer*	g_renderer ;

	Effekseer::Effect*				g_effect;
	Effekseer::Handle				g_handle = -1;

};

