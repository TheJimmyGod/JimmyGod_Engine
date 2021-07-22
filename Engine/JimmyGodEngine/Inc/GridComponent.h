#pragma once

#include "Component.h"

namespace JimmyGod
{
	class TransformComponent;

	class GridComponent final : public Component
	{
	public:
		META_CLASS_DECLARE


		void EnableDebug() { if (!isDebugUI) isDebugUI = true; else isDebugUI = false; }

		void Initialize() override;
		void Terminate() override;
		void DebugUI() override;

		void CreateGrid(int columns, int rows, int tileSizes);
		void DisplayClosedListIn2D();
		void DisplayClosedListIn3D();
		int GetIndex(int x, int y) const;

		void FindPath(const AI::Coord& from, const AI::Coord& to, AI::Path& path);
		void SetPathFind(const char* name);


		JimmyGod::Math::Vector2 worldPos = JimmyGod::Math::Vector2::Zero;
		bool is3D= true;
		float tileSize = 0.0f;
		float radius = 0.0f;

	private:
		bool isDebugUI = true;
		bool isActive = true;

		AI::PathFind mPathFind = AI::PathFind::AStar;
		AI::AStarTypes mAStarType = AI::AStarTypes::Manhattan;
		AI::Graph mGraph;
		AI::AStar mAStar;
		AI::Dijkstras mDijkstra;
		AI::BFS mBFS;
		AI::DFS mDFS;
		std::vector<AI::Node> mNode;
		std::vector<int> mTiles;
		const TransformComponent* mTransformComponent = nullptr;
	};
}