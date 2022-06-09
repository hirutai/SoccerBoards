#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "DirectXInitialize.h"

// 前方宣言
class TextureManager;

/// <summary>
/// スプライトクラス
/// </summary>
class Sprite
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

#pragma region インナークラス
private:
	class Common // 共通データ
	{
		friend class Sprite; // Spriteをフレンドクラス

	public:
		/// <summary>
		/// グラフィックスパイプラインの初期化
		/// </summary>
		void InitializeGraphicsPipeline();

		DirectXInitialize* dxInit = nullptr; // DirectX初期化
	private:
		//TextureManager* textureManager = nullptr; // テクスチャマネージャー
		ComPtr<ID3D12RootSignature> rootSignature; // ルートシグネチャ
		ComPtr<ID3D12PipelineState> pipelineState; // パイプラインステート
		XMMATRIX matProjection{}; // 射影行列
	};
#pragma endregion

#pragma region サブクラス
private:
	// 頂点データ構造体
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT2 uv; // uv座標
	};

	// 定数バッファデータ構造体
	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color; // 色(RGBA)
		DirectX::XMMATRIX mat; //3D変換行列
	};
#pragma endregion

#pragma region 静的メンバ関数
public:
	/// <summary>
	/// 静的メンバの初期化
	/// </summary>
	/// <param name="dxInit">DirectX初期化</param>
	static void StaticInitialize(DirectXInitialize* dxInit);

	/// <summary>
	/// 静的メンバの解放
	/// </summary>
	static void StaticFinalize();

	/// <summary>
	/// パイプラインの設定
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	static void SetPipeline();
#pragma endregion

#pragma region 静的メンバ変数
private:
	static Common* common; // 共有データ

#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	void Initialize(UINT texNumber);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 頂点バッファにデータ転送
	/// </summary>
	void TransferVertices();
#pragma endregion

#pragma region setter
	/// <summary>
	/// ポジションの設定
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	void SetPosition(float x, float y);

	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="rotation">回転角</param>
	void SetRotation(float rotation);

	/// <summary>
	/// サイズの設定
	/// </summary>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	void SetSize(float width, float height);

	/// <summary>
	/// アンカーポイントの設定
	/// </summary>
	/// <param name="anchorpoint">アンカーポイント</param>
	void SetAnchorPoint(XMFLOAT2 anchorpoint);

	/// <summary>
	/// 切り取り範囲指定の設定
	/// </summary>
	/// <param name="tex_x">テクスチャのx原点</param>
	/// <param name="tex_y">テクスチャのy原点</param>
	/// <param name="tex_width">テクスチャの横幅</param>
	/// <param name="tex_height">テクスチャの縦幅</param>
	void SetRect(float tex_x, float tex_y, float tex_width, float tex_height);
#pragma endregion

#pragma region getter
	/// <summary>
	/// ポジションの取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 GetPosition();
#pragma endregion

#pragma region メンバ変数
private:
	ComPtr<ID3D12Resource> vertBuff; // 頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW VBView; // 頂点バッファビュー
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	UINT texNumber;	// テクスチャ番号
	XMVECTOR position{}; // 座標
	float rotation = 0;	// 回転角
	XMFLOAT4 color{ 1,1,1,1 }; // 色
	XMMATRIX matWorld{}; // ワールド行列
	float width = 100.0f; // 横幅
	float height = 100.0f; // 縦幅
	XMFLOAT2 anchorpoint{};	// アンカーポイント
	bool isFlipX = false; // 左右反転
	bool isFlipY = false; // 上下反転
	float tex_x = 0.0f; // 切り取り範囲
	float tex_y = 0.0f;
	float tex_width = 100.0f;
	float tex_height = 100.0f;
#pragma endregion
};