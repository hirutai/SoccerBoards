#pragma once  
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <cstdlib>
#include <wrl.h>

#include "WindowsAPI.h"

/// <summary>
/// DirectX初期化クラス
/// </summary>
class DirectXInitialize
{
#pragma region エイリアス
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApi">ウィンドウ</param>
	void Initialize(WindowsApi* winApi);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void BeforeDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void AfterDraw();

	/// <summary>
	/// レンダーターゲットクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// 深度バッファクリア
	/// </summary>
	void ClearDepthBuffer();
#pragma endregion

#pragma region getter
public:
	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDevice() { return device.Get(); }

	/// <summary>
	/// コマンドリストの取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }
#pragma endregion

#pragma region メンバ関数
private:
	// DXGIデバイス生成
	bool CreateDXGIDevice();
	// コマンド関連生成
	bool CreateCommand();
	// スワップチェーン生成
	bool CreateSwapChain();
	// レンダーターゲット生成
	bool CreateRenderTargets();
	// 深度バッファ生成
	bool CreateDepthBuffer();
	// フェンス生成
	bool CreateFence();
#pragma endregion

#pragma region メンバ変数
private:
	WindowsApi* winApi = nullptr; // ウィンドウ

	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};
#pragma endregion
};