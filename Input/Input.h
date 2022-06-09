#pragma once

#define DIRECTINPUT_VERSION 0x0800 // DirectInput�̃o�[�W�����w��
#include <Windows.h>
#include <dinput.h>
#include <wrl.h>

/// <summary>
/// ���̓N���X
/// </summary>
class Input
{
#pragma region �G�C���A�X
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

#pragma region �ÓI�����o�֐�
public:
	// �C���X�^���X�̐���
	static Input* GetInstance();
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="hInstance">�E�B���h�E�C���X�^���X</param>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	void Initialize(HINSTANCE hInstance, HWND hwnd);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �L�[�̉����̃`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�</param>
	/// <returns>true=�����Ă���Afalse=�����ĂȂ�</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// �L�[�̃g���K�[�̃`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�</param>
	/// <returns>true=�g���K�[�Afalse=�g���K�[�ł͂Ȃ�</returns>
	bool TriggerKey(BYTE keyNumber);
#pragma endregion

#pragma region �����o�ϐ�
private:
	ComPtr<IDirectInputDevice8> deviceKeyBoard;	// �L�[�{�[�h�f�o�C�X�p
	BYTE key[256] = {}; // �L�[���ۑ��p
	BYTE keyPre[256] = {}; // �O��̃L�[���ۑ��p
#pragma endregion
};