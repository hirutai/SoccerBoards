#pragma once 
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXTex.h>

#include "DirectXInitialize.h"

/// <summary>
/// �e�N�X�`���}�l�[�W��
/// </summary>
class TextureManager
{
#pragma region �ÓI�����o�֐�
public:
	// �C���X�^���X�̎擾
	static TextureManager* GetInstance();
	// �ÓI������
	static void StaticInitialize(DirectXInitialize* DxInit);
#pragma endregion

#pragma region �ÓI�����o�ϐ�
public:
	// DirectX������
	static DirectXInitialize* dxInit;
#pragma endregion

#pragma region �G�C���A�X
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

#pragma region �萔
private:
	static const int maxTextureNumber = 512;   //�e�N�X�`���̍ő喇��
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <param name="filename">�t�@�C����</param>
	void LoadTexture(UINT texNumber, const wchar_t* filename);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̐ݒ�
	/// </summary>
	/// <param name="commandList">�R�}���h���X�g</param>
	void SetDescriptorHeaps(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�̐ݒ�
	/// </summary>
	/// <param name="commandList">�R�}���h���X�g</param>
	/// <param name="RootparameterIndex">���[�g�p�����[�^�[�̔ԍ�</param>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	void SetShaderResourcesView(ID3D12GraphicsCommandList* commandList, UINT RootparameterIndex, UINT texNumber);
#pragma endregion

#pragma region getter
public:
	/// <summary>
	/// �e�N�X�`���o�b�t�@�̎擾
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns></returns>
	ID3D12Resource* GetSpriteTexBuff(UINT texNumber);
#pragma endregion

#pragma region �����o�ϐ�
private:
	//DirectXInitialize* dxInit = nullptr; // DirectX������

	ComPtr<ID3D12DescriptorHeap> descHeap; // �f�X�N���v�^�q�[�v
	ComPtr<ID3D12Resource> textureBuff[maxTextureNumber]; // �e�N�X�`���o�b�t�@
	UINT sizeSRV; // SRV�̃T�C�Y�̊i�[�p
#pragma endregion
};