#include "Precompiled.h"
#include "Graph.h"
#include "PathFindTypes.h"

using namespace JimmyGod;
using namespace JimmyGod::AI;

void Graph::Resize(int columns, int rows)
{
	mColumns = columns;
	mRows = rows;
	mNodes.resize(columns * rows); // <= It's better than mNodes.resize(mColumns * mRows); since Load-hit-store

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			JimmyGod::AI::Coord present_Coord = { x, y };
			int rowDirection[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
			int colDirection[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
			int index = GetIndex(present_Coord);

			for (int i = 0; i < 8; ++i)
			{
				if (GetNode({ x + rowDirection[i],y + colDirection[i] }))
				{
					mNodes[index].neighbors.push_back({ x + rowDirection[i],y + colDirection[i] });
				}
			}
			// Homework : Connect neighbors to each node (up to 8, maybe less)
		}
	}
}

int Graph::GetColumns() const
{
	return mColumns;
}

int Graph::GetRows() const
{
	return mRows;
}
// Const_cast means remove const