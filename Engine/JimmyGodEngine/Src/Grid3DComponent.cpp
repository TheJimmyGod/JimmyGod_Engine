#include "Precompiled.h"
#include "Grid3DComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

META_DERIVED_BEGIN(Grid3DComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(worldPos, "WorldPos")
		META_FIELD(tileRadius3D,"TileRadius3D")
	META_FIELD_END
META_CLASS_END

void JimmyGod::Grid3DComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mGraph.Resize3D(worldPos.x, worldPos.y, tileRadius3D, mTransformComponent->GetPosition());
	mTiles.resize(mGraph.GetColumns() * mGraph.GetRows(), 0);
	mNode = mGraph.GetNodes();
}
void JimmyGod::Grid3DComponent::Terminate() 
{
	mTiles.clear();
	mNode.clear();
}

void JimmyGod::Grid3DComponent::Update(float deltaTime)
{
	auto inputSystem = JimmyGod::Input::InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		if(maxY - 1 > mCurrentCoordinate.y)
			mCurrentCoordinate.y += 1;
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		if (minY < mCurrentCoordinate.y)
			mCurrentCoordinate.y -= 1;
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		if (maxX -1 > mCurrentCoordinate.x)
			mCurrentCoordinate.x += 1;
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		if(minX < mCurrentCoordinate.x)
			mCurrentCoordinate.x -= 1;
	}
}

void JimmyGod::Grid3DComponent::DebugUI()
{
	if (mNode.empty() || !isDebugUI) return;
	
	for (int y = 0; y < mGraph.GetRows(); y++)
	{
		for (int x = 0; x < mGraph.GetColumns(); x++)
		{
			const int index = GetIndex(x, y);

			if (mGraph.GetNode(AI::Coord{ x,y }))
			{
				if(mNode[index].GetWalkable())
					JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(mNode[index].position, 0.5f), Colors::Green);
				else
					JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(mNode[index].position, 2.0f), Colors::Red);
			}
		}
	}
	DisplayClosedListIn3D();
}

void JimmyGod::Grid3DComponent::ObjectPosition(const Math::Vector3 pos)
{
	mCurrentCoordinate = mGraph.GetNode(pos)->coordinate;
}

void JimmyGod::Grid3DComponent::CreateGrid(int columns, int rows, int tileSize2Ds)
{
	if (columns == 0 || rows == 0)
		return;

	mTiles.clear();
	mGraph.clear();
	mNode.clear();

	mGraph.Resize3D(worldPos.x, worldPos.y, tileRadius3D, mTransformComponent->GetPosition());
	mTiles.resize(mGraph.GetColumns() * mGraph.GetRows(), 0);
	mNode = mGraph.GetNodes();
}

void JimmyGod::Grid3DComponent::DisplayClosedListIn2D()
{
	std::list<JimmyGod::AI::Coord> closedList;
	JimmyGod::AI::Path parent;
	switch (mPathFind)
	{
	case JimmyGod::AI::PathFind::BFS:
	{
		closedList = mBFS.GetClosedList();
		parent = mBFS.GetParentList();
		break;
	}
	case JimmyGod::AI::PathFind::DFS:
	{
		closedList = mDFS.GetClosedList();
		parent = mDFS.GetParentList();
		break;
	}
	case JimmyGod::AI::PathFind::Dijkstra:
	{
		closedList = mDijkstra.GetClosedList();
		parent = mDijkstra.GetParentList();
		break;
	}
	case JimmyGod::AI::PathFind::AStar:
	{
		closedList = mAStar.GetClosedList();
		parent = mAStar.GetParentList();
		break;
	}
	default: break;
	}
}

void JimmyGod::Grid3DComponent::DisplayClosedListIn3D()
{
}

void JimmyGod::Grid3DComponent::DisplayAreaCube(int area, const Vector3& pos, const JimmyGod::Graphics::Color& color)
{
	CalculateGrid(area, pos);

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			const int index = GetIndex(x, y);
			if (mGraph.GetNode(AI::Coord{ x,y }))
			{
				if (mNode[index].GetWalkable())
					JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(mNode[index].position, 1.0f), color);
			}
		}
	}

	auto n = mGraph.GetNode(mCurrentCoordinate);
	if (n != nullptr)
		JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(n->position, 1.0f), Colors::Blue);
}

JimmyGod::Math::Vector3 JimmyGod::Grid3DComponent::FindClosestPath(int area, const Math::Vector3& curr, const Math::Vector3& dest)
{
	CalculateGrid(area, curr);

	float minDist = FLT_MAX;
	JimmyGod::Math::Vector3 nodePos = JimmyGod::Math::Vector3::Zero;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			const int index = GetIndex(x, y);
			if (mGraph.GetNode(AI::Coord{ x,y }))
			{
				if (mNode[index].GetWalkable() == false)
					continue;
				auto pos = mNode[index].position;
				const float dist = Distance(pos, dest);
				if (minDist > dist)
				{
					minDist = dist;
					nodePos = pos;
				}
			}
		}
	}

	return nodePos;
}

void JimmyGod::Grid3DComponent::CalculateGrid(int area, const JimmyGod::Math::Vector3& pos)
{
	AI::Coord c = mGraph.GetNode(pos)->coordinate;
	minY = Max(c.y - area + 1, 1);
	minX = Max(c.x - area + 1, 1);
	maxY = Min(c.y + area, mGraph.GetRows());
	maxX = Min(c.x + area, mGraph.GetColumns());

	int newMaxX = maxX;
	int newMaxY = maxY;
	int newMinX = minX;
	int newMinY = minY;

	for (int y = minY; y < maxY; y++)
	{
		int unWalkableMinY = 0;
		int unWalkableMaxY = 0;
		int unWalkableMaxX = 0;
		int unWalkableMinX = 0;
		for (int x = minX; x < maxX; x++)
		{
			const int index = GetIndex(x, y);
			if (mGraph.GetNode(AI::Coord{ x,y }))
			{
				if (mNode[index].GetWalkable() == false)
				{
					if (x < maxX / 2)
						unWalkableMinX++;
					else
						unWalkableMaxX++;

					if (y < maxY / 2)
						unWalkableMinY++;
					else
						unWalkableMinY++;
				}
			}
		}
		if (unWalkableMinY > area - 2)
			newMinY++;
		if (unWalkableMaxY > area - 2)
			newMaxY--;
		if (unWalkableMinX > area - 2)
			newMinX++;
		if (unWalkableMinX > area - 2)
			newMaxX--;
	}

	minY = newMinY;
	minX = newMinX;
	maxY = newMaxY;
	maxX = newMaxX;
}

int JimmyGod::Grid3DComponent::GetIndex(int x, int y) const
{
	return x + (y * mGraph.GetColumns());
}

void JimmyGod::Grid3DComponent::FindPath(const AI::Coord& from, const AI::Coord& to, float maxDistance, std::vector<Math::Vector3>& newPath, AI::PathFind type)
{
	AI::Path path;
	newPath.clear();

	AI::Coord start = { from.x,from.y};
	AI::Coord end = { to.x,to.y};

	auto funcBlocked = [this](AI::Coord coord)
	{
		// 3D check
		if (mNode[GetIndex(coord.x, coord.y)].GetWalkable()) return false;
		else return true;
	};

	auto funcGetCost = [this](AI::Coord from, AI::Coord to)
	{
		return (from.x != to.x && from.y != to.y) ? 1.414f : 1.0f;
	};

	switch (mPathFind)
	{
	case JimmyGod::AI::PathFind::BFS:
	{
		path = mBFS.Search(mGraph, start, end, funcBlocked, maxDistance);
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
		default: break;
		}
		break;
	}
	default: break;
	}
	for (auto& p : path)
	{
		auto node = mGraph.GetNode(p);
		Vector3 pos = node->position;
		newPath.push_back(pos);
	}
}

void JimmyGod::Grid3DComponent::SetPathFind(const char* name)
{
	if (name == "BFS") mPathFind = AI::PathFind::BFS;
	else if (name == "DFS") mPathFind = AI::PathFind::DFS;
	else if (name == "Dijkstra") mPathFind = AI::PathFind::Dijkstra;
	else if (name == "AStar") mPathFind = AI::PathFind::AStar;
	else return;
}

bool JimmyGod::Grid3DComponent::CheckMaximumAndMinimumGird(const AI::Coord& coord) const
{
	if (coord.x > maxX - 1 || coord.y > maxY - 1 || coord.x < minX || coord.y < minY)
		return false;
	else
		return true;
}
