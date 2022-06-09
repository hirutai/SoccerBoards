#include "Title.h"
#include "GamePlay.h"
#include "SceneManager.h"
#include "Input.h"

using namespace std;

void Title::Initialize()
{
	TextureManager* texManager = TextureManager::GetInstance();
	texManager->LoadTexture(7, L"Resources/Sprite/background.png");
	texManager->LoadTexture(8, L"Resources/Sprite/title.png");

	// 背景画像
	backGroundSprite = make_unique<Sprite>();
	backGroundSprite->Initialize(7);

	// タイトル画像
	titleSprite = make_unique<Sprite>();
	titleSprite->Initialize(8);
	titleSprite->SetAnchorPoint(vertCenter); // 頂点を中心に設定
	titleSprite->SetPosition(screenUpCenter.x, screenUpCenter.y); // 画面中央に配置
}

void Title::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		BaseScene* scene = new GamePlay();
		sceneManager->SetNextScene(scene);
	}
}

void Title::DrawBackSprite()
{
	Sprite::SetPipeline();

	backGroundSprite->Draw(); // 描画

	titleSprite->Draw();
}

void Title::DrawObject()
{
}

void Title::DrawFrontSprite()
{
	titleSprite->Draw();
}

void Title::Finalize()
{
}