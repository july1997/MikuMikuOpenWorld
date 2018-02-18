#include "Bullet_physics.h"

//Bullet_physics.hを参照してください

Bullet_physics::Bullet_physics()
{
}


Bullet_physics::~Bullet_physics()
{
}


int Bullet_physics::createWorld(VECTOR WorldMin, VECTOR WorldMax, int maxProxies, VECTOR Gravity)
{
	WorldHandel++;

	// ワールドの広さ
	btVector3 worldAabbMin = btvDxv(WorldMin);
	btVector3 worldAabbMax = btvDxv(WorldMax);

	// broadphaseの作成（SAP法）
	broadphase.push_back(new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies));

	// デフォルトの衝突設定とディスパッチャの作成
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	
	//コリジョンオブジェクトの異なるタイプ（btGImpactMeshShapeを使用して、例えばメッシュ）を導入した場合、あなたは衝突が認識取得する衝突アルゴリズムを登録する必要がある場合があります。
	//btGImpactCollisionAlgorithm::registerAlgorithm(dynamicsWorld[WorldHandle]->getDispatcher());

	// 衝突解決ソルバ
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	//std::vectorで追加
	dynamicsWorld.push_back(new btDiscreteDynamicsWorld(dispatcher, broadphase[WorldHandel], solver, collisionConfiguration));

	// 重力の設定
	dynamicsWorld[WorldHandel]->setGravity(btvDxv(Gravity));

	return WorldHandel;
}

int Bullet_physics::getWorldNun()
{
	return WorldHandel;
}

int Bullet_physics::createGroundBodytoMesh(int ModelHandel, int WorldHandle)
{
	BodyHandel++;

	//モデルデータから地面の作成
	// 参照用メッシュの構築
	MV1SetupReferenceMesh(ModelHandel, -1, TRUE);

	// 参照用メッシュの取得
	MV1_REF_POLYGONLIST RefMesh = MV1GetReferenceMesh(ModelHandel, -1, TRUE);

	//地面のメッシュからトライアングルリスト作成
	btTriangleMesh* VartexGround = new btTriangleMesh();

	// ポリゴンの数だけ繰り返し
	for (int i = 0; i < RefMesh.PolygonNum; i++)
	{
		float a0 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[0]].Position.x;
		float a1 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[0]].Position.y;
		float a2 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[0]].Position.z;
		float b0 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[1]].Position.x;
		float b1 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[1]].Position.y;
		float b2 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[1]].Position.z;
		float c0 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[2]].Position.x;
		float c1 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[2]].Position.y;
		float c2 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[2]].Position.z;
		btVector3 A(a0, a1, a2);
		btVector3 B(b0, b1, b2);
		btVector3 C(c0, c1, c2);
		VartexGround->addTriangle(A, B, C, false);
	}

	// 地面の衝突形状の作成
	btBvhTriangleMeshShape* groundShape = new btBvhTriangleMeshShape(VartexGround, true, true);

	// 地面のMotionStateの設定
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	// 地面の初期情報を設定
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));

	// 地面の剛体の作成
	body.push_back(new btRigidBody(groundRigidBodyCI));

	if (UseSoftWorld)
	{
		//ソフトワールドに追加
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ワールドに地面の剛体を追加
		dynamicsWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}

	return BodyHandel;
}

int Bullet_physics::getBodyNun()
{
	return BodyHandel;
}

int Bullet_physics::createBoxBody(int WorldHandle, VECTOR boxHalfExtents, VECTOR pos, VECTOR rot, float mass, VECTOR inertia)
{
	BodyHandel++;

	// 箱
	btCollisionShape* boxshape = new btBoxShape(btvDxv(boxHalfExtents));

	// 箱のMotionStateの設定
	btDefaultMotionState* moptionstate = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.y, rot.z, 1), btvDxv(pos)));

	btVector3 tes = btvDxv(inertia);

	// 質量、慣性の設定
	boxshape->calculateLocalInertia(btScalar(mass), tes);//inertia　慣性

	// 箱の初期情報を設定
	btRigidBody::btRigidBodyConstructionInfo screenshapebodyCI(mass, moptionstate, boxshape, btvDxv(inertia));

	// 箱の剛体の作成
	body.push_back(new btRigidBody(screenshapebodyCI));

	if (UseSoftWorld)
	{
		//ソフトワールドに追加
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ワールドに箱の剛体を追加
		dynamicsWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	return BodyHandel;
}

VECTOR Bullet_physics::getBodyPos(int BodyHandel, VECTOR subtractVC)
{
	btTransform trans;
	body[BodyHandel]->getMotionState()->getWorldTransform(trans);

	// 位置取得
	VECTOR pos = btVGet(trans.getOrigin());
	pos = VGet(pos.x - subtractVC.x, pos.y - subtractVC.y, pos.z - subtractVC.z);
	return pos;
}

VECTOR Bullet_physics::getBodyRot(int BodyHandel)
{
	btTransform trans;
	body[BodyHandel]->getMotionState()->getWorldTransform(trans);

	// 角度取得
	btVector3 euler;

	btMatrix3x3 basis = trans.getBasis();
	basis.getEulerZYX(euler[2], euler[1], euler[0]);
	VECTOR rot = btVGet(euler);

	return rot;
}

void Bullet_physics::stepSimulation(int WorldHandle, float fps)
{
	if (UseSoftWorld)
	{
		softWorld[WorldHandle]->stepSimulation(btScalar(1 / fps), 10);
	}
	else
	{
		dynamicsWorld[WorldHandle]->stepSimulation(btScalar(1 / fps), 10);
	}
}

int Bullet_physics::createSphereBody(int WorldHandle, float radius, VECTOR pos, VECTOR rot, float mass, VECTOR inertia)
{
	BodyHandel++;

	//球
	btCollisionShape* sphereshape = new btSphereShape(radius);
	// 箱のMotionStateの設定
	btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.y, rot.z, 1), btvDxv(pos)));

	btVector3 tes = btvDxv(inertia);
	// 質量、慣性の設定
	sphereshape->calculateLocalInertia(mass, tes);
	// 箱の初期情報を設定
	btRigidBody::btRigidBodyConstructionInfo sphereshapebodyCI(mass, motionstate, sphereshape, btvDxv(inertia));
	// 箱の剛体の作成
	body.push_back(new btRigidBody(sphereshapebodyCI));

	if (UseSoftWorld)
	{
		//ソフトワールドに追加
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ワールドに箱の剛体を追加
		dynamicsWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}

	return BodyHandel;
}

int Bullet_physics::createBodytoMesh(int DXLibeModelHandel, int WorldHandle, VECTOR pos, VECTOR rot, float mass, VECTOR inertia)
{
	BodyHandel++;

	//モデルデータから地面の作成
	// 参照用メッシュの構築
	MV1SetupReferenceMesh(DXLibeModelHandel, -1, TRUE);

	// 参照用メッシュの取得
	MV1_REF_POLYGONLIST RefMesh = MV1GetReferenceMesh(DXLibeModelHandel, -1, TRUE);

	//衝突形状のインスタンスの作成
	btTriangleIndexVertexArray* indexVertexArrays = new btTriangleIndexVertexArray(RefMesh.PolygonNum,
		&RefMesh.Polygons[0].VIndex[0], sizeof(MV1_REF_POLYGON), RefMesh.VertexNum, &RefMesh.Vertexs[0].Position.x, sizeof(MV1_REF_VERTEX));

	// 衝突形状の作成
	btGImpactMeshShape *shape = new btGImpactMeshShape(indexVertexArrays);
	//updateBound()を忘れないように
	shape->updateBound();

	// 箱のMotionStateの設定
	btDefaultMotionState* moptionstate = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.y, rot.z, 1), btvDxv(pos)));

	btVector3 tes = btvDxv(inertia);

	// 質量、慣性の設定
	shape->calculateLocalInertia(btScalar(mass), tes);//inertia　慣性

	// 箱の初期情報を設定
	btRigidBody::btRigidBodyConstructionInfo screenshapebodyCI(mass, moptionstate, shape, btvDxv(inertia));

	// 箱の剛体の作成
	body.push_back(new btRigidBody(screenshapebodyCI));

	if (UseSoftWorld)
	{
		//ソフトワールドに追加
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ワールドに箱の剛体を追加
		dynamicsWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	return BodyHandel;
}

int Bullet_physics::createBodytoMesh2(int DXLibeModelHandel, int WorldHandle, VECTOR pos, VECTOR rot, float mass, VECTOR inertia)
{
	BodyHandel++;

	//モデルデータから地面の作成
	// 参照用メッシュの構築
	MV1SetupReferenceMesh(DXLibeModelHandel, -1, TRUE);

	// 参照用メッシュの取得
	MV1_REF_POLYGONLIST RefMesh = MV1GetReferenceMesh(DXLibeModelHandel, -1, TRUE);

	VECTOR *Points;

	Points = (VECTOR *)malloc(sizeof(VECTOR) * RefMesh.PolygonNum * 3);
	for (int i = 0; i < RefMesh.PolygonNum; i++)
	{
		Points[i * 3] = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[0]].Position;
		Points[i * 3 + 1] = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[1]].Position;
		Points[i * 3 + 2] = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[2]].Position;
	}

	// 衝突形状の作成
	btConvexHullShape *shape = new btConvexHullShape((btScalar *)Points, RefMesh.PolygonNum * 3, sizeof(VECTOR));

	// 箱のMotionStateの設定
	btDefaultMotionState* moptionstate = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.y, rot.z, 1), btvDxv(pos)));

	btVector3 tes = btvDxv(inertia);

	// 質量、慣性の設定
	shape->calculateLocalInertia(btScalar(mass), tes);//inertia　慣性

																  // 箱の初期情報を設定
	btRigidBody::btRigidBodyConstructionInfo screenshapebodyCI(mass, moptionstate, shape, btvDxv(inertia));

	// 箱の剛体の作成
	body.push_back(new btRigidBody(screenshapebodyCI));

	if (UseSoftWorld)
	{
		//ソフトワールドに追加
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ワールドに箱の剛体を追加
		dynamicsWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	free(Points);

	return BodyHandel;
}

int Bullet_physics::removeRigidBody(int WorldHandle, int BodyHandel)
{
	if (UseSoftWorld)
	{
		//ソフトワールドに追加
		softWorld[WorldHandle]->removeRigidBody(body[BodyHandel]);
	}
	else
	{
		// ワールドに地面の剛体を追加
		dynamicsWorld[WorldHandle]->removeRigidBody(body[BodyHandel]);
	}

	body.erase(body.begin() + BodyHandel);

	BodyHandel--;

	return 0;
}

btDiscreteDynamicsWorld* Bullet_physics::getWorld(int WorldHandel)
{
	if (UseSoftWorld)
	{
		return softWorld[WorldHandel];
	}
	else
	{
		return dynamicsWorld[WorldHandel];
	}
}

btAxisSweep3* Bullet_physics::getBroadphase(int WorldHandel)
{
	return broadphase[WorldHandel];
}

btDiscreteDynamicsWorld* Bullet_physics::getbtDiscreteDynamicsWorld(int WorldHandel)
{
	return dynamicsWorld[WorldHandel];
}

btRigidBody* Bullet_physics::getbtRigidBody(int BodyHandel)
{
	return body[BodyHandel];
}

int Bullet_physics::createCar(int *CarBodyHandel, int WorldHandle, VECTOR boxHalfExtents, VECTOR pos, float mass, VECTOR rot, float wheelFriction, float wheelWidth, float wheelRadius,
	float suspensionStiffness, float suspensionDamping, float suspensionCompression, float rollInfluence, float connectionHeight, VECTOR Gravity, VECTOR wheelDirectionCS0, VECTOR wheelAxleCS, float suspensionRestLength)
{
	BodyHandel++;
	CarHandel++;

	//箱の作成

	// 箱の衝突形状の作成							↓中心から
	btCollisionShape* boxShape = new btBoxShape(btvDxv(boxHalfExtents));
	// 箱のMotionStateの設定
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btVector3 fallInertia(0, 0, 0);
	boxShape->calculateLocalInertia(mass, fallInertia);
	// 箱の初期情報を設定
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI3(mass, fallMotionState, boxShape, fallInertia);
	// 箱の剛体の作成
	body.push_back(new btRigidBody(fallRigidBodyCI3));

	if (UseSoftWorld)
	{
		//ソフトワールドに追加
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ワールドに箱の剛体を追加
		dynamicsWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	*CarBodyHandel = BodyHandel;
	//--------------------------------------------

	//簡易なBullet車両モデル
	btRaycastVehicle::btVehicleTuning	m_tuning;

	//ソフトワールドの時
	if (UseSoftWorld)
	{
		btVehicleRaycaster* m_vehicleRayCaster = new btDefaultVehicleRaycaster(softWorld[WorldHandle]);
		vehicle.push_back(new btRaycastVehicle(m_tuning, body[BodyHandel], m_vehicleRayCaster));
		body[BodyHandel]->setActivationState(DISABLE_DEACTIVATION);//無効化を拒否
		softWorld[WorldHandle]->addVehicle(vehicle[CarHandel]);

		btCollisionShape* m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

		body[BodyHandel]->setCenterOfMassTransform(btTransform::getIdentity());
		body[BodyHandel]->setAngularVelocity(btvDxv(rot));//車の角度を指定
		body[BodyHandel]->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btvDxv(pos)));//位置を指定
		body[BodyHandel]->setGravity(btvDxv(Gravity));//重力の設定

		softWorld[WorldHandle]->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(body[BodyHandel]->getBroadphaseHandle(), softWorld[WorldHandle]->getDispatcher());
	}
	else
	{
		//簡易なBullet車両モデル
		btVehicleRaycaster* m_vehicleRayCaster = new btDefaultVehicleRaycaster(dynamicsWorld[WorldHandle]);
		vehicle.push_back(new btRaycastVehicle(m_tuning, body[BodyHandel], m_vehicleRayCaster));
		body[BodyHandel]->setActivationState(DISABLE_DEACTIVATION);//無効化を拒否
		dynamicsWorld[WorldHandle]->addVehicle(vehicle[CarHandel]);

		btCollisionShape* m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

		body[BodyHandel]->setCenterOfMassTransform(btTransform::getIdentity());
		body[BodyHandel]->setAngularVelocity(btvDxv(rot));//車の角度を指定
		body[BodyHandel]->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btvDxv(pos)));//位置を指定
		body[BodyHandel]->setGravity(btvDxv(Gravity));//重力の設定

		dynamicsWorld[WorldHandle]->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(body[BodyHandel]->getBroadphaseHandle(), dynamicsWorld[WorldHandle]->getDispatcher());
	}

	if (vehicle[CarHandel])
	{
		vehicle[CarHandel]->resetSuspension();
		for (int i = 0; i < vehicle[CarHandel]->getNumWheels(); i++)
		{
			//synchronize the wheels with the (interpolated) chassis worldtransform
			vehicle[CarHandel]->updateWheelTransform(i, true);
		}
	}

	vehicle[CarHandel]->setCoordinateSystem(0, 1, 2);//座標系を選択します

	bool isFrontWheel = false;
	btVector3 connectionPointCS0(boxHalfExtents.x - (0.3*wheelWidth), connectionHeight, 2.1 * 10.f - wheelRadius);
	vehicle[CarHandel]->addWheel(connectionPointCS0, btvDxv(wheelDirectionCS0), btvDxv(wheelAxleCS), suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);


	connectionPointCS0 = btVector3(-boxHalfExtents.x + (0.3*wheelWidth), connectionHeight, 2.1 * 10.f - wheelRadius);
	vehicle[CarHandel]->addWheel(connectionPointCS0, btvDxv(wheelDirectionCS0), btvDxv(wheelAxleCS), suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);


	isFrontWheel = true;
	connectionPointCS0 = btVector3(-boxHalfExtents.x + (0.3*wheelWidth), connectionHeight, -2.5 * 10.f + wheelRadius);
	vehicle[CarHandel]->addWheel(connectionPointCS0, btvDxv(wheelDirectionCS0), btvDxv(wheelAxleCS), suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);


	connectionPointCS0 = btVector3(boxHalfExtents.x - (0.3*wheelWidth), connectionHeight, -2.5 * 10.f + wheelRadius);
	vehicle[CarHandel]->addWheel(connectionPointCS0, btvDxv(wheelDirectionCS0), btvDxv(wheelAxleCS), suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

	for (int i = 0; i < vehicle[CarHandel]->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehicle[CarHandel]->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}


#ifdef  B2D_DEBUG_DRAW
	// 箱のMotionStateの設定
	btDefaultMotionState *fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	m_wheelShape->calculateLocalInertia(1, btVector3(0, 0, 0));
	// 箱の初期情報を設定
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0, fallMotionState, m_wheelShape, btVector3(0, 0, 0));
	// 箱の剛体の作成
	en = new btRigidBody(fallRigidBodyCI);//ポインタ初期化↓このポインタに色々追加する
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCII(0, fallMotionState, m_wheelShape, btVector3(0, 0, 0));
	en2 = new btRigidBody(fallRigidBodyCII);
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCIII(0, fallMotionState, m_wheelShape, btVector3(0, 0, 0));
	en3 = new btRigidBody(fallRigidBodyCIII);
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCIIII(0, fallMotionState, m_wheelShape, btVector3(0, 0, 0));
	en4 = new btRigidBody(fallRigidBodyCIIII);
	// ワールドに箱の剛体を追加
	dynamicsWorld[WorldHandle]->addRigidBody(en, 1, 2);
	dynamicsWorld[WorldHandle]->addRigidBody(en2, 1, 2);
	dynamicsWorld[WorldHandle]->addRigidBody(en3, 1, 2);
	dynamicsWorld[WorldHandle]->addRigidBody(en4, 1, 2);
	en->setCollisionFlags(en->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	en->setActivationState(DISABLE_DEACTIVATION);
	en2->setCollisionFlags(en2->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	en2->setActivationState(DISABLE_DEACTIVATION);
	en3->setCollisionFlags(en3->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	en3->setActivationState(DISABLE_DEACTIVATION);
	en4->setCollisionFlags(en4->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	en4->setActivationState(DISABLE_DEACTIVATION);
#endif

	return CarHandel;
}

void Bullet_physics::controlCar(int CarHandel, float gEngineForce, float gBreakingForce, float gVehicleSteering)
{
	int wheelIndex = 0;
	vehicle[CarHandel]->applyEngineForce(gEngineForce, wheelIndex);
	vehicle[CarHandel]->setBrake(gBreakingForce, wheelIndex);
	vehicle[CarHandel]->setSteeringValue(0, wheelIndex);
	wheelIndex = 1;
	vehicle[CarHandel]->applyEngineForce(gEngineForce, wheelIndex);
	vehicle[CarHandel]->setBrake(gBreakingForce, wheelIndex);
	vehicle[CarHandel]->setSteeringValue(0, wheelIndex);


	wheelIndex = 2;
	vehicle[CarHandel]->setSteeringValue(gVehicleSteering, wheelIndex);
	wheelIndex = 3;
	vehicle[CarHandel]->setSteeringValue(gVehicleSteering, wheelIndex);

#ifdef  B2D_DEBUG_DRAW
	en->getMotionState()->setWorldTransform(vehicle[CarHandel]->getWheelInfo(0).m_worldTransform);
	en2->getMotionState()->setWorldTransform(vehicle[CarHandel]->getWheelInfo(1).m_worldTransform);
	en3->getMotionState()->setWorldTransform(vehicle[CarHandel]->getWheelInfo(2).m_worldTransform);
	en4->getMotionState()->setWorldTransform(vehicle[CarHandel]->getWheelInfo(3).m_worldTransform);
#endif
}

void Bullet_physics::controlWheel(int CarHandel, int CarModelHandel, int frameFL, int frameFR, int frameRL, int frameRR)
{
	int i;
	for (i = 0; i < vehicle[CarHandel]->getNumWheels(); i++)
	{
		vehicle[CarHandel]->updateWheelTransform(i, true);

		//btTransform w_trans = vehicle[CarHandel]->getWheelInfo(i).m_worldTransform;

		//VECTOR pos = btVGet(w_trans.getOrigin());

		btScalar Steering = vehicle[CarHandel]->getSteeringValue(i);//ステアリング

		btMatrix3x3 wrt = vehicle[CarHandel]->getWheelTransformWS(i).getBasis();
		btVector3 euler;
		wrt.getEulerZYX(euler[2], euler[1], euler[0]);

		//座標を得る→getOrigin
		//回転を得る→getBasis

		MATRIX Matrix;
		switch (i)
		{
		case 0:
			MV1ResetFrameUserLocalMatrix(CarModelHandel, frameRR);
			Matrix = MGetIdent();
			Matrix = MMult(MGetRotX(-btVGet(euler).x), MV1GetFrameLocalMatrix(CarModelHandel, frameRR));
			//Matrix = MMult(Matrix, MGetTranslate(pos));
			MV1SetFrameUserLocalMatrix(CarModelHandel, frameRR, Matrix);
			break;
		case 1:
			MV1ResetFrameUserLocalMatrix(CarModelHandel, frameRL);
			Matrix = MGetIdent();
			Matrix = MMult(MGetRotX(-btVGet(euler).x), MV1GetFrameLocalMatrix(CarModelHandel, frameRL));
			//Matrix = MMult(Matrix, MGetTranslate(pos));
			MV1SetFrameUserLocalMatrix(CarModelHandel, frameRL, Matrix);
			break;
		case 2:
			MV1ResetFrameUserLocalMatrix(CarModelHandel, frameFR);
			Matrix = MGetIdent();
			Matrix = MMult(MGetRotX(-btVGet(euler).x), MGetRotY(Steering));//Yを先に回さないとおかしくなるので注意
			//Matrix = MMult(Matrix, MGetTranslate(pos));
			MV1SetFrameUserLocalMatrix(CarModelHandel, frameFR, Matrix);
			break;
		case 3:
			MV1ResetFrameUserLocalMatrix(CarModelHandel, frameFL);
			Matrix = MGetIdent();
			Matrix = MMult(MGetRotX(-btVGet(euler).x), MGetRotY(Steering));
			//Matrix = MMult(Matrix, MGetTranslate(pos));
			MV1SetFrameUserLocalMatrix(CarModelHandel, frameFL, Matrix);
			break;
		}
	}
}

btRaycastVehicle* Bullet_physics::getbtRaycastVehicle(int CarHandel)
{
	return vehicle[CarHandel];
}

int Bullet_physics::createCharacter(int WorldHandle, VECTOR pos, float stepHeight, float characterHeight, float characterWidth)
{
	CharacterHandel++;

	btTransform startTransform;
	startTransform.setIdentity();
	//startTransform.setOrigin (btVector3(0.0, 4.0, 0.0));
	startTransform.setOrigin(btVector3(0.0, 0.0, 0.0));

	ghostObject.push_back(new btPairCachingGhostObject());
	ghostObject[CharacterHandel]->setWorldTransform(startTransform);

	//ソフトワールドの時
	if (UseSoftWorld)
	{
		SoftWorldBroadphase[WorldHandle]->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());//ここにこれがないと地面を突き抜けます
	}
	else
	{
		broadphase[WorldHandle]->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());//ここにこれがないと地面を突き抜けます
	}

	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	ghostObject[CharacterHandel]->setCollisionShape(capsule);
	btVector3 tes = btvDxv(VGet(0,100,0));
	ghostObject[CharacterHandel]->getCollisionShape()->calculateLocalInertia(btScalar(10.f), tes);
	//ghostObject[CharacterHandel]->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	//モノに当たっても動かなくなる設定↓
	ghostObject[CharacterHandel]->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
	//m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	character.push_back(new btKinematicCharacterController(ghostObject[CharacterHandel], capsule, stepHeight));

	//これを設定しないとおかしくなる
	character[CharacterHandel]->setGravity(btVector3(0, -9.8f, 0));

	//ソフトワールドの時
	if (UseSoftWorld)
	{
		softWorld[WorldHandle]->addCollisionObject(ghostObject[CharacterHandel], btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

		softWorld[WorldHandle]->addAction(character[CharacterHandel]);

		softWorld[WorldHandle]->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(ghostObject[CharacterHandel]->getBroadphaseHandle(), softWorld[WorldHandle]->getDispatcher());

		character[CharacterHandel]->reset(softWorld[WorldHandle]);
	}
	else
	{
		dynamicsWorld[WorldHandle]->addCollisionObject(ghostObject[CharacterHandel], btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

		dynamicsWorld[WorldHandle]->addAction(character[CharacterHandel]);

		dynamicsWorld[WorldHandle]->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(ghostObject[CharacterHandel]->getBroadphaseHandle(), dynamicsWorld[WorldHandle]->getDispatcher());

		character[CharacterHandel]->reset(dynamicsWorld[WorldHandle]);
	}
	///WTF
	character[CharacterHandel]->warp(btvDxv(pos));

	return CharacterHandel;
}

btVector3 Bullet_physics::controlCharacter(int CharacterHandel, int Left, int Right, int Forward, int Backward, int Jump, float walkspeed, float TurningSpeed)
{
	///set walkDirection for our character
	btTransform xform;
	xform = ghostObject[CharacterHandel]->getWorldTransform();

	btVector3 forwardDir = xform.getBasis()[2];
	//	printf("forwardDir=%f,%f,%f\n",forwardDir[0],forwardDir[1],forwardDir[2]);
	btVector3 upDir = xform.getBasis()[1];
	btVector3 strafeDir = xform.getBasis()[0];
	forwardDir.normalize();
	upDir.normalize();
	strafeDir.normalize();

	btVector3 updir = btVector3(0.0, 4.0, 0.0);

	btVector3 walkDirection;
	if (character[CharacterHandel]->onGround())//地面の上にいない時だけ重力をかける
	{
		walkDirection = btVector3(0.0, 0.0, 0.0);
	}
	else
	{
		//walkDirection = btVector3(0.0, -3.0, 0.0);
		walkDirection = btVector3(0.0, -0.2, 0.0);
	}

	btScalar walkVelocity = btScalar(walkspeed); // 4 km/h -> 1.1 m/s
	btScalar walkSpeed = walkVelocity;

	//rotate view
	if (Left)
	{
		btMatrix3x3 orn = ghostObject[CharacterHandel]->getWorldTransform().getBasis();
		orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), TurningSpeed));
		ghostObject[CharacterHandel]->getWorldTransform().setBasis(orn);
	}

	if (Right)
	{
		btMatrix3x3 orn = ghostObject[CharacterHandel]->getWorldTransform().getBasis();
		orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), -TurningSpeed));
		ghostObject[CharacterHandel]->getWorldTransform().setBasis(orn);
	}

	if (Forward)
		walkDirection += forwardDir;

	if (Backward)
		walkDirection -= forwardDir;

	if (Jump)
		walkDirection += updir;

	character[CharacterHandel]->setWalkDirection(walkDirection*walkSpeed);

#ifdef B2D_DEBUG_DRAW
	btManifoldArray	manifoldArray;
	btBroadphasePairArray& pairArray = m_ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
	int numPairs = pairArray.size();

	for (int i = 0; i < numPairs; i++)
	{
		manifoldArray.clear();

		const btBroadphasePair& pair = pairArray[i];

		btBroadphasePair* collisionPair = broadphase->getOverlappingPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
		if (!collisionPair)
			continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

		for (int j = 0; j < manifoldArray.size(); j++)
		{
			btPersistentManifold* manifold = manifoldArray[j];
			for (int p = 0; p < manifold->getNumContacts(); p++)
			{
				const btManifoldPoint&pt = manifold->getContactPoint(p);

				btVector3 color(255, 255, 255);
				dynamicsWorld->getDebugDrawer()->drawContactPoint(pt.getPositionWorldOnB(), pt.m_normalWorldOnB, pt.getDistance(), pt.getLifeTime(), color);
			}
}
	}
#endif

	return walkDirection * walkSpeed;
}

btPairCachingGhostObject* Bullet_physics::getbtGhostObject(int CaracterHandel)
{
	return ghostObject[CaracterHandel];
}

btKinematicCharacterController* Bullet_physics::getCharacterController(int CaracterHandel)
{
	return character[CaracterHandel];
}

int Bullet_physics::pretendCreateCharacter()
{
	CharacterHandel++;

	ghostObject.push_back(new btPairCachingGhostObject());

	character.push_back(new btKinematicCharacterController(ghostObject[CharacterHandel], 0, 0));

	return CharacterHandel;
}

void Bullet_physics::setCharacterPosRot(int CaracterHandel, VECTOR pos, btQuaternion &rot)
{

	ghostObject[CaracterHandel]->setWorldTransform(btTransform(rot, btVector3(pos.x, pos.y, pos.z)));
}

btQuaternion Bullet_physics::getCharacterRotRaw(int CaracterHandel)
{
	btQuaternion q = ghostObject[CaracterHandel]->getWorldTransform().getRotation();

	return btQuaternion(q.getX(), q.getY(), q.getZ(), q.getW());
}

VECTOR Bullet_physics::getCharacterPos(int CaracterHandel, VECTOR subtractVC)
{
	btVector3 mpos = ghostObject[CaracterHandel]->getWorldTransform().getOrigin();
	VECTOR Pos = btVGet(mpos);
	Pos = VGet(Pos.x - subtractVC.x, Pos.y - subtractVC.y, Pos.z - subtractVC.z);
	return Pos;
}

VECTOR Bullet_physics::getCharacterRot(int CaracterHandel)
{
	btMatrix3x3 rot = ghostObject[CaracterHandel]->getWorldTransform().getBasis();
	btVector3 euler;
	rot.getEulerZYX(euler[2], euler[1], euler[0]);
	VECTOR Rot = btVGet(euler);
	return Rot;
}

void Bullet_physics::setCharacterTurningSpeed(int CharacterHandel, FLOAT dir)
{
	btMatrix3x3 orn = ghostObject[CharacterHandel]->getWorldTransform().getBasis();
	orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), dir));
	ghostObject[CharacterHandel]->getWorldTransform().setBasis(orn);
}

void Bullet_physics::setCharacterDirection(int CharacterHandel,btVector3 dir)
{
	character[CharacterHandel]->setWalkDirection(dir);
}

int Bullet_physics::createSoftWorld(VECTOR WorldMin, VECTOR WorldMax, int maxProxies, VECTOR Gravity)
{
	SoftWorldHandel++;

	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher*	m_dispatcher;
	btConstraintSolver*	m_solver;


	m_collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	m_softBodyWorldInfo.m_dispatcher = m_dispatcher;

	// ワールドの広さ
	btVector3 worldAabbMin = btvDxv(WorldMin);
	btVector3 worldAabbMax = btvDxv(WorldMax);

	// broadphaseの作成（SAP法）
	SoftWorldBroadphase.push_back(new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies));

	m_softBodyWorldInfo.m_broadphase = SoftWorldBroadphase[SoftWorldHandel];

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	m_solver = solver;

	btSoftBodySolver* softBodySolver = 0;

	softWorld.push_back(new btSoftRigidDynamicsWorld(m_dispatcher, SoftWorldBroadphase[SoftWorldHandel], m_solver, m_collisionConfiguration, softBodySolver));
	//m_dynamicsWorld = world;
	//m_dynamicsWorld->setInternalTickCallback(pickingPreTickCallback, this, true);


	softWorld[SoftWorldHandel]->getDispatchInfo().m_enableSPU = true;
	softWorld[SoftWorldHandel]->setGravity(btvDxv(Gravity));
	m_softBodyWorldInfo.m_gravity.setValue(0, -10, 0);

	m_softBodyWorldInfo.m_sparsesdf.Initialize();

	m_softBodyWorldInfo.air_density = (btScalar)1.2;
	m_softBodyWorldInfo.water_density = 0;
	m_softBodyWorldInfo.water_offset = 0;
	m_softBodyWorldInfo.water_normal = btVector3(0, 0, 0);
	m_softBodyWorldInfo.m_gravity.setValue(0, -10, 0);

	//ソフトワールドに剛体を追加するフラグを有効化
	UseSoftWorld = 1;

	return SoftWorldHandel;
}

int Bullet_physics::createSoftBox(int SoftWorldHandle)
{
	SoftBodyHandel++;

	const btVector3	h = btVector3(10.0f, 10.0f, 10.0f);
	const btVector3	p = btVector3(10.0f, 10.0f, 10.0f);
	const btVector3	c[] = { p + h*btVector3(-1, -1, -1),
		p + h*btVector3(+1, -1, -1),
		p + h*btVector3(-1, +1, -1),
		p + h*btVector3(+1, +1, -1),
		p + h*btVector3(-1, -1, +1),
		p + h*btVector3(+1, -1, +1),
		p + h*btVector3(-1, +1, +1),
		p + h*btVector3(+1, +1, +1) };

	Softbody.push_back(new btSoftBody(&m_softBodyWorldInfo));//?

	Softbody[SoftBodyHandel] = btSoftBodyHelpers::CreateFromConvexHull(m_softBodyWorldInfo, c, 8);
	Softbody[SoftBodyHandel]->generateBendingConstraints(2);

	Softbody[SoftBodyHandel]->transform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(100, 0, 0)));

	softWorld[SoftWorldHandle]->addSoftBody(Softbody[SoftBodyHandel]);

	return SoftBodyHandel;
}

btSoftRigidDynamicsWorld* Bullet_physics::getbtSoftRigidDynamicsWorld(int SoftWorldHandel)
{
	return softWorld[SoftWorldHandel];
}

btSoftBody* Bullet_physics::getbtSoftBody(int SoftBodyHandel)
{
	return Softbody[SoftBodyHandel];
}

void Bullet_physics::useSoftWorld(bool Flag)
{
	UseSoftWorld = Flag;
}

int Bullet_physics::createSoftSphere(int SoftWorldHandle)
{
	SoftBodyHandel++;

	//TRACEDEMO
	Softbody.push_back(new btSoftBody(&m_softBodyWorldInfo));//?
	Softbody[SoftBodyHandel] = btSoftBodyHelpers::CreateEllipsoid(m_softBodyWorldInfo, btVector3(35, 25, 0),
		btVector3(1, 1, 1) * 3,
		512);
	Softbody[SoftBodyHandel]->m_materials[0]->m_kLST = 0.45;
	Softbody[SoftBodyHandel]->m_cfg.kVC = 20;
	Softbody[SoftBodyHandel]->setTotalMass(50, true);
	Softbody[SoftBodyHandel]->setPose(true, false);
	softWorld[SoftWorldHandle]->addSoftBody(Softbody[SoftBodyHandel]);

	return SoftBodyHandel;
}

int Bullet_physics::createSoftBodyConvexHull(int ModelHandel, int SoftWorldHandle)
{
	SoftBodyHandel++;

	// モデル全体の参照用メッシュを構築
	MV1SetupReferenceMesh(ModelHandel, -1, TRUE);

	// 参照用メッシュ情報の取得
	MV1_REF_POLYGONLIST RefPoly = MV1GetReferenceMesh(ModelHandel, -1, TRUE);

	std::vector< btVector3 > vertices;//頂点

	for (int i = 0; i < RefPoly.PolygonNum; i++)
	{
		vertices.push_back(btVector3(RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[0]].Position.x,
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[0]].Position.y, RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[0]].Position.z));

		vertices.push_back(btVector3(RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[1]].Position.x,
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[1]].Position.y, RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[1]].Position.z));

		vertices.push_back(btVector3(RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[2]].Position.x,
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[2]].Position.y, RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[2]].Position.z));

	}

	//生成
	Softbody.push_back(new btSoftBody(&m_softBodyWorldInfo));//?

	//CreateFromTriMeshではなくCreateFromConvexHullにしたら出来ました												↓わからないから適当に
	Softbody[SoftBodyHandel] = btSoftBodyHelpers::CreateFromConvexHull(m_softBodyWorldInfo, vertices.data(), RefPoly.PolygonNum * 2);//凸型

	Softbody[SoftBodyHandel]->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;//他のソフトボディとの衝突を有効化

	Softbody[SoftBodyHandel]->setTotalMass(1, true);

	Softbody[SoftBodyHandel]->setPose(true, true);
	Softbody[SoftBodyHandel]->m_cfg.kMT = 0.1;

	Softbody[SoftBodyHandel]->transform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 100)));//初期位置の設定 

	softWorld[SoftWorldHandle]->addSoftBody(Softbody[SoftBodyHandel]);

	return SoftBodyHandel;
}

void Bullet_physics::addAnchor(int SoftBodyHandel, int node, int RigidBodyHandel, bool InvalidCollision, float influence)
{
	Softbody[SoftBodyHandel]->appendAnchor(node, body[RigidBodyHandel], InvalidCollision, influence);
}