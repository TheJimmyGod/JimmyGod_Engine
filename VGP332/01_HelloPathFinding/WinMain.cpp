#include "TileMap.h"
#include <XEngine.h>
#include "Cat.h"

using namespace FSM;

TileMap tilemap;

void GameInit()
{

	tilemap.Load();
}

void GameCleanup()
{
	tilemap.Unload();
}

bool GameLoop(float deltaTime)
{

	tilemap.Update(deltaTime);
	tilemap.Render();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}

// Pathfinding is graph search
// Graph search = {node, edge}
// Big O notation is specifically the worst case of scenario
// Uninformed = not wait
// Breadth-first Search = parallel look friendly