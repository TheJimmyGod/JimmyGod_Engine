#include "Precompiled.h"
#include "Graph.h"
#include "PathFindTypes.h"

using namespace JimmyGod;
using namespace JimmyGod::AI;

void Graph::Resize(int columns, int rows, const JimmyGod::Math::Vector3& pos)
{
	mColumns = columns;
	mRows = rows;
	mNodes.resize(columns * rows); // <= It's better than mNodes.resize(mColumns * mRows); since Load-hit-store

	Vector3 startPos = Vector3::Zero;
	if (IsZero(pos) == false)
		startPos = pos - (Vector3::XAxis * columns / 2) - (Vector3::ZAxis * rows / 2);

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			JimmyGod::AI::Coord present_Coord = { x, y };
			int rowDirection[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
			int colDirection[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
			int index = GetIndex(present_Coord);
			Vector3 point = Vector3::Zero;
			if (IsZero(startPos) == false)
				point = startPos + Vector3::XAxis * (x * 0.25f + 0.25f) + Vector3::ZAxis * (y * 0.25f + 0.25f);
			mNodes[index].position = point;
			for (int i = 0; i < 8; ++i)
			{
				if (GetNode(Coord{ x + rowDirection[i],y + colDirection[i] }))
				{
					mNodes[index].neighbors.push_back({ x + rowDirection[i],y + colDirection[i] });
				}
			}
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