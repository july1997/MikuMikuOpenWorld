#pragma once

//�t���[�����[�g�֌W
//��ʂ̍X�V���x�����ɂ���

class FpsManager
{
private:
	int mStartTime = 0,
		mCount = 0,
		FPS = 60,
		t = 0;
	float mFps=0;

public:
	//�R���X�g���N�^
	FpsManager();
	//�f�X�g���N�^
	~FpsManager();

	//�t���[�����[�g�𐧌䂵�A��ʂ��t���b�v���܂��B
	void controlFps();

	//�t���[�����[�g�l��\�����܂�
	void displayFps(int x, int y, int color = -1);

	//�t���[�����[�g�l�̐ݒ��ύX���܂�
	void setFps(int fps);

	//���݂̃t���[�����[�g�l���擾���܂�
	float getNowFps();

	//�t���[�����[�g�l�̐ݒ���擾���܂�
	int getDefaultFps();

	//�t���[�����[�g�𐧌䂷�邾���̊֐�
	void controlWaitFps();

	//�t���[�����[�g�𐧌䂵�A��ʂ��t���b�v���A����ʂ����������܂��B
	void controlClearDrawScreenFps();

	int getWaitTime();
};