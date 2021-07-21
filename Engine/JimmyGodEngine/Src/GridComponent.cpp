#include "Precompiled.h"
#include "GridComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

META_DERIVED_BEGIN(GridComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(column, "Column")
		META_FIELD(row, "Row")
		META_FIELD(tileSize,"TileSize")
	META_FIELD_END
META_CLASS_END

void JimmyGod::GridComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mTiles.resize(column * row, 0);
	mGraph.Resize(column, row, mTransformComponent->GetPosition());
	mNode = mGraph.GetNodes();
}
void JimmyGod::GridComponent::Terminate() 
{
	mTiles.clear();
	mNode.clear();
}

void JimmyGod::GridComponent::DebugUI()
{
	if (isDebugUI == false)
		return;
	if (mNode.empty())
		return;
	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < column; x++)
		{
			const int index = GetIndex(x, y);
			float halfSize = tileSize / 2.0f;
			Vector2 pos
			{
				static_cast<float>(x) * tileSize,
				static_cast<float>(y) * tileSize
			};
			if (mGraph.GetNode(AI::Coord{ x,y }))
			{
				for (size_t i = 0; i < mNode[index].neighbors.size(); i++)
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(pos.x + halfSize, pos.y + halfSize), Vector2(
						static_cast<float>(mNode[index].neighbors[i].x * tileSize + halfSize),
						static_cast<float>(mNode[index].neighbors[i].y * tileSize + halfSize)), JimmyGod::Graphics::Colors::Red);
				}
				if(IsZero(mNode[index].position) == false)
					JimmyGod::Graphics::SimpleDraw::AddSphere(mNode[index].position, 0.75f, Colors::Azure, 12, 12);
			}
		}
	}

	switch (mPathFind)
	{
	case JimmyGod::AI::PathFind::BFS:
	{
		auto& closedList = mBFS.GetClosedList();
		auto& parent = mBFS.GetParentList();

		if (closedList.size())
		{
			for (auto node : closedList)
			{
				if (parent[mGraph.GetIndex(node)].IsValid())
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(
						static_cast<float>(node.x) * tileSize + (tileSize / 2),
						static_cast<float>(node.y) * tileSize + (tileSize / 2)),
						Vector2(
							static_cast<float>(parent[mGraph.GetIndex(node)].x) * tileSize + (tileSize / 2),
							static_cast<float>(parent[mGraph.GetIndex(node)].y) * tileSize + (tileSize / 2)),
						JimmyGod::Graphics::Colors::White);

				}
			}
		}
		break;
	}
	case JimmyGod::AI::PathFind::DFS:
	{
		auto& closedList = mDFS.GetClosedList();
		auto& parent = mDFS.GetParentList();
		if (closedList.size())
		{
			for (auto node : closedList)
			{
				if (parent[mGraph.GetIndex(node)].IsValid())
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(
						static_cast<float>(node.x) * tileSize + (tileSize / 2),
						static_cast<float>(node.y) * tileSize + (tileSize / 2)),
						Vector2(
							static_cast<float>(parent[mGraph.GetIndex(node)].x) * tileSize + (tileSize / 2),
							static_cast<float>(parent[mGraph.GetIndex(node)].y) * tileSize + (tileSize / 2)),
						JimmyGod::Graphics::Colors::White);
				}
			}
		}
		break;
	}
	case JimmyGod::AI::PathFind::Dijkstra:
	{
		auto& closedList = mDijkstra.GetClosedList();
		auto& parent = mDijkstra.GetParentList();
		if (closedList.size())
		{
			for (auto node : closedList)
			{
				if (parent[mGraph.GetIndex(node)].IsValid())
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(
						static_cast<float>(node.x) * tileSize + (tileSize / 2),
						static_cast<float>(node.y) * tileSize + (tileSize / 2)),
						Vector2(
							static_cast<float>(parent[mGraph.GetIndex(node)].x) * tileSize + (tileSize / 2),
							static_cast<float>(parent[mGraph.GetIndex(node)].y) * tileSize + (tileSize / 2)),
						JimmyGod::Graphics::Colors::White);
				}
			}
		}
		break;
	}
	case JimmyGod::AI::PathFind::AStar:
	{
		auto& closedList = mAStar.GetClosedList();
		auto& parent = mAStar.GetParentList();
		if (closedList.size())
		{
			for (auto node : closedList)
			{
				if (parent[mGraph.GetIndex(node)].IsValid())
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(
						static_cast<float>(node.x) * tileSize + (tileSize / 2),
						static_cast<float>(node.y) * tileSize + (tileSize / 2)),
						Vector2(
							static_cast<float>(parent[mGraph.GetIndex(node)].x) * tileSize + (tileSize / 2),
							static_cast<float>(parent[mGraph.GetIndex(node)].y) * tileSize + (tileSize / 2)),
						JimmyGod::Graphics::Colors::White);
				}
			}
		}
		break;
	}
	default:
		break;
	}
}

void JimmyGod::GridComponent::CreateGrid(int columns, int rows, int tileSizes)
{
	column = (columns == 0 ? column : columns);
	row = (rows == 0 ? row : rows);
	tileSize = (tileSizes == 0 ? tileSize : tileSizes);

	mTiles.clear();
	mGraph.clear();
	mNode.clear();

	mTiles.resize(column * row, 0);
	mGraph.Resize(column, row, mTransformComponent->GetPosition());
	mNode = mGraph.GetNodes();
}

int JimmyGod::GridComponent::GetIndex(int x, int y) const
{
	return x + (y * column);
}

void JimmyGod::GridComponent::FindPath(const AI::Coord& from, const AI::Coord& to, AI::Path& path)
{
	path.clear();

	AI::Coord start = { from.x,from.y};
	AI::Coord end = { to.x,to.y};

	auto funcBlocked = [this](AI::Coord coord)
	{
		int tile = mTiles[GetIndex(coord.x, coord.y)];
		if (tile == 0)
			return false;
		return true;
	};

	auto funcGetCost = [this](AI::Coord from, AI::Coord to)
	{
		return (from.x != to.x && from.y != to.y) ? 1.414f : 1.0f;
	};

	switch (mPathFind)
	{
	case JimmyGod::AI::PathFind::BFS:
	{
		path = mBFS.Search(mGraph, start, end, funcBlocked);
		break;
	}
	case JimmyGod::AI::PathFind::DFS:
	{
		path = mDFS.Search(mGraph, start, end, funcBlocked);
		break;
	}
	case JimmyGod::AI::PathFind::Dijkstra:
	{
		path = mDijkstra.Search(mGraph, start, end, funcBlocked, funcGetCost);
		break;
	}
	case JimmyGod::AI::PathFind::AStar:
	{
		switch (mAStarType)
		{
		case AI::AStarTypes::Euclidian:
		{
			auto funcGetHeuristic = [this](AI::Coord from, AI::Coord to)
			{
				float xdiff = static_cast<float>(from.x - to.x);
				float ydiff = static_cast<float>(from.y - to.y);
				return sqrtf(xdiff * xdiff + ydiff * ydiff);
			};
			path = mAStar.Search(mGraph, start, end, funcBlocked, funcGetCost, funcGetHeuristic);
			break;
		}
		case AI::AStarTypes::Manhattan:
		{
			auto funcGetHeuristic = [this](AI::Coord from, AI::Coord to)
			{
				return abs(from.x - to.x) + abs(from.y - to.y);
			};
			path = mAStar.Search(mGraph, start, end, funcBlocked, funcGetCost, funcGetHeuristic);
			break;
		}
		case AI::AStarTypes::Chebyshev:
		{
			auto funcGetHeuristic = [this](AI::Coord from, AI::Coord to)
			{
				return JimmyGod::Math::Max((from.x - to.x), (from.y - to.y));
			};
			path = mAStar.Search(mGraph, start, end, funcBlocked, funcGetCost, funcGetHeuristic);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}

}

void JimmyGod::GridComponent::SetPathFind(const char* name)
{
	if (name == "BFS")
		mPathFind = AI::PathFind::BFS;
	else if (name == "DFS")
		mPathFind = AI::PathFind::DFS;
	else if (name == "Dijkstra")
		mPathFind = AI::PathFind::Dijkstra;
	else if (name == "AStar")
		mPathFind = AI::PathFind::AStar;
	else
		return;
}
