#include "Precompiled.h"
#include "DFS.h"

using namespace JimmyGod::AI;

void DFS::Reset()
{
	opened.clear();
	closed.clear();
	openList.clear();
	closedList.clear();
	parent.clear();
}

Path DFS::Search(const Graph & graph, const Coord & start, const Coord & end, std::function<bool(Coord)> isBlocked)
{
	Reset();
	parent.resize(graph.GetColumns() * graph.GetRows());
	opened.resize(graph.GetColumns()* graph.GetRows());
	closed.resize(graph.GetColumns()* graph.GetRows());

	// Add Start to the open list
	openList.push_back(start);
	opened[graph.GetIndex(start)] = true;

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
			const Graph::Node* currentNode = graph.GetNode(current);
			for (auto neighbor : currentNode->neighbors)
			{
				int index = graph.GetIndex(neighbor);
				//isBlocked = [&](Coord coord) {return graph.GetNode(coord); };
				if (!opened[index] && !isBlocked(neighbor))
				{
					openList.push_front(neighbor);
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
