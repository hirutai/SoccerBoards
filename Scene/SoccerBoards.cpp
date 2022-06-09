#include "SoccerBoards.h"
#include "Title.h"
#include "Operation.h"
#include "GamePlay.h"
#include "Result.h"

void SoccerBoards::Initialize()
{
#pragma region ���N���X�̏�����
	BaseGame::Initialize();
#pragma endregion

#pragma region �V�[���̏�����
	BaseScene* scene = new Title();
	sceneManager->SetNextScene(scene); // �V�[���}�l�[�W���ɍŏ��̃V�[���̐ݒ�
#pragma endregion
}

void SoccerBoards::Update()
{
#pragma region ���N���X�̍X�V
	BaseGame::Update();
#pragma endregion
}

void SoccerBoards::Draw()
{
#pragma region ���N���X�̕`��
	BaseGame::Draw();
#pragma endregion
}

void SoccerBoards::Finalize()
{
#pragma region ���N���X�̉��
	BaseGame::Finalize();
#pragma endregion
}
