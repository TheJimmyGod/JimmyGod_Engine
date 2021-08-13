#include "Precompiled.h"

#include "BFS.h"

using namespace JimmyGod::AI;

void JimmyGod::AI::BFS::Reset()
{
	opened.clear();
	closed.clear();
	openList.clear();
	closedList.clear();
	parent.clear();
	closedListForNode.clear();
}

Path BFS::Search(const Graph & graph, Coord start, Coord end, std::function<bool(Coord)> isBlocked, float maxDistance = 5.0f)
{
	Reset();
	parent.resize(graph.GetColumns() * graph.GetRows());
	opened.resize(graph.GetColumns()* graph.GetRows());
	closed.resize(graph.GetColumns()* graph.GetRows());
	g.resize(graph.GetColumns() * graph.GetRows());

	// Add Start to the open list
	openList.push_back(start);
	opened[graph.GetIndex(start)] = true;

	// Keep searching until we are done
	bool found = false;
	while (!found && !openList.empty())
	{	
		// Pick the next node from the open list
		Coord current = openList.front();
		const Node* currentNode = graph.GetNode(current);
		openList.pop_front();
		// If node is end, we are done
		if (current == end)
		{
			found = true;
		}
		else // else expand node
		{

			for (auto neighbor : currentNode->neighbors)
			{
				int index = graph.GetIndex(neighbor);
				float gCost = g[index] + 1;
				if (!opened[index] && !isBlocked(neighbor) && g[index] < maxDistance)
				{
					openList.push_back(neighbor);
					g[index] = gCost;
					opened[index] = true;
					parent[index] = current;
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

// DeMorgan's Theorem -> !(A || B || C) === !A && !B && !C
//					  -> !(A && B && C) === !A || !B || !C
// {} = narrowing check