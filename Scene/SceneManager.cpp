#include "SceneManager.h"

SceneManager::~SceneManager()
{
	// �Ō�̃V�[���̏I���Ɖ��
	scene_->Finalize();
	delete scene_;
}

void SceneManager::Update()
{
	if (nextScene_)	// �V�[���؂�ւ������ꂽ��
	{
		if (scene_) // ���V�[��������Ȃ�
		{
			scene_->Finalize();
			delete scene_;
		}

		// �V�[���̐؂�ւ�
		scene_ = nextScene_;
		nextScene_ = nullptr;

		// �V�[���}�l�[�W���̐ݒ�
		scene_->SetSceneManager(this);

		// �V�V�[���̏�����
		scene_->Initialize();
	}

	scene_->Update();
}

void SceneManager::DrawBackSprite()
{
	scene_->DrawBackSprite();
}

void SceneManager::DrawObject()
{
	scene_->DrawObject();
}

void SceneManager::DrawFrontSprite()
{
	scene_->DrawFrontSprite();
}

void SceneManager::Finalize()
{
	scene_->Finalize();
}