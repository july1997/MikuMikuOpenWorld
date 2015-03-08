#pragma once
#include "btBulletDynamicsCommon.h"

#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"


//�v���O�}�錾�����J
#pragma comment( lib, "BulletCollision.lib" )
#pragma comment( lib, "BulletDynamics.lib" )
#pragma comment( lib, "BulletSoftBody.lib" )
#pragma comment( lib, "LinearMath.lib" )

//obj�ǂݍ���
#include "rx_model.h"

#ifdef _DEBUG
#pragma comment (lib, "rx_modeld.lib")
#else
#pragma comment (lib, "rx_model.lib")
#endif


#include <vector>

#include "DxLib.h"



class Bullet_physics
{
public:
	//�R���X�g���N�^
	Bullet_physics();
	//�f�X�g���N�^
	~Bullet_physics();

	//�_�C�i�~�N�X���[���h���쐬���܂� CreateWorld(���[���h�L���i�ŏ��j,���[���h�L���i�ő�j,�v���L�V�̍ő吔�A�d��)�@�߂�l�F���[���h�n���h��
	int createWorld(VECTOR WorldMin = VGet(-10000.f, -10000.f, -10000.f), VECTOR WorldMax = VGet(10000.f, 10000.f, 10000.f), int maxProxies = 1024, VECTOR Gravity = VGet(0, -9.8f, 0));

	//�쐬�����_�C�i�~�N�X���[���h�̐����擾���܂�
	int getWorldNun();

	//���[���h�Ƀ��f���f�[�^����n�ʂ��쐬�����܂� �߂�l�F���̂̃n���h��
	int createGroundBodytoMesh(int ModelHandel, int WorldHandle);

	//�쐬�������̂̐����擾���܂�
	int getBodyNun();

	//���[���h�ɔ����쐬���܂� CreateBoxBody(���[���h�n���h��,��肽�����̔����̑傫���i���̒��S����j,���W,��]�l,����(kg),����) �߂�l�F���̂̃n���h��
	int createBoxBody(int WorldHandle, VECTOR boxHalfExtents, VECTOR pos = VGet(0.f, 0.f, 0.f), VECTOR rot = VGet(0.f, 0.f, 0.f), float mass = 1, VECTOR inertia = VGet(0.f, 0.f, 0.f));

	//���̂̍��W���擾���܂�
	VECTOR getBodyPos(int BodyHandel, VECTOR subtractVC = VGet(0, 0, 0));
	//���̂̉�]���擾���܂�
	VECTOR getBodyRot(int BodyHandel);

	//�V�~�����[�V�������Ԃ�i�߂܂�
	void stepSimulation(int WorldHandle, float fps=60.f);

	//���[���h�ɋ��̂��쐬���܂� ceateSphereBody(���[���h�n���h��,���a,���W,��]�l,����(kg),����) �߂�l�F���̂̃n���h��
	int ceateSphereBody(int WorldHandle, float radius, VECTOR pos = VGet(0.f, 0.f, 0.f), VECTOR rot = VGet(0.f, 0.f, 0.f), float mass = 1, VECTOR inertia = VGet(0.f, 0.f, 0.f));

	//broadphase���擾
	btAxisSweep3* getBroadphase(int WorldHandel);
	//���[���h���擾
	btDiscreteDynamicsWorld* getbtDiscreteDynamicsWorld(int WorldHandel);
	//btRigidBody���擾
	btRigidBody* getbtRigidBody(int BodyHandel);


	//�ԗ��n--------------------------------------------------------------------------

	//�ԗ�(4��)���쐬���܂�
	int createCar(int *CarBodyHandel, int WorldHandle, VECTOR boxHalfExtents, VECTOR pos, float mass,
		VECTOR rot = VGet(0, 0, 0), float wheelFriction = 3.5f, float wheelWidth = 1.5f,
		float wheelRadius = 4.25f, float suspensionStiffness = 40.f, float suspensionDamping = 4.3f,
		float suspensionCompression = 4.4f, float rollInfluence = 0.025f, float connectionHeight = 0.f,
		VECTOR Gravity = VGet(0, -100.f, 0), VECTOR wheelDirectionCS0 = VGet(0, -1, 0),
		VECTOR wheelAxleCS = VGet(1, 0, 0), float suspensionRestLength = 6.f);

	//�Ԃ̑�������܂�
	void controlCar(int CarHandel, float gEngineForce, float gBreakingForce, float gVehicleSteering);

	//�Ԃ̃z�C�[���𓮂����܂�
	void controlWheel(int CarHandel, int CarModelHandel, int frameFL, int frameFR, int frameRL, int frameRR);

	//btRaycastVehicle���擾
	btRaycastVehicle* getbtRaycastVehicle(int CarHandel);




	//�L�����N�^�[�n---------------------------------------------------------------------

	//�L�����N�^�[���쐬���܂�
	int createCharacter(int WorldHandle, VECTOR pos, float stepHeight=0.35f, float characterHeight = 16.f, float characterWidth = 2.f);

	//�v���[���[�̑�������܂�
	void controlCharacter(int CharacterHandel, int Left, int Right, int Forward, int Backward, int Jump, float walkspeed = 1.4f);

	//btPairCachingGhostObject�̎擾
	btPairCachingGhostObject* getbtGhostObject(int CaracterHandel);
	//btKinematicCharacterController�̎擾
	btKinematicCharacterController* getCharacterController(int CaracterHandel);

	//�L�����N�^�[�̍��W���擾���܂�
	VECTOR getCharacterPos(int CaracterHandel, VECTOR subtractVC= VGet(0,0,0));
	//�L�����N�^�[�̉�]���擾���܂�
	VECTOR getCharacterRot(int CaracterHandel);

	//�L�����N�^�[�̍��W�Ɖ�]��ݒ肵�܂�
	void setCharacterPosRot(int CaracterHandel, VECTOR pos, btQuaternion &rot);
	int pretendCreateCharacter();
	//btQuaternion�̎擾
	btQuaternion getCharacterRotRaw(int CaracterHandel);


	//�\�t�g�{�f�B�n---------------------------------------------------------------------

	//�\�t�g�{�f�B���������[���h���쐬���܂� CreateSoftWorld(���[���h�L���i�ŏ��j,���[���h�L���i�ő�j,�v���L�V�̍ő吔�A�d��)�@�߂�l�F���[���h�n���h��
	int createSoftWorld(VECTOR WorldMin = VGet(-10000.f, -10000.f, -10000.f), VECTOR WorldMax = VGet(10000.f, 10000.f, 10000.f), int maxProxies = 1024, VECTOR Gravity = VGet(0, -10.f, 0));

	//�\�t�g�{�f�B�̔��̍쐬
	int createSoftBox(int SoftWorldHandle);

	//btSoftRigidDynamicsWorld�̎擾
	btSoftRigidDynamicsWorld* getbtSoftRigidDynamicsWorld(int WorldHandel);
	//btSoftBody�̎擾
	btSoftBody* getbtSoftBody(int SoftBodyHandel);

	//�\�t�g�{�f�B���������[���h�ɍ��㍄�̂�ǉ����邩���Ȃ���(�\�t�g���[���h�����Ǝ�����ON)
	void useSoftWorld(bool Flag);

	int createSoftSphere(int SoftWorldHandle);

	//���f���f�[�^����softbody���쐬���܂� ���b�V���͊ȈՉ�����܂�
	int createSoftBodyConvexHull(int ModelHandel, int SoftWorldHandle);

	//obj�t�@�C������softbody���쐬���܂��@http://slis.tsukuba.ac.jp/~fujisawa.makoto.fu/ �l�̃��f���ǂݍ��݃��C�u�����g�p
	int createSoftBodyFromTriMesh(char *FileName, int SoftWorldHandle, VECTOR Scale = VGet(1.f, 1.f, 1.f), VECTOR Pos = VGet(0.f, 0.f, 0.f), VECTOR Rot = VGet(0.f, 0.f, 0.f),float mass=1);

	//���̂ƃ\�t�g�{�f�B���Ȃ��A���J�[��ǉ����܂�
	void addAnchor(int SoftBodyHandel,int node, int RigidBodyHandel, bool InvalidCollision = FALSE, float influence = 1.f);


	//�֗��@�\
	//VECTOR �� btVector3 �ɕϊ�
	btVector3 btvDxv(const VECTOR &inv){ return btVector3(inv.x, inv.y, inv.z); }
	//btVctor3 �� VECTOR �ɕϊ�
	VECTOR btVGet(const btVector3 &inv){ return VGet(inv.getX(), inv.getY(), inv.getZ()); }
	//VECTOR �� dx���C�u�����̉�]�l �ɕϊ�
	VECTOR VGetRot(const VECTOR &inv){ return VGet(-inv.x, -inv.y, inv.z); }

private:

	int WorldHandel = -1;
	int BodyHandel = -1;
	int SoftBodyHandel = -1;
	int CarHandel = -1;
	int CharacterHandel = -1;
	int SoftWorldHandel = -1;

	//�v�f��ǉ����邽�߂�std::vector���g��
	std::vector<btDiscreteDynamicsWorld *> dynamicsWorld;
	std::vector<btSoftRigidDynamicsWorld *> softWorld;
	std::vector<btAxisSweep3 *> broadphase;
	std::vector<btAxisSweep3 *> SoftWorldBroadphase;
	std::vector<btRaycastVehicle *> vehicle;
	std::vector<btRigidBody *> body;
	std::vector<btSoftBody *> Softbody;
	std::vector<btPairCachingGhostObject *> ghostObject;
	std::vector<btKinematicCharacterController *> character;

	btSoftBodyWorldInfo	m_softBodyWorldInfo;
	btCollisionDispatcher*	m_dispatcher;
	bool UseSoftWorld = 0;
	
#ifdef  B2D_DEBUG_DRAW
	btRigidBody* en, *en2, *en3, *en4;
#endif
};

