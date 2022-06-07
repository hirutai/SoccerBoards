#pragma once 
#include <Windows.h>

/// <summary>
/// �E�B���h�E�N���X
/// </summary>
class WindowsApi
{
#pragma region �ÓI�����o�֐�
public:
	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	/// <param name="msg">���b�Z�[�W�ԍ�</param>
	/// <param name="wparam">���b�Z�[�W���P</param>
	/// <param name="lparam">���b�Z�[�W���Q</param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#pragma endregion

#pragma region �ÓI�����o�ϐ�S
public:
	// �E�B���h�E�T�C�Y
	static const int windowWidth = 1280; // ����
	static const int windowHeight = 760; // �c��
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �f�X�X�g���N�^
	/// </summary>
	~WindowsApi();

	/// <summary>
	///������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <returns>true=�I���Afalse=���s</returns>
	bool Update();
#pragma endregion

#pragma region getter
public:
	/// <summary>
	/// �E�B���h�E�n���h���̎擾
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() { return hwnd; }

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetInstance() { return wndClass.hInstance; }
#pragma endregion

#pragma region �����o�ϐ�
private:
	HWND hwnd = nullptr; // �E�B���h�E�n���h��
	WNDCLASSEX wndClass{}; // �E�B���h�E�N���X
#pragma endregion
};