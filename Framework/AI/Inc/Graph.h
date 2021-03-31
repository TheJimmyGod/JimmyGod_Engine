#pragma once

#include "Common.h"
// namespace JimmyGod::AI
// {
// }

// Generally
namespace JimmyGod::AI
{
	struct Coord
	{
		int x = -1;
		int y = -1;

		bool IsValid() const
		{
			return x != -1 && y != -1;
		}

		constexpr bool operator== (const Coord& rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}

		constexpr Coord operator- (const Coord& rhs) const
		{
			return { x - rhs.x, y - rhs.y };
		}
	};

	using Path = std::vector<Coord>;

	class Graph
	{
	public:
		using NodeID = size_t;
		struct Node
		{
			NodeID id;
			std::vector<Coord> neighbors;

			constexpr bool operator== (const Node* other)
			{
				return id == other->id;
			}

			constexpr bool operator!= (const Node* other)
			{
				return id != other->id;
			}

		};



		void Resize(int columns, int rows);

		Node* GetNode(Coord coord);
		const Node* GetNode(const Coord& coord) const;
		int GetColumns() const;
		int GetRows() const;
		int GetIndex(Coord coord) const;
		std::vector<Node> GetNodes() const { return mNodes; }
		// Node* ->
		// Node& == Node* const
	private:
		std::vector<Node> mNodes;
		int mColumns;
		int mRows;
	};

	
}

// inline = You don't have to jump it, (odr = One definition rule)