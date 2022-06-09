#pragma once

#include <memory>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "WindowsApi.h"
#include "Sprite.h"
#include "Obj3dModel.h"
#include "Obj3dObject.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

// �O���錾
class Stadium; // �X�^�W�A���N���X
class Goal; // �S�[���N���X
class SoccerBall; // �T�b�J�[�{�[��

#pragma region �e�N�X�`���ԍ�
enum texnumber
{
	DebugFont,
	// �I�u�W�F�N�g�̐F�p
	SOCCERBALL, // �{�[���̐F
	MyPlayer, // �����I��̐F
	CpuPlayer, // ����I��̐F
	Pitch, // �s�b�`�̐F
	Wall, // �ǂ̐F
	Net, // �S�[���̐F
	// �O�i�A�w�i�p
	BackGround, // �w�i
	TITLE, // �^�C�g��
	Operation, // ����
	KickOff, // �L�b�N�I�t
	Score, // �X�R�A
	GOAL, // �S�[���I
	Result, // ��������
};
#pragma endregion

#pragma region �V�[���ԍ�
enum class Scene
{
	CameraRotate, // �J�����̉�]
	KickOffDirecting, // �L�b�N�I�t���̉��o
	Game, // �Q�[��
	MyGoal, // �����S�[����
	MyGoalDirecting, // �����S�[�����̉��o
	CpuGoal, // ����S�[����
	CpuGoalDirecting, // ����S�[�����̉��o
	ResetStatus, // �Q�[�����̃��Z�b�g
};
#pragma endregion

#pragma region �I��̏��
enum class PlayerStatus
{
	Normal, // �ʏ펞
	LeftRotating, // ����]��
	RightRotating, // �E��]��
};
#pragma endregion

/// <summary>
/// �Q�[���v���C�V�[��
/// </summary>
class GamePlay : public BaseScene
{
#pragma region �G�C���A�X
private:
	// std::���ȗ�
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region �����o�֐�
public:
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �w�i�`��
	void DrawBackSprite();
	// 3D�I�u�W�F�N�g�`��
	void DrawObject();
	// �O�i�`��
	void DrawFrontSprite();
	// ���
	void Finalize();
#pragma endregion

#pragma region �J����
#define VERTICALEYE -30.0f // �c�������_

	XMFLOAT3 cameraEye{ 0.0f, 30.0f, VERTICALEYE - cameraEye.x }; // �J�����̎��_
	XMFLOAT3 cameraTarget{ cameraEye.x / 4, 0.0f, 0.0f }; // �J���������_

	float cameraRotation = 0.18f; // ��]��
#pragma endregion

#pragma region �I�u�W�F�N�g�S�̐ݒ�
private: // �����o
	const float BodyHeight = 3.5f; // �s�b�`�̍������O�Ƃ������̃I�u�W�F�N�g�̍���
	const float footHeight = 1.5f; // �@�@�@�@�@�@�V�@�@�@�@�@�@���̍���
#pragma endregion

#pragma region �X�v���C�g �A���J�[�|�C���g�ݒ�p
	XMFLOAT2 vertCenter{ 0.5f,0.5f }; // �������_
	XMFLOAT2 vertLeftCenter{ 0.0f,0.5f }; // ���������_
	XMFLOAT2 vertRightCenter{ 1.0f,0.5f }; // �E�������_
#pragma endregion

#pragma region �X�v���C�g �������W�w��p
	XMFLOAT2 screenCenter{ WindowsApi::windowWidth / 2, WindowsApi::windowHeight / 2 }; // ��ʒ���
#pragma endregion

#pragma region �T�b�J�[�{�[��
private: // �����o
	SoccerBall* soccerBall = nullptr; // �T�b�J�[�{�[��

	// �����p
	btVector3 ballStartPos{ 0, 20, 0 }; // �{�[���̏������W

	const float ballRadius = 3.0f / 2; // �{�[���̔��a
	btCollisionShape* cShapeBall = nullptr; // �Փˌ`��
	btRigidBody* rBodyBall = nullptr; // ����
#pragma endregion

#pragma region �I��S�̐ݒ�
private: // �萔
#define PLAYERNUM 5 // �e�`�[���t�B�[���h�v���C���[��
#define KEAPERNUM 1 // �e�`�[���S�[���L�[�p�[��

#define PI 3.14159265 // �~����
#define CIRCLELENGTH PI * 2.0f // ����̒��� 

#define L1STARTPOS 11.0f // �P���C���ڂ̑I��̏���Z���W
#define PLAYERINTERVAL 5.5f // �I���Z���W�̊Ԋu

private: // �����o
	const XMFLOAT2 bodySize{ 3.0f / 2, 6.5f }; // �̂̑傫���ix ���a�A y �����j
	const XMFLOAT3 footSize{ 5.5f / 2, 1.5f / 2, 1.5f / 2 }; // ���̑傫���ix �� / 2�A y ���� / 2�A z �[�� / 2�j
#pragma endregion

#pragma region �����I��ݒ�
#define mL1MINPOS -16.0f // �P���C���ڂ̑I��̈ړ��͈�
#define mL1MAXPOS 16.0f
#define mL2MINPOS 1.0f // �Q���C���ڂ̑I��̈ړ��͈�
#define mL2MAXPOS 16.0f
#define mL3MINPOS -16.0f // �R���C���ڂ̑I��̈ړ��͈�
#define mL3MAXPOS -1.0f
#define mL4MINPOS 1.0f // �S���C���ڂ̑I��̈ړ��͈�
#define mL4MAXPOS 16.0f
#define mL5MINPOS -16.0f // �T���C���ڂ̑I��̈ړ��͈�
#define mL5MAXPOS 16.0f
#define mKMINPOS -5.0f // �L�[�p�[�̈ړ��͈�
#define mKMAXPOS 5.0f

#define myPlayerSpeVal 0.2f // �t�B�[���h�v���C���̑���
#define myPlayerRotVal 0.05f // �����I��̉�]��

#define myKeaperSpeVal 0.1f // �L�[�p�[�̑���

#define TWOSECONDS 120.0f // 2�b 

private: // �t�B�[���h�v���C���[
	// �`��p
	unique_ptr<Obj3dModel> modelMyBody; // ���f��
	unique_ptr<Obj3dObject> objectMyBody[PLAYERNUM]; // 3D�I�u�W�F�N�g

	XMFLOAT3 myPlayerPos[PLAYERNUM]{}; // ���W
	XMFLOAT3 myPlayerAng[PLAYERNUM]{}; // ��]�p

	PlayerStatus myPlayerStatus = PlayerStatus::Normal; // �I��̏��

	// �S�̕����p
	btVector3 myPlayerVel[PLAYERNUM]{}; // ���x
	btVector3 myPlayerRot[PLAYERNUM]{}; // ��]��

	float myPlayerRotationValue = 0; // ��]�p�ϐ�

	// �̗p
	// �����p
	btCollisionShape* cShapeMyBody[PLAYERNUM]; // �Փˌ`��
	btRigidBody* rBodyMyBody[PLAYERNUM]; // ����

	btVector3 myBodyStartPos[PLAYERNUM]{}; // �̂̏������W

	// ���p
	// �����p
	btCollisionShape* cShapeMyFoot[PLAYERNUM]; // �Փˌ`��
	btRigidBody* rBodyMyFoot[PLAYERNUM]; // ����

	btVector3 myFootStartPos[PLAYERNUM]{}; // ���̏������W
private: // �S�[���L�[�p�[
	// �`��p
	unique_ptr<Obj3dModel> modelMyKeaper; // ���f��
	unique_ptr<Obj3dObject> objectMyKeaper; // 3D�I�u�W�F�N�g

	XMFLOAT3 myKeaperPos{}; // ���W
	XMFLOAT3 myKeaperAng{}; // ��]�p

	// �����p
	btCollisionShape* cShapeMyKeaper = nullptr; // �Փˌ`��
	btRigidBody* rBodyMyKeaper = nullptr; // ����

	btVector3 myKeaperVel{}; // ���x

	btVector3 myKeaperStartPos{ -23, BodyHeight, 0 }; // �S�[���L�[�p�[�̏������W
#pragma endregion

#pragma region ����I��
private: // �萔
#define cL1MINPOS -16.0f // �P���C���ڂ̑I��̈ړ��͈�
#define cL1MAXPOS 16.0f
#define cL2MINPOS -16.0f // �Q���C���ڂ̑I��̈ړ��͈�
#define cL2MAXPOS -1.0f
#define cL3MINPOS 1.0f // �R���C���ڂ̑I��̈ړ��͈�
#define cL3MAXPOS 16.0f
#define cL4MINPOS -16.0f // �S���C���ڂ̑I��̈ړ��͈�
#define cL4MAXPOS -1.0f
#define cL5MINPOS -16.0f // �T���C���ڂ̑I��̈ړ��͈�
#define cL5MAXPOS 16.0f
#define cKMINPOS -5.0f // �L�[�p�[�̈ړ��͈�
#define cKMAXPOS 5.0f

private: // �t�B�[���h�v���C���[
	// �S�̕`��p
	unique_ptr<Obj3dModel> modelCpuBody; // ���f��
	unique_ptr<Obj3dObject> objectCpuBody[PLAYERNUM]; // 3D�I�u�W�F�N�g

	XMFLOAT3 cpuPlayerPos[PLAYERNUM]{}; // ���W
	XMFLOAT3 cpuPlayerAng[PLAYERNUM]{}; // �p�x

	// �S�̕����p
	btVector3 cpuPlayerVel[PLAYERNUM]{}; // ���x
	btVector3 cpuPlayerRot[PLAYERNUM]{}; // ��]��

	// ��
	// �����p
	btCollisionShape* cShapeCpuBody[PLAYERNUM]; // �Փˌ`��
	btRigidBody* rBodyCpuBody[PLAYERNUM]; // ����

	btVector3 cpuBodyStartPos[PLAYERNUM]{}; // ����̑̂̏������W

	// ���p
	// �����p
	btCollisionShape* cShapeCpuFoot[PLAYERNUM]; // �Փˌ`��
	btRigidBody* rBodyCpuFoot[PLAYERNUM]; // ����

	btVector3 cpuFootStartPos[PLAYERNUM]{}; // ���̏������W

private: // �S�[���L�[�p�[
	unique_ptr<Obj3dModel> modelCpuKeaper; // ���f��
	unique_ptr<Obj3dObject> objectCpuKeaper; // 3D�I�u�W�F�N�g

	XMFLOAT3 cpuKeaperPos{}; // ���W
	XMFLOAT3 cpuKeaperAng{}; // �p�x

	// �����p
	btCollisionShape* cShapeCpuKeaper = nullptr; // �Փˌ`��
	btRigidBody* rBodyCpuKeaper = nullptr; // ����

	btVector3 cpuKeaperVel{}; // �ړ���

	btVector3 cpuKeaperStartPos{ 23, BodyHeight, 0 }; // ����S�[���L�[�p�[�̏������W
#pragma endregion

	// AI�p
	float pSwitchingVelTime = 0; // �t�B�[���h�v���C���̈ړ��ʐ؂�ւ��p
	float pVelSwitching = 0.1f; // �ړ��ʐ؂�ւ��p
	float pSwitchingRotTime = 0; // �t�B�[���h�v���C���̉�]�ʐ؂�ւ��p
	bool rotatePlayerFlag = false; // ��]����
	float cpuPlayerRotationValue = 0.0f; // ��]�p�ϐ�
	float laRSwitching = 0.1f; // ���E�؂�ւ��p

	float kSwitchingTime = 0; // �L�[�p�[�̈ړ��ʐ؂�ւ��p
	float kVelSwitching = 0.1f; // �ړ��ʐ؂�ւ��p

#pragma region �X�^�W�A��
	Stadium* stadium = nullptr; // �X�^�W�A��

private: // �����o
	const XMFLOAT3 pitchSize{ 48.0f / 2, 0.0f, 36.0f / 2 }; // �s�b�`��x �� / 2�A y ���� / 2�A z �[�� / 2

	// �s�b�`�̕����p
	btCollisionShape* cShapeGround = nullptr; // �Փˌ`��
	btRigidBody* rBodyGround = nullptr; // ����

#define lrWALLNUM 2 // ���E�ǂ̐�

	float lrWallHalf = 11.0f / 2; // ���E�ǂ�Z���̔����̃T�C�Y

	const XMFLOAT3 lrWallSize{ 0.1f / 2, 100.0f / 2, lrWallHalf }; // ���E�ǂ�x �� / 2�A y ���� / 2�A z �[�� / 2

	// ���ǂ̕����p
	btCollisionShape* cShapeLWall[2]; // �Փˌ`��
	btRigidBody* rBodyLWall[2]; // ����

	// �E�ǂ̕����p
	btCollisionShape* cShapeRWall[2]; // �Փˌ`��
	btRigidBody* rBodyRWall[2]; // ����

	const XMFLOAT3 fbWallSize{ 48.0f / 2, 100.0f / 2, 0.1f / 2 }; // �O���ǂ�x �� / 2, y ���� / 2�A z �[�� / 2

	// �O���ǂ̕����p
	btCollisionShape* cShapeFBWall[2]; // �Փˌ`��
	btRigidBody* rBodyFBWall[2]; // ����
#pragma endregion

#pragma region �S�[��
	Goal* myGoal = nullptr; // �����S�[��

	// �����S�[���̕����p
	btCollisionShape* cShapeMyGoal = nullptr; // �Փˌ`��
	btRigidBody* rBodyMyGoal = nullptr; // ����

	XMFLOAT3 myGoalPos{ -24.0f, 0.0f, 0.0f }; // ���W

	Goal* cpuGoal = nullptr; // ����S�[��

	// ����S�[���̕����p
	btCollisionShape* cShapeCpuGoal = nullptr; // �Փˌ`��
	btRigidBody* rBodyCpuGoal = nullptr; // ����

	XMFLOAT3 cpuGoalPos{ 24.0f, 0.0f, 0.0f }; // ���W
#pragma endregion

#pragma region �w�i�摜
	unique_ptr<Sprite> backGround; // �w�i
#pragma endregion

#pragma region �L�b�N�I�t�摜
#define KICKOFFVEL 7.5f // �����̈ړ���
#define LEFTEND -WindowsApi::windowWidth / 2 // ���[

	unique_ptr<Sprite> kickoff; // �L�b�N�I�t�̕���

	XMFLOAT2 kickoffStartPos{ WindowsApi::windowWidth, WindowsApi::windowHeight / 2 }; // �L�b�N�I�t�̏������W

	XMFLOAT2 kickoffPos{ kickoffStartPos }; // �L�b�N�I�t�̍��W

	int stopTime = 0; // �����Ɏ~�܂��Ă��鎞�Ԃ��J�E���g�p
#pragma endregion

#pragma region �X�R�A�摜
#define VICTORYSCORE 3 // �����X�R�A

	unique_ptr<Sprite> myScoreNumber; // �������_�p����
	unique_ptr<Sprite> cpuScoreNumber; // ���蓾�_�p����

	unsigned int myScore = 0; // �����̓��_
	XMFLOAT2 myScoreOrigin = { 0.5f, 0.0f }; // �������_�̌��_
	XMFLOAT2 myScoreSize = { 100.0f, 100.0f }; // �������_�̃T�C�Y

	XMFLOAT2 myScorePos{ WindowsApi::windowWidth / 2 - 200, 50 }; // �������_�̍��W

	unsigned int cpuScore = 0; // ����̓��_
	XMFLOAT2 cpuScoreOrigin = { 0.5f, 0.0f }; // ����̓��_�摜�̌��_
	XMFLOAT2 cpuScoreSize = { 100.0f, 100.0f }; // ����̓��_�摜�̃T�C�Y

	XMFLOAT2 cpuScorePos{ WindowsApi::windowWidth / 2 + 200, 50 }; // ���蓾�_�̍��W
#pragma endregion

#pragma region �S�[���摜
#define GOALCHARWIDTH 500.0f // �S�[���I�̕����̌�����
#define GOALCHARHEIGHT 100.0f // �@�@�@�@�@�@  �̌��c��

#define ORIGINALSIZE 2.0f // ���̃T�C�Y�̔{��

#define EXPRATE 0.01f // �g�嗦

	unique_ptr<Sprite> goalCha; // �S�[���I�̕���

	float goalChaMag = 0.1f; // �S�[���I�̕����̔{��

	XMFLOAT2 goalStartSize{ GOALCHARWIDTH, GOALCHARHEIGHT }; // �S�[���I�̏����T�C�Y

	XMFLOAT2 goalChaSize{ goalStartSize }; // �S�[���I�̃T�C�Y

#pragma region ����
private: // �����o
	// �������E�̏�����
	void PhysicsInit();

	// �����I�u�W�F�N�g�̍쐬
	// �n�ʂ̍쐬
	void CreateGround();
	// �ǂ̍쐬
	void CreateWall();
	// �{�[���̍쐬
	void CreateBall();
	// �����t�B�[���h�v���C���[�̍쐬�i�̂Ƒ��j
	void CreateMyPlayer();
	// �����L�[�p�[�̍쐬
	void CreateMyKeaper();
	// ����t�B�[���h�v���C���[�̍쐬
	void CreateCpuPlayer();
	// ����L�[�p�[�̍쐬
	void CreateCpuKeaper();

	// �������E�̍X�V
	void PhysicsUpdate();

	// �����I�u�W�F�N�g�ړ����̍X�V
	void PhysicsMoveUpdate();

	// �����I�u�W�F�N�g�ƕ`��I�u�W�F�N�g�̍��W�̃����N
	void SetPosition(XMFLOAT3& objPos, const btVector3& phyPos);
	// ��]�p�̃����N
	void SetRotation(XMFLOAT3& objRot, const btVector3& phyRot);

	// �������E�p�ϐ�
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr; // �Փˍ\��
	btCollisionDispatcher* dispatcher = nullptr; // �Փ˃f�B�X�p�b�`���[
	btBroadphaseInterface* overlappingPairCache = nullptr; // �u���[�h�t�F�[�Y
	btSequentialImpulseConstraintSolver* solver = nullptr; // ����\���o
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr; // �������E
	btAlignedObjectArray<btCollisionShape*> collisionShapes; // �Փˌ`��z��
#pragma endregion

#pragma region �Q�[��
private:
	// �X�v���C�g�����֐�
	void CreateSprite();

	// �I�u�W�F�N�g�̍X�V����
	void ObjectUpdate();

	// �Q�[���̃��Z�b�g
	void ResetGame();

	// �Q�[�����̃��Z�b�g
	void ResetStatus();

	// �����̃S�[��
	void MyGoal();

	// ����̃S�[��
	void CpuGoal();

	Scene scene = Scene::CameraRotate; // �Q�[���V�[�����̃V�[���Ǘ�
#pragma endregion
};