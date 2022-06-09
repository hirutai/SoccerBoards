#include "GamePlay.h"
#include "SceneManager.h"
#include "Input.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Stadium.h"
#include "Goal.h"
#include "SoccerBall.h"
#include "Result.h"
#include "Title.h"

using namespace std;
using namespace DirectX;

void GamePlay::Initialize()
{
#pragma region �e�N�X�`���ǂݍ���
	TextureManager* texManager = TextureManager::GetInstance();

	texManager->LoadTexture(DebugFont, L"Resources/debugfont.png");
	texManager->LoadTexture(SOCCERBALL, L"Resources/Sprite/soccerball.png");
	texManager->LoadTexture(MyPlayer, L"Resources/Sprite/red.png");
	texManager->LoadTexture(CpuPlayer, L"Resources/Sprite/blue.png");
	texManager->LoadTexture(Pitch, L"Resources/Sprite/pitch.png");
	texManager->LoadTexture(Wall, L"Resources/Sprite/green.png");
	texManager->LoadTexture(Net, L"Resources/Sprite/net.png");
	texManager->LoadTexture(BackGround, L"Resources/Sprite/background.png");
	texManager->LoadTexture(KickOff, L"Resources/Sprite/kickoff.png");
	texManager->LoadTexture(Score, L"Resources/Sprite/score.png");
	texManager->LoadTexture(GOAL, L"Resources/Sprite/goal!.png");
#pragma endregion

#pragma region �X�v���C�g����
	// �L�b�N�I�t�摜
	kickoff = make_unique<Sprite>();
	kickoff->Initialize(KickOff);
	kickoff->SetAnchorPoint(vertCenter); // ���_�𒆐S�ɐݒ�
	kickoff->SetPosition(kickoffPos.x, kickoffPos.y); // ��ʉE�ɔz�u

	// �X�R�A�p�摜
	// �����X�R�A
	myScoreNumber = make_unique<Sprite>();
	myScoreNumber->Initialize(Score);
	myScoreNumber->SetAnchorPoint(vertLeftCenter); // ���_�����Ӓ����ɐݒ�
	myScoreNumber->SetPosition(myScorePos.x, myScorePos.y);
	myScoreNumber->SetRect(myScoreOrigin.x, myScoreOrigin.y, myScoreSize.x, myScoreSize.y);

	// ����X�R�A
	cpuScoreNumber = make_unique<Sprite>();
	cpuScoreNumber->Initialize(Score);
	cpuScoreNumber->SetAnchorPoint(vertRightCenter); // ���_�����Ӓ����ɐݒ�
	cpuScoreNumber->SetPosition(cpuScorePos.x, cpuScorePos.y);
	cpuScoreNumber->SetRect(cpuScoreOrigin.x, cpuScoreOrigin.y, cpuScoreSize.x, cpuScoreSize.y);

	// �S�[���摜
	goalCha = make_unique<Sprite>();
	goalCha->Initialize(GOAL);
	goalCha->SetAnchorPoint(vertCenter); // ���_�𒆐S�ɐݒ�
	goalCha->SetPosition(screenCenter.x, screenCenter.y); // ��ʒ����ɔz�u
	goalCha->SetSize(goalChaSize.x, goalChaSize.y);

	// �w�i���
	backGround = make_unique<Sprite>();
	backGround->Initialize(BackGround);
#pragma endregion

#pragma region 3D���f������
	// �����t�B�[���h�v���C���[
	modelMyBody = make_unique<Obj3dModel>();
	modelMyBody->Initialize("player", MyPlayer);

	// �����S�[���L�[�p�[
	modelMyKeaper = make_unique<Obj3dModel>();
	modelMyKeaper->Initialize("player", MyPlayer);

	// ����t�B�[���h�v���C���[
	modelCpuBody = make_unique<Obj3dModel>();
	modelCpuBody->Initialize("player", CpuPlayer);

	// ����S�[���L�[�p�[
	modelCpuKeaper = make_unique<Obj3dModel>();
	modelCpuKeaper->Initialize("player", CpuPlayer);
#pragma endregion

#pragma region 3D�I�u�W�F�N�g����
	for (int i = 0; i < PLAYERNUM; i++)
	{
		// �����t�B�[���h�v���C���[�i�́j
		objectMyBody[i] = make_unique<Obj3dObject>();
		objectMyBody[i]->Initialize(modelMyBody.get());

		// ����t�B�[���h�v���C���[�i�́j
		objectCpuBody[i] = make_unique<Obj3dObject>();
		objectCpuBody[i]->Initialize(modelCpuBody.get());
	}

	// �����S�[���L�[�p�[
	objectMyKeaper = make_unique<Obj3dObject>();
	objectMyKeaper->Initialize(modelMyKeaper.get());

	// ����S�[���L�[�p�[
	objectCpuKeaper = make_unique<Obj3dObject>();
	objectCpuKeaper->Initialize(modelCpuKeaper.get());
#pragma endregion

	// �X�^�W�A��
	stadium = new Stadium();
	stadium->Initialize(Pitch, Wall); // ������

	// �����S�[��
	myGoal = new Goal();
	myGoal->Initialzie("mygoal", Net);
	myGoal->SetPosition(XMFLOAT3(-4, 0, 0));

	// ����S�[��
	cpuGoal = new Goal();
	cpuGoal->Initialzie("cpugoal", Net);
	cpuGoal->SetPosition(XMFLOAT3(4, 0, 0));

	// �T�b�J�[�{�[��
	soccerBall = new SoccerBall();
	soccerBall->Initialize("ball", SOCCERBALL);

#pragma region ���̑�������
	// �������E�̏�����
	PhysicsInit();

	// �����I�u�W�F�N�g�쐬
	// �n��
	CreateGround();
	// ��
	CreateWall();
	// �S�[����
	CreateGoalWall();
	// �V��
	CreateCeiling();
	// �{�[��
	CreateBall();
	// �����t�B�[���h�v���C���[�i�̂Ƒ��j
	CreateMyPlayer();
	// �����S�[���L�[�p�[
	CreateMyKeaper();
	// ����t�B�[���h�v���C���[
	CreateCpuPlayer();
	// ����S�[���L�[�p�[
	CreateCpuKeaper();

	ResetGame();
#pragma endregion
}

void GamePlay::Update()
{
	Input* input = Input::GetInstance();

	switch (scene)
	{
	case Scene::CameraRotate: // �J������]
		if (cameraEye.x > VERTICALEYE)
		{
			cameraEye.x -= cameraRotation; // �c�����ɂȂ�܂ŉ�]
		}
		else
		{
			cameraEye.x = VERTICALEYE; // �c�����ɌŒ�
			scene = Scene::KickOffDirecting; // �L�b�N�I�t���o��
		}

		Camera::GetInstance()->SetEye(XMFLOAT3(cameraEye.x, 30.0f, VERTICALEYE - cameraEye.x));

		Camera::GetInstance()->SetTarget(XMFLOAT3(cameraEye.x / 4, 0.0f, 0.0f));

		PhysicsUpdate();

		ObjectUpdate();
		break;
	case Scene::KickOffDirecting: // �L�b�N�I�t���o�^�C�~���O

		if (kickoffPos.x > WindowsApi::windowWidth / 2) // �L�b�N�I�t�̕�������ʒ����Ɏ~�܂�܂�
		{
			kickoffPos.x -= KICKOFFVEL;
		}
		else
		{
			stopTime++; // �����Ɏ~�܂��Ă��鎞�Ԃ��J�E���g

			if (stopTime > TWOSECONDS) // �Q�b�o�߂�����
			{
				if (kickoffPos.x > LEFTEND) // �L�b�N�I�t�̕��������[�ɓ��B����܂�
				{
					kickoffPos.x -= KICKOFFVEL;
				}
				else
				{
					stopTime = 0;
					scene = Scene::Game; // �Q�[����
				}
			}
		}

		kickoff->SetPosition(kickoffPos.x, kickoffPos.y);

		break;
	case Scene::Game: // �Q�[��

		for (int i = 0; i < PLAYERNUM; i++)
		{
			cpuPlayerVel[i].setValue(pVelSwitching, 0.0f, 0.0f);

			if (pSwitchingVelTime >= TWOSECONDS * 5.0f) // ��莞�Ԍo������
			{
				pSwitchingVelTime = 0.0f; // ���Ԃ̃��Z�b�g
				pVelSwitching *= -1; // �ړ��ʐ؂�ւ�
			}
			else
			{
				pSwitchingVelTime++;
			}
		}

		cpuKeaperVel.setValue(0.0f, 0.0f, kVelSwitching);

		if (kSwitchingTime >= TWOSECONDS * 5.0f)
		{
			kSwitchingTime = 0.0f;
			kVelSwitching *= -1;
		}
		else
		{
			kSwitchingTime++;
		}

		// �������E�̍X�V
		PhysicsUpdate();

		ObjectUpdate();

		XMFLOAT3 ballPos = soccerBall->GetPosition();

		if (ballPos.x > cpuGoalPos.x) // ����S�[���Ƀ{�[������������
		{
			scene = Scene::MyGoal; // �����S�[�����V�[����
		}
		if (ballPos.x < myGoalPos.x) // �����S�[���Ƀ{�[������������
		{
			scene = Scene::CpuGoal; // ����S�[�����V�[����
		}

		break;
	case Scene::MyGoal: // �����S�[����
		// �����̃S�[��
		MyGoal();

		ObjectUpdate();

		scene = Scene::MyGoalDirecting; // �S�[�����o��

		break;
	case Scene::MyGoalDirecting: // �����S�[�����o�^�C�~���O
		if (goalChaMag >= ORIGINALSIZE) // ���̃T�C�Y�Ɋg�傳���܂�
		{
			//goalChaMag = 0.1f;

			stopTime++; // �����Ɏ~�܂��Ă��鎞�Ԃ��J�E���g

			if (stopTime > TWOSECONDS) // �Q�b�o�߂�����
			{
				stopTime = 0;

				scene = Scene::ResetStatus; // ���Z�b�g
			}
		}
		else
		{
			goalChaMag += EXPRATE; // �S�[���I�̕������g��
		}

		goalChaSize = XMFLOAT2(GOALCHARWIDTH * goalChaMag, GOALCHARHEIGHT * goalChaMag);
		goalCha->SetSize(goalChaSize.x, goalChaSize.y);

		break;
	case Scene::CpuGoal: // ����S�[����
		// ����̃S�[��
		CpuGoal();

		ObjectUpdate();

		scene = Scene::CpuGoalDirecting; // �S�[�����o��

		break;
	case Scene::CpuGoalDirecting: // ����S�[�����o�^�C�~���O
		if (goalChaMag >= ORIGINALSIZE) // ���̃T�C�Y�Ɋg�傳���܂�
		{
			//goalChaMag = 0.1f;

			stopTime++; // �����Ɏ~�܂��Ă��鎞�Ԃ��J�E���g

			if (stopTime > TWOSECONDS) // �Q�b�o�߂�����
			{
				stopTime = 0;

				scene = Scene::ResetStatus; // ���Z�b�g
			}
		}
		else
		{
			goalChaMag += EXPRATE; // �S�[���I�̕������g��
		}

		goalChaSize = XMFLOAT2(GOALCHARWIDTH * goalChaMag, GOALCHARHEIGHT * goalChaMag);
		goalCha->SetSize(goalChaSize.x, goalChaSize.y);

		break;
	case Scene::ResetStatus: // �Q�[�����̃��Z�b�g
		ResetStatus();

		if (myScore == 3 || cpuScore == 3)
		{
			// �^�C�g����
			BaseScene* scene = new Title();
			sceneManager->SetNextScene(scene);
		}

		scene = Scene::KickOffDirecting; // ���X�^�[�g

		break;
	}
}

void GamePlay::DrawBackSprite()
{
#pragma region �w�i�`��
	Sprite::SetPipeline();

	backGround->Draw(); // �`��
#pragma endregion
}

void GamePlay::DrawObject()
{
#pragma region 3D�I�u�W�F�N�g�`��
	// �X�^�W�A��
	stadium->Draw();

	// �����S�[��
	myGoal->Draw();
	// ����S�[��
	cpuGoal->Draw();

	// �����t�B�[���h�v���C���[
	for (int i = 0; i < PLAYERNUM; i++)
	{
		// ��
		objectMyBody[i]->Draw();
	}

	// ����t�B�[���h�v���C���[
	for (int i = 0; i < PLAYERNUM; i++)
	{
		// ��
		objectCpuBody[i]->Draw();
	}

	// �����S�[���L�[�p�[
	objectMyKeaper->Draw();
	// ����S�[���L�[�p�[
	objectCpuKeaper->Draw();

	// �{�[��
	soccerBall->Draw();
#pragma endregion
}

void GamePlay::DrawFrontSprite()
{
#pragma region �X�v���C�g�`��
	// �X�v���C�g�`��O����
	Sprite::SetPipeline();

	switch (scene)
	{
	case Scene::KickOffDirecting:
		myScoreNumber->Draw();
		cpuScoreNumber->Draw();
		kickoff->Draw();

		break;
	case Scene::Game:
		myScoreNumber->Draw();
		cpuScoreNumber->Draw();

		break;
	case Scene::MyGoalDirecting:
		myScoreNumber->Draw();
		cpuScoreNumber->Draw();
		goalCha->Draw();

		break;
	case Scene::CpuGoalDirecting:
		myScoreNumber->Draw();
		cpuScoreNumber->Draw();
		goalCha->Draw();

		break;
	}
#pragma endregion
}

void GamePlay::Finalize()
{
#pragma region �����֘A�̉��
	// ���̂̍폜
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}

		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	// �Փˌ`��̍폜
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	collisionShapes.clear();

	// �������E�̍폜
	delete dynamicsWorld;

	// �\���o�̍폜
	delete solver;

	// �u���[�h�t�F�[�Y�̍폜
	delete overlappingPairCache;

	// �Փ˃f�B�X�p�b�`���[�̍폜
	delete dispatcher;

	// �Փˍ\���̍폜
	delete collisionConfiguration;
#pragma endregion

	// �X�^�W�A���̍폜
	delete stadium;
}

void GamePlay::PhysicsInit()
{
	// �Փˍ\��
	collisionConfiguration = new btDefaultCollisionConfiguration();

	// �Փ˃f�B�X�p�b�`���[
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// �u���[�h�t�F�[�Y
	overlappingPairCache = new btDbvtBroadphase();

	// ����\���o
	solver = new btSequentialImpulseConstraintSolver();

	// �������E�쐬
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}

void GamePlay::CreateGround()
{
	// �Փˌ`��̐ݒ�
	cShapeGround = new btBoxShape(btVector3(btScalar(pitchSize.x), btScalar(pitchSize.y), btScalar(pitchSize.z)));
	// �Փˌ`��z��ɒǉ�
	collisionShapes.push_back(cShapeGround);

	btTransform startTransform; // �������W�p
	startTransform.setIdentity();

	btScalar mass(0.); // ����

	btVector3 localInertia(0, 0, 0); // ����

	// �������W��ݒ�
	startTransform.setOrigin(btVector3(0, 0, 0));

	// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeGround->calculateLocalInertia(mass, localInertia);
	}

	// �ړ����
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// ���̍쐬�ɕK�v�ȏ��
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeGround, localInertia);

	// ���̍쐬
	rBodyGround = new btRigidBody(rbInfo);
	rBodyGround->setFriction(0.1f);

	// �������E�ɍ��̂�o�^
	dynamicsWorld->addRigidBody(rBodyGround);
}

void GamePlay::CreateWall()
{
	for (int i = 0; i < lrWALLNUM; i++)
	{
		btScalar posZ = 12.75 - 12.75 * (i + i); // ���E�ǂ�Z���W i=0��12.75(�S�[�����̕�) i=1��-12.75(�S�[����O�̕�)

		btScalar mass(0.); // ����
		bool isDynamic = (mass != 0.f);

		// ���̕�
		{
			float lWallPos = -24; // �e���ǂ̍��W

			// �Փˌ`��̐ݒ�
			cShapeLWall[i] = new btBoxShape(btVector3(btScalar(lrWallSize.x), btScalar(lrWallSize.y), btScalar(lrWallSize.z)));
			// �Փˌ`��z��ɒǉ�
			collisionShapes.push_back(cShapeLWall[i]);

			btTransform startTransform; // �������W�p
			startTransform.setIdentity();

			btVector3 localInertia(0, 0, 0); // ����

			// �������W��ݒ�
			startTransform.setOrigin(btVector3(lWallPos, lrWallSize.y, posZ));

			// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
			if (isDynamic)
			{
				cShapeLWall[i]->calculateLocalInertia(mass, localInertia);
			}

			// �ړ����
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// ���̍쐬�ɕK�v�ȏ��
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeLWall[i], localInertia);

			// ���̍쐬
			rBodyLWall[i] = new btRigidBody(rbInfo);

			// �������E�ɍ��̂�o�^
			dynamicsWorld->addRigidBody(rBodyLWall[i]);
		}

		// �E�̕�
		{
			float rWallPos = 24; // �e�E�ǂ̍��W

			// �Փˌ`��̐ݒ�
			cShapeRWall[i] = new btBoxShape(btVector3(btScalar(lrWallSize.x), btScalar(lrWallSize.y), btScalar(lrWallSize.z)));
			// �Փˌ`��z��ɒǉ�
			collisionShapes.push_back(cShapeRWall[i]);

			btTransform startTransform; // �������W�p
			startTransform.setIdentity();

			btVector3 localInertia(0, 0, 0); // ����

			// �������W��ݒ�
			startTransform.setOrigin(btVector3(rWallPos, lrWallSize.y, posZ));

			// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
			if (isDynamic)
			{
				cShapeRWall[i]->calculateLocalInertia(mass, localInertia);
			}

			// �ړ����
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// ���̍쐬�ɕK�v�ȏ��
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeRWall[i], localInertia);

			// ���̍쐬
			rBodyRWall[i] = new btRigidBody(rbInfo);

			// �������E�ɍ��̂�o�^
			dynamicsWorld->addRigidBody(rBodyRWall[i]);
		}

		// �㉺�̕�
		{
			float bWallStartPos = 18; // ���ǂ̍��W

			// �Փˌ`��̐ݒ�
			cShapeFBWall[i] = new btBoxShape(btVector3(btScalar(fbWallSize.x), btScalar(fbWallSize.y), btScalar(fbWallSize.z)));
			// �Փˌ`��z��ɒǉ�
			collisionShapes.push_back(cShapeFBWall[i]);

			btTransform startTransform; // �������W�p
			startTransform.setIdentity();

			btVector3 localInertia(0, 0, 0); // ����

			// i = 0 �O�ǂ�z���W i = 1 ���ǂ�z���W
			btScalar posZ = bWallStartPos - bWallStartPos * (i + i);

			// �������W��ݒ�
			startTransform.setOrigin(btVector3(0, fbWallSize.y, posZ));

			// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
			if (isDynamic)
			{
				cShapeFBWall[i]->calculateLocalInertia(mass, localInertia);
			}

			// �ړ����
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// ���̍쐬�ɕK�v�ȏ��
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeFBWall[i], localInertia);

			// ���̍쐬
			rBodyFBWall[i] = new btRigidBody(rbInfo);

			// �������E�ɍ��̂�o�^
			dynamicsWorld->addRigidBody(rBodyFBWall[i]);
		}
	}
}

void GamePlay::CreateGoalWall()
{
	for (int i = 0; i < 2; i++)
	{
		// �Փˌ`��̐ݒ�
		cShapeGoalWall[i] = new btBoxShape(btVector3(btScalar(0.1f), btScalar(6.0f), btScalar(7.25f)));
		// �Փˌ`��z��ɒǉ�
		collisionShapes.push_back(cShapeGoalWall[i]);

		btTransform startTransform; // �������W�p
		startTransform.setIdentity();

		btScalar mass(0.); // ����

		btVector3 localInertia(0, 0, 0); // ����

		btScalar posX = -24.0f + 48.0f * i;

		// �������W��ݒ�
		startTransform.setOrigin(btVector3(posX, 12.0f, 0.0f));

		bool isDynamic = (mass != 0.f);

		// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
		if (isDynamic)
		{
			cShapeGoalWall[i]->calculateLocalInertia(mass, localInertia);
		}

		// �ړ����
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		// ���̍쐬�ɕK�v�ȏ��
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeGoalWall[i], localInertia);

		// ���̍쐬
		rBodyGoalWall[i] = new btRigidBody(rbInfo);

		// �������E�ɍ��̂�o�^
		dynamicsWorld->addRigidBody(rBodyGoalWall[i]);
	}
}

void GamePlay::CreateCeiling()
{
	// �Փˌ`��̐ݒ�
	cShapeCeiling = new btBoxShape(btVector3(btScalar(pitchSize.x), btScalar(pitchSize.y), btScalar(pitchSize.z)));
	// �Փˌ`��z��ɒǉ�
	collisionShapes.push_back(cShapeCeiling);

	btTransform startTransform; // �������W�p
	startTransform.setIdentity();

	btScalar mass(0.); // ����

	btVector3 localInertia(0, 0, 0); // ����

	// �������W��ݒ�
	startTransform.setOrigin(btVector3(0, 12.0f, 0));

	// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeCeiling->calculateLocalInertia(mass, localInertia);
	}

	// �ړ����
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// ���̍쐬�ɕK�v�ȏ��
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeCeiling, localInertia);

	// ���̍쐬
	rBodyCeiling = new btRigidBody(rbInfo);

	// �������E�ɍ��̂�o�^
	dynamicsWorld->addRigidBody(rBodyCeiling);
}

void GamePlay::CreateBall()
{
	// �Փˌ`��̐ݒ�
	cShapeBall = new btSphereShape(btScalar(ballRadius));

	// �Փˌ`��z��ɒǉ�
	collisionShapes.push_back(cShapeBall);

	btTransform startTransform;	// �������W�p
	startTransform.setIdentity();

	btScalar mass(1.f); // ����

	btVector3 localInertia(0, 0, 0); // ����

	// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeBall->calculateLocalInertia(mass, localInertia);
	}

	// �������W��ݒ�
	startTransform.setOrigin(ballStartPos);

	// �ړ����
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// ���̍쐬�ɕK�v�ȏ��
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeBall, localInertia);

	// ���̍쐬
	rBodyBall = new btRigidBody(rbInfo);

	// ���C�̐ݒ�
	rBodyBall->setRollingFriction(0.5f);
	rBodyBall->setSpinningFriction(0.5f);
	rBodyBall->setFriction(0.5f);
	rBodyBall->setAnisotropicFriction(cShapeBall->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

	// �������E�ɍ��̂�o�^
	dynamicsWorld->addRigidBody(rBodyBall);
}

void GamePlay::CreateMyPlayer()
{
	for (int i = 0; i < PLAYERNUM; i++)
	{
		btScalar posZ = L1STARTPOS - PLAYERINTERVAL * i;

		// �e�I��̑̂̏������W��ݒ�
		myBodyStartPos[i] = btVector3(mL1MINPOS, BodyHeight, posZ); // �P���
		// �e�I��̑��̏������W��ݒ�
		myFootStartPos[i] = btVector3(mL1MINPOS, footHeight, posZ); // �P���
	}

	for (int i = 0; i < PLAYERNUM; i++)
	{
		// ��
		{
			// �Փˌ`��̐ݒ�
			cShapeMyBody[i] = new btCapsuleShape(btScalar(bodySize.x), btScalar(bodySize.y));
			// �Փˌ`��z��ɒǉ�
			collisionShapes.push_back(cShapeMyBody[i]);

			btTransform startTransform; // �������W�p
			startTransform.setIdentity();

			btScalar mass(0.f); // ����

			btVector3 localInertia(0, 0, 0); // ����

			// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
			{
				cShapeMyBody[i]->calculateLocalInertia(mass, localInertia);
			}

			// �������W��ݒ�
			startTransform.setOrigin(myBodyStartPos[i]);

			// �ړ����
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// ���̍쐬�ɕK�v�ȏ��
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeMyBody[i], localInertia);

			// ���̍쐬
			rBodyMyBody[i] = new btRigidBody(rbInfo);

			rBodyMyBody[i]->setCollisionFlags(rBodyMyBody[i]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rBodyMyBody[i]->setActivationState(DISABLE_DEACTIVATION);

			// �������E�ɍ��̂�o�^
			dynamicsWorld->addRigidBody(rBodyMyBody[i]);
		}

		// ��
		{
			// �Փˌ`��̐ݒ�
			cShapeMyFoot[i] = new btBoxShape(btVector3(btScalar(footSize.x), btScalar(footSize.y), btScalar(footSize.z)));
			// �Փˌ`��z��ɒǉ�
			collisionShapes.push_back(cShapeMyFoot[i]);

			btTransform startTransform; // �������W�p
			startTransform.setIdentity();

			btScalar mass(0.f); // ����

			btVector3 localInertia(0, 0, 0); // ����

			// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
			{
				cShapeMyFoot[i]->calculateLocalInertia(mass, localInertia);
			}

			// �������W��ݒ�
			startTransform.setOrigin(myFootStartPos[i]);

			// �ړ����
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// ���̍쐬�ɕK�v�ȏ��
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeMyFoot[i], localInertia);

			// ���̍쐬
			rBodyMyFoot[i] = new btRigidBody(rbInfo);

			rBodyMyFoot[i]->setCollisionFlags(rBodyMyFoot[i]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rBodyMyFoot[i]->setActivationState(DISABLE_DEACTIVATION);

			// �������E�ɍ��̂�o�^
			dynamicsWorld->addRigidBody(rBodyMyFoot[i]);
		}
	}
}

void GamePlay::CreateMyKeaper()
{
	// �Փˌ`��̐ݒ�
	cShapeMyKeaper = new btCapsuleShape(btScalar(bodySize.x), btScalar(bodySize.y));
	// �Փˌ`��z��ɒǉ�
	collisionShapes.push_back(cShapeMyKeaper);

	btTransform startTransform; // �������W�p
	startTransform.setIdentity();

	btScalar mass(0.f); // ����

	btVector3 localInertia(0, 0, 0); // ����

	// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeMyKeaper->calculateLocalInertia(mass, localInertia);
	}

	// �������W��ݒ�
	startTransform.setOrigin(myKeaperStartPos);

	// �ړ����
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// ���̍쐬�ɕK�v�ȏ��
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeMyKeaper, localInertia);

	// ���̍쐬
	rBodyMyKeaper = new btRigidBody(rbInfo);

	rBodyMyKeaper->setCollisionFlags(rBodyMyKeaper->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	rBodyMyKeaper->setActivationState(DISABLE_DEACTIVATION);

	// �������E�ɍ��̂�o�^
	dynamicsWorld->addRigidBody(rBodyMyKeaper);
}

void GamePlay::CreateCpuPlayer()
{
	for (int i = 0; i < PLAYERNUM; i++)
	{
		btScalar posZ = L1STARTPOS - PLAYERINTERVAL * i;

		// �e�I��̏������W��ݒ�
		cpuBodyStartPos[i] = btVector3(cL1MINPOS, BodyHeight, posZ); // �P���
		// �e�I��̑��̏������W��ݒ�
		cpuFootStartPos[i] = btVector3(cL1MINPOS, footHeight, posZ); // �P���
	}

	for (int i = 0; i < PLAYERNUM; i++)
	{
		// ��
		{
			// �Փˌ`��̐ݒ�
			cShapeCpuBody[i] = new btCapsuleShape(btScalar(bodySize.x), btScalar(bodySize.y));
			// �Փˌ`��z��ɒǉ�
			collisionShapes.push_back(cShapeCpuBody[i]);

			btTransform startTransform; // �������W�p
			startTransform.setIdentity();

			btScalar mass(0.f); // ����

			btVector3 localInertia(0, 0, 0); // ����

			// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
			{
				cShapeCpuBody[i]->calculateLocalInertia(mass, localInertia);
			}

			// �������W��ݒ�
			startTransform.setOrigin(cpuBodyStartPos[i]);

			// �ړ����
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// ���̍쐬�ɕK�v�ȏ��
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeCpuBody[i], localInertia);

			// ���̍쐬
			rBodyCpuBody[i] = new btRigidBody(rbInfo);

			rBodyCpuBody[i]->setCollisionFlags(rBodyCpuBody[i]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rBodyCpuBody[i]->setActivationState(DISABLE_DEACTIVATION);

			// �������E�ɍ��̂�o�^
			dynamicsWorld->addRigidBody(rBodyCpuBody[i]);
		}

		// ��
		{
			// �Փˌ`��̐ݒ�
			cShapeCpuFoot[i] = new btBoxShape(btVector3(btScalar(footSize.x), btScalar(footSize.y), btScalar(footSize.z)));
			// �Փˌ`��z��ɒǉ�
			collisionShapes.push_back(cShapeCpuFoot[i]);

			btTransform startTransform; // �������W�p
			startTransform.setIdentity();

			btScalar mass(0.f); // ����

			btVector3 localInertia(0, 0, 0); // ����

			// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
			{
				cShapeCpuFoot[i]->calculateLocalInertia(mass, localInertia);
			}

			// �������W��ݒ�
			startTransform.setOrigin(cpuFootStartPos[i]);

			// �ړ����
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// ���̍쐬�ɕK�v�ȏ��
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeCpuFoot[i], localInertia);

			// ���̍쐬
			rBodyCpuFoot[i] = new btRigidBody(rbInfo);

			rBodyCpuFoot[i]->setCollisionFlags(rBodyCpuFoot[i]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rBodyCpuFoot[i]->setActivationState(DISABLE_DEACTIVATION);

			// �������E�ɍ��̂�o�^
			dynamicsWorld->addRigidBody(rBodyCpuFoot[i]);
		}
	}
}

void GamePlay::CreateCpuKeaper()
{
	// �Փˌ`��̐ݒ�
	cShapeCpuKeaper = new btCapsuleShape(btScalar(bodySize.x), btScalar(bodySize.y));
	// �Փˌ`��z��ɒǉ�
	collisionShapes.push_back(cShapeCpuKeaper);

	btTransform startTransform; // �������W�p
	startTransform.setIdentity();

	btScalar mass(0.f); // ����

	btVector3 localInertia(0, 0, 0); // ����

	// ���ʂ��O�ȊO�Ȃ瓮�I�iTRUE�j�A����ȊO�͐ÓI�iFALSE�j
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeCpuKeaper->calculateLocalInertia(mass, localInertia);
	}

	// �������W��ݒ�
	startTransform.setOrigin(cpuKeaperStartPos);

	// �ړ����
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// ���̍쐬�ɕK�v�ȏ��
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeCpuKeaper, localInertia);

	// ���̍쐬
	rBodyCpuKeaper = new btRigidBody(rbInfo);

	rBodyCpuKeaper->setCollisionFlags(rBodyCpuKeaper->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	rBodyCpuKeaper->setActivationState(DISABLE_DEACTIVATION);

	// �������E�ɍ��̂�o�^
	dynamicsWorld->addRigidBody(rBodyCpuKeaper);
}

void GamePlay::PhysicsUpdate()
{
	Input* input = Input::GetInstance();

	// �������E�̍X�V
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);

#pragma region �{�[���̍X�V
	{
		btTransform trans; // �{�[���̏�Ԋi�[�p

		if (rBodyBall && rBodyBall->getMotionState()) // �{�[���̏�Ԃ��擾
		{
			rBodyBall->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = rBodyBall->getWorldTransform();
		}

		// �`��I�u�W�F�N�g�֘A
		// �{�[���̍��W�̎擾
		btVector3 transPos = trans.getOrigin();
		// XMFLOAT3�^�ɕϊ�
		XMFLOAT3 physicsPos = XMFLOAT3(transPos.getX(), transPos.getY(), transPos.getZ());
		// ���W�̐ݒ�
		soccerBall->SetPosition(physicsPos);

		// ��]�ʂ̎擾
		btQuaternion transRot = trans.getRotation();
		// �I�C���[�p�̎擾
		btScalar rollX, pitchY, yawZ;
		transRot.getEulerZYX(yawZ, pitchY, rollX);
		// �x�ɕϊ�
		btVector3 convertRot = btVector3(XMConvertToDegrees((float)rollX), XMConvertToDegrees((float)pitchY), XMConvertToDegrees((float)yawZ));
		// XMFLOAT3�^�ɕϊ�
		XMFLOAT3 physicsRot = XMFLOAT3(convertRot.getX(), convertRot.getY(), convertRot.getZ());
		// �p�x�̐ݒ�
		soccerBall->SetAngle(physicsRot);
	}
#pragma endregion

#pragma region �����t�B�[���h�v���C���[�̍X�V
	{
		btTransform trans;  // �̂̏�Ԋi�[�p
		btTransform trans2; // ���̏�Ԋi�[�p

		for (int i = 0; i < PLAYERNUM; i++)
		{
			// �����I�u�W�F�N�g�̏�Ԃ��擾
			if (rBodyMyBody[i] && rBodyMyBody[i]->getMotionState()) // �I��̏�Ԃ��擾
			{
				rBodyMyBody[i]->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = rBodyMyBody[i]->getWorldTransform();
			}

			if (rBodyMyFoot[i] && rBodyMyFoot[i]->getMotionState()) // �I��̏�Ԃ��擾
			{
				rBodyMyFoot[i]->getMotionState()->getWorldTransform(trans2);
			}
			else
			{
				trans2 = rBodyMyFoot[i]->getWorldTransform();
			}

			// �ړ�
			myPlayerVel[i].setValue(0, 0, 0);

			if (input->PushKey(DIK_W)) // ���ֈړ�
			{
				myPlayerVel[i].setX(myPlayerSpeVal);
			}
			else if (input->PushKey(DIK_S)) // �O�ֈړ�
			{
				myPlayerVel[i].setX(-myPlayerSpeVal);
			}

			// �́A���̍��W�̍X�V
			trans.setOrigin(trans.getOrigin() + myPlayerVel[i]);
			trans2.setOrigin(trans2.getOrigin() + myPlayerVel[i]);

			bool cond1 = myPlayerStatus == PlayerStatus::Normal; // �ʏ펞
			bool cond2 = myPlayerStatus == PlayerStatus::LeftRotating; // ����]��
			bool cond3 = myPlayerStatus == PlayerStatus::RightRotating; // �E��]��

			if (cond1) // ����]���ĂȂ���
			{
				if (input->TriggerKey(DIK_Q))
				{
					myPlayerStatus = PlayerStatus::LeftRotating; // ����]�J�n
				}
			}
			else if (cond2)
			{
				if (myPlayerRotationValue < -CIRCLELENGTH)
				{
					myPlayerRotationValue = 0;
					myPlayerStatus = PlayerStatus::Normal;
				}
				else
				{
					myPlayerRotationValue -= myPlayerRotVal;
				}
			}

			if (cond1) // �E��]���ĂȂ���
			{
				if (input->TriggerKey(DIK_E)) // �E��]
				{
					myPlayerStatus = PlayerStatus::RightRotating; // �E��]�J�n
				}
			}
			else if (cond3)
			{
				if (myPlayerRotationValue > CIRCLELENGTH)
				{
					myPlayerRotationValue = 0;
					myPlayerStatus = PlayerStatus::Normal;
				}
				else
				{
					myPlayerRotationValue += myPlayerRotVal;
				}
			}

			myPlayerRot[i].setX(myPlayerRotationValue);

			// �́A���̊p�x�̍X�V
			btQuaternion transRot = trans.getRotation();

			btQuaternion q = btQuaternion(myPlayerRot[i].getX(), myPlayerRot[i].getY(), myPlayerRot[i].getZ());
			trans.setRotation(q);

			btQuaternion trans2Rot = trans2.getRotation();

			trans2.setRotation(q);

			// �ړ�����
			const btScalar& bx = trans.getOrigin().getX();
			const btScalar& by = trans.getOrigin().getY();
			const btScalar& bz = trans.getOrigin().getZ();

			const btScalar& fx = trans2.getOrigin().getX();
			const btScalar& fy = trans2.getOrigin().getY();
			const btScalar& fz = trans2.getOrigin().getZ();
			switch (i)
			{
			case 0: // �P���
				// �̂ɂ���
				if (bx < mL1MINPOS)
				{
					trans.setOrigin(btVector3(mL1MINPOS, by, bz));
				}
				else if (bx > mL1MAXPOS)
				{
					trans.setOrigin(btVector3(mL1MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < mL1MINPOS)
				{
					trans2.setOrigin(btVector3(mL1MINPOS, fy, fz));
				}
				else if (fx > mL1MAXPOS)
				{
					trans2.setOrigin(btVector3(mL1MAXPOS, fy, fz));
				}

				break;
			case 1: // �Q���
				if (bx < mL2MINPOS)
				{
					trans.setOrigin(btVector3(mL2MINPOS, by, bz));
				}
				else if (bx > mL2MAXPOS)
				{
					trans.setOrigin(btVector3(mL2MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < mL2MINPOS)
				{
					trans2.setOrigin(btVector3(mL2MINPOS, fy, fz));
				}
				else if (fx > mL2MAXPOS)
				{
					trans2.setOrigin(btVector3(mL2MAXPOS, fy, fz));
				}

				break;
			case 2: // �R���
				if (bx < mL3MINPOS)
				{
					trans.setOrigin(btVector3(mL3MINPOS, by, bz));
				}
				else if (bx > mL3MAXPOS)
				{
					trans.setOrigin(btVector3(mL3MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < mL3MINPOS)
				{
					trans2.setOrigin(btVector3(mL3MINPOS, fy, fz));
				}
				else if (fx > mL3MAXPOS)
				{
					trans2.setOrigin(btVector3(mL3MAXPOS, fy, fz));
				}

				break;
			case 3: // �S���
				if (bx < mL4MINPOS)
				{
					trans.setOrigin(btVector3(mL4MINPOS, by, bz));
				}
				else if (bx > mL4MAXPOS)
				{
					trans.setOrigin(btVector3(mL4MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < mL1MINPOS)
				{
					trans2.setOrigin(btVector3(mL4MINPOS, fy, fz));
				}
				else if (fx > mL4MAXPOS)
				{
					trans2.setOrigin(btVector3(mL4MAXPOS, fy, fz));
				}

				break;
			default: // �T���
				if (bx < mL5MINPOS)
				{
					trans.setOrigin(btVector3(mL5MINPOS, by, bz));
				}
				else if (bx > mL5MAXPOS)
				{
					trans.setOrigin(btVector3(mL5MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < mL5MINPOS)
				{
					trans2.setOrigin(btVector3(mL5MINPOS, fy, fz));
				}
				else if (fx > mL5MAXPOS)
				{
					trans2.setOrigin(btVector3(mL5MAXPOS, fy, fz));
				}

				break;
			}

			rBodyMyBody[i]->getMotionState()->setWorldTransform(trans);
			rBodyMyFoot[i]->getMotionState()->setWorldTransform(trans2);

			// �̊֘A
			{
				// �`��I�u�W�F�N�g�֘A
				// ���W�̃Z�b�g
				btVector3 physicsPos = trans.getOrigin();
				SetPosition(myPlayerPos[i], physicsPos);

				// �p�x�̃Z�b�g
				btQuaternion physicsRot = trans.getRotation();

				btScalar rollX, pitchY, yawZ;

				physicsRot.getEulerZYX(yawZ, pitchY, rollX);

				btVector3 rot = btVector3(XMConvertToDegrees((float)rollX), XMConvertToDegrees((float)pitchY), XMConvertToDegrees((float)yawZ));

				SetRotation(myPlayerAng[i], rot);
			}
		}
	}
#pragma endregion

#pragma region �����L�[�p�[�̍X�V
	{
		btTransform trans;  // �L�[�p�[�̏�Ԋi�[�p

		// �����I�u�W�F�N�g�֘A
		if (rBodyMyKeaper && rBodyMyKeaper->getMotionState()) // �I��̏�Ԃ��擾
		{
			rBodyMyKeaper->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = rBodyMyKeaper->getWorldTransform();
		}

		// �ړ�
		myKeaperVel.setValue(0, 0, 0);

		if (input->PushKey(DIK_A))
		{
			myKeaperVel.setZ(myKeaperSpeVal);
		}
		else if (input->PushKey(DIK_D))
		{
			myKeaperVel.setZ(-myKeaperSpeVal);
		}

		// ���W�̍X�V
		trans.setOrigin(trans.getOrigin() + myKeaperVel);

		// �ړ�����
		if (trans.getOrigin().getZ() < mKMINPOS)
		{
			trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), mKMINPOS));
		}
		else if (trans.getOrigin().getZ() > mKMAXPOS)
		{
			trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), mKMAXPOS));
		}

		rBodyMyKeaper->getMotionState()->setWorldTransform(trans);

		// �`��I�u�W�F�N�g�֘A
		// ���W�̃Z�b�g
		btVector3 physicsPos = trans.getOrigin();
		SetPosition(myKeaperPos, physicsPos);
	}
#pragma endregion

#pragma region ����t�B�[���h�v���C���[�̍X�V
	{
		btTransform trans;  // �̂̏�Ԋi�[�p
		btTransform trans2; // ���̏�Ԋi�[�p

		for (int i = 0; i < PLAYERNUM; i++)
		{
			if (rBodyCpuBody[i] && rBodyCpuBody[i]->getMotionState()) // �I��̏�Ԃ��擾
			{
				rBodyCpuBody[i]->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = rBodyCpuBody[i]->getWorldTransform();
			}

			if (rBodyCpuFoot[i] && rBodyCpuFoot[i]->getMotionState()) // �I��̏�Ԃ��擾
			{
				rBodyCpuFoot[i]->getMotionState()->getWorldTransform(trans2);
			}
			else
			{
				trans2 = rBodyCpuFoot[i]->getWorldTransform();
			}

			//cpuPlayerVel[i].setValue(pVelSwitching, 0.0f, 0.0f);

			//if (pSwitchingVelTime >= TWOSECONDS * 5.0f) // ��莞�Ԍo������
			//{
			//	pSwitchingVelTime = 0.0f; // ���Ԃ̃��Z�b�g
			//	pVelSwitching *= -1; // �ړ��ʐ؂�ւ�
			//}
			//else
			//{
			//	pSwitchingVelTime++;
			//}

			// �́A���̍��W�̍X�V
			trans.setOrigin(trans.getOrigin() + cpuPlayerVel[i]);
			trans2.setOrigin(trans2.getOrigin() + cpuPlayerVel[i]);

			// �́A���̊p�x�̍X�V
			btQuaternion transRot = trans.getRotation();

			btQuaternion q = btQuaternion(cpuPlayerRot[i].getX(), cpuPlayerRot[i].getY(), cpuPlayerRot[i].getZ());
			trans.setRotation(q);

			btQuaternion trans2Rot = trans2.getRotation();

			trans2.setRotation(q);

			// �ړ�����
			const btScalar& bx = trans.getOrigin().getX();
			const btScalar& by = trans.getOrigin().getY();
			const btScalar& bz = trans.getOrigin().getZ();

			const btScalar& fx = trans2.getOrigin().getX();
			const btScalar& fy = trans2.getOrigin().getY();
			const btScalar& fz = trans2.getOrigin().getZ();
			switch (i)
			{
			case 0: // �P���
				// �̂ɂ���
				if (bx < cL1MINPOS)
				{
					trans.setOrigin(btVector3(cL1MINPOS, by, bz));
				}
				else if (bx > cL1MAXPOS)
				{
					trans.setOrigin(btVector3(cL1MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < cL1MINPOS)
				{
					trans2.setOrigin(btVector3(cL1MINPOS, fy, fz));
				}
				else if (fx > cL1MAXPOS)
				{
					trans2.setOrigin(btVector3(cL1MAXPOS, fy, fz));
				}

				break;
			case 1: // �Q���
				if (bx < cL2MINPOS)
				{
					trans.setOrigin(btVector3(cL2MINPOS, by, bz));
				}
				else if (bx > cL2MAXPOS)
				{
					trans.setOrigin(btVector3(cL2MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < cL2MINPOS)
				{
					trans2.setOrigin(btVector3(cL2MINPOS, fy, fz));
				}
				else if (fx > cL2MAXPOS)
				{
					trans2.setOrigin(btVector3(cL2MAXPOS, fy, fz));
				}

				break;
			case 2: // �R���
				if (bx < cL3MINPOS)
				{
					trans.setOrigin(btVector3(cL3MINPOS, by, bz));
				}
				else if (bx > cL3MAXPOS)
				{
					trans.setOrigin(btVector3(cL3MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < cL3MINPOS)
				{
					trans2.setOrigin(btVector3(cL3MINPOS, fy, fz));
				}
				else if (fx > cL3MAXPOS)
				{
					trans2.setOrigin(btVector3(cL3MAXPOS, fy, fz));
				}

				break;
			case 3: // �S���
				if (bx < cL4MINPOS)
				{
					trans.setOrigin(btVector3(cL4MINPOS, by, bz));
				}
				else if (bx > cL4MAXPOS)
				{
					trans.setOrigin(btVector3(cL4MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < cL1MINPOS)
				{
					trans2.setOrigin(btVector3(cL4MINPOS, fy, fz));
				}
				else if (fx > cL4MAXPOS)
				{
					trans2.setOrigin(btVector3(cL4MAXPOS, fy, fz));
				}

				break;
			default: // �T���
				if (bx < cL5MINPOS)
				{
					trans.setOrigin(btVector3(cL5MINPOS, by, bz));
				}
				else if (bx > cL5MAXPOS)
				{
					trans.setOrigin(btVector3(cL5MAXPOS, by, bz));
				}

				// ���ɂ���
				if (fx < cL5MINPOS)
				{
					trans2.setOrigin(btVector3(cL5MINPOS, fy, fz));
				}
				else if (fx > cL5MAXPOS)
				{
					trans2.setOrigin(btVector3(cL5MAXPOS, fy, fz));
				}

				break;
			}

			rBodyCpuBody[i]->getMotionState()->setWorldTransform(trans);
			rBodyCpuFoot[i]->getMotionState()->setWorldTransform(trans2);

			// �`��I�u�W�F�N�g�֘A
			// ���W�̃Z�b�g
			btVector3 physicsPos = trans.getOrigin();
			SetPosition(cpuPlayerPos[i], physicsPos);

			// �p�x�̃Z�b�g
			btQuaternion physicsRot = trans.getRotation();

			btScalar rollx, pitchy, yawz;

			physicsRot.getEulerZYX(yawz, pitchy, rollx);

			btVector3 rot = btVector3(XMConvertToDegrees((float)rollx), XMConvertToDegrees((float)pitchy), XMConvertToDegrees((float)yawz));

			SetRotation(cpuPlayerAng[i], rot);
		}
	}
#pragma endregion

#pragma region ����L�[�p�[�̍X�V
	{
		btTransform trans;  // �L�[�p�[�̏�Ԋi�[�p

		if (rBodyCpuKeaper && rBodyCpuKeaper->getMotionState()) // �I��̏�Ԃ��擾
		{
			rBodyCpuKeaper->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = rBodyCpuKeaper->getWorldTransform();
		}

		/*cpuKeaperVel.setValue(0.0f, 0.0f, kVelSwitching);

		if (kSwitchingTime >= TWOSECONDS * 5.0f)
		{
			kSwitchingTime = 0.0f;
			kVelSwitching *= -1;
		}
		else
		{
			kSwitchingTime++;
		}*/

		if (trans.getOrigin().getZ() < cKMINPOS)
		{
			trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), cKMINPOS));
		}
		else if (trans.getOrigin().getZ() > cKMAXPOS)
		{
			trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), cKMAXPOS));
		}

		// �����I�u�W�F�N�g�֘A
		// ���W�̍X�V
		trans.setOrigin(trans.getOrigin() + cpuKeaperVel);

		rBodyCpuKeaper->getMotionState()->setWorldTransform(trans);

		// �`��I�u�W�F�N�g�֘A
		// ���W�̃Z�b�g
		btVector3 physicsPos = trans.getOrigin();
		SetPosition(cpuKeaperPos, physicsPos);
	}
#pragma endregion
}

void GamePlay::SetPosition(XMFLOAT3& objPos, const btVector3& phyPos)
{
	objPos = XMFLOAT3(float(phyPos.getX()), float(phyPos.getY()), float(phyPos.getZ()));
}

void GamePlay::SetRotation(XMFLOAT3& objRot, const btVector3& phyRot)
{
	objRot = XMFLOAT3(float(phyRot.getX()), float(phyRot.getY()), float(phyRot.getZ()));
}

void GamePlay::ResetGame()
{
	ResetStatus(); // �Q�[�����̏�Ԃ̃��Z�b�g

	// �X�R�A�̏�����
	myScore = 0;
	cpuScore = 0;
}

void GamePlay::ResetStatus()
{
	CreateBall(); // �{�[�����Đ���

	//CreateMyPlayer();
	//CreateCpuPlayer();

	kickoffPos = kickoffStartPos; // �����ʒu�Ƀ��Z�b�g

	goalCha->SetSize(goalStartSize.x, goalStartSize.y); // �����T�C�Y�Ƀ��Z�b�g
	goalChaMag = 0.1f;
}

void GamePlay::ObjectUpdate()
{
	// �J�����̍X�V
	Camera* camera = Camera::GetInstance();

	camera->Update();

#pragma region ���� ����t�B�[���h�v���C���[�̍X�V
	for (int i = 0; i < PLAYERNUM; i++)
	{
		// �����v���C���i�́j
		objectMyBody[i]->SetPosition(myPlayerPos[i]);
		objectMyBody[i]->SetRotation(myPlayerAng[i]);
		objectMyBody[i]->Update(true);

		// ����v���C���i�́j
		objectCpuBody[i]->SetPosition(cpuPlayerPos[i]);
		objectCpuBody[i]->SetRotation(cpuPlayerAng[i]);
		objectCpuBody[i]->Update(true);
	}
#pragma endregion

#pragma region �����S�[���L�[�p�[�̍X�V
	objectMyKeaper->SetPosition(myKeaperPos);
	objectMyKeaper->Update();
#pragma endregion

#pragma region ����S�[���L�[�p�[�̍X�V
	objectCpuKeaper->SetPosition(cpuKeaperPos);
	objectCpuKeaper->Update();
#pragma endregion

#pragma region �{�[���̍X�V
	soccerBall->Update();
#pragma endregion

#pragma region �X�R�A�̍X�V
	// ����
	myScoreNumber->SetRect(myScoreOrigin.x + myScore * myScoreSize.x, myScoreOrigin.y, myScoreSize.x, myScoreSize.y);

	// ����
	cpuScoreNumber->SetRect(cpuScoreOrigin.x + cpuScore * cpuScoreSize.x, cpuScoreOrigin.y, cpuScoreSize.x, cpuScoreSize.y);

#pragma endregion

#pragma region �X�^�W�A���̍X�V
	// �X�^�W�A��
	stadium->Update();

	// �����S�[��
	myGoal->Update();
	// ����S�[��
	cpuGoal->Update();
#pragma endregion
}

void GamePlay::MyGoal()
{
	myScore++; // �����X�R�A���Z
}

void GamePlay::CpuGoal()
{
	cpuScore++; // ����X�R�A���Z
}