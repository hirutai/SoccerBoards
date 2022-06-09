#pragma once

#include <memory>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "Sprite.h"
#include "TextureManager.h"

/// <summary>
/// ��������V�[��
/// </summary>
class Operation : public BaseScene
{
#pragma region �G�C���A�X
private:
	// std::���ȗ�
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
#pragma endregion

#pragma region �����o�֐�
public:
	// ������
	void Initialize() override;
	// �X�V
	void Update() override;
	// �w�i�`��
	void DrawBackSprite() override;
	// 3D�I�u�W�F�N�g�`��
	void DrawObject() override;
	// �O�i�`��
	void DrawFrontSprite() override;
	// ���
	void Finalize() override;
#pragma endregion

#pragma region �����o�ϐ�
private:
	unique_ptr<Sprite> backGroundSprite; // �w�i

	unique_ptr<Sprite> operationSprite; // �������
};