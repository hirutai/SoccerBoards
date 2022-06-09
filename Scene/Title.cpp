#include "Title.h"
#include "SceneManager.h"
#include "Input.h"
#include "Operation.h"

using namespace std;

void Title::Initialize()
{
	TextureManager* texManager = TextureManager::GetInstance();
	texManager->LoadTexture(7, L"Resources/Sprite/background.png");
	texManager->LoadTexture(8, L"Resources/Sprite/title.png");

	// �w�i�摜
	backGroundSprite = make_unique<Sprite>();
	backGroundSprite->Initialize(7);

	// �^�C�g���摜
	titleSprite = make_unique<Sprite>();
	titleSprite->Initialize(8);
	titleSprite->SetAnchorPoint(vertCenter); // ���_�𒆐S�ɐݒ�
	titleSprite->SetPosition(screenUpCenter.x, screenUpCenter.y); // ��ʒ����ɔz�u
}

void Title::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		BaseScene* scene = new Operation();
		sceneManager->SetNextScene(scene);
	}
}

void Title::DrawBackSprite()
{
	// �p�C�v���C���̐ݒ�
	Sprite::SetPipeline();
	// �w�i�̕`��
	backGroundSprite->Draw();
}

void Title::DrawObject()
{
}

void Title::DrawFrontSprite()
{
	// �^�C�g���摜�̕`��
	titleSprite->Draw();
}

void Title::Finalize()
{
}