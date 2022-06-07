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
	dxInit = make_unique< DirectXInitialize>();
	dxInit->Initialize(winApi.get());
#pragma endregion

#pragma region ���͋@�\
	input = make_unique<Input>();
	input->Initialize(winApi->GetInstance(), winApi->GetHwnd());
#pragma endregion

#pragma region �T�E���h�}�l�[�W��
	soundManager = make_unique<SoundManager>();
	soundManager->Initialize();
#pragma endregion

#pragma region �e�N�X�`���}�l�[�W��
	textureManager = make_unique<TextureManager>();
	textureManager->Initialize(dxInit.get());
#pragma endregion

#pragma region �J����
	camera = make_unique<Camera>();
	camera->Initialize();
#pragma endregion

#pragma region �X�v���C�g
	// �X�v���C�g�̋��ʏ�����
	Sprite::StaticInitialize(dxInit.get(), textureManager.get());
#pragma endregion

#pragma region .obj�ǂݍ��݃��f��
	// .obj�I�u�W�F�N�g���f���̐ÓI������
	Obj3dModel::StaticInitialize(dxInit->GetDevice(), textureManager.get());
#pragma endregion

#pragma region .obj�ǂݍ��݃I�u�W�F�N�g
	// .obj�I�u�W�F�N�g�̐ÓI������
	Obj3dObject::StaticInitialize(dxInit.get(), camera.get());
#pragma endregion

#pragma region �f�o�b�O�e�L�X�g
	debugText = make_unique<DebugText>();
	debugText->Initialize(0);
#pragma endregion
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
}

void BaseGame::Finalize()
{
	Sprite::StaticFinalize();
	Obj3dObject::StaticFinalize();
}
