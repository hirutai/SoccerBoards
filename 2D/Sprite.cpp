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

	// �O���t�B�b�N�X�p�C�v���C������
	common->InitializeGraphicsPipeline();

	// �ˉe�s��̌v�Z
	common->matProjection = XMMatrixOrthographicOffCenterLH(0.0f, WindowsApi::windowWidth, WindowsApi::windowHeight, 0.0f, 0.0f, 1.0f);
}

void Sprite::StaticFinalize()
{
	delete common;
}

void Sprite::SetPipeline(ID3D12GraphicsCommandList* cmdList)
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(common->pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(common->rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
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

	//���_�o�b�t�@����
	result = common->dxInit->GetDevice()->
		CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff)
		);

	assert(SUCCEEDED(result)); // �x��

	TransferVertices();

	//���_�o�b�t�@�r���[�̍쐬
	VBView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	VBView.SizeInBytes = sizeof(VertexPosUv) * 4;
	VBView.StrideInBytes = sizeof(VertexPosUv);

	//�萔�o�b�t�@�̐���
	result = common->dxInit->GetDevice()->
		CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ, 
			nullptr,
			IID_PPV_ARGS(&constBuff)
		);

	assert(SUCCEEDED(result)); // �x��

	//�萔�o�b�t�@�Ƀf�[�^�]��
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
	// ���[���h�s��̍X�V
	matWorld = XMMatrixIdentity();
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	matWorld *= XMMatrixTranslationFromVector(position);

	// �s��̍X�V
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);   //�F�w��iRGBA�j
	constMap->mat = matWorld * common->matProjection;
	constBuff->Unmap(0, nullptr);

	common->textureManager->SetDescriptorHeaps(cmdList);

	common->textureManager->SetShaderResourcesView(cmdList, 1, texNumber);

	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &VBView);

	//�萔�o�b�t�@�̃Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//�`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// ���_�f�[�^
	VertexPosUv vertices[] =
	{
		{{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}}, // ����
		{{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}}, // ����
		{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}}, // �E��
		{{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}}, // �E��
	};

	// �����A����A�E���A�E��
	enum
	{ LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * width;
	float right = (1.0f - anchorpoint.x) * width;
	float top = (0.0f - anchorpoint.y) * height;
	float bottom = (1.0f - anchorpoint.y) * height;

	if (isFlipX)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	// �e�N�X�`�����擾
	if (common->textureManager->GetSpriteTexBuff(texNumber))
	{
		D3D12_RESOURCE_DESC resDesc = common->textureManager->GetSpriteTexBuff(texNumber)->GetDesc();

		float tex_left = tex_x / resDesc.Width;
		float tex_right = (tex_x + tex_width) / resDesc.Width;
		float tex_top = tex_y / resDesc.Height;
		float tex_bottom = (tex_y + tex_height) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // ����
		vertices[LT].uv = { tex_left,	tex_top }; // ����
		vertices[RB].uv = { tex_right,	tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,	tex_top }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
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

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = 
		D3DCompileFromFile(
			L"Resources/Shaders/SpriteVertexShader.hlsl", // �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&vsBlob, &errorBlob
		);

	if (FAILED(result)) 
	{
		// errorBlob ����G���[���e�� string �^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = 
		D3DCompileFromFile(
			L"Resources/Shaders/SpritePixelShader.hlsl", // �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&psBlob, &errorBlob
		);

	if (FAILED(result))
	{
		// errorBlob ����G���[���e�� string �^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{//xy���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{//uv���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);   //��U�W���l���Z�b�g
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;   //�w�ʃJ�����O�����Ȃ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);   //��U�W���l���Z�b�g
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;   //��ɏ㏑�����[��

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);   //t0 ���W�X�^

	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);   //�萔�o�b�t�@�r���[�Ƃ��ď�����(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	//���[�g�V�O�l�`���̐���
	result = dxInit->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	assert(SUCCEEDED(result)); // �x��

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�ɃZ�b�g
	gpipeline.pRootSignature = rootSignature.Get();

	result = dxInit->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));

	assert(SUCCEEDED(result)); // �x��
}