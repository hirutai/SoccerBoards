#pragma once
#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
#include <memory>

class Obj3dModel;
class Obj3dObject;

/// <summary>
/// スタジアムクラス
/// </summary>
class Stadium
{
#pragma region エイリアス
private:
	// std::を省略
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化
	void Initialize(const UINT& pitchTexNum, const UINT& wallTexNum);
	// 更新
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
#pragma endregion

#pragma region メンバ関数
private:
	// 3dモデルの初期化
	void ModelInit(std::unique_ptr<Obj3dModel>& model, const std::string& modelName, const UINT& texNum);

	// 3dオブジェクトの初期化
	void ObjectInit(std::unique_ptr<Obj3dObject>& object, Obj3dModel* model);
#pragma endregion

#pragma region メンバ変数
private:
	std::unique_ptr<Obj3dModel> modelPitch; // 3dモデル
	std::unique_ptr<Obj3dObject> objectPitch; // 3dオブジェクト

	std::unique_ptr<Obj3dModel> modelWall; // 3dモデル
	std::unique_ptr<Obj3dObject> objectWall; // 3dオブジェクト
#pragma endregion
};