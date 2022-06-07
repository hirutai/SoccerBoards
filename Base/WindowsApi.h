#pragma once 
#include <Windows.h>

/// <summary>
/// ウィンドウクラス
/// </summary>
class WindowsApi
{
#pragma region 静的メンバ関数
public:
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ番号</param>
	/// <param name="wparam">メッセージ情報１</param>
	/// <param name="lparam">メッセージ情報２</param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#pragma endregion

#pragma region 静的メンバ変数S
public:
	// ウィンドウサイズ
	static const int windowWidth = 1280; // 横幅
	static const int windowHeight = 760; // 縦幅
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// デスストラクタ
	/// </summary>
	~WindowsApi();

	/// <summary>
	///初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>true=終了、false=続行</returns>
	bool Update();
#pragma endregion

#pragma region getter
public:
	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() { return hwnd; }

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetInstance() { return wndClass.hInstance; }
#pragma endregion

#pragma region メンバ変数
private:
	HWND hwnd = nullptr; // ウィンドウハンドル
	WNDCLASSEX wndClass{}; // ウィンドウクラス
#pragma endregion
};