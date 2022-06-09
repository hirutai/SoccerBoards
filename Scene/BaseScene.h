#pragma once

// 前方宣言
class SceneManager;

/// <summary>
/// シーンの基底
/// </summary>
class BaseScene
{
#pragma region 純粋仮想関数
public:
	// 仮想デスストラクタ
	virtual ~BaseScene() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update() = 0;
	// 背景描画
	virtual void DrawBackSprite() = 0;
	// 3Dオブジェクト描画
	virtual void DrawObject() = 0;
	// 前景描画
	virtual void DrawFrontSprite() = 0;
	// 解放
	virtual void Finalize() = 0;
	// シーンマネージャの設定
	virtual void SetSceneManager(SceneManager* sceneManager_) { sceneManager = sceneManager_; }
#pragma endregion

#pragma region メンバ変数
protected:
	SceneManager* sceneManager = nullptr; // シーンマネージャ(入れ物)
#pragma endregion
};