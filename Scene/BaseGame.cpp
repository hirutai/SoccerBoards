#include "BaseGame.h"

using namespace std;

void BaseGame::Run()
{
	// ������
	Initialize();

	while (true)
	{
		if (endFlag)
		{
			break;
		}

		// �X�V
		Update();

		// �`��
		Draw();
	}

	Finalize();
}

void BaseGame::Initialize()
{
#pragma region �E�B���h�E
	winApi = make_unique<WindowsApi>();
	winApi->Initialize();
#pragma endregion

#pragma region DirectX������
	dxInit = make_unique<DirectXInitialize>();
	dxInit->Initialize(winApi.get());
#pragma endregion

#pragma region �J����
	camera = Camera::GetInstance();
	camera->Initialize();
#pragma endregion

#pragma region ���͋@�\
	input = Input::GetInstance();
	input->Initialize(winApi->GetInstance(), winApi->GetHwnd());
#pragma endregion

#pragma region �e�N�X�`���}�l�[�W��
	TextureManager::StaticInitialize(dxInit.get());
	textureManager = TextureManager::GetInstance();
	textureManager->Initialize();
#pragma endregion

#pragma region �X�v���C�g
	// �X�v���C�g�̋��ʏ�����
	Sprite::StaticInitialize(dxInit.get());
#pragma endregion

#pragma region �f�o�b�O�e�L�X�g
	debugText = make_unique<DebugText>();
	debugText->Initialize(0);
#pragma endregion

#pragma region �T�E���h�}�l�[�W��
	soundManager = make_unique<SoundManager>();
	soundManager->Initialize();
#pragma endregion

#pragma region .obj3D���f��
	// .obj�I�u�W�F�N�g���f���̐ÓI������
	Obj3dModel::StaticInitialize(dxInit->GetDevice());
#pragma endregion

#pragma region .obj3D�I�u�W�F�N�g
	// .obj�I�u�W�F�N�g�̐ÓI������
	Obj3dObject::StaticInitialize(dxInit.get(), camera);
#pragma endregion

	// �V�[���}�l�[�W���[�̏�����
	sceneManager = new SceneManager();
}

void BaseGame::Update()
{
	// �E�B���h�E�̍X�V
	if (winApi->Update())
	{
		endFlag = true;
	}

	// ���͋@�\�̍X�V
	input->Update();

	// �V�[���̍X�V
	sceneManager->Update();
}

void BaseGame::Draw()
{
	// �`��J�n
	dxInit->BeforeDraw();

	sceneManager->DrawBackSprite();

	dxInit->ClearDepthBuffer();

	sceneManager->DrawObject();

	sceneManager->DrawFrontSprite();

	// �`��I��
	dxInit->AfterDraw();
}

void BaseGame::Finalize()
{
	// �V�[���}�l�[�W���̉��
	delete sceneManager;

	// �I�u�W�F�N�g�̐ÓI���
	Obj3dObject::StaticFinalize();

	// �X�v���C�g�̐ÓI���
	Sprite::StaticFinalize();
}
