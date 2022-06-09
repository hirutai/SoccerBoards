#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "DirectXInitialize.h"

// �O���錾
class TextureManager;

/// <summary>
/// �X�v���C�g�N���X
/// </summary>
class Sprite
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

#pragma region �C���i�[�N���X
private:
	class Common // ���ʃf�[�^
	{
		friend class Sprite; // Sprite���t�����h�N���X

	public:
		/// <summary>
		/// �O���t�B�b�N�X�p�C�v���C���̏�����
		/// </summary>
		void InitializeGraphicsPipeline();

		DirectXInitialize* dxInit = nullptr; // DirectX������
	private:
		//TextureManager* textureManager = nullptr; // �e�N�X�`���}�l�[�W���[
		ComPtr<ID3D12RootSignature> rootSignature; // ���[�g�V�O�l�`��
		ComPtr<ID3D12PipelineState> pipelineState; // �p�C�v���C���X�e�[�g
		XMMATRIX matProjection{}; // �ˉe�s��
	};
#pragma endregion

#pragma region �T�u�N���X
private:
	// ���_�f�[�^�\����
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT2 uv; // uv���W
	};

	// �萔�o�b�t�@�f�[�^�\����
	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color; // �F(RGBA)
		DirectX::XMMATRIX mat; //3D�ϊ��s��
	};
#pragma endregion

#pragma region �ÓI�����o�֐�
public:
	/// <summary>
	/// �ÓI�����o�̏�����
	/// </summary>
	/// <param name="dxInit">DirectX������</param>
	static void StaticInitialize(DirectXInitialize* dxInit);

	/// <summary>
	/// �ÓI�����o�̉��
	/// </summary>
	static void StaticFinalize();

	/// <summary>
	/// �p�C�v���C���̐ݒ�
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void SetPipeline();
#pragma endregion

#pragma region �ÓI�����o�ϐ�
private:
	static Common* common; // ���L�f�[�^

#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	void Initialize(UINT texNumber);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���_�o�b�t�@�Ƀf�[�^�]��
	/// </summary>
	void TransferVertices();
#pragma endregion

#pragma region setter
	/// <summary>
	/// �|�W�V�����̐ݒ�
	/// </summary>
	/// <param name="x">x���W</param>
	/// <param name="y">y���W</param>
	void SetPosition(float x, float y);

	/// <summary>
	/// ��]�p�̐ݒ�
	/// </summary>
	/// <param name="rotation">��]�p</param>
	void SetRotation(float rotation);

	/// <summary>
	/// �T�C�Y�̐ݒ�
	/// </summary>
	/// <param name="width">����</param>
	/// <param name="height">�c��</param>
	void SetSize(float width, float height);

	/// <summary>
	/// �A���J�[�|�C���g�̐ݒ�
	/// </summary>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	void SetAnchorPoint(XMFLOAT2 anchorpoint);

	/// <summary>
	/// �؂���͈͎w��̐ݒ�
	/// </summary>
	/// <param name="tex_x">�e�N�X�`����x���_</param>
	/// <param name="tex_y">�e�N�X�`����y���_</param>
	/// <param name="tex_width">�e�N�X�`���̉���</param>
	/// <param name="tex_height">�e�N�X�`���̏c��</param>
	void SetRect(float tex_x, float tex_y, float tex_width, float tex_height);
#pragma endregion

#pragma region getter
	/// <summary>
	/// �|�W�V�����̎擾
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 GetPosition();
#pragma endregion

#pragma region �����o�ϐ�
private:
	ComPtr<ID3D12Resource> vertBuff; // ���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW VBView; // ���_�o�b�t�@�r���[
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	UINT texNumber;	// �e�N�X�`���ԍ�
	XMVECTOR position{}; // ���W
	float rotation = 0;	// ��]�p
	XMFLOAT4 color{ 1,1,1,1 }; // �F
	XMMATRIX matWorld{}; // ���[���h�s��
	float width = 100.0f; // ����
	float height = 100.0f; // �c��
	XMFLOAT2 anchorpoint{};	// �A���J�[�|�C���g
	bool isFlipX = false; // ���E���]
	bool isFlipY = false; // �㉺���]
	float tex_x = 0.0f; // �؂���͈�
	float tex_y = 0.0f;
	float tex_width = 100.0f;
	float tex_height = 100.0f;
#pragma endregion
};