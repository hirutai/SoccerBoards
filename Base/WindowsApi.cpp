#include "WindowsAPI.h" 

// �E�B���h�E�v���V�[�W��
LRESULT WindowsApi::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY: // �E�B���h�E���j�����ꂽ
		PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}

	// �W���̏������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowsApi::~WindowsApi()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

void WindowsApi::Initialize()
{
	// �E�B���h�E�N���X�̐ݒ�
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W��
	wndClass.lpszClassName = L"SoccerBoards"; // �E�B���h�E�N���X��
	wndClass.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

	// �E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&wndClass);

	RECT wrc = { 0, 0, windowWidth, windowHeight }; // �E�B���h�E�T�C�Y{ X���W�AY���W�A�����A�c�� }
	// �����ŃT�C�Y�␳
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(
		wndClass.lpszClassName, // �N���X��
		L"SoccerBoards",			// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	// �^�C�g���o�[�Ƌ��E��������E�B���h�E
		CW_USEDEFAULT,			// �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,			// �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,	// �E�B���h�E����
		wrc.bottom - wrc.top,	// �E�B���h�E�c��
		nullptr,				// �e�E�B���h�E�n���h��
		nullptr,				// ���j���[�n���h��
		wndClass.hInstance,		// �Ăяo���A�v���P�[�V�����n���h��
		nullptr					// �I�v�V����
	);

	// �E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

bool WindowsApi::Update()
{
	MSG msg{};	// ���b�Z�[�W

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // ���b�Z�[�W������H
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	if (msg.message == WM_QUIT) // �I�����b�Z�[�W�������烋�[�v�𔲂���
	{
		return true;
	}

	return false;
}