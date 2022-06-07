#include "TextureManager.h" 
#include <cassert>

using namespace DirectX;

void TextureManager::Initialize(DirectXInitialize* dxInit)
{
	HRESULT result = S_FALSE;

	this->dxInit = dxInit;

	//�f�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = maxTextureNumber;
	result = dxInit->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
	
	assert(SUCCEEDED(result)); // �x��

	// SRV�̃T�C�Y�̎擾
	sizeSRV = dxInit->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TextureManager::LoadTexture(UINT texNumber, const wchar_t* filename)
{
	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata;
	ScratchImage scratchlmg;

	result = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, scratchlmg);

	assert(SUCCEEDED(result)); // �x��

	const Image* img = scratchlmg.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
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
			D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
			nullptr,
			IID_PPV_ARGS(&textureBuff[texNumber])
		);

	assert(SUCCEEDED(result)); // �x��

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = 
		textureBuff[texNumber]->WriteToSubresource(
			0,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,//�P���C���T�C�Y
			(UINT)img->slicePitch //�ꖇ�T�C�Y
		);

	assert(SUCCEEDED(result)); // �x��

	//�V�F�[�_�[���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	// �V�F�[�_�[���\�[�X�r���[�̃n���h��
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = 
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeap->GetCPUDescriptorHandleForHeapStart(),
			texNumber,
			dxInit->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

	dxInit->GetDevice()->CreateShaderResourceView(
		textureBuff[texNumber].Get(),   //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,   // �e�N�X�`���ݒ���
		cpuDescHandleSRV
	);
}

void TextureManager::SetDescriptorHeaps(ID3D12GraphicsCommandList* commandList)
{
	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void TextureManager::SetShaderResourcesView(ID3D12GraphicsCommandList* commandList, UINT RootparameterIndex, UINT texNumber)
{
	// �f�X�N���v�^�e�[�u���̐擪�̎擾
	D3D12_GPU_DESCRIPTOR_HANDLE start = descHeap.Get()->GetGPUDescriptorHandleForHeapStart();

	// SRV��GPU�n���h���̎擾
	D3D12_GPU_DESCRIPTOR_HANDLE handle = 
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			start,
			texNumber,
			sizeSRV
		);

	// SRV�̃Z�b�g
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