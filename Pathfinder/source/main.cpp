#include "FrameworkPCH.h"
#include "GameCore/Game.h"

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Game pathfinderVisualizer;
	
    pathfinderVisualizer.Run();

    system("pause");

    return 0;
}