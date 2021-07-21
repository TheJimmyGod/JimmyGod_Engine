#include "Precompiled.h"
#include "AStar.h"

using namespace JimmyGod::AI;

void AStar::Reset()
{
	opened.clear();
	closed.clear();
	openList.clear();
	closedList.clear();
	parent.clear();
	g.clear();
	h.clear();
}

Path AStar::Search(
	const Graph& graph, 
	const Coord& start, 
	const Coord& end, 
	std::function<bool(Coord)> isBlocked, 
	std::function<float(Coord, Coord)> getCost, 
	std::function<float(Coord, Coord)> getHeuristic)
{
	Reset();
	const int nodeCount = graph.GetColumns() * graph.GetRows();
	parent.resize(nodeCount);
	opened.resize(nodeCount);
	closed.resize(nodeCount);
	g.resize(nodeCount, 0.0f);
	h.resize(nodeCount, 0.0f);
	// Add Start to the open list
	openList.push_back(start);
	opened[graph.GetIndex(start)] = true;
	h[graph.GetIndex(start)] = getHeuristic(start, end);

	// Keep searching until we are done
	bool found = false;
	while (!found && !openList.empty())
	{
		// Pick the next node from the open list
		Coord current = openList.front();
		openList.pop_front();
		// If node is end, we are done
		if (current == end)
		{
			found = true;
		}
		else // else expand node
		{
			const Node* currentNode = graph.GetNode(current);
			for (auto neighbor : currentNode->neighbors)
			{
				int index = graph.GetIndex(neighbor);

				const float cost = g[graph.GetIndex(current)] + getCost(current, neighbor);
				const float f = cost + getHeuristic(neighbor, end);
				// if the neighbor is blocked, skip it
				if (isBlocked(neighbor) || closed[index])
					continue;

				if (!opened[index])
				{
					opened[index] = true;
					parent[index] = current;
					g[index] = cost;
					h[index] = getHeuristic(neighbor, end);

					// Wrong! Cannot just add to back
					// openList.push_back(neighbor);

					auto it = openList.begin();
					for (; it != openList.end(); it++)
					{
						int getIterIndex = graph.GetIndex(Coord{ it->x, it->y });

						if (f < g[getIterIndex] + h[getIterIndex])
						{
							break;
						}
					}
					openList.insert(it, neighbor);
				}
				else if (cost < g[index])
				{
					openList.remove(neighbor);
					parent[index] = current;
					g[index] = cost;
					// keep h (no code)
					// Remove and re-insert using new f = g + h to sort

					auto it = openList.begin();
					for (; it != openList.end(); it++)
					{
						int getIterIndex = graph.GetIndex(Coord{ it->x, it->y });

						if (f < g[getIterIndex] + h[getIterIndex])
						{
							break;
						}
					}
					openList.insert(it, neighbor);
				}
			}
		}
		// Close node
		closedList.push_back(current);
		closed[graph.GetIndex(current)] = true;
	}

	Path path;
	if (found)
	{
		std::list<Coord> trace;
		Coord next = end;
		while (next.IsValid())
		{
			trace.push_front(next);
			next = parent[graph.GetIndex(next)];
		}
		path.reserve(trace.size());
		for (auto node : trace)
			path.push_back(node);
	}

	return path;
}