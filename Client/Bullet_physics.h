#pragma once
#include "btBulletDynamicsCommon.h"

#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"


//プラグマ宣言リンカ
#pragma comment( lib, "BulletCollision.lib" )
#pragma comment( lib, "BulletDynamics.lib" )
#pragma comment( lib, "BulletSoftBody.lib" )
#pragma comment( lib, "LinearMath.lib" )

//obj読み込み
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
	//コンストラクタ
	Bullet_physics();
	//デストラクタ
	~Bullet_physics();

	//ダイナミクスワールドを作成します CreateWorld(ワールド広さ（最小）,ワールド広さ（最大）,プロキシの最大数、重力)　戻り値：ワールドハンドル
	int createWorld(VECTOR WorldMin = VGet(-10000.f, -10000.f, -10000.f), VECTOR WorldMax = VGet(10000.f, 10000.f, 10000.f), int maxProxies = 1024, VECTOR Gravity = VGet(0, -9.8f, 0));

	//作成したダイナミクスワールドの数を取得します
	int getWorldNun();

	//ワールドにモデルデータから地面を作成をします 戻り値：剛体のハンドル
	int createGroundBodytoMesh(int ModelHandel, int WorldHandle);

	//作成した剛体の数を取得します
	int getBodyNun();

	//ワールドに箱を作成します CreateBoxBody(ワールドハンドル,作りたい箱の半分の大きさ（箱の中心から）,座標,回転値,質量(kg),慣性) 戻り値：剛体のハンドル
	int createBoxBody(int WorldHandle, VECTOR boxHalfExtents, VECTOR pos = VGet(0.f, 0.f, 0.f), VECTOR rot = VGet(0.f, 0.f, 0.f), float mass = 1, VECTOR inertia = VGet(0.f, 0.f, 0.f));

	//剛体の座標を取得します
	VECTOR getBodyPos(int BodyHandel, VECTOR subtractVC = VGet(0, 0, 0));
	//剛体の回転を取得します
	VECTOR getBodyRot(int BodyHandel);

	//シミュレーション時間を進めます
	void stepSimulation(int WorldHandle, float fps=60.f);

	//ワールドに球体を作成します ceateSphereBody(ワールドハンドル,半径,座標,回転値,質量(kg),慣性) 戻り値：剛体のハンドル
	int ceateSphereBody(int WorldHandle, float radius, VECTOR pos = VGet(0.f, 0.f, 0.f), VECTOR rot = VGet(0.f, 0.f, 0.f), float mass = 1, VECTOR inertia = VGet(0.f, 0.f, 0.f));

	//broadphaseを取得
	btAxisSweep3* getBroadphase(int WorldHandel);
	//ワールドを取得
	btDiscreteDynamicsWorld* getbtDiscreteDynamicsWorld(int WorldHandel);
	//btRigidBodyを取得
	btRigidBody* getbtRigidBody(int BodyHandel);


	//車両系--------------------------------------------------------------------------

	//車両(4輪)を作成します
	int createCar(int *CarBodyHandel, int WorldHandle, VECTOR boxHalfExtents, VECTOR pos, float mass,
		VECTOR rot = VGet(0, 0, 0), float wheelFriction = 3.5f, float wheelWidth = 1.5f,
		float wheelRadius = 4.25f, float suspensionStiffness = 40.f, float suspensionDamping = 4.3f,
		float suspensionCompression = 4.4f, float rollInfluence = 0.025f, float connectionHeight = 0.f,
		VECTOR Gravity = VGet(0, -100.f, 0), VECTOR wheelDirectionCS0 = VGet(0, -1, 0),
		VECTOR wheelAxleCS = VGet(1, 0, 0), float suspensionRestLength = 6.f);

	//車の操作をします
	void controlCar(int CarHandel, float gEngineForce, float gBreakingForce, float gVehicleSteering);

	//車のホイールを動かします
	void controlWheel(int CarHandel, int CarModelHandel, int frameFL, int frameFR, int frameRL, int frameRR);

	//btRaycastVehicleを取得
	btRaycastVehicle* getbtRaycastVehicle(int CarHandel);




	//キャラクター系---------------------------------------------------------------------

	//キャラクターを作成します
	int createCharacter(int WorldHandle, VECTOR pos, float stepHeight=0.35f, float characterHeight = 16.f, float characterWidth = 2.f);

	//プレーヤーの操作をします
	void controlCharacter(int CharacterHandel, int Left, int Right, int Forward, int Backward, int Jump, float walkspeed = 1.4f);

	//btPairCachingGhostObjectの取得
	btPairCachingGhostObject* getbtGhostObject(int CaracterHandel);
	//btKinematicCharacterControllerの取得
	btKinematicCharacterController* getCharacterController(int CaracterHandel);

	//キャラクターの座標を取得します
	VECTOR getCharacterPos(int CaracterHandel, VECTOR subtractVC= VGet(0,0,0));
	//キャラクターの回転を取得します
	VECTOR getCharacterRot(int CaracterHandel);

	//キャラクターの座標と回転を設定します
	void setCharacterPosRot(int CaracterHandel, VECTOR pos, btQuaternion &rot);
	int pretendCreateCharacter();
	//btQuaternionの取得
	btQuaternion getCharacterRotRaw(int CaracterHandel);


	//ソフトボディ系---------------------------------------------------------------------

	//ソフトボディを扱うワールドを作成します CreateSoftWorld(ワールド広さ（最小）,ワールド広さ（最大）,プロキシの最大数、重力)　戻り値：ワールドハンドル
	int createSoftWorld(VECTOR WorldMin = VGet(-10000.f, -10000.f, -10000.f), VECTOR WorldMax = VGet(10000.f, 10000.f, 10000.f), int maxProxies = 1024, VECTOR Gravity = VGet(0, -10.f, 0));

	//ソフトボディの箱の作成
	int createSoftBox(int SoftWorldHandle);

	//btSoftRigidDynamicsWorldの取得
	btSoftRigidDynamicsWorld* getbtSoftRigidDynamicsWorld(int WorldHandel);
	//btSoftBodyの取得
	btSoftBody* getbtSoftBody(int SoftBodyHandel);

	//ソフトボディを扱うワールドに今後剛体を追加するかしないか(ソフトワールドを作ると自動でON)
	void useSoftWorld(bool Flag);

	int createSoftSphere(int SoftWorldHandle);

	//モデルデータからsoftbodyを作成します メッシュは簡易化されます
	int createSoftBodyConvexHull(int ModelHandel, int SoftWorldHandle);

	//objファイルからsoftbodyを作成します　http://slis.tsukuba.ac.jp/~fujisawa.makoto.fu/ 様のモデル読み込みライブラを使用
	int createSoftBodyFromTriMesh(char *FileName, int SoftWorldHandle, VECTOR Scale = VGet(1.f, 1.f, 1.f), VECTOR Pos = VGet(0.f, 0.f, 0.f), VECTOR Rot = VGet(0.f, 0.f, 0.f),float mass=1);

	//剛体とソフトボディをつなぐアンカーを追加します
	void addAnchor(int SoftBodyHandel,int node, int RigidBodyHandel, bool InvalidCollision = FALSE, float influence = 1.f);


	//便利機能
	//VECTOR を btVector3 に変換
	btVector3 btvDxv(const VECTOR &inv){ return btVector3(inv.x, inv.y, inv.z); }
	//btVctor3 を VECTOR に変換
	VECTOR btVGet(const btVector3 &inv){ return VGet(inv.getX(), inv.getY(), inv.getZ()); }
	//VECTOR を dxライブラリの回転値 に変換
	VECTOR VGetRot(const VECTOR &inv){ return VGet(-inv.x, -inv.y, inv.z); }

private:

	int WorldHandel = -1;
	int BodyHandel = -1;
	int SoftBodyHandel = -1;
	int CarHandel = -1;
	int CharacterHandel = -1;
	int SoftWorldHandel = -1;

	//要素を追加するためにstd::vectorを使う
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

