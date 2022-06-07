#include "Obj3dObject.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

Obj3dObject::Common* Obj3dObject::common = nullptr;

void Obj3dObject::StaticInitialize(DirectXInitialize* dxInit, Camera* camera)
{
	common = new Common();
	common->dxInit = dxInit;

	// �O���t�B�b�N�X�p�C�v���C���̏�����
	common->InitializeGraphicsPipeline(dxInit);
	// �f�X�N���v�^�q�[�v�̏�����
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
	// nullptr�`�F�b�N
	assert(model);

	SetModel(model);

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = common->dxInit->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);
	constBuffB0->SetName(L"constBuffB0");

	assert(SUCCEEDED(result)); // �x��

	// �萔�o�b�t�@�̐���
	result = common->dxInit->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1)
	);
	constBuffB1->SetName(L"constBuffB1");

	assert(SUCCEEDED(result)); // �x��

	assert(common->descHeapIndex <= maxObjectCount - 1);

	// �f�X�N���v�^�P���̃T�C�Y
	UINT descHandleIncrementSize = common->dxInit->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// �f�X�N���v�^�q�[�v���ł̒萔�o�b�t�@�r���[�̃A�h���X���v�Z
	cpuDescHandleCBV = CD3DX12_CPU_DESCRIPTOR_HANDLE(common->basicDescHeap->GetCPUDescriptorHandleForHeapStart(), common->descHeapIndex, descHandleIncrementSize);
	gpuDescHandleCBV = CD3DX12_GPU_DESCRIPTOR_HANDLE(common->basicDescHeap->GetGPUDescriptorHandleForHeapStart(), common->descHeapIndex, descHandleIncrementSize);

	// �萔�o�b�t�@�r���[�̐���
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuffB0->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuffB0->GetDesc().Width;
	common->dxInit->GetDevice()->CreateConstantBufferView(&cbvDesc, cpuDescHandleCBV);
	// �f�X�N���v�^�q�[�v�̎g�p�ԍ����P�i�߂�
	common->descHeapIndex++;
}

void Obj3dObject::Update(bool physicsObjectFlag)
{
	HRESULT result;

	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();

	if (physicsObjectFlag) // �����I�u�W�F�N�g�ƃ����N���Ă���I�u�W�F�N�g�̎�
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

	// ���[���h�s�������
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr)
	{
		matWorld *= parent->matWorld; // �e�I�u�W�F�N�g�̃��[���h�s����|����
	}

	const XMMATRIX& matView = common->camera->GetMatView();
	const XMMATRIX& matProjection = common->camera->GetMatProjection();

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	//constMap->color = XMFLOAT4(1, 1, 1, 1);
	constMap->mat = matWorld * matView * matProjection;
	constBuffB0->Unmap(0, nullptr);

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = model->GetAmbient();
	constMap1->diffuse = model->GetDiffuse();
	constMap1->specular = model->GetSpecular();
	constMap1->alpha = model->GetAlpha();
	constBuffB1->Unmap(0, nullptr);
}

void Obj3dObject::Draw(ID3D12GraphicsCommandList* cmdList)
{
#pragma region ���ʕ`��R�}���h
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(common->pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(common->rootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region �ʕ`��R�}���h
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	assert(model); // ���f����null��������x��

	// ���f���`��
	model->Draw(cmdList);
#pragma endregion
}

void Obj3dObject::Common::InitializeGraphicsPipeline(DirectXInitialize* dxInit)
{
	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob>vsBlob;
	ComPtr<ID3DBlob>psBlob;
	ComPtr<ID3DBlob>errorBlob;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/ObjVertexShader.hlsl", // �V�F�[�_�t�@�C����
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
	result = D3DCompileFromFile(
		L"Resources/Shaders/ObjPixelShader.hlsl", // �V�F�[�_�t�@�C����
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
		{//xy���W
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
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
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; 
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);   //��U�W���l���Z�b�g
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;   //�B��Ă�����͕̂`�悵�Ȃ�

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

	//CD3DX12_ROOT_PARAMETER rootparams[2];
	//rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);   //�萔�o�b�t�@�r���[�Ƃ��ď�����(b0)
	//rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_ROOT_PARAMETER rootparams[3]{};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL); // ���W�ϊ��p
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL); // �}�e���A���p
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	//���[�g�V�O�l�`���̐���
	result = dxInit->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	rootSignature->SetName(L"rootSignature");

	assert(SUCCEEDED(result)); // �x��

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�ɃZ�b�g
	gpipeline.pRootSignature = rootSignature.Get();

	result = dxInit->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));
	pipelineState->SetName(L"pipelineState");

	assert(SUCCEEDED(result)); // �x��
}

void Obj3dObject::Common::InitializeDescriptorHeap(DirectXInitialize* dxInit)
{
	HRESULT result = S_FALSE;
	// �f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	descHeapDesc.NumDescriptors = maxObjectCount;
	result = dxInit->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
	basicDescHeap->SetName(L"basicDescHeap");

	assert(SUCCEEDED(result)); // �x��
}