#include "..\02_HelloSteering\TileMap.h"
#include "TileMap.h"
#include <ImGui/Inc/imgui.h>
void TileMap::Load()
{
	mCat.Load();
	mTextureIds[0] = X::LoadTexture("grass.png");
	mTextureIds[1] = X::LoadTexture("flower.png");
	mTextureIds[2] = X::LoadTexture("tree0.png");
	mTextureIds[3] = X::LoadTexture("tree1.png");
	mTextureIds[4] = X::LoadTexture("tree2.png");
	mTextureIds[5] = X::LoadTexture("tree3.png");
	mColumns = 25;
	mRows = 20;

	mTiles.resize(mColumns * mRows, 0);

	mGraph.Resize(mColumns,mRows);
	mNode = mGraph.GetNodes();
	

}

void TileMap::Unload()
{
	mCat.Unload();
	mNode.clear();
}

void TileMap::Update(float deltaTime)
{
	fps = 1.f / deltaTime;
	int column = X::GetMouseScreenX() / mTileSize;
	int row = X::GetMouseScreenY() / mTileSize;
	DebugUI();
	// Check bound and make sure we are within the map
	// Check if mouse is clicked
	// Index into mTiles and change value

	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		const int index = GetIndex(column, row);
		if ((column >= mColumns || row >= mRows))
		{
			return;
		}

		if (placeStart)
		{
			mCurrentTile = 0;
			startX = X::GetMouseScreenX() / mTileSize; startY = X::GetMouseScreenY() / mTileSize;
			placeStart = !placeStart;

			mCat.SetPosition(X::Math::Vector2(static_cast<float>(startX * mTileSize), static_cast<float>(startY * mTileSize)));
		}
		if (placeEnd)
		{
			mCurrentTile = 0;
			endX = X::GetMouseScreenX() / mTileSize; endY = X::GetMouseScreenY() / mTileSize;
			placeEnd = !placeEnd;
		}

		if (isChosenTile)
		{
			mTiles[index] = mCurrentTile;
		}
	}

	if (isDisplayed)
	{
		maximumWayPoint = mPath.size() - 1;
		X::Math::Vector2 current = { static_cast<float>(mPath[wayPoint].x* mTileSize), static_cast<float>(mPath[wayPoint].y* mTileSize) };
		if (X::Math::Distance(mCat.GetPosition(), current) > 1.0f)
		{
			X::Math::Vector2 direction = X::Math::Normalize(current - mCat.GetPosition());
			X::Math::Vector2 newPosition = mCat.GetPosition() + (direction * 50.0f * deltaTime);
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
	else if (isDisplayedDFS)
	{
		maximumWayPoint = mPathDFS.size() -1;
		X::Math::Vector2 current = { static_cast<float>(mPathDFS[wayPoint].x* mTileSize), static_cast<float>(mPathDFS[wayPoint].y* mTileSize) };
		if (X::Math::Distance(mCat.GetPosition(), current) > 1.0f)
		{
			X::Math::Vector2 direction = X::Math::Normalize(current - mCat.GetPosition()); // Desired velocity
			// Flee = (current pos - destination);
			X::Math::Vector2 newPosition = mCat.GetPosition() + (direction * 50.0f * deltaTime);
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
	else if (isDisplayedDijkstar)
	{
		maximumWayPoint = mPathDijkstra.size() -1;
		X::Math::Vector2 current = { static_cast<float>(mPathDijkstra[wayPoint].x* mTileSize), static_cast<float>(mPathDijkstra[wayPoint].y* mTileSize) };
		if (X::Math::Distance(mCat.GetPosition(), current) > 1.0f)
		{
			X::Math::Vector2 direction = X::Math::Normalize(current - mCat.GetPosition());
			X::Math::Vector2 newPosition = mCat.GetPosition() + (direction * 50.0f * deltaTime);
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
	else if (isDisplayedAStar)
	{
		maximumWayPoint = mPathAStar.size() -1;
		X::Math::Vector2 current = { static_cast<float>( mPathAStar[wayPoint].x * mTileSize), static_cast<float>(mPathAStar[wayPoint].y* mTileSize) };
		if (X::Math::Distance(mCat.GetPosition(), current) > 1.0f)
		{
			X::Math::Vector2 direction = X::Math::Normalize(current - mCat.GetPosition());
			X::Math::Vector2 newPosition = mCat.GetPosition() + (direction * 50.0f * deltaTime);
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

	mCat.Update(deltaTime);
}

void TileMap::Render()
{
	float halfSize = mTileSize * 0.5f;
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			const int index = GetIndex(x, y);
			X::Math::Vector2 pos
			{
				static_cast<float>(x) * mTileSize,
				static_cast<float>(y) * mTileSize
			};
			X::DrawSprite(mTextureIds[mTiles[index]], pos , X::Pivot::TopLeft);
			if (mGraph.GetNode({ x,y }))
			{
				for (int i = 0; i < mNode[index].neighbors.size(); i++)
				{
					X::DrawScreenLine(X::Math::Vector2(pos.x + halfSize,pos.y + halfSize), X::Math::Vector2(
						static_cast<float>(mNode[index].neighbors[i].x * mTileSize + halfSize),
						static_cast<float>(mNode[index].neighbors[i].y * mTileSize + halfSize)), X::Colors::Gray);
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

	if (isDisplayed)
	{
		if (closedList.size())
		{
			for (auto node : closedList)
			{
				if (parent[mGraph.GetIndex(node)].IsValid())
				{
					X::DrawScreenLine(X::Math::Vector2(
						static_cast<float>(node.x)* mTileSize + halfSize, 
						static_cast<float>(node.y)* mTileSize + halfSize),
						X::Math::Vector2(
						static_cast<float>(parent[mGraph.GetIndex(node)].x) * mTileSize + halfSize,
						static_cast<float>(parent[mGraph.GetIndex(node)].y)* mTileSize + halfSize),
						X::Colors::DarkGreen);

				}
			}
		}

		for (int i = 0; i + 1 < mPath.size(); i++)
		{
			auto from = mPath[i];
			auto to = mPath[i + 1];
			X::DrawScreenLine(X::Math::Vector2(static_cast<float>(from.x)* mTileSize + halfSize, static_cast<float>(from.y)* mTileSize + halfSize),
				X::Math::Vector2(static_cast<float>(to.x)* mTileSize + halfSize, static_cast<float>(to.y)* mTileSize + halfSize), X::Colors::Crimson);
			X::DrawScreenCircle(from.x * mTileSize + halfSize, from.y*mTileSize + halfSize, 3.0f, X::Colors::LightGray);
			X::DrawScreenCircle(to.x * mTileSize + halfSize, to.y*mTileSize + halfSize, 3.0f, X::Colors::LightGray);
		}
	}

	if (isDisplayedDFS)
	{
		if (closedListDFS.size())
		{
			for (auto node : closedListDFS)
			{
				if (parentDFS[mGraph.GetIndex(node)].IsValid())
				{
					X::DrawScreenLine(X::Math::Vector2(static_cast<float>(node.x)* mTileSize + halfSize, static_cast<float>(node.y)* mTileSize + halfSize),
						X::Math::Vector2(static_cast<float>(parentDFS[mGraph.GetIndex(node)].x)* mTileSize + halfSize, static_cast<float>(parentDFS[mGraph.GetIndex(node)].y)* mTileSize + halfSize), X::Colors::DarkGreen);
				}
			}
		}

		for (int i = 0; i + 1 < mPathDFS.size(); i++)
		{
			auto from = mPathDFS[i];
			auto to = mPathDFS[i + 1];
			X::DrawScreenLine(X::Math::Vector2(static_cast<float>(from.x)* mTileSize + halfSize, static_cast<float>(from.y)* mTileSize + halfSize),
				X::Math::Vector2(static_cast<float>(to.x)* mTileSize + halfSize, static_cast<float>(to.y)* mTileSize + halfSize), X::Colors::Crimson);

			X::DrawScreenCircle(from.x * mTileSize + halfSize, from.y*mTileSize + halfSize, 3.0f, X::Colors::LightGray);
			X::DrawScreenCircle(to.x * mTileSize + halfSize, to.y*mTileSize + halfSize, 3.0f, X::Colors::LightGray);
		}
	}

	if (isDisplayedDijkstar)
	{
		if (closedListDij.size())
		{
			for (auto node : closedListDij)
			{
				if (parentDij[mGraph.GetIndex(node)].IsValid())
				{
					X::DrawScreenLine(X::Math::Vector2(static_cast<float>(node.x)* mTileSize + halfSize, static_cast<float>(node.y)* mTileSize + halfSize),
						X::Math::Vector2(static_cast<float>(parentDij[mGraph.GetIndex(node)].x)* mTileSize + halfSize, static_cast<float>(parentDij[mGraph.GetIndex(node)].y)* mTileSize + halfSize), X::Colors::DarkGreen);
				}
			}
		}

		for (int i = 0; i + 1 < mPathDijkstra.size(); i++)
		{
			auto from = mPathDijkstra[i];
			auto to = mPathDijkstra[i + 1];
			X::DrawScreenLine(X::Math::Vector2(static_cast<float>(from.x)* mTileSize + halfSize, static_cast<float>(from.y)* mTileSize + halfSize),
				X::Math::Vector2(static_cast<float>(to.x)* mTileSize + halfSize, static_cast<float>(to.y)* mTileSize + halfSize), X::Colors::Crimson);

			X::DrawScreenCircle(from.x * mTileSize + halfSize, from.y*mTileSize + halfSize, 3.0f, X::Colors::LightGray);
			X::DrawScreenCircle(to.x * mTileSize + halfSize, to.y*mTileSize + halfSize, 3.0f, X::Colors::LightGray);
		}
	}

	if (isDisplayedAStar)
	{
		if (closedListAStar.size())
		{
			for (auto node : closedListAStar)
			{
				if (parentAStar[mGraph.GetIndex(node)].IsValid())
				{
					X::DrawScreenLine(X::Math::Vector2(static_cast<float>(node.x)* mTileSize + halfSize, static_cast<float>(node.y)* mTileSize + halfSize),
						X::Math::Vector2(static_cast<float>(parentAStar[mGraph.GetIndex(node)].x)* mTileSize + halfSize, static_cast<float>(parentAStar[mGraph.GetIndex(node)].y)* mTileSize + halfSize), X::Colors::DarkGreen);
				}
			}
		}

		for (int i = 0; i + 1 < mPathAStar.size(); i++)
		{
			auto from = mPathAStar[i];
			auto to = mPathAStar[i + 1];
			X::DrawScreenLine(X::Math::Vector2(static_cast<float>(from.x)* mTileSize + halfSize, static_cast<float>(from.y)* mTileSize + halfSize),
				X::Math::Vector2(static_cast<float>(to.x)* mTileSize + halfSize, static_cast<float>(to.y)* mTileSize + halfSize), X::Colors::Crimson);

			X::DrawScreenCircle(from.x * mTileSize + halfSize, from.y*mTileSize + halfSize, 3.0f, X::Colors::LightGray);
			X::DrawScreenCircle(to.x * mTileSize + halfSize, to.y*mTileSize + halfSize, 3.0f, X::Colors::LightGray);
		}
	}

	mCat.Render();
}

int TileMap::GetIndex(int x, int y) const
{
	return x + (y * mColumns);
}

void TileMap::DebugUI()
{
	ImGui::Begin("AI Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %.2f", fps);

	ImGui::Separator();

	if (ImGui::Button("Place Start"))
	{
		placeStart = true;
		placeEnd = false;
	}
	if (ImGui::Button("Place End"))
	{
		placeStart = false;
		placeEnd = true;
	}


	ImGui::DragInt("Start X", &startX, 0.5f, 0, mColumns -1);
	ImGui::DragInt("Start Y", &startY, 0.5f, 0, mRows - 1);
	ImGui::DragInt("End X", &endX, 0.5f, 0, mColumns - 1);
	ImGui::DragInt("End Y", &endY, 0.5f, 0, mRows - 1);
	
	ImGui::Separator();
	
	for (int i = 0; i < mTextureIds.size(); i++)
	{
		if (ImGui::ImageButton(X::GetSprite(mTextureIds[i]), { static_cast<float>(mTileSize),static_cast<float>(mTileSize) }))
		{
			mCurrentTile = i;
			isChosenTile = true;
		}
		if (i % 3 != 2)
			ImGui::SameLine();
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Breadth First Search", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Generate BFS"))
		{
			arrived = false;
			wayPoint = 0;
			if (!isDisplayed)
			{
				isDisplayed = true;
				isDisplayedDFS = false;
				isDisplayedDijkstar = false;
				isDisplayedAStar = false;
			}
			AI::Coord start = { startX,startY };
			AI::Coord end = { endX,endY};
			mPath = mBFS.Search(mGraph, start, end, [this](AI::Coord coord)
			{
				int tile = mTiles[GetIndex(coord.x, coord.y)];
				if (tile == 0)
					return false;
				return true;
			});
		}
		if (ImGui::Button("Reset BFS"))
		{
			isDisplayed = false;
			mPath.clear();
		}
	}
	if (ImGui::CollapsingHeader("Depth First Search", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Generate DFS"))
		{
			arrived = false;
			wayPoint = 0;
			if (!isDisplayedDFS)
			{
				isDisplayedDFS = true;
				isDisplayed = false;
				isDisplayedDijkstar = false;
				isDisplayedAStar = false;
			}
			AI::Coord start = { startX,startY };
			AI::Coord end = { endX,endY };

			mPathDFS = mDFS.Search(mGraph, start, end, [this](AI::Coord coord)
			{
				int tile = mTiles[GetIndex(coord.x, coord.y)];
				if (tile == 0)
					return false;
				return true;
			});
		}
		if (ImGui::Button("Reset DFS"))
		{
			isDisplayedDFS = false;
			mPathDFS.clear();
		}
	}

	if (ImGui::CollapsingHeader("Dijkstra Search", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Generate Dijkstra"))
		{
			arrived = false;
			wayPoint = 0;
			if (!isDisplayedDijkstar)
			{
				isDisplayedDijkstar = true;
				isDisplayed = false;
				isDisplayedDFS = false;
				isDisplayedAStar = false;
			}
			AI::Coord start = { startX,startY };
			AI::Coord end = { endX,endY };

			auto funcBlokced = [this](AI::Coord coord)
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

			mPathDijkstra = mDijkstra.Search(mGraph, start, end, funcBlokced, funcGetCost);
		}
		if (ImGui::Button("Reset Dijkstra"))
		{
			isDisplayedDijkstar = false;
			mPathDijkstra.clear();
		}
	}

	if (ImGui::CollapsingHeader("AStar Search", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::RadioButton("Euclidian", &mAStarNum, 0);
		ImGui::RadioButton("Manhattan", &mAStarNum, 1);
		ImGui::RadioButton("Chebyshev", &mAStarNum, 2);

		switch (mAStarNum)
		{
		case 0:
			mAStarType = AStarTypes::Euclidian;
			break;
		case 1:
			mAStarType = AStarTypes::Manhattan;
			break;
		case 2:
			mAStarType = AStarTypes::Chebyshev;
			break;
		default:
			break;
		}

		if (ImGui::Button("Generate AStar"))
		{
			arrived = false;
			wayPoint = 0;
			if (!isDisplayedAStar)
			{
				isDisplayedAStar = true;
				isDisplayedDijkstar = false;
				isDisplayed = false;
				isDisplayedDFS = false;
			}
			AI::Coord start = { startX,startY };
			AI::Coord end = { endX,endY };

			auto funcBlokced = [this](AI::Coord coord)
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


			switch (mAStarType)
			{
			case AStarTypes::Euclidian:
			{
				auto funcGetHeuristic = [this](AI::Coord from, AI::Coord to)
				{
					float xdiff = from.x - to.x;
					float ydiff = from.y - to.y;
					return sqrtf(xdiff * xdiff + ydiff * ydiff);
				};
				mPathAStar = mAStar.Search(mGraph, start, end, funcBlokced, funcGetCost, funcGetHeuristic);
				break;
			}
			case AStarTypes::Manhattan:
			{
				auto funcGetHeuristic = [this](AI::Coord from, AI::Coord to)
				{
					return abs(from.x - to.x) + abs(from.y - to.y);
				};
				mPathAStar = mAStar.Search(mGraph, start, end, funcBlokced, funcGetCost, funcGetHeuristic);
				break;
			}
			case AStarTypes::Chebyshev:
			{
				auto funcGetHeuristic = [this](AI::Coord from, AI::Coord to)
				{
					return max((from.x - to.x) , (from.y - to.y));
				};
				mPathAStar = mAStar.Search(mGraph, start, end, funcBlokced, funcGetCost, funcGetHeuristic);
				break;
			}
			default:
				break;
			}

		}
		if (ImGui::Button("Reset AStar"))
		{
			isDisplayedAStar = false;
			mPathAStar.clear();
		}
	}

	ImGui::End();
}