#include "Precompiled.h"
#include "GridComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

META_DERIVED_BEGIN(GridComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(worldPos, "WorldPos")
		META_FIELD(is3D, "Demension")
		META_FIELD(tileSize2D,"TileSize2D")
		META_FIELD(tileRadius3D,"TileRadius3D")
	META_FIELD_END
META_CLASS_END

void JimmyGod::GridComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	if (is3D)
		mGraph.Resize3D(worldPos.x, worldPos.y, tileRadius3D, mTransformComponent->GetPosition());
	else
	{
		mTextureIds[0] = SpriteRenderManager::Get()->LoadTexture("grass.png");
		mTextureIds[1] = SpriteRenderManager::Get()->LoadTexture("flower.png");
		mTextureIds[2] = SpriteRenderManager::Get()->LoadTexture("tree0.png");
		mTextureIds[3] = SpriteRenderManager::Get()->LoadTexture("tree1.png");
		mTextureIds[4] = SpriteRenderManager::Get()->LoadTexture("tree2.png");
		mTextureIds[5] = SpriteRenderManager::Get()->LoadTexture("tree3.png");

		mGraph.Resize(worldPos.x, worldPos.y);
	}
	mTiles.resize(mGraph.GetColumns() * mGraph.GetRows(), 0);
	mNode = mGraph.GetNodes();
}
void JimmyGod::GridComponent::Terminate() 
{
	mTiles.clear();
	mNode.clear();
}

void JimmyGod::GridComponent::Update(float deltaTime)
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

void JimmyGod::GridComponent::DebugUI()
{
	if (mNode.empty())
		return;
	if (isDebugUI == false)
		return;
	
	for (int y = 0; y < mGraph.GetRows(); y++)
	{
		for (int x = 0; x < mGraph.GetColumns(); x++)
		{
			const int index = GetIndex(x, y);
			float halfSize = tileSize2D / 2.0f;
			Vector2 pos
			{
				static_cast<float>(x) * tileSize2D,
				static_cast<float>(y) * tileSize2D
			};

			if(is3D == false)
				SpriteRenderManager::Get()->DrawSprite(mTextureIds[mTiles[index]], pos, Pivot::TopLeft, Flip::None);

			if (mGraph.GetNode(AI::Coord{ x,y }))
			{
				if(is3D)
					JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(mNode[index].position,1.0f),Colors::Red);
				else
					for (size_t i = 0; i < mNode[index].neighbors.size(); i++)
					{

						JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(pos.x + halfSize, pos.y + halfSize), Vector2(
							static_cast<float>(mNode[index].neighbors[i].x * tileSize2D + halfSize),
							static_cast<float>(mNode[index].neighbors[i].y * tileSize2D + halfSize)), JimmyGod::Graphics::Colors::Red);
					}
			}
		}
	}
	if (is3D)
		DisplayClosedListIn3D();
	else
		DisplayClosedListIn2D();
}

void JimmyGod::GridComponent::ObjectPosition(const Math::Vector3 pos)
{
	mCurrentCoordinate = mGraph.GetNode(pos)->coordinate;
}

void JimmyGod::GridComponent::CreateGrid(int columns, int rows, int tileSize2Ds)
{
	if (columns == 0 || rows == 0)
		return;

	mTiles.clear();
	mGraph.clear();
	mNode.clear();

	if (is3D)
		mGraph.Resize3D(worldPos.x, worldPos.y, tileRadius3D, mTransformComponent->GetPosition());
	else
		mGraph.Resize(worldPos.x, worldPos.y);
	mTiles.resize(mGraph.GetColumns() * mGraph.GetRows(), 0);
	mNode = mGraph.GetNodes();
}

void JimmyGod::GridComponent::DisplayClosedListIn2D()
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

	if (closedList.size())
	{
		for (auto node : closedList)
		{
			if (parent[mGraph.GetIndex(node)].IsValid())
			{
				JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(
					static_cast<float>(node.x) * tileSize2D + (tileSize2D / 2),
					static_cast<float>(node.y) * tileSize2D + (tileSize2D / 2)),
					Vector2(
						static_cast<float>(parent[mGraph.GetIndex(node)].x) * tileSize2D + (tileSize2D / 2),
						static_cast<float>(parent[mGraph.GetIndex(node)].y) * tileSize2D + (tileSize2D / 2)),
					JimmyGod::Graphics::Colors::White);
			}
		}
	}
}

void JimmyGod::GridComponent::DisplayClosedListIn3D()
{
}

void JimmyGod::GridComponent::DisplayAreaCube(int area, const Vector3& pos)
{
	AI::Coord c = mGraph.GetNode(pos)->coordinate;
	minY = Max(c.y - area+1,1);
	minX = Max(c.x - area+1,1);
	maxY = Min(c.y + area, mGraph.GetRows() - 1);
	maxX = Min(c.x + area, mGraph.GetColumns() - 1);
	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			const int index = GetIndex(x, y);
			if (mGraph.GetNode(AI::Coord{ x,y }))
			{
				if (is3D)
				{
					if(mNode[index].GetWalkable())
						JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(mNode[index].position, 1.0f), Colors::Green);
				}
			}
		}
	}

	auto n = mGraph.GetNode(mCurrentCoordinate);
	if (n != nullptr)
		JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(n->position, 1.0f), Colors::Blue);
}

int JimmyGod::GridComponent::GetIndex(int x, int y) const
{
	return x + (y * mGraph.GetColumns());
}

void JimmyGod::GridComponent::FindPath(const AI::Coord& from, const AI::Coord& to, float maxDistance, std::vector<Math::Vector3>& newPath)
{
	AI::Path path;
	newPath.clear();

	AI::Coord start = { from.x,from.y};
	AI::Coord end = { to.x,to.y};

	auto funcBlocked = [this](AI::Coord coord)
	{
		// 3D check
		if (mNode[GetIndex(coord.x, coord.y)].GetWalkable()) return false;
		// 2D check
		int tile = mTiles[GetIndex(coord.x, coord.y)];
		if (tile == 0) return false;
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

void JimmyGod::GridComponent::SetPathFind(const char* name)
{
	if (name == "BFS") mPathFind = AI::PathFind::BFS;
	else if (name == "DFS") mPathFind = AI::PathFind::DFS;
	else if (name == "Dijkstra") mPathFind = AI::PathFind::Dijkstra;
	else if (name == "AStar") mPathFind = AI::PathFind::AStar;
	else return;
}

bool JimmyGod::GridComponent::CheckMaximumAndMinimumGird(const AI::Coord& coord) const
{
	if (coord.x > maxX - 1 || coord.y > maxY - 1 || coord.x < minX || coord.y < minY)
		return false;
	else
		return true;
}
