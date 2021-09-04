#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace JimmyCom
{
	class GridManager
	{
	public:
		static void StaticInitialize(JimmyGod::GameWorld* gameWorld);
		static void StaticTerminate();
		static GridManager* Get();

	public:
		void Initialize(JimmyGod::GameWorld* gameWorld);

		const Grid3DComponent& GetGrid() const { return *mGrid->GetComponent<Grid3DComponent>(); }
		Grid3DComponent& GetGird() { return *mGrid->GetComponent<Grid3DComponent>(); }
		const AI::Graph& GetGraph() const { return mGrid->GetComponent<Grid3DComponent>()->GetGraph(); }
	private:
		JimmyGod::GameObject* mGrid = nullptr;
	};
}
