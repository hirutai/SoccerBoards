#include "DirectXInitialize.h"
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

void DirectXInitialize::Initialize(WindowsApi* winApi)
{
	// nullptrチェック
	assert(winApi);

	this->winApi = winApi;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	// DXGIデバイス初期化
	if (!CreateDXGIDevice()) { assert(0); }

	// コマンド関連初期化
	if (!CreateCommand()) { assert(0); }

	// スワップチェーンの生成
	if (!CreateSwapChain()) { assert(0); }

	// レンダーターゲット生成
	if (!CreateRenderTargets()) { assert(0); }

	// 深度バッファ生成
	if (!CreateDepthBuffer()) { assert(0); }

	// フェンス生成
	if (!CreateFence()) { assert(0); }
}

void DirectXInitialize::BeforeDraw()
{
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// リソースバリアを変更（表示状態→描画対象）
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
	commandList->ResourceBarrier(1, &barrierDesc);

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	// レンダーターゲットをセット
	commandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// 全画面クリア
	ClearRenderTarget();
	// 深度バッファクリア
	ClearDepthBuffer();

	// ビューポートの設定
	D3D12_VIEWPORT viewport{};
	viewport.Width = WindowsApi::windowWidth;
	viewport.Height = WindowsApi::windowHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	commandList->RSSetViewports(1, &viewport);

	// シザリング矩形の設定
	D3D12_RECT scissorrect{};
	scissorrect.left = 0; //切り抜き座標左
	scissorrect.right = scissorrect.left + WindowsApi::windowWidth; //切り抜き座標右
	scissorrect.top = 0; //切り抜き座標上
	scissorrect.bottom = scissorrect.top + WindowsApi::windowHeight; //切り抜き座標下
	commandList->RSSetScissorRects(1, &scissorrect);
}

void DirectXInitialize::AfterDraw()
{
	// リソースバリアを変更（描画対象→表示状態）
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;// 表示に
	commandList->ResourceBarrier(1, &barrierDesc);

	// 命令のクローズ
	commandList->Close();

	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { commandList.Get() }; // コマンドリストの配列
	commandQueue->ExecuteCommandLists(1, cmdLists);

	// コマンドリストの実行完了を待つ
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	commandAllocator->Reset(); // キューをクリア
	commandList->Reset(commandAllocator.Get(), nullptr);	// 再びコマンドリストを貯める準備

	// バッファをフリップ
	swapchain->Present(1, 0);
}

void DirectXInitialize::ClearRenderTarget()
{
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);

	// 全画面クリア        Red   Green Blue  Alpha
	float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
	commandList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DirectXInitialize::ClearDepthBuffer()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	commandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

bool DirectXInitialize::CreateDXGIDevice()
{
	HRESULT result = S_FALSE;

	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter> tmpAdapter;

	for (int i = 0;
		dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter);	// 動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc;
		adapters[i]->GetDesc(&adesc);	// アダプターの情報を取得

		std::wstring strDesc = adesc.Description;	// アダプター名

		// Intel UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Microsoft") == std::wstring::npos
			&& strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i].Get();	// 採用
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	result = S_FALSE;

	for (int i = 0; i < _countof(levels); i++)
	{
		// デバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
		device->SetName(L"device");

		if (SUCCEEDED(result))
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}

bool DirectXInitialize::CreateCommand()
{
	HRESULT result = S_FALSE;

	// コマンドアロケータを生成
	result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	commandAllocator->SetName(L"commandAllocator");

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// コマンドリストを生成
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
	commandList->SetName(L"commandList");

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue));
	commandQueue->SetName(L"commandQueue");

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}

bool DirectXInitialize::CreateSwapChain()
{
	HRESULT result = S_FALSE;

	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = WindowsApi::windowWidth;
	swapchainDesc.Height = WindowsApi::windowHeight;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 色情報の書式を一般的なものに
	swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	// バックバッファとして使えるように
	swapchainDesc.BufferCount = 2;	// バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は速やかに破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // フルスクリーン切り替えを許可

	ComPtr<IDXGISwapChain1> swapchain1;
	result = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), winApi->GetHwnd(), &swapchainDesc, nullptr, nullptr, &swapchain1);

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	swapchain1.As(&swapchain);

	return true;
}

bool DirectXInitialize::CreateRenderTargets()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// 各種設定をしてディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// レンダーターゲットビュー
	heapDesc.NumDescriptors = swcDesc.BufferCount;
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	rtvHeaps->SetName(L"rtvHeaps");

	if (FAILED(result))
	{
		assert(0);
		return false;;
	}

	// 裏表の２つ分について
	backBuffers.resize(swcDesc.BufferCount);

	for (int i = 0; i < backBuffers.size(); i++)
	{
		// スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//backBuffers[i]->SetName(L"BackBuffer" + i);

		if (FAILED(result))
		{
			assert(0);
			return false;
		}

		// ディスクリプタヒープのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeaps->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);

		// レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i].Get(), nullptr, handle);
	}

	return true;
}

bool DirectXInitialize::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	// リソース設定
	D3D12_RESOURCE_DESC depthResDesc{};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = WindowsApi::windowWidth;   //レンダーターゲットに合わせる
	depthResDesc.Height = WindowsApi::windowHeight;   //レンダーターゲットに合わせる
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;   //深度値フォーマット
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;   //デプスステンシル

	// リソースの生成
	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;   //深度値 1.0f（最大値）でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;   //深度値フォーマット

	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,   //深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuffer)
	);
	depthBuffer->SetName(L"depthBuffer");

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	dsvHeap->SetName(L"dsvHeap");

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());

	return true;
}

bool DirectXInitialize::CreateFence()
{
	HRESULT result = S_FALSE;

	// フェンスの生成
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	fence->SetName(L"fence");

	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}