#include "Sprite.h" 

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

Sprite::Common* Sprite::common = nullptr;

void Sprite::StaticInitialize(DirectXInitialize* dxInit, TextureManager* textureManager)
{
	common = new Common();
	common->dxInit = dxInit;
	common->textureManager = textureManager;

	// グラフィックスパイプライン生成
	common->InitializeGraphicsPipeline();

	// 射影行列の計算
	common->matProjection = XMMatrixOrthographicOffCenterLH(0.0f, WindowsApi::windowWidth, WindowsApi::windowHeight, 0.0f, 0.0f, 1.0f);
}

void Sprite::StaticFinalize()
{
	delete common;
}

void Sprite::SetPipeline(ID3D12GraphicsCommandList* cmdList)
{
	//パイプラインステートの設定
	cmdList->SetPipelineState(common->pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(common->rootSignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::Initialize(UINT texNumber)
{
	HRESULT result = S_FALSE;

	this->texNumber = texNumber;

	if (common->textureManager->GetSpriteTexBuff(texNumber))
	{
		D3D12_RESOURCE_DESC resDesc = common->textureManager->GetSpriteTexBuff(texNumber)->GetDesc();
		width = (float)resDesc.Width;
		height = (float)resDesc.Height;

		tex_width = (float)resDesc.Width;
		tex_height = (float)resDesc.Height;
	}

	//頂点バッファ生成
	result = common->dxInit->GetDevice()->
		CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff)
		);

	assert(SUCCEEDED(result)); // 警告

	TransferVertices();

	//頂点バッファビューの作成
	VBView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	VBView.SizeInBytes = sizeof(VertexPosUv) * 4;
	VBView.StrideInBytes = sizeof(VertexPosUv);

	//定数バッファの生成
	result = common->dxInit->GetDevice()->
		CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ, 
			nullptr,
			IID_PPV_ARGS(&constBuff)
		);

	assert(SUCCEEDED(result)); // 警告

	//定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = color;
		constMap->mat = common->matProjection;
		constBuff->Unmap(0, nullptr);
	}
}

void Sprite::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// ワールド行列の更新
	matWorld = XMMatrixIdentity();
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	matWorld *= XMMatrixTranslationFromVector(position);

	// 行列の更新
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);   //色指定（RGBA）
	constMap->mat = matWorld * common->matProjection;
	constBuff->Unmap(0, nullptr);

	common->textureManager->SetDescriptorHeaps(cmdList);

	common->textureManager->SetShaderResourcesView(cmdList, 1, texNumber);

	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &VBView);

	//定数バッファのセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// 頂点データ
	VertexPosUv vertices[] =
	{
		{{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}}, // 左下
		{{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}}, // 左上
		{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}}, // 右下
		{{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}}, // 右上
	};

	// 左下、左上、右下、右上
	enum
	{ LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * width;
	float right = (1.0f - anchorpoint.x) * width;
	float top = (0.0f - anchorpoint.y) * height;
	float bottom = (1.0f - anchorpoint.y) * height;

	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	// テクスチャ情報取得
	if (common->textureManager->GetSpriteTexBuff(texNumber))
	{
		D3D12_RESOURCE_DESC resDesc = common->textureManager->GetSpriteTexBuff(texNumber)->GetDesc();

		float tex_left = tex_x / resDesc.Width;
		float tex_right = (tex_x + tex_width) / resDesc.Width;
		float tex_top = tex_y / resDesc.Height;
		float tex_bottom = (tex_y + tex_height) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,	tex_top }; // 左上
		vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,	tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

void Sprite::SetPosition(float x, float y)
{
	this->position = { x,y,0,1 };

	TransferVertices();
}

void Sprite::SetRotation(float rotation)
{
	this->rotation = rotation;

	TransferVertices();
}

void Sprite::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;

	TransferVertices();
}

void Sprite::SetAnchorPoint(XMFLOAT2 anchorpoint)
{
	this->anchorpoint = anchorpoint;

	TransferVertices();
}

void Sprite::SetRect(float tex_x, float tex_y, float tex_width, float tex_height)
{
	this->tex_x = tex_x;
	this->tex_y = tex_y;
	this->tex_width = tex_width;
	this->tex_height = tex_height;

	this->width = tex_width;
	this->height = tex_height;

	TransferVertices();
}

XMFLOAT2 Sprite::GetPosition()
{
	return XMFLOAT2(position.m128_f32[0], position.m128_f32[1]);
}

void Sprite::Common::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob>vsBlob;
	ComPtr<ID3DBlob>psBlob;
	ComPtr<ID3DBlob>errorBlob;

	// 頂点シェーダの読み込みとコンパイル
	result = 
		D3DCompileFromFile(
			L"Resources/Shaders/SpriteVertexShader.hlsl", // シェーダファイル名
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
	result = 
		D3DCompileFromFile(
			L"Resources/Shaders/SpritePixelShader.hlsl", // シェーダファイル名
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
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;   //背面カリングをしない
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);   //一旦標準値をセット
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;   //常に上書きルール

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

	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);   //定数バッファビューとして初期化(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	//ルートシグネチャの生成
	result = dxInit->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	assert(SUCCEEDED(result)); // 警告

	//グラフィックスパイプラインの設定にセット
	gpipeline.pRootSignature = rootSignature.Get();

	result = dxInit->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));

	assert(SUCCEEDED(result)); // 警告
}