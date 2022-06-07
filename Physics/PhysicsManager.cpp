#include "PhysicsManager.h"
#include <stdlib.h>

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
	// ワールド破壊
	delete g_pDynamicsWorld->getBroadphase();
	delete g_pDynamicsWorld;
}

void PhysicsManager::BulletInitialize()
{
	// 衝突検出方法の選択
	btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);

	// ブロードフェーズ法の設定(Dynamic AABB tree method)
	btDbvtBroadphase* broadphase = new btDbvtBroadphase();

	// 拘束(剛体間リンク)のソルバ設定
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	// Bulletワールド作成
	g_pDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
	g_pDynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}
