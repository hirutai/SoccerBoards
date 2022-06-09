#include "Operation.h"
#include "SceneManager.h"
#include "Input.h"
#include "GamePlay.h"

using namespace std;

void Operation::Initialize()
{
#pragma region テクスチャの読み込み
	TextureManager* texManager = TextureManager::GetInstance();
	texManager->LoadTexture(7, L"Resources/Sprite/background.png");
	texManager->LoadTexture(9, L"Resources/Sprite/ex.png");
#pragma endregion

#pragma region スプライトの初期化
	// 背景画面
	backGroundSprite = make_unique<Sprite>();
	backGroundSprite->Initialize(7);

	// 操作説明
	operationSprite = make_unique<Sprite>();
	operationSprite->Initialize(9);
#pragma endregion
}

void Operation::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) // スペースキーを押したら
	{
		// ゲームシーンへ
		BaseScene* scene = new GamePlay();
		sceneManager->SetNextScene(scene);
	}
}

void Operation::DrawBackSprite()
{
	// パイプラインの設定
	Sprite::SetPipeline();
	// 背景の描画
	backGroundSprite->Draw();
}

void Operation::DrawObject()
{
}

void Operation::DrawFrontSprite()
{
	// 操作画像の描画
	operationSprite->Draw();
}

void Operation::Finalize()
{
}
