#include "FrameworkPCH.h"
#include "Game.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	const Game pathfinderVisualizer("Pathfinder Visualizer", { 1200, 600 });
	pathfinderVisualizer.Run();

	return 0;
}