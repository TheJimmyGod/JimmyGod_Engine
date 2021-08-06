#pragma once

#include "Graph.h"

namespace JimmyGod::AI
{
	class BFS
	{
	public:
		void Reset();
		Path Search(const Graph& graph, Coord start, Coord end, std::function<bool(Coord)> isBlocked, float maxDistance);

		const std::list<Coord>& GetClosedList() const { return closedList; }
		const std::vector<Coord>& GetParentList() const { return parent; }
	private:
		std::vector<bool> opened;
		std::vector<bool> closed;
		std::vector<float> g;
		std::list<Coord> openList;
		std::list<Coord> closedList;
		std::vector<Coord> parent;
	};

}