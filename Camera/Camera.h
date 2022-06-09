#pragma once
#include <DirectXMath.h>

#include "WindowsApi.h"

/// <summary>
/// カメラクラス
/// </summary>
class Camera
{
#pragma region 静的メンバ関数
public:
	static Camera* GetInstance();
#pragma endregion

#pragma region エイリアス
public:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュー行列の更新
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// プロジェクションの更新
	/// </summary>
	void UpdateProjectionMatrix();
#pragma endregion

#pragma region setter
public:
	/// <summary>
	/// ビュー行列の設定
	/// </summary>
	/// <param name="matView">ビュー行列</param>
	void SetMatView(const XMMATRIX& matView) { this->matView = matView; }

	/// <summary>
	/// プロジェクション行列の設定
	/// </summary>
	/// <param name="matProjection">プロジェクション行列</param>
	void SetMatProjection(const XMMATRIX& matProjection) { this->matProjection = matProjection; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="eye">視点座標</param>
	void SetEye(const XMFLOAT3& eye) { this->eye = eye; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="target">注視点座標</param>
	void SetTarget(const XMFLOAT3& target) { this->target = target; }

	/// <summary>
	/// 上方向ベクトルの設定
	/// </summary>
	/// <param name="up">上方向ベクトル</param>
	void SetUp(const XMFLOAT3& up) { this->up = up; }
#pragma endregion

#pragma region getter
public:
	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns></returns>
	const XMMATRIX& GetMatView() { return matView; }

	/// <summary>
	/// プロジェクション行列の取得
	/// </summary>
	/// <returns></returns>
	const XMMATRIX& GetMatProjection() { return matProjection; }

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// 上方向ベクトルの取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& GetUp() { return up; }
#pragma endregion

#pragma region メンバ変数
private:
	XMMATRIX matView; // ビュー行列
	XMMATRIX matProjection; // 射影行列
	XMFLOAT3 eye; // 視点座標
	XMFLOAT3 target; // 注視点座標
	XMFLOAT3 up; // 上方向ベクトル
#pragma endregion
};