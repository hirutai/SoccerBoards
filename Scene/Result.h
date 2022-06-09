#pragma once

#include <memory>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "WindowsApi.h"
#include "Sprite.h"
#include "TextureManager.h"

/// <summary>
/// リザルトシーン
/// </summary>
class Result : public BaseScene
{
#pragma region エイリアス
private:
	// std::を省略
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
#pragma endregion

#pragma region メンバ関数
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
#pragma endregion

#pragma region メンバ変数
private:
	unique_ptr<Sprite> backGroundSprite; // 背景

	unique_ptr<Sprite> resultSprite; // 試合結果
};