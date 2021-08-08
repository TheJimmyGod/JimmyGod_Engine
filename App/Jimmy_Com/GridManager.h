#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class GridManager
{
public:
	static void StaticInitialize(JimmyGod::GameWorld* gameWorld);
	static void StaticTerminate();
	static GridManager* Get();

public:
	void Initialize(JimmyGod::GameWorld* gameWorld);
	void Terminate();

	const GridComponent& GetGrid() const { return *mGrid->GetComponent<GridComponent>(); }
	GridComponent& GetGird() { return *mGrid->GetComponent<GridComponent>(); }
	const AI::Graph& GetGraph() const { return mGrid->GetComponent<GridComponent>()->GetGraph(); }
private:
	JimmyGod::GameObject* mGrid = nullptr;
};