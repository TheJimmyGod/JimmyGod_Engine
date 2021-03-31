#pragma once

#include "Graph.h"

namespace JimmyGod::AI
{
	class Dijkstras
	{
	public:
		void Reset();
		Path Search(const Graph& graph, const Coord& start, const Coord& end, std::function<bool(Coord)> isBlocked, std::function<float(Coord, Coord)> getCost);

		const std::list<Coord>& GetClosedList() const { return closedList; }
		const std::vector<Coord>& GetParentList() const { return parent; }
	private:
		std::vector<bool> opened;
		std::vector<bool> closed;
		std::vector<float> g;
		std::vector<Coord> parent;
		std::list<Coord> openList;
		std::list<Coord> closedList;
	};

}

// Edge relation

// Graph -> Shortest path tree (SPT)

//
//bool operator[](int i)
//{
//	uint8_t byte = data[i < 8];
//	return (byte & 0x1 >> i % 8);
//}