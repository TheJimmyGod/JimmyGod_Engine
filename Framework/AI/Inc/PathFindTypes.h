#pragma once

namespace JimmyGod::AI
{
	enum Direction
	{
		North,
		South,
		East,
		West,
		NorthEast,
		NorthWest,
		SouthEast,
		SouthWest
	};

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


	struct Node
	{
		using NodeID = size_t;
		NodeID id;
		std::vector<Coord> neighbors;
		JimmyGod::Math::Vector3 position = JimmyGod::Math::Vector3::Zero;
		JimmyGod::Math::Sphere collider;
		Coord coordinate;
		bool walkable = true;

		constexpr bool operator== (const Node* other)
		{
			return id == other->id;
		}

		constexpr bool operator!= (const Node* other)
		{
			return id != other->id;
		}

		constexpr bool GetWalkable() const
		{
			return walkable;
		}

		void SetWalkable(bool b)
		{
			walkable = b;
		}
	};

	enum class AStarTypes
	{
		Euclidian,
		Manhattan,
		Chebyshev
	};

	enum class PathFind
	{
		BFS, DFS, Dijkstra, AStar
	};

}