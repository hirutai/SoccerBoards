#include "Operation.h"
#include "SceneManager.h"
#include "Input.h"
#include "GamePlay.h"

using namespace std;

void Operation::Initialize()
{
#pragma region �e�N�X�`���̓ǂݍ���
	TextureManager* texManager = TextureManager::GetInstance();
	texManager->LoadTexture(7, L"Resources/Sprite/background.png");
	texManager->LoadTexture(9, L"Resources/Sprite/ex.png");
#pragma endregion

#pragma region �X�v���C�g�̏�����
	// �w�i���
	backGroundSprite = make_unique<Sprite>();
	backGroundSprite->Initialize(7);

	// �������
	operationSprite = make_unique<Sprite>();
	operationSprite->Initialize(9);
#pragma endregion
}

void Operation::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) // �X�y�[�X�L�[����������
	{
		// �Q�[���V�[����
		BaseScene* scene = new GamePlay();
		sceneManager->SetNextScene(scene);
	}
}

void Operation::DrawBackSprite()
{
	// �p�C�v���C���̐ݒ�
	Sprite::SetPipeline();
	// �w�i�̕`��
	backGroundSprite->Draw();
}

void Operation::DrawObject()
{
}

void Operation::DrawFrontSprite()
{
	// ����摜�̕`��
	operationSprite->Draw();
}

void Operation::Finalize()
{
}
