#include "SoccerBoards.h"
#include "Title.h"
#include "Operation.h"
#include "GamePlay.h"
#include "Result.h"

void SoccerBoards::Initialize()
{
#pragma region 基底クラスの初期化
	BaseGame::Initialize();
#pragma endregion

#pragma region シーンの初期化
	BaseScene* scene = new Title();
	sceneManager->SetNextScene(scene); // シーンマネージャに最初のシーンの設定
#pragma endregion
}

void SoccerBoards::Update()
{
#pragma region 基底クラスの更新
	BaseGame::Update();
#pragma endregion
}

void SoccerBoards::Draw()
{
#pragma region 基底クラスの描画
	BaseGame::Draw();
#pragma endregion
}

void SoccerBoards::Finalize()
{
#pragma region 基底クラスの解放
	BaseGame::Finalize();
#pragma endregion
}
