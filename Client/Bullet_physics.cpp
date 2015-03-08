#include "Bullet_physics.h"

//Bullet_physics.h���Q�Ƃ��Ă�������

Bullet_physics::Bullet_physics()
{
}


Bullet_physics::~Bullet_physics()
{
}


int Bullet_physics::createWorld(VECTOR WorldMin, VECTOR WorldMax, int maxProxies, VECTOR Gravity)
{
	WorldHandel++;

	// ���[���h�̍L��
	btVector3 worldAabbMin = btvDxv(WorldMin);
	btVector3 worldAabbMax = btvDxv(WorldMax);

	// broadphase�̍쐬�iSAP�@�j
	broadphase.push_back(new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies));
	
	// �f�t�H���g�̏Փːݒ�ƃf�B�X�p�b�`���̍쐬
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// �Փˉ����\���o
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	//std::vector�Œǉ�
	dynamicsWorld.push_back(new btDiscreteDynamicsWorld(dispatcher, broadphase[WorldHandel], solver, collisionConfiguration));

	// �d�͂̐ݒ�
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

	//���f���f�[�^����n�ʂ̍쐬
	// �Q�Ɨp���b�V���̍\�z
	MV1SetupReferenceMesh(ModelHandel, -1, TRUE);

	// �Q�Ɨp���b�V���̎擾
	MV1_REF_POLYGONLIST RefMesh = MV1GetReferenceMesh(ModelHandel, -1, TRUE);

	//�n�ʂ̃��b�V������g���C�A���O�����X�g�쐬
	btTriangleMesh* VartexGround = new btTriangleMesh();

	// �|���S���̐������J��Ԃ�
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

	// �n�ʂ̏Փˌ`��̍쐬
	btBvhTriangleMeshShape* groundShape = new btBvhTriangleMeshShape(VartexGround, true, true);

	// �n�ʂ�MotionState�̐ݒ�
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	// �n�ʂ̏�������ݒ�
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI( 0, groundMotionState, groundShape, btVector3(0, 0, 0));
	
	// �n�ʂ̍��̂̍쐬
	body.push_back(new btRigidBody(groundRigidBodyCI));

	if (UseSoftWorld)
	{
		//�\�t�g���[���h�ɒǉ�
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ���[���h�ɒn�ʂ̍��̂�ǉ�
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

	// ��
	btCollisionShape* boxshape = new btBoxShape(btvDxv(boxHalfExtents));

	// ����MotionState�̐ݒ�
	btDefaultMotionState* moptionstate = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.y, rot.z, 1), btvDxv(pos)));

	// ���ʁA�����̐ݒ�
	boxshape->calculateLocalInertia(btScalar(mass), btvDxv(inertia));//inertia�@����

	// ���̏�������ݒ�
	btRigidBody::btRigidBodyConstructionInfo screenshapebodyCI(mass, moptionstate, boxshape, btvDxv(inertia));

	// ���̍��̂̍쐬
	body.push_back(new btRigidBody(screenshapebodyCI));

	if (UseSoftWorld)
	{
		//�\�t�g���[���h�ɒǉ�
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ���[���h�ɔ��̍��̂�ǉ�
		dynamicsWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	return BodyHandel;
}

VECTOR Bullet_physics::getBodyPos(int BodyHandel, VECTOR subtractVC)
{
	btTransform trans;
	body[BodyHandel]->getMotionState()->getWorldTransform(trans);

	// �ʒu�擾
	VECTOR pos = btVGet(trans.getOrigin());
	pos = VGet(pos.x - subtractVC.x, pos.y - subtractVC.y, pos.z - subtractVC.z);
	return pos;
}

VECTOR Bullet_physics::getBodyRot(int BodyHandel)
{
	btTransform trans;
	body[BodyHandel]->getMotionState()->getWorldTransform(trans);

	// �p�x�擾
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
		softWorld[WorldHandle]->stepSimulation(1 / fps, 10);
	}
	else
	{
		dynamicsWorld[WorldHandle]->stepSimulation(1 / fps, 10);
	}
}

int Bullet_physics::ceateSphereBody(int WorldHandle, float radius, VECTOR pos, VECTOR rot, float mass, VECTOR inertia)
{
	BodyHandel++;

	//��
	btCollisionShape* sphereshape = new btSphereShape(radius);
	// ����MotionState�̐ݒ�
	btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.y, rot.z, 1), btvDxv(pos)));
	// ���ʁA�����̐ݒ�
	sphereshape->calculateLocalInertia(mass, btvDxv(inertia));
	// ���̏�������ݒ�
	btRigidBody::btRigidBodyConstructionInfo sphereshapebodyCI(mass, motionstate, sphereshape, btvDxv(inertia));
	// ���̍��̂̍쐬
	body.push_back(new btRigidBody(sphereshapebodyCI));

	if (UseSoftWorld)
	{
		//�\�t�g���[���h�ɒǉ�
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ���[���h�ɔ��̍��̂�ǉ�
		dynamicsWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}

	return BodyHandel;
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

	//���̍쐬
	//createBox�ł͂ł��Ȃ��̂ŕ������Ȃ�-------

	// ���̏Փˌ`��̍쐬							�����S����
	btCollisionShape* boxShape = new btBoxShape(btvDxv(boxHalfExtents));
	// ����MotionState�̐ݒ�
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btVector3 fallInertia(0, 0, 0);
	boxShape->calculateLocalInertia( mass, fallInertia );
	// ���̏�������ݒ�
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI3( mass, fallMotionState, boxShape, fallInertia);
	// ���̍��̂̍쐬
	body.push_back(new btRigidBody(fallRigidBodyCI3));

	if (UseSoftWorld)
	{
		//�\�t�g���[���h�ɒǉ�
		softWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	else
	{
		// ���[���h�ɔ��̍��̂�ǉ�
		dynamicsWorld[WorldHandle]->addRigidBody(body[BodyHandel]);
	}
	*CarBodyHandel = BodyHandel;
	//--------------------------------------------

	//�ȈՂ�Bullet�ԗ����f��
	btRaycastVehicle::btVehicleTuning	m_tuning;

	//�\�t�g���[���h�̎�
	if (UseSoftWorld)
	{
		btVehicleRaycaster* m_vehicleRayCaster = new btDefaultVehicleRaycaster(softWorld[WorldHandle]);
		vehicle.push_back(new btRaycastVehicle(m_tuning, body[BodyHandel], m_vehicleRayCaster));
		body[BodyHandel]->setActivationState(DISABLE_DEACTIVATION);//������������
		softWorld[WorldHandle]->addVehicle(vehicle[CarHandel]);

		btCollisionShape* m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

		body[BodyHandel]->setCenterOfMassTransform(btTransform::getIdentity());
		body[BodyHandel]->setAngularVelocity(btvDxv(rot));//�Ԃ̊p�x���w��
		body[BodyHandel]->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btvDxv(pos)));//�ʒu���w��
		body[BodyHandel]->setGravity(btvDxv(Gravity));//�d�͂̐ݒ�

		softWorld[WorldHandle]->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(body[BodyHandel]->getBroadphaseHandle(), softWorld[WorldHandle]->getDispatcher());
	}
	else
	{
		//�ȈՂ�Bullet�ԗ����f��
		btVehicleRaycaster* m_vehicleRayCaster = new btDefaultVehicleRaycaster(dynamicsWorld[WorldHandle]);
		vehicle.push_back(new btRaycastVehicle(m_tuning, body[BodyHandel], m_vehicleRayCaster));
		body[BodyHandel]->setActivationState(DISABLE_DEACTIVATION);//������������
		dynamicsWorld[WorldHandle]->addVehicle(vehicle[CarHandel]);

		btCollisionShape* m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

		body[BodyHandel]->setCenterOfMassTransform(btTransform::getIdentity());
		body[BodyHandel]->setAngularVelocity(btvDxv(rot));//�Ԃ̊p�x���w��
		body[BodyHandel]->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btvDxv(pos)));//�ʒu���w��
		body[BodyHandel]->setGravity(btvDxv(Gravity));//�d�͂̐ݒ�

		dynamicsWorld[WorldHandle]->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(body[BodyHandel]->getBroadphaseHandle(), dynamicsWorld[WorldHandle]->getDispatcher());
	}

	if (vehicle[CarHandel])
	{
		vehicle[CarHandel]->resetSuspension();
		for (int i = 0; i<vehicle[CarHandel]->getNumWheels(); i++)
		{
			//synchronize the wheels with the (interpolated) chassis worldtransform
			vehicle[CarHandel]->updateWheelTransform(i, true);
		}
	}

	vehicle[CarHandel]->setCoordinateSystem(0, 1, 2);//���W�n��I�����܂�

	bool isFrontWheel = false;
	btVector3 connectionPointCS0  ( boxHalfExtents.x - (0.3*wheelWidth), connectionHeight, 2.1 * 10.f - wheelRadius);
	vehicle[CarHandel]->addWheel(connectionPointCS0, btvDxv(wheelDirectionCS0), btvDxv(wheelAxleCS), suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);


	connectionPointCS0 = btVector3( -boxHalfExtents.x + (0.3*wheelWidth), connectionHeight, 2.1 * 10.f - wheelRadius);
	vehicle[CarHandel]->addWheel(connectionPointCS0, btvDxv(wheelDirectionCS0), btvDxv(wheelAxleCS), suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);


	isFrontWheel = true;
	connectionPointCS0 = btVector3( -boxHalfExtents.x + (0.3*wheelWidth), connectionHeight, -2.5 * 10.f + wheelRadius);
	vehicle[CarHandel]->addWheel(connectionPointCS0, btvDxv(wheelDirectionCS0), btvDxv(wheelAxleCS), suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);


	connectionPointCS0 = btVector3( boxHalfExtents.x - (0.3*wheelWidth), connectionHeight, -2.5 * 10.f + wheelRadius);
	vehicle[CarHandel]->addWheel(connectionPointCS0, btvDxv(wheelDirectionCS0), btvDxv(wheelAxleCS), suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

	for (int i = 0; i<vehicle[CarHandel]->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehicle[CarHandel]->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}


#ifdef  B2D_DEBUG_DRAW
	// ����MotionState�̐ݒ�
	btDefaultMotionState *fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	m_wheelShape->calculateLocalInertia(1, btVector3(0, 0, 0));
	// ���̏�������ݒ�
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0, fallMotionState, m_wheelShape, btVector3(0, 0, 0));
	// ���̍��̂̍쐬
	en = new btRigidBody(fallRigidBodyCI);//�|�C���^�����������̃|�C���^�ɐF�X�ǉ�����
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCII(0, fallMotionState, m_wheelShape, btVector3(0,0,0));
	en2 = new btRigidBody(fallRigidBodyCII);
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCIII(0, fallMotionState, m_wheelShape, btVector3(0, 0, 0));
	en3 = new btRigidBody(fallRigidBodyCIII);
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCIIII(0, fallMotionState, m_wheelShape, btVector3(0, 0, 0));
	en4 = new btRigidBody(fallRigidBodyCIIII);
	// ���[���h�ɔ��̍��̂�ǉ�
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
	for (i = 0; i<vehicle[CarHandel]->getNumWheels(); i++)
	{
		vehicle[CarHandel]->updateWheelTransform(i, true);

		//btTransform w_trans = vehicle[CarHandel]->getWheelInfo(i).m_worldTransform;

		//VECTOR pos = btVGet(w_trans.getOrigin());

		btScalar Steering = vehicle[CarHandel]->getSteeringValue(i);//�X�e�A�����O

		btMatrix3x3 wrt = vehicle[CarHandel]->getWheelTransformWS(i).getBasis();
		btVector3 euler;
		wrt.getEulerZYX(euler[2], euler[1], euler[0]);

		//���W�𓾂遨getOrigin
		//��]�𓾂遨getBasis

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
			Matrix = MMult(MGetRotX(-btVGet(euler).x), MGetRotY(Steering));//Y���ɉ񂳂Ȃ��Ƃ��������Ȃ�̂Œ���
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

	//�\�t�g���[���h�̎�
	if (UseSoftWorld)
	{
		SoftWorldBroadphase[WorldHandle]->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());//�����ɂ��ꂪ�Ȃ��ƒn�ʂ�˂������܂�
	}
	else
	{
		broadphase[WorldHandle]->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());//�����ɂ��ꂪ�Ȃ��ƒn�ʂ�˂������܂�
	}

	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	ghostObject[CharacterHandel]->setCollisionShape(capsule);
	ghostObject[CharacterHandel]->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	//m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	character.push_back(new btKinematicCharacterController(ghostObject[CharacterHandel], capsule, stepHeight));

	//�\�t�g���[���h�̎�
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

void Bullet_physics::controlCharacter(int CharacterHandel, int Left, int Right, int Forward, int Backward, int Jump , float walkspeed)
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
	if (character[CharacterHandel]->onGround())//�n�ʂ̏�ɂ��Ȃ��������d�͂�������
	{
		walkDirection = btVector3(0.0, -0.00001, 0.0);
	}
	else
	{
		walkDirection = btVector3(0.0, -3.0, 0.0);
	}

	btScalar walkVelocity = btScalar(walkspeed); // 4 km/h -> 1.1 m/s
	btScalar walkSpeed = walkVelocity;

	//rotate view
	if (Left)
	{
		btMatrix3x3 orn = ghostObject[CharacterHandel]->getWorldTransform().getBasis();
		orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), 0.03));
		ghostObject[CharacterHandel]->getWorldTransform().setBasis(orn);
	}

	if (Right)
	{
		btMatrix3x3 orn = ghostObject[CharacterHandel]->getWorldTransform().getBasis();
		orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), -0.03));
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

	for (int i = 0; i<numPairs; i++)
	{
		manifoldArray.clear();

		const btBroadphasePair& pair = pairArray[i];

		btBroadphasePair* collisionPair = broadphase->getOverlappingPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
		if (!collisionPair)
			continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

		for (int j = 0; j<manifoldArray.size(); j++)
		{
			btPersistentManifold* manifold = manifoldArray[j];
			for (int p = 0; p<manifold->getNumContacts(); p++)
			{
				const btManifoldPoint&pt = manifold->getContactPoint(p);

				btVector3 color(255, 255, 255);
				dynamicsWorld->getDebugDrawer()->drawContactPoint(pt.getPositionWorldOnB(), pt.m_normalWorldOnB, pt.getDistance(), pt.getLifeTime(), color);
			}
		}
	}
#endif
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

	character.push_back(new btKinematicCharacterController(ghostObject[CharacterHandel], 0,0));

	return CharacterHandel;
}

void Bullet_physics::setCharacterPosRot(int CaracterHandel, VECTOR pos, btQuaternion &rot)
{

	ghostObject[CaracterHandel]->setWorldTransform(btTransform(rot, btVector3(pos.x, pos.y, pos.z)));
}

btQuaternion Bullet_physics::getCharacterRotRaw(int CaracterHandel)
{
	btQuaternion q = ghostObject[CaracterHandel]->getWorldTransform().getRotation();

	return btQuaternion(q.getX(), q.getY(), q.getZ(),q.getW());
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

int Bullet_physics::createSoftWorld(VECTOR WorldMin, VECTOR WorldMax, int maxProxies, VECTOR Gravity)
{
	SoftWorldHandel++;

	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher*	m_dispatcher;
	btConstraintSolver*	m_solver;


	m_collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	m_softBodyWorldInfo.m_dispatcher = m_dispatcher;

	// ���[���h�̍L��
	btVector3 worldAabbMin = btvDxv(WorldMin);
	btVector3 worldAabbMax = btvDxv(WorldMax);

	// broadphase�̍쐬�iSAP�@�j
	SoftWorldBroadphase.push_back(new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies));

	m_softBodyWorldInfo.m_broadphase = SoftWorldBroadphase[SoftWorldHandel];

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	m_solver = solver;

	btSoftBodySolver* softBodySolver = 0;

	softWorld.push_back( new btSoftRigidDynamicsWorld(m_dispatcher, SoftWorldBroadphase[SoftWorldHandel], m_solver, m_collisionConfiguration, softBodySolver));
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

	//�\�t�g���[���h�ɍ��̂�ǉ�����t���O��L����
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

	// ���f���S�̂̎Q�Ɨp���b�V�����\�z
	MV1SetupReferenceMesh(ModelHandel, -1, TRUE);

	// �Q�Ɨp���b�V�����̎擾
	MV1_REF_POLYGONLIST RefPoly = MV1GetReferenceMesh(ModelHandel, -1, TRUE);

	std::vector< btVector3 > vertices;//���_

	for (int i = 0; i < RefPoly.PolygonNum; i++)
	{
		vertices.push_back(btVector3(RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[0]].Position.x,
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[0]].Position.y, RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[0]].Position.z));

		vertices.push_back(btVector3(RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[1]].Position.x,
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[1]].Position.y, RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[1]].Position.z));

		vertices.push_back(btVector3(RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[2]].Position.x,
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[2]].Position.y, RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[2]].Position.z));

	}

	//����
	Softbody.push_back(new btSoftBody(&m_softBodyWorldInfo));//?

	//CreateFromTriMesh�ł͂Ȃ�CreateFromConvexHull�ɂ�����o���܂���												���킩��Ȃ�����K����
	Softbody[SoftBodyHandel] = btSoftBodyHelpers::CreateFromConvexHull(m_softBodyWorldInfo, vertices.data(), RefPoly.PolygonNum * 2);//�ʌ^

	Softbody[SoftBodyHandel]->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;//���̃\�t�g�{�f�B�Ƃ̏Փ˂�L����

	Softbody[SoftBodyHandel]->setTotalMass(1, true);

	Softbody[SoftBodyHandel]->setPose(true, true);
	Softbody[SoftBodyHandel]->m_cfg.kMT = 0.1;

	Softbody[SoftBodyHandel]->transform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 100)));//�����ʒu�̐ݒ� 

	softWorld[SoftWorldHandle]->addSoftBody(Softbody[SoftBodyHandel]);

	return SoftBodyHandel;
}

int Bullet_physics::createSoftBodyFromTriMesh(char *FileName, int SoftWorldHandle, VECTOR Scale, VECTOR Pos, VECTOR Rot, float mass)
{
	SoftBodyHandel++;

	// 3D�t�@�C���ǂݍ���
	rxPolygons poly;
	RxModel::Read(FileName, poly);

	int vertex_count = (int)poly.vertices.size(); // �����_��
	int index_count = (int)poly.faces.size(); // ���|���S����
	btScalar *vertices = new btScalar[vertex_count * 3]; // ���_���W���i�[����z��
	int *indices = new int[index_count * 3]; // �|���S�����\�����钸�_�ԍ����i�[����z��
	// ���_���W�̎��o��
	for (int i = 0; i < vertex_count; ++i){
		vertices[3 * i] = poly.vertices[i][0];
		vertices[3 * i + 1] = poly.vertices[i][1];
		vertices[3 * i + 2] = poly.vertices[i][2];
	}
	// �|���S�����\�����钸�_�ԍ��̎��o��
	for (int i = 0; i < index_count; ++i){
		indices[3 * i] = poly.faces[i][0];
		indices[3 * i + 1] = poly.faces[i][1];
		indices[3 * i + 2] = poly.faces[i][2];
	}

	// �O�p���b�V������SoftBody���쐬�ibtSoftBodyHelpers���g�p�j
	//obj�t�@�C�����炵������Ɏ��s�ł��܂���
	Softbody.push_back(new btSoftBody(&m_softBodyWorldInfo));
	Softbody[SoftBodyHandel] = btSoftBodyHelpers::CreateFromTriMesh(m_softBodyWorldInfo, vertices, indices, index_count, false);
	Softbody[SoftBodyHandel]->getCollisionShape()->setMargin(0.01);

	Softbody[SoftBodyHandel]->scale(btVector3(Scale.x, Scale.y, Scale.z));

	//�߂��̒��_��A������
	Softbody[SoftBodyHandel]->generateBendingConstraints(10.f);

	//�Փ˃N���X�^��L����
	Softbody[SoftBodyHandel]->generateClusters(index_count);
	Softbody[SoftBodyHandel]->m_cfg.collisions = btSoftBody::fCollision::CL_SS +
		btSoftBody::fCollision::CL_RS;

	//�����_���ɑI�񂾒��_�Ԃ��S���A�S�̂̌`���ۂ�
	//randomizeConstraints�������face(�O�p�`�|���S��)�̏��Ԃ��ς���Ă��܂��̂Œ��ӂ���
	//Softbox->randomizeConstraints();

	Softbody[SoftBodyHandel]->setPose(1, 1);
	Softbody[SoftBodyHandel]->m_cfg.kMT = 0.5;

	//�ʒu
	Softbody[SoftBodyHandel]->transform(btTransform(btQuaternion(Rot.x, Rot.y, Rot.z, 1), btVector3(Pos.x, Pos.y, Pos.z)));

	//����
	Softbody[SoftBodyHandel]->setTotalMass(mass);

	Softbody[SoftBodyHandel]->m_cfg.kDP = 0.5;//�_���p�[

	softWorld[SoftWorldHandle]->addSoftBody(Softbody[SoftBodyHandel]);

	return SoftBodyHandel;
}

void Bullet_physics::addAnchor(int SoftBodyHandel ,int node, int RigidBodyHandel, bool InvalidCollision, float influence)
{
	Softbody[SoftBodyHandel]->appendAnchor(node, body[RigidBodyHandel], InvalidCollision, influence);
}