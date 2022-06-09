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
		BaseScene* scene = new Operation();
		sceneManager->SetNextScene(scene);
	}
}

void Title::DrawBackSprite()
{
	// パイプラインの設定
	Sprite::SetPipeline();
	// 背景の描画
	backGroundSprite->Draw();
}

void Title::DrawObject()
{
}

void Title::DrawFrontSprite()
{
	// タイトル画像の描画
	titleSprite->Draw();
}

void Title::Finalize()
{
}