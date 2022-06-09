#include "Result.h"
#include "SceneManager.h"
#include "Input.h"
#include "Title.h"

using namespace std;

void Result::Initialize()
{
#pragma region テクスチャ読み込み
	TextureManager* texManager = TextureManager::GetInstance();
	texManager->LoadTexture(14, L"Resources/Sprite/result.png");
#pragma endregion

#pragma region スプライト初期化
	// 試合結果
	resultSprite = make_unique<Sprite>();
	resultSprite->Initialize(14);
#pragma endregion
}

void Result::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		// タイトルへ
		BaseScene* scene = new Title();
		sceneManager->SetNextScene(scene);
	}
}

void Result::DrawBackSprite()
{
	// パイプラインの設定
	Sprite::SetPipeline();
	// 背景の描画
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
