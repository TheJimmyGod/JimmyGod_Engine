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

		inline Node* GetNode(Coord coord)
		{
			// Graph* me = this; (this) is a const pointer
			return const_cast<Node*>(static_cast<const Graph*>(this)->GetNode(coord));
		}
		inline const Node* GetNode(const Coord& coord) const
		{
			if (coord.x < 0 || coord.y < 0 || coord.x >= mColumns || coord.y >= mRows)
			{
				return nullptr;
			}
			int index = GetIndex(coord);

			return &mNodes[index];
		}
		int GetColumns() const;
		int GetRows() const;
		inline int GetIndex(Coord coord) const
		{
			return coord.x + (coord.y * mColumns);
		}
		std::vector<Node> GetNodes() const { return mNodes; }
		// Node* ->
		// Node& == Node* const
	private:
		std::vector<JimmyGod::AI::Node> mNodes;
		int mColumns;
		int mRows;
	};

	
}

// inline = You don't have to jump it, (odr = One definition rule)