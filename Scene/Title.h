#pragma once

#include <memory>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "WindowsApi.h"
#include "Sprite.h"
#include "TextureManager.h"

/// <summary>
/// タイトルシーン
/// </summary>
class Title : public BaseScene
{
private:
	// std::を省略
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;

private:
	const XMFLOAT2 vertCenter{ 0.5f,0.5f }; // 中央頂点
	const XMFLOAT2 screenUpCenter{ WindowsApi::windowWidth / 2, 50.0f }; // 画面上中央

public:
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 背景描画
	void DrawBackSprite() override;
	// 3Dオブジェクト描画
	void DrawObject() override;
	// 前景描画
	void DrawFrontSprite() override;
	// 解放
	void Finalize() override;

private:
#pragma region 背景画像
	unique_ptr<Sprite> backGroundSprite; // 背景
#pragma endregion

#pragma region タイトル画像
	unique_ptr<Sprite> titleSprite; // タイトル
#pragma endregion
};