#pragma once
#include "BaseGame.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

class SoccerBoards : public BaseGame
{
#pragma region �����o�֐�
public:
	// ������
	void Initialize() override;
	// �X�V
	void Update() override;
	// �`��
	void Draw() override;
	// ���
	void Finalize() override;
#pragma endregion
};