#pragma once

#include "Graph.h"

namespace JimmyGod::AI
{
	class AStar
	{
	public:

		void Reset();
		Path Search(const Graph& graph, 
			const Coord& start, 
			const Coord& end, 
			std::function<bool(Coord)> isBlocked, 
			std::function<float(Coord, Coord)> getCost, 
			std::function<float(Coord,Coord)> getHeuristic);

		const std::list<Coord>& GetClosedList() const { return closedList; }
		const std::vector<Coord>& GetParentList() const { return parent; }
		
	private:
		std::vector<bool> opened;
		std::vector<bool> closed;
		std::vector<float> g;
		std::vector<float> h;
		std::vector<Coord> parent;
		std::list<Coord> openList;
		std::list<Coord> closedList;
	};

}