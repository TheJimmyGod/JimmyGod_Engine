#pragma once

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

	struct Node
	{
		using NodeID = size_t;
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


}