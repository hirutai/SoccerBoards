#include "Result.h"
#include "SceneManager.h"
#include "Input.h"
#include "Title.h"

using namespace std;

void Result::Initialize()
{
#pragma region �e�N�X�`���ǂݍ���
	TextureManager* texManager = TextureManager::GetInstance();
	texManager->LoadTexture(14, L"Resources/Sprite/result.png");
#pragma endregion

#pragma region �X�v���C�g������
	// ��������
	resultSprite = make_unique<Sprite>();
	resultSprite->Initialize(14);
#pragma endregion
}

void Result::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		// �^�C�g����
		BaseScene* scene = new Title();
		sceneManager->SetNextScene(scene);
	}
}

void Result::DrawBackSprite()
{
	// �p�C�v���C���̐ݒ�
	Sprite::SetPipeline();
	// �w�i�̕`��
	backGroundSprite->Draw();
}

void Result::DrawObject()
{
}

void Result::DrawFrontSprite()
{
	resultSprite->Draw();
}

void Result::Finalize()
{
}
