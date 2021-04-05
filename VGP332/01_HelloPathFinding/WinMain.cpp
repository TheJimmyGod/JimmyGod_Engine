#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "GameState.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	JimmyGod::MainApp().AddState<GameState>("GameState");
	JimmyGod::MainApp().Run({ "Hello PathFinding, JimmyGod",1280,720 });
	return 0;
}

// Pathfinding is graph search
// Graph search = {node, edge}
// Big O notation is specifically the worst case of scenario
// Uninformed = not wait
// Breadth-first Search = parallel look friendly