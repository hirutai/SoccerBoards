#pragma once

#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
#include <memory>

class Obj3dModel;
class Obj3dObject;

/// <summary>
/// サッカーボール
/// </summary>
class SoccerBall
{
#pragma region エイリアス
private:
	// std::を省略
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region メンバ関数(public)
public:
	// 初期化
	void Initialize(const std::string& modelName, const UINT& ballTexNum);
	// 更新
	void Update();
	// 描画
	void Draw();

	// 座標の設定
	void SetPosition(const XMFLOAT3& physicsPosition) { position = physicsPosition; }
	// 角度の設定
	void SetAngle(const XMFLOAT3& physicsAngle) { angle = physicsAngle; }

	// 座標の取得
	const XMFLOAT3& GetPosition() { return position; }
	// 角度の取得
	const XMFLOAT3& GetAngle() { return angle; }
#pragma endregion

#pragma region メンバ関数(private)
private:
	// 3dモデルの初期化
	void ModelInit(const std::string& modelName, const UINT& ballTexNum);

	// 3dオブジェクトの初期化
	void ObjectInit();
#pragma endregion

#pragma region メンバ変数
private:
	unique_ptr<Obj3dModel> model; // モデル
	unique_ptr<Obj3dObject> object; // オブジェクト

	XMFLOAT3 position{}; // 座標
	XMFLOAT3 angle{}; // 角度
#pragma endregion
};