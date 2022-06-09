#pragma once
#include "BaseScene.h"

/// <summary>
/// �V�[���Ǘ�
/// </summary>
class SceneManager
{
#pragma region �����o�֐�
public:
	// �f�X�X�g���N�^
	~SceneManager();
	// �X�V
	void Update();
	// �w�i�`��
	void DrawBackSprite();
	// 3D�I�u�W�F�N�g�`��
	void DrawObject();
	// �O�i�`��
	void DrawFrontSprite();
	// ���
	void Finalize();

	// ���̃V�[���̐ݒ�
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

#pragma region �����o�ϐ�
private:
	BaseScene* scene_ = nullptr; // ���݂̃V�[��

	BaseScene* nextScene_ = nullptr; // ���̃V�[��
#pragma endregion
};