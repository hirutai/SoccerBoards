#include "SceneManager.h"

SceneManager::~SceneManager()
{
	// 最後のシーンの終了と解放
	scene_->Finalize();
	delete scene_;
}

void SceneManager::Update()
{
	if (nextScene_)	// シーン切り替えがされたら
	{
		if (scene_) // 旧シーンがあるなら
		{
			scene_->Finalize();
			delete scene_;
		}

		// シーンの切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		// シーンマネージャの設定
		scene_->SetSceneManager(this);

		// 新シーンの初期化
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