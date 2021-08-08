#include "GridManager.h"
#include <iostream>

namespace
{
	std::unique_ptr<GridManager> sInstance;
}

void GridManager::StaticInitialize(JimmyGod::GameWorld* gameWorld)
{
	sInstance = std::make_unique<GridManager>();
	gameWorld->Create("../../Assets/Templates/Grid.json", "Grid");
	sInstance->Initialize(gameWorld);
}

void GridManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

GridManager* GridManager::Get()
{
	return sInstance.get();
}

void GridManager::Initialize(JimmyGod::GameWorld* gameWorld)
{
	mGrid = gameWorld->Find("Grid").Get();
}

void GridManager::Terminate()
{
	mGrid->Terminate();
}
