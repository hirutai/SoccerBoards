#include "Input.h" 
#include <cassert>

#pragma comment(lib, "dinput8.lib")

void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInput�̑匳
	ComPtr<IDirectInput8> dinput;

	// DirectInput�I�u�W�F�N�g�̐���		
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	assert(SUCCEEDED(result)); // �x��

	// �L�[�{�[�h�f�o�C�X�̐���
	result = dinput->CreateDevice(GUID_SysKeyboard, &deviceKeyBoard, NULL);

	assert(SUCCEEDED(result)); // �x��

	// ���̓f�[�^�`���̃Z�b�g
	result = deviceKeyBoard->SetDataFormat(&c_dfDIKeyboard); // �W���`��

	assert(SUCCEEDED(result)); // �x��

	// �r�����䃌�x���̃Z�b�g
	result = deviceKeyBoard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	assert(SUCCEEDED(result)); // �x��
}

void Input::Update()
{
	HRESULT result = S_FALSE;

	// ���̓f�o�C�X�̐�����J�n
	result = deviceKeyBoard->Acquire();

	// �O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));

	// ���̓f�o�C�X�i�L�[�j�̓��͏����擾
	result = deviceKeyBoard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �P�̎������Ă���
	if (key[keyNumber]) 
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��0�A���񂪂P�̎��g���K�[
	if (!keyPre[keyNumber] && key[keyNumber]) 
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}