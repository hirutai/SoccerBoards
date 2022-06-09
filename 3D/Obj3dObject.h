#pragma once
#include <d3d12.h>
#include <d3dx12.h>

#include "DirectXInitialize.h"
#include "Obj3dModel.h"
#include "Camera.h"

/// <summary>
/// .objオブジェクトクラス
/// </summary>
class Obj3dObject
{
#pragma region エイリアス
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
#pragma endregion

#pragma region 定数
public:
	static const int maxObjectCount = 512; // 3Dオブジェクトの最大数
#pragma endregion

#pragma region インナークラス
public:
	class Common // 共通データ
	{
		friend class Obj3dObject; // Obj3dObjectをフレンドクラス

	public:
		/// <summary>
		/// グラフィックスパイプラインの初期化
		/// </summary>
		/// <param name="dxInit">DirectX初期化</param>
		void InitializeGraphicsPipeline(DirectXInitialize* dxInit);

		/// <summary>
		/// デスクリプタヒープの初期化
		/// </summary>
		/// <param name="dxInit">DirectX初期化</param>
		void InitializeDescriptorHeap(DirectXInitialize* dxInit);
	private:
		DirectXInitialize* dxInit = nullptr; // DirectX初期化
		ComPtr<ID3D12RootSignature> rootSignature; // ルートシグネチャ
		ComPtr<ID3D12PipelineState> pipelineState; // パイプラインステート
		ComPtr<ID3D12DescriptorHeap> basicDescHeap; // デスクリプタヒープ（定数バッファビュー用）
		int descHeapIndex = 0; // 次に使うデスクリプタヒープ番号
		Camera* camera = nullptr; // カメラ
	};
#pragma endregion

#pragma region サブクラス
	// 定数バッファ用構造体
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色(RGBA)
		XMMATRIX mat; // 3D変換行列
	};
	// 定数バッファ用構造体
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; // アンビエント係数
		float pad1; // パディング
		XMFLOAT3 diffuse; // ディフューズ係数
		float pad2; // パディング
		XMFLOAT3 specular; // スペキュラー係数
		float alpha; // アルファ
	};
#pragma endregion

#pragma region 静的メンバ関数
public:
	/// <summary>
	/// 静的メンバの初期化
	/// </summary>
	/// <param name="dxInit">DirectX初期化</param>
	/// <param name="model">モデルデータ</param>
	/// <param name="camera">カメラ</param>
	static void StaticInitialize(DirectXInitialize* dxInit, Camera* camera);

	/// <summary>
	/// デスクリプタヒープのリセット
	/// </summary>
	static void ResetDescriptorHeap();

	/// <summary>
	/// 静的メンバの解放
	/// </summary>
	static void StaticFinalize();
#pragma endregion

#pragma region 静的メンバ変数
private:
	static Common* common;
#pragma endregion

#pragma region メンバ関数(public)
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Obj3dModel* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool physicsObjectFlag = false);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
#pragma endregion

#pragma region メンバ関数(public)
private:
	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model"></param>
	void SetModel(Obj3dModel* model) { this->model = model; }
#pragma endregion

#pragma region setter
public:

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="rotation">回転角</param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	/// <summary>
	/// 親オブジェクトの設定
	/// </summary>
	/// <param name="parentObject">親オブジェクト</param>
	void SetParent(Obj3dObject* parentObject) { this->parent = parentObject; }
#pragma endregion

#pragma region getter
public:
	/// <summary>
	///  座標の取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetPosition() { return position; }

	/// <summary>
	/// スケールの取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetScale() { return scale; }

	/// <summary>
	/// 回転角の取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetRotation() { return rotation; }

	inline XMMATRIX GetMatWorld() { return matWorld; }
#pragma endregion

#pragma region メンバ変数
private:
	Obj3dModel* model = nullptr; // モデルデータ

	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV; // 定数バッファビューのハンドル（CPU）
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV; // 定数バッファビューのハンドル（GPU）
	XMFLOAT3 position = { 0,0,0 }; // 座標
	XMFLOAT3 scale = { 1,1,1 }; // スケール
	XMFLOAT3 rotation = { 0,0,0 }; // 回転角
	XMMATRIX matWorld{}; // ワールド変換行列
	Obj3dObject* parent = nullptr;	// 親オブジェクト
#pragma endregion
};