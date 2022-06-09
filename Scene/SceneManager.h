#pragma once
#include "BaseScene.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
#pragma region メンバ関数
public:
	// デスストラクタ
	~SceneManager();
	// 更新
	void Update();
	// 背景描画
	void DrawBackSprite();
	// 3Dオブジェクト描画
	void DrawObject();
	// 前景描画
	void DrawFrontSprite();
	// 解放
	void Finalize();

	// 次のシーンの設定
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

#pragma region メンバ変数
private:
	BaseScene* scene_ = nullptr; // 現在のシーン

	BaseScene* nextScene_ = nullptr; // 次のシーン
#pragma endregion
};