#pragma once 
#include "WindowsApi.h"
#include "DirectXInitialize.h"
#include "Input.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Obj3dModel.h"
#include "Obj3dObject.h"
#include "Sprite.h"
#include "DebugText.h"

#include <memory>

/// <summary>
/// ゲームの基盤
/// </summary>
class BaseGame
{
#pragma region エイリアス
protected:
	// std::を省略
	//template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// ゲーム（Run関数）
	/// </summary>
	virtual void Run();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 解放
	/// </summary>
	virtual void Finalize();
#pragma endregion

#pragma region メンバ変数
protected:
	bool endFlag = false; // 継続フラグ

	std::unique_ptr<WindowsApi> winApi; // ウィンドウ
	std::unique_ptr<DirectXInitialize> dxInit; // DirectX初期化
	std::unique_ptr<Input> input; // 入力機能

	std::unique_ptr<SoundManager> soundManager; // サウンドマネージャ
	std::unique_ptr<TextureManager> textureManager; // テクスチャマネージャ
	std::unique_ptr<Camera> camera; // カメラ

	std::unique_ptr<DebugText> debugText; // デバッグテキスト
#pragma endregion
};