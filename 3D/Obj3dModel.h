#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include <string>

#include "DirectXInitialize.h"

/// <summary>
/// .objモデルクラス
/// </summary>
class Obj3dModel
{
#pragma region エイリアス
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region 定数
public:
	//int division = 3; // 分割数
	float radius = 5.0f; // 底面の半径
	float prizmHeight = 8.0f; // 柱の高さ
	//int planeCount = division * 2 + division * 2; // 面の数
	//int vertexCount = planeCount * 3; // 頂点数
#pragma endregion

#pragma region サブクラス
public:
	// マテリアル
	struct Material
	{
		std::string name; // マテリアル名
		XMFLOAT3 ambient; // アンビエント影響度
		XMFLOAT3 diffuse; // ディフューズ影響度
		XMFLOAT3 specular; // スペキュラー影響度
		float alpha; // アルファ
		std::string textureFilename; // テクスチャファイル名
		// コンストラクタ
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	// 頂点データ構造体
	struct  Vertex
	{
		DirectX::XMFLOAT3 pos; // 座標
		DirectX::XMFLOAT3 normal; // 法線
		DirectX::XMFLOAT2 uv; // uv
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color; // 色 (RGBA)
		DirectX::XMMATRIX mat; // ３Ｄ変換行列
	};
#pragma endregion

#pragma region 静的メンバ関数
public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);
#pragma endregion

#pragma region 静的メンバ変数
private:
	// デバイス
	static ID3D12Device* device;
#pragma endregion

#pragma region メンバ関数(public)
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelName">モデル名</param>
	/// <param name="texNumber">テクスチャ番号</param>
	void Initialize(const std::string& modelName, UINT texNumber);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
#pragma endregion

#pragma region メンバ関数(private)
private:
	/// <summary>
	/// 頂点データの追加
	/// </summary>
	/// <param name="vertex"></param>
	void AddVertex(const Vertex& vertex);

	/// <summary>
	/// インデックスデータの追加
	/// </summary>
	/// <param name="index"></param>
	void AddIndex(unsigned short index);

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
#pragma endregion

#pragma region getter
public:
	XMFLOAT3 GetAmbient() { return material.ambient; }

	XMFLOAT3 GetDiffuse() { return material.diffuse; }

	XMFLOAT3 GetSpecular() { return material.specular; }

	float GetAlpha() { return material.alpha; }
#pragma endregion

#pragma region メンバ変数
private:
	UINT texNumber = 0;	// テクスチャ番号
	ComPtr<ID3D12Resource> vertBuff; // 頂点バッファ
	ComPtr<ID3D12Resource> indexBuff; // インデックスバッファ
	D3D12_VERTEX_BUFFER_VIEW vbView{}; // 頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};	// インデックスバッファビュー

	std::vector<Vertex> vertices; // 頂点データ
	std::vector<unsigned short> indices; // インデックスデータ

	Material material; // マテリアル
};