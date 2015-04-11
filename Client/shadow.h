#pragma once
#include "DxLib.h"

class shadow
{
	// �e�p�̐[�x�l��ۑ�����`��\�O���t�B�b�N
	int DepthBufferGraphHandle;

	// �e�p�̐[�x�L�^�摜���쐬�����ۂ̃J�����̃r���[�s��Ǝˉe�s��
	MATRIX LightCamera_ViewMatrix;
	MATRIX LightCamera_ProjectionMatrix;

	// �[�x�L�^�摜�ւ̕`��p�̍��̃��b�V���p���_�V�F�[�_�[��
	// �X�L�j���O���b�V���p�̒��_�V�F�[�_�[
	int Skin4_DepthShadow_Step1_VertexShader;
	int Normal_DepthShadow_Step1_VertexShader;

	// �[�x�L�^�摜�ւ̕`��p�̃s�N�Z���V�F�[�_�[
	int DepthShadow_Step1_PixelShader;

	// �[�x�L�^�摜���g�����f�B���N�V���i�����C�g��t���̕`��p�̍��̃��b�V���p���_�V�F�[�_�[��
	// �X�L�j���O���b�V���p�̒��_�V�F�[�_�[
	int Skin4_DirLight_DepthShadow_Step2_VertexShader;
	int Normal_DirLight_DepthShadow_Step2_VertexShader;

	// �[�x�L�^�摜���g�����f�B���N�V���i�����C�g��t���̕`��p�̃s�N�Z���V�F�[�_�[
	int DirLight_DepthShadow_Step2_PixelShader;


	VECTOR defright, defdirection;

public:

	shadow();

	void SetupDepthImage(VECTOR playerpos)
	{
		// �`�����e�p�[�x�L�^�摜�ɂ���
		SetDrawScreen(DepthBufferGraphHandle);

		// �e�p�[�x�L�^�摜��^�����ɃN���A
		SetBackgroundColor(255, 255, 255);
		ClearDrawScreen();
		SetBackgroundColor(0, 0, 0);

		SetupCamera_Ortho(500.f);

		SetCameraNearFar(3.f, 2000.f);

		SetCameraPositionAndTargetAndUpVec(VAdd(playerpos, VGet(2.0f, 1000.0f, -35.0f)), VAdd(playerpos, VGet(0.0f, 12.0f, 0.0f)), VGet(0.0f, 0.7f, 0.6f));

		// �ݒ肵���J�����̃r���[�s��Ǝˉe�s����擾���Ă���
		LightCamera_ViewMatrix = GetCameraViewMatrix();
		LightCamera_ProjectionMatrix = GetCameraProjectionMatrix();

		// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɐݒ肷��
		MV1SetUseOrigShader(TRUE);

		// �[�x�L�^�摜�ւ̕`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
		SetUsePixelShader(DepthShadow_Step1_PixelShader);

		// �[�x�L�^�摜�ւ̍��̃��b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(Normal_DepthShadow_Step1_VertexShader);
	}
	void SetupDepthImage2()
	{
		// �[�x�L�^�摜�ւ̃X�L�j���O���b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(Skin4_DepthShadow_Step1_VertexShader);
	}
	void SetupDepthImage3()
	{
		// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɂ����ݒ������
		MV1SetUseOrigShader(FALSE);

		SetDrawScreen(DX_SCREEN_BACK);//����ʂɕ`��ݒ�

		SetCameraNearFar(3.0f, 2000.0f);
	}

	void DrawModelWithDepthShadow(void)
	{
		// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɐݒ肷��
		MV1SetUseOrigShader(TRUE);

		// �[�x�L�^�摜���g�����e�{�f�B���N�V���i�����C�g��`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
		SetUsePixelShader(DirLight_DepthShadow_Step2_PixelShader);

		// �e�p�[�x�L�^�摜��`�悵���Ƃ��̃J�����̃r���[�s��Ǝˉe�s���萔�ɐݒ肷��
		SetVSConstFMtx(43, LightCamera_ViewMatrix);
		SetVSConstFMtx(47, LightCamera_ProjectionMatrix);

		// �e�p�[�x�L�^�摜���e�N�XSetUseVertexShader�`���P�ɃZ�b�g
		SetUseTextureToShader(1, DepthBufferGraphHandle);

		// �[�x�L�^�摜���g�����e�{�f�B���N�V���i�����C�g��̍��̃��b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(Normal_DirLight_DepthShadow_Step2_VertexShader);
	}
	void DrawModelWithDepthShadow2(void)
	{
		// �[�x�L�^�摜���g�����e�{�f�B���N�V���i�����C�g��̃X�L�j���O���b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(Skin4_DirLight_DepthShadow_Step2_VertexShader);
	}
	void DrawModelWithDepthShadow3(void)
	{
		// �g�p�e�N�X�`���̐ݒ������
		SetUseTextureToShader(1, -1);

		// �ݒ肵���萔������
		ResetVSConstF(43, 16);
		ResetVSConstF(47, 16);

		// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɂ����ݒ������
		MV1SetUseOrigShader(FALSE);
	}

};

