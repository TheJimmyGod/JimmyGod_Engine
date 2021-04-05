#include "TileMap.h"
#include <ImGui/Inc/imgui.h>

using namespace JimmyGod;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;

void TileMap::Load()
{

	mTextureIds[0] = SpriteRenderManager::Get()->LoadTexture("grass.png");
	mTextureIds[1] = SpriteRenderManager::Get()->LoadTexture("flower.png");
	mTextureIds[2] = SpriteRenderManager::Get()->LoadTexture("tree0.png");
	mTextureIds[3] = SpriteRenderManager::Get()->LoadTexture("tree1.png");
	mTextureIds[4] = SpriteRenderManager::Get()->LoadTexture("tree2.png");
	mTextureIds[5] = SpriteRenderManager::Get()->LoadTexture("tree3.png");
	mColumns = 25;
	mRows = 20;

	mTiles.resize(static_cast<size_t>(mColumns * mRows), 0);

	mGraph.Resize(mColumns,mRows);
	mNode = mGraph.GetNodes();
}

void TileMap::Unload()
{

	mNode.clear();
}

void TileMap::Update(float deltaTime)
{
	fps = 1.f / deltaTime;
	int column = JimmyGod::Input::InputSystem::Get()->GetMouseScreenX() / mTileSize;
	int row = JimmyGod::Input::InputSystem::Get()->GetMouseScreenY() / mTileSize;
	// Check bound and make sure we are within the map
	// Check if mouse is clicked
	// Index into mTiles and change value

	if (InputSystem::Get()->IsMousePressed(JimmyGod::Input::MouseButton::LBUTTON))
	{
		const int index = mGraph.GetIndex(Coord{ column, row });
		if ((column >= mColumns || row >= mRows))
		{
			return;
		}

		if (placeStart)
		{
			mCurrentTile = 0;
			mDimention.startX = JimmyGod::Input::InputSystem::Get()->GetMouseScreenX() / mTileSize; 
			mDimention.startY = JimmyGod::Input::InputSystem::Get()->GetMouseScreenY() / mTileSize;
			placeStart = !placeStart;

			mCat.SetPosition(Vector2(static_cast<float>(mDimention.startX * mTileSize), static_cast<float>(mDimention.startY * mTileSize)));
		}
		if (placeEnd)
		{
			mCurrentTile = 0;
			mDimention.endX = JimmyGod::Input::InputSystem::Get()->GetMouseScreenX() / mTileSize;
			mDimention.endY = JimmyGod::Input::InputSystem::Get()->GetMouseScreenY() / mTileSize;
			placeEnd = !placeEnd;
		}

		if (isChosenTile)
		{
			mTiles[index] = mCurrentTile;
		}
	}

	Vector2 current = Vector2::Zero;
	maximumWayPoint = mPath.size() - 1;
	if(mPath.size() > 0)
		current = { static_cast<float>(mPath[wayPoint].x* mTileSize), static_cast<float>(mPath[wayPoint].y* mTileSize) };

	if (maximumWayPoint > 0)
	{
		if (Distance(mCat.GetPosition(), current) > 1.0f)
		{
			Vector2 direction = Normalize(current - mCat.GetPosition());
			Vector2 newPosition = mCat.GetPosition() + (direction * 50.0f * deltaTime);
			mCat.SetPosition(newPosition);
		}
		else
		{
			if (wayPoint != maximumWayPoint)
			{
				wayPoint++;
			}
			else
			{
				if (!arrived)
				{
					mCat.ChangeState("Meow");
					arrived = true;
				}

			}
		}
	}

}

void TileMap::Render()
{
	float halfSize = mTileSize * 0.5f;
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			const int index = GetIndex(x,y);
			Vector2 pos
			{
				static_cast<float>(x) * mTileSize,
				static_cast<float>(y) * mTileSize
			};
			SpriteRenderManager::Get()->DrawSprite(mTextureIds[mTiles[index]], pos , Pivot::TopLeft, Flip::None);
			SpriteRenderManager::Get()->DrawScreenText("T", pos.x, pos.y, 20, JimmyGod::Graphics::Colors::Aqua);
			if (mGraph.GetNode({ x,y }))
			{
				for (size_t i = 0; i < mNode[index].neighbors.size(); i++)
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(pos.x + halfSize,pos.y + halfSize), Vector2(
						static_cast<float>(mNode[index].neighbors[i].x * mTileSize + halfSize),
						static_cast<float>(mNode[index].neighbors[i].y * mTileSize + halfSize)), JimmyGod::Graphics::Colors::Gray);
				}
			}

			
		}
	}
	auto& closedList = mBFS.GetClosedList();
	auto& parent = mBFS.GetParentList();

	auto& closedListDFS = mDFS.GetClosedList();
	auto& parentDFS = mDFS.GetParentList();

	auto& closedListDij = mDijkstra.GetClosedList();
	auto& parentDij = mDijkstra.GetParentList();

	auto& closedListAStar = mAStar.GetClosedList();
	auto& parentAStar = mAStar.GetParentList();

	if (mPathFind == PathFind::BFS)
	{
		if (closedList.size())
		{
			for (auto node : closedList)
			{
				if (parent[mGraph.GetIndex(node)].IsValid())
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(
						static_cast<float>(node.x)* mTileSize + halfSize, 
						static_cast<float>(node.y)* mTileSize + halfSize),
						Vector2(
						static_cast<float>(parent[mGraph.GetIndex(node)].x) * mTileSize + halfSize,
						static_cast<float>(parent[mGraph.GetIndex(node)].y)* mTileSize + halfSize),
						JimmyGod::Graphics::Colors::DarkGreen);

				}
			}
		}
	}

	if (mPathFind == PathFind::DFS)
	{
		if (closedListDFS.size())
		{
			for (auto node : closedListDFS)
			{
				if (parentDFS[mGraph.GetIndex(node)].IsValid())
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(static_cast<float>(node.x)* mTileSize + halfSize, static_cast<float>(node.y)* mTileSize + halfSize),
						Vector2(static_cast<float>(parentDFS[mGraph.GetIndex(node)].x)* mTileSize + halfSize, static_cast<float>(parentDFS[mGraph.GetIndex(node)].y)* mTileSize + halfSize), JimmyGod::Graphics::Colors::DarkGreen);
				}
			}
		}
	}

	if (mPathFind == PathFind::Dijkstra)
	{
		if (closedListDij.size())
		{
			for (auto node : closedListDij)
			{
				if (parentDij[mGraph.GetIndex(node)].IsValid())
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(static_cast<float>(node.x)* mTileSize + halfSize, static_cast<float>(node.y)* mTileSize + halfSize),
						Vector2(static_cast<float>(parentDFS[mGraph.GetIndex(node)].x)* mTileSize + halfSize, static_cast<float>(parentDFS[mGraph.GetIndex(node)].y)* mTileSize + halfSize), JimmyGod::Graphics::Colors::DarkGreen);
				}
			}
		}
	}

	if (mPathFind == PathFind::AStar)
	{
		if (closedListAStar.size())
		{
			for (auto node : closedListAStar)
			{
				if (parentAStar[mGraph.GetIndex(node)].IsValid())
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(static_cast<float>(node.x)* mTileSize + halfSize, static_cast<float>(node.y)* mTileSize + halfSize),
						Vector2(static_cast<float>(parentDFS[mGraph.GetIndex(node)].x)* mTileSize + halfSize, static_cast<float>(parentDFS[mGraph.GetIndex(node)].y)* mTileSize + halfSize), JimmyGod::Graphics::Colors::DarkGreen);
				}
			}
		}
	}

	if (mPath.size() > 0)
	{
		for (int i = 0; i + 1 < mPath.size(); i++)
		{
			auto from = mPath[i];
			auto to = mPath[i + 1];
			JimmyGod::Graphics::SimpleDraw::AddScreenLine(Vector2(static_cast<float>(from.x)* mTileSize + halfSize, static_cast<float>(from.y)* mTileSize + halfSize),
				Vector2(static_cast<float>(to.x)* mTileSize + halfSize, static_cast<float>(to.y)* mTileSize + halfSize), JimmyGod::Graphics::Colors::Crimson);
			JimmyGod::Graphics::SimpleDraw::AddScreenCircle(Circle(Vector2(from.x * mTileSize + halfSize, from.y*mTileSize + halfSize), 3.0f), JimmyGod::Graphics::Colors::LightGray);
			JimmyGod::Graphics::SimpleDraw::AddScreenCircle(Circle(Vector2(to.x * mTileSize + halfSize, to.y*mTileSize + halfSize), 3.0f), JimmyGod::Graphics::Colors::LightGray);
		}
	}

	mCat.Render();
}
bool TileMap::funcBlokced(Coord coord)
{
	int tile = mTiles[mGraph.GetIndex(coord)];
	if (tile == 0)
		return false;
	return true;
}

float TileMap::funcGetCost(Coord from, Coord to)
{
	return (from.x != to.x && from.y != to.y) ? 1.414f : 1.0f;
}

int TileMap::GetIndex(int x, int y) const
{
	return x + (y * mColumns);
}

void TileMap::DebugUI()
{

}

void TileMap::Clear()
{
	arrived = false;
	wayPoint = 0;
	mPath.clear();
}