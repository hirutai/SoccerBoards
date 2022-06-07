#pragma once 
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXTex.h>

#include "DirectXInitialize.h"

/// <summary>
/// テクスチャマネージャ
/// </summary>
class TextureManager
{
#pragma region エイリアス
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

#pragma region 定数
private:
	static const int maxTextureNumber = 512;   //テクスチャの最大枚数
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxInit">DirectX初期化</param>
	void Initialize(DirectXInitialize* dxInit);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <param name="filename">ファイル名</param>
	void LoadTexture(UINT texNumber, const wchar_t* filename);

	/// <summary>
	/// デスクリプタヒープの設定
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void SetDescriptorHeaps(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// シェーダーリソースビューの設定
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="RootparameterIndex">ルートパラメーターの番号</param>
	/// <param name="texNumber">テクスチャ番号</param>
	void SetShaderResourcesView(ID3D12GraphicsCommandList* commandList, UINT RootparameterIndex, UINT texNumber);
#pragma endregion

#pragma region getter
public:
	/// <summary>
	/// テクスチャバッファの取得
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns></returns>
	ID3D12Resource* GetSpriteTexBuff(UINT texNumber);
#pragma endregion

#pragma region メンバ変数
private:
	DirectXInitialize* dxInit = nullptr; // DirectX初期化

	ComPtr<ID3D12DescriptorHeap> descHeap; // デスクリプタヒープ
	ComPtr<ID3D12Resource> textureBuff[maxTextureNumber]; // テクスチャバッファ
	UINT sizeSRV; // SRVのサイズの格納用
#pragma endregion
};