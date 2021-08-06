#include "Precompiled.h"
#include "Graph.h"
#include "PathFindTypes.h"

using namespace JimmyGod;
using namespace JimmyGod::AI;

void Graph::Resize(int columns, int rows)
{
	if (columns == 0 || rows == 0)
		return;

	mColumns = static_cast<int>(columns);
	mRows = static_cast<int>(rows);
	mNodes.resize(columns * rows); // <= It's better than mNodes.resize(mColumns * mRows); since Load-hit-store;

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
				if (GetNode(Coord{ x + rowDirection[i],y + colDirection[i] }))
				{
					mNodes[index].neighbors.push_back({ x + rowDirection[i],y + colDirection[i] });
				}
			}
		}
	}
}

void Graph::Resize3D(float columns, float rows, float radius, const JimmyGod::Math::Vector3& pos)
{
	if (columns == 0 || rows == 0)
		return;

	float nodeDistance = radius * 2.0f;
	worldPos.x = columns;
	worldPos.y = rows;
	mColumns = static_cast<int>(roundf(columns / nodeDistance));
	mRows = static_cast<int>(roundf(rows / nodeDistance));
	mNodes.resize(mColumns * mRows);

	Vector3 startPos = pos - (Vector3::XAxis * columns / 2) - (Vector3::ZAxis * rows / 2);

	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			JimmyGod::AI::Coord present_Coord = { x, y };
			int rowDirection[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
			int colDirection[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
			int index = GetIndex(present_Coord);
			Vector3 point = startPos + (Vector3::XAxis * (x * nodeDistance + radius) + Vector3::ZAxis * (y * nodeDistance + radius));
			
			mNodes[index].position = point;
			mNodes[index].walkable = true;
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