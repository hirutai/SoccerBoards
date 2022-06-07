#pragma once
#include "btBulletDynamicsCommon.h"

class PhysicsManager
{
#pragma region メンバ関数
public:
	// コンストラクタ
	PhysicsManager();

	// デスストラクタ
	~PhysicsManager();

	// 初期化
	void BulletInitialize();
#pragma endregion

#pragma region Getter
	// 物理世界の取得
	//const b2World& GetWorld() { return world; }
#pragma endregion

#pragma region メンバ変数
	//b2World world; // 物理世界
	btDynamicsWorld* g_pDynamicsWorld = nullptr; // 物理ワールド
#pragma endregion
};
