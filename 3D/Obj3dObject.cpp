#include "Obj3dObject.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

Obj3dObject::Common* Obj3dObject::common = nullptr;

void Obj3dObject::StaticInitialize(DirectXInitialize* dxInit, Camera* camera)
{
	common = new Common();
	common->dxInit = dxInit;

	// グラフィックスパイプラインの初期化
	common->InitializeGraphicsPipeline(dxInit);
	// デスクリプタヒープの初期化
	common->InitializeDescriptorHeap(dxInit);

	common->camera = camera;
}

void Obj3dObject::ResetDescriptorHeap()
{
	common->descHeapIndex = 0;
}

void Obj3dObject::StaticFinalize()
{
	delete common;
	common = nullptr;
}

void Obj3dObject::Initialize(Obj3dModel* model)
{
	// nullptrチェック
	assert(model);

	SetModel(model);

	HRESULT result;

	// 定数バッファの生成
	result = common->dxInit->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);
	constBuffB0->SetName(L"constBuffB0");

	assert(SUCCEEDED(result)); // 警告

	// 定数バッファの生成
	result = common->dxInit->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1)
	);
	constBuffB1->SetName(L"constBuffB1");

	assert(SUCCEEDED(result)); // 警告

	assert(common->descHeapIndex <= maxObjectCount - 1);

	// デスクリプタ１つ分のサイズ
	UINT descHandleIncrementSize = common->dxInit->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// デスクリプタヒープ内での定数バッファビューのアドレスを計算
	cpuDescHandleCBV = CD3DX12_CPU_DESCRIPTOR_HANDLE(common->basicDescHeap->GetCPUDescriptorHandleForHeapStart(), common->descHeapIndex, descHandleIncrementSize);
	gpuDescHandleCBV = CD3DX12_GPU_DESCRIPTOR_HANDLE(common->basicDescHeap->GetGPUDescriptorHandleForHeapStart(), common->descHeapIndex, descHandleIncrementSize);

	// 定数バッファビューの生成
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuffB0->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuffB0->GetDesc().Width;
	common->dxInit->GetDevice()->CreateConstantBufferView(&cbvDesc, cpuDescHandleCBV);
	// デスクリプタヒープの使用番号を１つ進める
	common->descHeapIndex++;
}

void Obj3dObject::Update(bool physicsObjectFlag)
{
	HRESULT result;

	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();

	if (physicsObjectFlag) // 物理オブジェクトとリンクしているオブジェクトの時
	{
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	}
	else
	{
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	}

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ワールド行列を合成
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	// 親オブジェクトがあれば
	if (parent != nullptr)
	{
		matWorld *= parent->matWorld; // 親オブジェクトのワールド行列を掛ける
	}

	const XMMATRIX& matView = common->camera->GetMatView();
	const XMMATRIX& matProjection = common->camera->GetMatProjection();

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	//constMap->color = XMFLOAT4(1, 1, 1, 1);
	constMap->mat = matWorld * matView * matProjection;
	constBuffB0->Unmap(0, nullptr);

	// 定数バッファへデータ転送
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = model->GetAmbient();
	constMap1->diffuse = model->GetDiffuse();
	constMap1->specular = model->GetSpecular();
	constMap1->alpha = model->GetAlpha();
	constBuffB1->Unmap(0, nullptr);
}

void Obj3dObject::Draw()
{
#pragma region 共通描画コマンド
	// パイプラインステートの設定
	common->dxInit->GetCommandList()->SetPipelineState(common->pipelineState.Get());
	// ルートシグネチャの設定
	common->dxInit->GetCommandList()->SetGraphicsRootSignature(common->rootSignature.Get());
	// プリミティブ形状を設定
	common->dxInit->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region 個別描画コマンド
	common->dxInit->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	common->dxInit->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	assert(model); // モデルがnullだったら警告

	// モデル描画
	model->Draw(common->dxInit->GetCommandList());
#pragma endregion
}

void Obj3dObject::Common::InitializeGraphicsPipeline(DirectXInitialize* dxInit)
{
	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob>vsBlob;
	ComPtr<ID3DBlob>psBlob;
	ComPtr<ID3DBlob>errorBlob;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ObjVertexShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob
	);

	if (FAILED(result))
	{
		// errorBlob からエラー内容を string 型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ObjPixelShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob
	);

	if (FAILED(result))
	{
		// errorBlob からエラー内容を string 型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{//xy座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//xy座標
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);   //一旦標準値をセット
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);   //一旦標準値をセット
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;   //隠れているものは描画しない

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);   //t0 レジスタ

	//CD3DX12_ROOT_PARAMETER rootparams[2];
	//rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);   //定数バッファビューとして初期化(b0)
	//rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_ROOT_PARAMETER rootparams[3]{};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL); // 座標変換用
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL); // マテリアル用
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	//ルートシグネチャの生成
	result = dxInit->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	rootSignature->SetName(L"rootSignature");

	assert(SUCCEEDED(result)); // 警告

	//グラフィックスパイプラインの設定にセット
	gpipeline.pRootSignature = rootSignature.Get();

	result = dxInit->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));
	pipelineState->SetName(L"pipelineState");

	assert(SUCCEEDED(result)); // 警告
}

void Obj3dObject::Common::InitializeDescriptorHeap(DirectXInitialize* dxInit)
{
	HRESULT result = S_FALSE;
	// デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	descHeapDesc.NumDescriptors = maxObjectCount;
	result = dxInit->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
	basicDescHeap->SetName(L"basicDescHeap");

	assert(SUCCEEDED(result)); // 警告
}