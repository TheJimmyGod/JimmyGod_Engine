#pragma once

#include "Common.h"
#include "PathFindTypes.h"

// Generally
namespace JimmyGod::AI
{
	using Path = std::vector<JimmyGod::AI::Coord>;

	class Graph
	{
	public:
		void Resize(int columns, int rows);
		void Resize3D(float columns, float rows, float radius = 1.0f, const JimmyGod::Math::Vector3& pos = JimmyGod::Math::Vector3::Zero);
		// Graph* me = this; (this) is a const pointer

		inline Node* GetNode(Coord coord)
		{
			return const_cast<Node*>(static_cast<const Graph*>(this)->GetNode(coord));
		}

		inline const Node* GetNode(const Coord& coord) const
		{
			if (coord.x < 0 || coord.y < 0 || coord.x >= mColumns || coord.y >= mRows) { return nullptr; }
			int index = GetIndex(coord);
			return &mNodes[index];
		}

		inline const Node* GetNode(const JimmyGod::Math::Vector3& pos) const
		{
			float persentX = static_cast<float>((pos.x + worldPos.x / 2) / worldPos.x);
			float persentY = static_cast<float>((pos.z + worldPos.y / 2) / worldPos.y);
			persentX = Math::Clamp(persentX, 0.0f, 1.0f);
			persentY = Math::Clamp(persentY, 0.0f, 1.0f);

			int x = static_cast<int>(roundf(static_cast<float>((GetColumns() - 1)) * persentX));
			int y = static_cast<int>(roundf(static_cast<float>((GetRows() - 1)) * persentY));
			int index = GetIndex(Coord{ x,y });
			return &mNodes[index];
		}

		int GetColumns() const;
		int GetRows() const;
		inline int GetIndex(Coord coord) const
		{
			return coord.x + (coord.y * mColumns);
		}
		const std::vector<Node>& GetNodes() const { return mNodes; }
		std::vector<Node>& GetNodes() { return mNodes; }
		void clear(){	mNodes.clear(); }
		// Node* ->
		// Node& == Node* const
	private:
		std::vector<JimmyGod::AI::Node> mNodes;
		JimmyGod::Math::Vector2 worldPos = JimmyGod::Math::Vector2::Zero;
		int mColumns;
		int mRows;
	};

	
}

// inline = You don't have to jump it, (odr = One definition rule)