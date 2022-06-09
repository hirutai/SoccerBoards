#pragma once
#include <d3d12.h>
#include <d3dx12.h>

#include "DirectXInitialize.h"
#include "Obj3dModel.h"
#include "Camera.h"

/// <summary>
/// .obj�I�u�W�F�N�g�N���X
/// </summary>
class Obj3dObject
{
#pragma region �G�C���A�X
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
#pragma endregion

#pragma region �萔
public:
	static const int maxObjectCount = 512; // 3D�I�u�W�F�N�g�̍ő吔
#pragma endregion

#pragma region �C���i�[�N���X
public:
	class Common // ���ʃf�[�^
	{
		friend class Obj3dObject; // Obj3dObject���t�����h�N���X

	public:
		/// <summary>
		/// �O���t�B�b�N�X�p�C�v���C���̏�����
		/// </summary>
		/// <param name="dxInit">DirectX������</param>
		void InitializeGraphicsPipeline(DirectXInitialize* dxInit);

		/// <summary>
		/// �f�X�N���v�^�q�[�v�̏�����
		/// </summary>
		/// <param name="dxInit">DirectX������</param>
		void InitializeDescriptorHeap(DirectXInitialize* dxInit);
	private:
		DirectXInitialize* dxInit = nullptr; // DirectX������
		ComPtr<ID3D12RootSignature> rootSignature; // ���[�g�V�O�l�`��
		ComPtr<ID3D12PipelineState> pipelineState; // �p�C�v���C���X�e�[�g
		ComPtr<ID3D12DescriptorHeap> basicDescHeap; // �f�X�N���v�^�q�[�v�i�萔�o�b�t�@�r���[�p�j
		int descHeapIndex = 0; // ���Ɏg���f�X�N���v�^�q�[�v�ԍ�
		Camera* camera = nullptr; // �J����
	};
#pragma endregion

#pragma region �T�u�N���X
	// �萔�o�b�t�@�p�\����
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// �F(RGBA)
		XMMATRIX mat; // 3D�ϊ��s��
	};
	// �萔�o�b�t�@�p�\����
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; // �A���r�G���g�W��
		float pad1; // �p�f�B���O
		XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
		float pad2; // �p�f�B���O
		XMFLOAT3 specular; // �X�y�L�����[�W��
		float alpha; // �A���t�@
	};
#pragma endregion

#pragma region �ÓI�����o�֐�
public:
	/// <summary>
	/// �ÓI�����o�̏�����
	/// </summary>
	/// <param name="dxInit">DirectX������</param>
	/// <param name="model">���f���f�[�^</param>
	/// <param name="camera">�J����</param>
	static void StaticInitialize(DirectXInitialize* dxInit, Camera* camera);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̃��Z�b�g
	/// </summary>
	static void ResetDescriptorHeap();

	/// <summary>
	/// �ÓI�����o�̉��
	/// </summary>
	static void StaticFinalize();
#pragma endregion

#pragma region �ÓI�����o�ϐ�
private:
	static Common* common;
#pragma endregion

#pragma region �����o�֐�(public)
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Obj3dModel* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(bool physicsObjectFlag = false);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
#pragma endregion

#pragma region �����o�֐�(public)
private:
	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model"></param>
	void SetModel(Obj3dModel* model) { this->model = model; }
#pragma endregion

#pragma region setter
public:

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// <summary>
	/// �X�P�[���̐ݒ�
	/// </summary>
	/// <param name="scale">�X�P�[��</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// ��]�p�̐ݒ�
	/// </summary>
	/// <param name="rotation">��]�p</param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	/// <summary>
	/// �e�I�u�W�F�N�g�̐ݒ�
	/// </summary>
	/// <param name="parentObject">�e�I�u�W�F�N�g</param>
	void SetParent(Obj3dObject* parentObject) { this->parent = parentObject; }
#pragma endregion

#pragma region getter
public:
	/// <summary>
	///  ���W�̎擾
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetPosition() { return position; }

	/// <summary>
	/// �X�P�[���̎擾
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetScale() { return scale; }

	/// <summary>
	/// ��]�p�̎擾
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetRotation() { return rotation; }

	inline XMMATRIX GetMatWorld() { return matWorld; }
#pragma endregion

#pragma region �����o�ϐ�
private:
	Obj3dModel* model = nullptr; // ���f���f�[�^

	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1; // �萔�o�b�t�@
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV; // �萔�o�b�t�@�r���[�̃n���h���iCPU�j
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV; // �萔�o�b�t�@�r���[�̃n���h���iGPU�j
	XMFLOAT3 position = { 0,0,0 }; // ���W
	XMFLOAT3 scale = { 1,1,1 }; // �X�P�[��
	XMFLOAT3 rotation = { 0,0,0 }; // ��]�p
	XMMATRIX matWorld{}; // ���[���h�ϊ��s��
	Obj3dObject* parent = nullptr;	// �e�I�u�W�F�N�g
#pragma endregion
};