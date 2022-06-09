#pragma once

#include <memory>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "WindowsApi.h"
#include "Sprite.h"
#include "TextureManager.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class Title : public BaseScene
{
private:
	// std::���ȗ�
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;

private:
	const XMFLOAT2 vertCenter{ 0.5f,0.5f }; // �������_
	const XMFLOAT2 screenUpCenter{ WindowsApi::windowWidth / 2, 50.0f }; // ��ʏ㒆��

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

private:
#pragma region �w�i�摜
	unique_ptr<Sprite> backGroundSprite; // �w�i
#pragma endregion

#pragma region �^�C�g���摜
	unique_ptr<Sprite> titleSprite; // �^�C�g��
#pragma endregion
};