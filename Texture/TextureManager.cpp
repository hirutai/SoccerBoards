#include "TextureManager.h" 
#include <cassert>

using namespace DirectX;

void TextureManager::Initialize(DirectXInitialize* dxInit)
{
	HRESULT result = S_FALSE;

	this->dxInit = dxInit;

	//デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = maxTextureNumber;
	result = dxInit->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
	
	assert(SUCCEEDED(result)); // 警告

	// SRVのサイズの取得
	sizeSRV = dxInit->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TextureManager::LoadTexture(UINT texNumber, const wchar_t* filename)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata;
	ScratchImage scratchlmg;

	result = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, scratchlmg);

	assert(SUCCEEDED(result)); // 警告

	const Image* img = scratchlmg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = 
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT)metadata.arraySize,
			(UINT)metadata.mipLevels
		);

	result = 
		dxInit->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
			nullptr,
			IID_PPV_ARGS(&textureBuff[texNumber])
		);

	assert(SUCCEEDED(result)); // 警告

	// テクスチャバッファにデータ転送
	result = 
		textureBuff[texNumber]->WriteToSubresource(
			0,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,//１ラインサイズ
			(UINT)img->slicePitch //一枚サイズ
		);

	assert(SUCCEEDED(result)); // 警告

	//シェーダーリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	// シェーダーリソースビューのハンドル
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = 
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeap->GetCPUDescriptorHandleForHeapStart(),
			texNumber,
			dxInit->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

	dxInit->GetDevice()->CreateShaderResourceView(
		textureBuff[texNumber].Get(),   //ビューと関連付けるバッファ
		&srvDesc,   // テクスチャ設定情報
		cpuDescHandleSRV
	);
}

void TextureManager::SetDescriptorHeaps(ID3D12GraphicsCommandList* commandList)
{
	//デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void TextureManager::SetShaderResourcesView(ID3D12GraphicsCommandList* commandList, UINT RootparameterIndex, UINT texNumber)
{
	// デスクリプタテーブルの先頭の取得
	D3D12_GPU_DESCRIPTOR_HANDLE start = descHeap.Get()->GetGPUDescriptorHandleForHeapStart();

	// SRVのGPUハンドルの取得
	D3D12_GPU_DESCRIPTOR_HANDLE handle = 
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			start,
			texNumber,
			sizeSRV
		);

	// SRVのセット
	commandList->SetGraphicsRootDescriptorTable(
			RootparameterIndex,
			handle
	);
}

ID3D12Resource* TextureManager::GetSpriteTexBuff(UINT texNumber)
{
	if (texNumber >= maxTextureNumber)
	{
		return nullptr;
	}

	return textureBuff[texNumber].Get();
}