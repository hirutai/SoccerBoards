#pragma once 
#include "WindowsApi.h"
#include "DirectXInitialize.h"
#include "Input.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Obj3dModel.h"
#include "Obj3dObject.h"
#include "Sprite.h"
#include "DebugText.h"

#include <memory>

/// <summary>
/// �Q�[���̊��
/// </summary>
class BaseGame
{
#pragma region �G�C���A�X
protected:
	// std::���ȗ�
	//template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �Q�[���iRun�֐��j
	/// </summary>
	virtual void Run();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ���
	/// </summary>
	virtual void Finalize();
#pragma endregion

#pragma region �����o�ϐ�
protected:
	bool endFlag = false; // �p���t���O

	std::unique_ptr<WindowsApi> winApi; // �E�B���h�E
	std::unique_ptr<DirectXInitialize> dxInit; // DirectX������
	std::unique_ptr<Input> input; // ���͋@�\

	std::unique_ptr<SoundManager> soundManager; // �T�E���h�}�l�[�W��
	std::unique_ptr<TextureManager> textureManager; // �e�N�X�`���}�l�[�W��
	std::unique_ptr<Camera> camera; // �J����

	std::unique_ptr<DebugText> debugText; // �f�o�b�O�e�L�X�g
#pragma endregion
};