#include "SoccerBoards.h"

// �^�[�Q�b�g��Debug�̎�
#ifdef _DEBUG
int main()
// �^�[�Q�b�g��Debug�łȂ���
#else

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	// ����
	auto game = new SoccerBoards();
	// ���s
	game->Run();
	// ���
	delete game;

	return 0;
}