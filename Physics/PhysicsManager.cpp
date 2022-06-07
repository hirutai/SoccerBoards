#include "PhysicsManager.h"
#include <stdlib.h>

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
	// ���[���h�j��
	delete g_pDynamicsWorld->getBroadphase();
	delete g_pDynamicsWorld;
}

void PhysicsManager::BulletInitialize()
{
	// �Փˌ��o���@�̑I��
	btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);

	// �u���[�h�t�F�[�Y�@�̐ݒ�(Dynamic AABB tree method)
	btDbvtBroadphase* broadphase = new btDbvtBroadphase();

	// �S��(���̊ԃ����N)�̃\���o�ݒ�
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	// Bullet���[���h�쐬
	g_pDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
	g_pDynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}
