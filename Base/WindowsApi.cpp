#include "WindowsAPI.h" 

// ウィンドウプロシージャ
LRESULT WindowsApi::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: // ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}

	// 標準の処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowsApi::~WindowsApi()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

void WindowsApi::Initialize()
{
	// ウィンドウクラスの設定
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャ
	wndClass.lpszClassName = L"SoccerBoards"; // ウィンドウクラス名
	wndClass.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&wndClass);

	RECT wrc = { 0, 0, windowWidth, windowHeight }; // ウィンドウサイズ{ X座標、Y座標、横幅、縦幅 }
	// 自動でサイズ補正
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(
		wndClass.lpszClassName, // クラス名
		L"SoccerBoards",			// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,			// 表示X座標（OSに任せる）
		CW_USEDEFAULT,			// 表示Y座標（OSに任せる）
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		wndClass.hInstance,		// 呼び出しアプリケーションハンドル
		nullptr					// オプション
	);

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

bool WindowsApi::Update()
{
	MSG msg{};	// メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}