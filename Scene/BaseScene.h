#pragma once

// �O���錾
class SceneManager;

/// <summary>
/// �V�[���̊��
/// </summary>
class BaseScene
{
#pragma region �������z�֐�
public:
	// ���z�f�X�X�g���N�^
	virtual ~BaseScene() = default;
	// ������
	virtual void Initialize() = 0;
	// �X�V
	virtual void Update() = 0;
	// �w�i�`��
	virtual void DrawBackSprite() = 0;
	// 3D�I�u�W�F�N�g�`��
	virtual void DrawObject() = 0;
	// �O�i�`��
	virtual void DrawFrontSprite() = 0;
	// ���
	virtual void Finalize() = 0;
	// �V�[���}�l�[�W���̐ݒ�
	virtual void SetSceneManager(SceneManager* sceneManager_) { sceneManager = sceneManager_; }
#pragma endregion

#pragma region �����o�ϐ�
protected:
	SceneManager* sceneManager = nullptr; // �V�[���}�l�[�W��(���ꕨ)
#pragma endregion
};