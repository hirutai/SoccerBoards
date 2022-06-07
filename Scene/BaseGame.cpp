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
	dxInit = make_unique< DirectXInitialize>();
	dxInit->Initialize(winApi.get());
#pragma endregion

#pragma region 入力機能
	input = make_unique<Input>();
	input->Initialize(winApi->GetInstance(), winApi->GetHwnd());
#pragma endregion

#pragma region サウンドマネージャ
	soundManager = make_unique<SoundManager>();
	soundManager->Initialize();
#pragma endregion

#pragma region テクスチャマネージャ
	textureManager = make_unique<TextureManager>();
	textureManager->Initialize(dxInit.get());
#pragma endregion

#pragma region カメラ
	camera = make_unique<Camera>();
	camera->Initialize();
#pragma endregion

#pragma region スプライト
	// スプライトの共通初期化
	Sprite::StaticInitialize(dxInit.get(), textureManager.get());
#pragma endregion

#pragma region .obj読み込みモデル
	// .objオブジェクトモデルの静的初期化
	Obj3dModel::StaticInitialize(dxInit->GetDevice(), textureManager.get());
#pragma endregion

#pragma region .obj読み込みオブジェクト
	// .objオブジェクトの静的初期化
	Obj3dObject::StaticInitialize(dxInit.get(), camera.get());
#pragma endregion

#pragma region デバッグテキスト
	debugText = make_unique<DebugText>();
	debugText->Initialize(0);
#pragma endregion
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
}

void BaseGame::Finalize()
{
	Sprite::StaticFinalize();
	Obj3dObject::StaticFinalize();
}
