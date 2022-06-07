#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include <string>

#include "DirectXInitialize.h"
#include "TextureManager.h"

/// <summary>
/// .obj���f���N���X
/// </summary>
class Obj3dModel
{
#pragma region �G�C���A�X
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region �萔
public:
	//int division = 3; // ������
	float radius = 5.0f; // ��ʂ̔��a
	float prizmHeight = 8.0f; // ���̍���
	//int planeCount = division * 2 + division * 2; // �ʂ̐�
	//int vertexCount = planeCount * 3; // ���_��
#pragma endregion

#pragma region �T�u�N���X
public:
	// �}�e���A��
	struct Material
	{
		std::string name; // �}�e���A����
		XMFLOAT3 ambient; // �A���r�G���g�e���x
		XMFLOAT3 diffuse; // �f�B�t���[�Y�e���x
		XMFLOAT3 specular; // �X�y�L�����[�e���x
		float alpha; // �A���t�@
		std::string textureFilename; // �e�N�X�`���t�@�C����
		// �R���X�g���N�^
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	// ���_�f�[�^�\����
	struct  Vertex
	{
		DirectX::XMFLOAT3 pos; // ���W
		DirectX::XMFLOAT3 normal; // �@��
		DirectX::XMFLOAT2 uv; // uv
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color; // �F (RGBA)
		DirectX::XMMATRIX mat; // �R�c�ϊ��s��
	};
#pragma endregion

#pragma region �ÓI�����o�֐�
public:
	
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="textureManager">�e�N�X�`���}�l�[�W���[</param>
	static void StaticInitialize(ID3D12Device* device, TextureManager* textureManager);
#pragma endregion

#pragma region �ÓI�����o�ϐ�
private:
	// �f�o�C�X
	static ID3D12Device* device;
	// �e�N�X�`���}�l�[�W���[
	static TextureManager* textureManager; // �e�N�X�`���}�l�[�W���[
#pragma endregion

#pragma region �����o�֐�(public)
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="modelName">���f����</param>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	void Initialize(const std::string& modelName, UINT texNumber);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
#pragma endregion

#pragma region �����o�֐�(private)
private:
	/// <summary>
	/// ���_�f�[�^�̒ǉ�
	/// </summary>
	/// <param name="vertex"></param>
	void AddVertex(const Vertex& vertex);

	/// <summary>
	/// �C���f�b�N�X�f�[�^�̒ǉ�
	/// </summary>
	/// <param name="index"></param>
	void AddIndex(unsigned short index);

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
#pragma endregion

#pragma region getter
public:
	XMFLOAT3 GetAmbient() { return material.ambient; }

	XMFLOAT3 GetDiffuse() { return material.diffuse; }

	XMFLOAT3 GetSpecular() { return material.specular; }

	float GetAlpha() { return material.alpha; }
#pragma endregion

#pragma region �����o�ϐ�
private:
	UINT texNumber = 0;	// �e�N�X�`���ԍ�
	ComPtr<ID3D12Resource> vertBuff; // ���_�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff; // �C���f�b�N�X�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{}; // ���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};	// �C���f�b�N�X�o�b�t�@�r���[

	std::vector<Vertex> vertices; // ���_�f�[�^
	std::vector<unsigned short> indices; // �C���f�b�N�X�f�[�^

	Material material; // �}�e���A��
};