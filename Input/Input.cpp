#include "Input.h" 
#include <cassert>

#pragma comment(lib, "dinput8.lib")

void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInputの大元
	ComPtr<IDirectInput8> dinput;

	// DirectInputオブジェクトの生成		
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	assert(SUCCEEDED(result)); // 警告

	// キーボードデバイスの生成
	result = dinput->CreateDevice(GUID_SysKeyboard, &deviceKeyBoard, NULL);

	assert(SUCCEEDED(result)); // 警告

	// 入力データ形式のセット
	result = deviceKeyBoard->SetDataFormat(&c_dfDIKeyboard); // 標準形式

	assert(SUCCEEDED(result)); // 警告

	// 排他制御レベルのセット
	result = deviceKeyBoard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	assert(SUCCEEDED(result)); // 警告
}

void Input::Update()
{
	HRESULT result = S_FALSE;

	// 入力デバイスの制御を開始
	result = deviceKeyBoard->Acquire();

	// 前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	// 入力デバイス（キー）の入力情報を取得
	result = deviceKeyBoard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// １の時押している
	if (key[keyNumber]) 
	{
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 前回が0、今回が１の時トリガー
	if (!keyPre[keyNumber] && key[keyNumber]) 
	{
		return true;
	}

	// トリガーでない
	return false;
}