#pragma once
#include "BaseGame.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

class SoccerBoards : public BaseGame
{
#pragma region メンバ関数
public:
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 解放
	void Finalize() override;
#pragma endregion
};