#include "BaseGame.h"

using namespace std;

void BaseGame::Run()
{
	// 初期化
	Initialize();

	while (true)
	{
		if (endFlag)
		{
			break;
		}

		// 更新
		Update();

		// 描画
		Draw();
	}

	Finalize();
}

void BaseGame::Initialize()
{
#pragma region ウィンドウ
	winApi = make_unique<WindowsApi>();
	winApi->Initialize();
#pragma endregion

#pragma region DirectX初期化
	dxInit = make_unique<DirectXInitialize>();
	dxInit->Initialize(winApi.get());
#pragma endregion

#pragma region カメラ
	camera = Camera::GetInstance();
	camera->Initialize();
#pragma endregion

#pragma region 入力機能
	input = Input::GetInstance();
	input->Initialize(winApi->GetInstance(), winApi->GetHwnd());
#pragma endregion

#pragma region テクスチャマネージャ
	TextureManager::StaticInitialize(dxInit.get());
	textureManager = TextureManager::GetInstance();
	textureManager->Initialize();
#pragma endregion

#pragma region スプライト
	// スプライトの共通初期化
	Sprite::StaticInitialize(dxInit.get());
#pragma endregion

#pragma region デバッグテキスト
	debugText = make_unique<DebugText>();
	debugText->Initialize(0);
#pragma endregion

#pragma region サウンドマネージャ
	soundManager = make_unique<SoundManager>();
	soundManager->Initialize();
#pragma endregion

#pragma region .obj3Dモデル
	// .objオブジェクトモデルの静的初期化
	Obj3dModel::StaticInitialize(dxInit->GetDevice());
#pragma endregion

#pragma region .obj3Dオブジェクト
	// .objオブジェクトの静的初期化
	Obj3dObject::StaticInitialize(dxInit.get(), camera);
#pragma endregion

	// シーンマネージャーの初期化
	sceneManager = new SceneManager();
}

void BaseGame::Update()
{
	// ウィンドウの更新
	if (winApi->Update())
	{
		endFlag = true;
	}

	// 入力機能の更新
	input->Update();

	// シーンの更新
	sceneManager->Update();
}

void BaseGame::Draw()
{
	// 描画開始
	dxInit->BeforeDraw();

	sceneManager->DrawBackSprite();

	dxInit->ClearDepthBuffer();

	sceneManager->DrawObject();

	sceneManager->DrawFrontSprite();

	// 描画終了
	dxInit->AfterDraw();
}

void BaseGame::Finalize()
{
	// シーンマネージャの解放
	delete sceneManager;

	// オブジェクトの静的解放
	Obj3dObject::StaticFinalize();

	// スプライトの静的解放
	Sprite::StaticFinalize();
}
