#pragma once
#include "btBulletDynamicsCommon.h"

class PhysicsManager
{
#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	PhysicsManager();

	// �f�X�X�g���N�^
	~PhysicsManager();

	// ������
	void BulletInitialize();
#pragma endregion

#pragma region Getter
	// �������E�̎擾
	//const b2World& GetWorld() { return world; }
#pragma endregion

#pragma region �����o�ϐ�
	//b2World world; // �������E
	btDynamicsWorld* g_pDynamicsWorld = nullptr; // �������[���h
#pragma endregion
};
