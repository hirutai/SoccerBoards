#include "SoccerBoards.h"

// ターゲットがDebugの時
#ifdef _DEBUG
int main()
// ターゲットがDebugでない時
#else

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	// 生成
	auto game = new SoccerBoards();
	// 実行
	game->Run();
	// 解放
	delete game;

	return 0;
}