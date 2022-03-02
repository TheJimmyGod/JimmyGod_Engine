#pragma once

#include "Component.h"

namespace JimmyGod
{
	class TransformComponent;

	class Grid3DComponent final : public Component
	{
	public:
		META_CLASS_DECLARE


		void EnableDebug() { if (!isDebugUI) isDebugUI = true; else isDebugUI = false; }

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		void CreateGrid(int columns, int rows, int tileSizes);
		void DisplayClosedListIn2D();
		void DisplayClosedListIn3D();
		void DisplayAreaCube(int area, const Math::Vector3& pos, const JimmyGod::Graphics::Color& color);
		JimmyGod::Math::Vector3 FindClosestPath(int area, const Math::Vector3& curr, const Math::Vector3& dest);

		int GetIndex(int x, int y) const;

		void FindPath(const AI::Coord& from, const AI::Coord& to, std::vector<Math::Vector3>& newPath);
		void SetPathFind(const char* name);

		bool Inside_Circle(const Math::Vector3& center, const Math::Vector3& tile, int radius) const;

		const int GetColumns() const { return mGraph.GetColumns(); }
		const int GetRows() const { return mGraph.GetRows(); }

		const AI::Graph& GetGraph() const { return mGraph; }
		AI::Graph& GetGraph() { return mGraph; }

		auto& GetNodes() { return mNode; }

		JimmyGod::Math::Vector2 worldPos = JimmyGod::Math::Vector2::Zero;
		float tileRadius3D = 0.0f;

	private:
		bool isDebugUI = false;
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
		
		JimmyGod::Math::Vector3 mPos = JimmyGod::Math::Vector3::Zero;
		JimmyGod::Graphics::TextureId mTextureIds[10];
		const TransformComponent* mTransformComponent = nullptr;
	};
}