#pragma once
#include "Graph.h"
namespace JimmyGod::AI
{
	class DFS
	{
	public:
		void Reset();
		Path Search(const Graph& graph, const Coord& start, const Coord& end, std::function<bool(Coord)> isBlocked);

		const std::list<Coord>& GetClosedList() const { return closedList; }
		const std::vector<Coord>& GetParentList() const { return parent; }
	private:
		std::vector<bool> opened;
		std::vector<bool> closed;
		std::list<Coord> openList;
		std::list<Coord> closedList;
		std::vector<Coord> parent;
	};
}