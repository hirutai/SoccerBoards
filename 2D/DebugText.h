#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>

#include "Sprite.h"

/// <summary>
/// �f�o�b�O�e�L�X�g�N���X
/// </summary>
class DebugText
{
#pragma region �萔
public:
	static const int maxCharCount = 256; // �ő啶����
	static const int fontWidth = 9; // �t�H���g�摜���P�������̉���
	static const int fontHeight = 18; // �t�H���g�摜���P�������̏c��
	static const int fontLineCount = 14; // �t�H���g�摜���P�s���̕�����
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �f�X�X�g���N�^
	/// </summary>
	~DebugText();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	void Initialize(UINT texNumber);

	/// <summary>
	/// �v�����g
	/// </summary>
	/// <param name="text">�e�L�X�g</param>
	/// <param name="x">x���W</param>
	/// <param name="y">y���W</param>
	/// <param name="scale">�X�P�[��</param>
	void Print(const std::string& text, float x, float y, float scale);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void DrawAll(ID3D12GraphicsCommandList* cmdList);
#pragma endregion

#pragma region �����o�ϐ�
private:
	Sprite* sprites[maxCharCount]; // �X�v���C�g�f�[�^�z��
	int spriteIndex = 0; // �X�v���C�g�f�[�^�z��̔ԍ�
#pragma endregion
};