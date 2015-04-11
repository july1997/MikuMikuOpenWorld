#include "shadow.h"


shadow::shadow()
{
	// �쐬����摜�̃t�H�[�}�b�g��s�������_�^�łP�`�����l���A�P�U�r�b�g�ɂ���
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);
	SetCreateDrawValidGraphChannelNum(1);
	SetCreateGraphColorBitDepth(16);

	// �e�p�[�x�L�^�摜�̍쐬
	DepthBufferGraphHandle = MakeScreen(2048, 2048, FALSE);

	// �ݒ�����ɖ߂�
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);

	// �[�x�L�^�摜�`��p�̒��_�V�F�[�_�[��ǂݍ���
	Skin4_DepthShadow_Step1_VertexShader = LoadVertexShader("Shader/shadow/Vso/SkinMesh4_DepthShadow_Step1VS.vso");
	Normal_DepthShadow_Step1_VertexShader = LoadVertexShader("Shader/shadow/Vso/NormalMesh_DepthShadow_Step1VS.vso");

	// �[�x�L�^�摜�`��p�̃s�N�Z���V�F�[�_�[��ǂݍ���
	DepthShadow_Step1_PixelShader = LoadPixelShader("Shader/shadow/Pso/DepthShadow_Step1PS.pso");

	// �[�x�L�^�摜���g�����f�B���N�V���i�����C�g��̕`��p���_�V�F�[�_�[��ǂݍ���
	Normal_DirLight_DepthShadow_Step2_VertexShader = LoadVertexShader("Shader/shadow/Vso/NormalMesh_DirLight_DepthShadow_Step2VS.vso");

	// �[�x�L�^�摜���g�����f�B���N�V���i�����C�g��̕`��p�s�N�Z���V�F�[�_�[��ǂݍ���
	DirLight_DepthShadow_Step2_PixelShader = LoadPixelShader("Shader/shadow/Pso/DirLight_DepthShadow_Step2PS.pso");
}


