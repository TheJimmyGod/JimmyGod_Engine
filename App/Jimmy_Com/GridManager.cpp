#include "GridManager.h"
#include <iostream>

using namespace JimmyGod;
using namespace JimmyCom;

namespace
{
	std::unique_ptr<GridManager> sInstance;
}

void GridManager::StaticInitialize(GameWorld* gameWorld)
{
	sInstance = std::make_unique<GridManager>();
	gameWorld->Create("../../Assets/Templates/Grid.json", "Grid");
	sInstance->Initialize(gameWorld);
}

void GridManager::StaticTerminate()
{
	if (sInstance != nullptr)
		sInstance.reset();
}

GridManager* GridManager::Get()
{
	return sInstance.get();
}

void GridManager::Initialize(GameWorld* gameWorld)
{
	mGrid = gameWorld->Find("Grid").Get();
}