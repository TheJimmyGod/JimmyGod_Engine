#pragma once

#include "Component.h"

namespace JimmyGod
{
	class GridComponent final : public Component
	{
	public:
		META_CLASS_DECLARE


		void EnableDebug() { if (!isDebugUI) isDebugUI = true; else isDebugUI = false; }

		void Initialize() override;
		void Terminate() override;
		void DebugUI() override;

		void CreateGrid(int columns, int rows, int tileSizes);
		int GetIndex(int x, int y) const;

		void FindPath(const AI::Coord& from, const AI::Coord& to, AI::Path& path);
		void SetPathFind(const char* name);

		int column = 0;
		int row = 0;
		int tileSize = 0;
		bool is2D = true;

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