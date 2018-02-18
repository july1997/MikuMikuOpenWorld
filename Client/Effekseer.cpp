#include "Effekseer.h"



Effekseer_DX::Effekseer_DX()
{
	// �`��p�C���X�^���X�̐���
	g_device = (ID3D11Device*)GetUseDirect3D11Device();
	g_context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();

	g_renderer = ::EffekseerRendererDX11::Renderer::Create(g_device, g_context, MAX_INSTANCE_NUM);

	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	g_manager = ::Effekseer::Manager::Create(2000);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	g_manager->SetSpriteRenderer(g_renderer->CreateSpriteRenderer());
	g_manager->SetRibbonRenderer(g_renderer->CreateRibbonRenderer());
	g_manager->SetRingRenderer(g_renderer->CreateRingRenderer());
	g_manager->SetTrackRenderer(g_renderer->CreateTrackRenderer());
	g_manager->SetModelRenderer(g_renderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	g_manager->SetTextureLoader(g_renderer->CreateTextureLoader());
	g_manager->SetModelLoader(g_renderer->CreateModelLoader());

	// DxLib�ɍ��킹�č�����W�n�ɕύX
	g_manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// ���ł���������摜�Ǎ�
	grHandle = LoadGraph("Effect/Texture/black.png");

	// �G�t�F�N�g�̓Ǎ�
	g_effect = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*)L"Effect/laser.efk");

	// �G�t�F�N�g�̍Đ�
	g_handle = g_manager->Play(g_effect, 0, 0, 0);
}


Effekseer_DX::~Effekseer_DX()
{
	// �G�t�F�N�g�̒�~
	g_manager->StopEffect(g_handle);

	// �G�t�F�N�g�̔j��
	ES_SAFE_RELEASE(g_effect);

	// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
	//g_manager->Destroy();

	// ���ɕ`��p�C���X�^���X��j��
	g_renderer->Destroy();
}


void Effekseer_DX::update()
{

	time++;
	// ����I�ɃG�t�F�N�g���Đ�����
	if (time % 60 == 0)
	{
		// �G�t�F�N�g�̍Đ�
		g_handle = g_manager->Play(g_effect, 0, 0, 0);
	}

	// 3. ���߃t���O�𗧂Ăĉ摜��`��
	// (����F(RGB = (0, 0, 0))�̉摜��p�ӁI)
	DrawGraph(0, 0, grHandle, TRUE);

	// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o��
	RenderVertex();

	// �G�t�F�N�g�̈ړ��������s��
	g_manager->AddLocation(g_handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	// �G�t�F�N�g�̍X�V����
	g_manager->Update();

	// DX���C�u�������̃J�����s��E�ˉe�s���
	// Effekseer���̃J�����s��E�ˉe�s��̓���
	// ���e�s���ݒ�
	MATRIX m = GetCameraProjectionMatrix();
	::Effekseer::Matrix44 f;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j<4; j++)
		{
			f.Values[i][j] = m.m[i][j];
		}
	}
	g_renderer->SetProjectionMatrix(f);

	// �J�����s���ݒ�
	m = GetCameraViewMatrix();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j<4; j++)
		{
			f.Values[i][j] = m.m[i][j];
		}
	}
	g_renderer->SetCameraMatrix(f);
	ClearDrawScreen();
	// 4. �G�t�F�N�g�̕`��J�n����
	g_renderer->BeginRendering();

	// 5. Effekseer���̃G�t�F�N�g�\��
	g_manager->Draw();

	// 6. �G�t�F�N�g�̕`��I������
	g_renderer->EndRendering();

	// 7. DX���C�u�����̐ݒ��߂�
	RefreshDxLibDirect3DSetting();
}