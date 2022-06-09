#pragma once
#include "BaseGame.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

class SoccerBoards : public BaseGame
{
#pragma region ƒƒ“ƒoŠÖ”
public:
	// ‰Šú‰»
	void Initialize() override;
	// XV
	void Update() override;
	// •`‰æ
	void Draw() override;
	// ‰ğ•ú
	void Finalize() override;
#pragma endregion
};