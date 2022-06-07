#pragma once

#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <Windows.h>
#include <dinput.h>
#include <wrl.h>

/// <summary>
/// 入力クラス
/// </summary>
class Input
{
#pragma region エイリアス
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="hInstance">ウィンドウインスタンス</param>
	/// <param name="hwnd">ウィンドウハンドル</param>
	void Initialize(HINSTANCE hInstance, HWND hwnd);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下のチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>true=押している、false=押してない</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーのチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>true=トリガー、false=トリガーではない</returns>
	bool TriggerKey(BYTE keyNumber);
#pragma endregion

#pragma region メンバ変数
private:
	ComPtr<IDirectInputDevice8> deviceKeyBoard;	// キーボードデバイス用
	BYTE key[256] = {}; // キー情報保存用
	BYTE keyPre[256] = {}; // 前回のキー情報保存用
#pragma endregion
};