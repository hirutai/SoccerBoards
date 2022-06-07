#pragma once
#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
#include <memory>

class Obj3dModel;
class Obj3dObject;
class Sprite;

/// <summary>
/// ゴール
/// </summary>
class Goal
{
#pragma region エイリアス
private:
	// std::を省略
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region 定数

#pragma endregion

#pragma region メンバ関数
public:
	// 初期化
	void Initialzie(const std::string& modelName, const UINT& netTexNum);
	// 更新
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
#pragma endregion

#pragma region メンバ関数
private:
	// 3dモデルの初期化
	void ModelInit(const std::string& modelName, const UINT& netTexNum);

	// 3dオブジェクトの初期化
	void ObjectInit();
#pragma endregion

#pragma region メンバ変数
private:
	// オブジェクト関連
	std::unique_ptr<Obj3dModel> modelGoal; // モデル
	std::unique_ptr<Obj3dObject> objectGoal; // オブジェクト
#pragma endregion
};