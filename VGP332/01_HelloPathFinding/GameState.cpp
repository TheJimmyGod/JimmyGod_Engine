#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetFov(60.0f * JimmyGod::Math::Constants::DegToRad);
	mCamera.SetNearPlane(0.01f);
	mCamera.SetFarPlane(10000.0f);
	tilemap.Load();

}

void GameState::Terminate()
{
	tilemap.Unload();
}

void GameState::Update(float deltaTime)
{
	tilemap.Update(deltaTime);
	
}

void GameState::Render()
{
	tilemap.Render();
	SpriteRenderManager::Get()->DrawScreenText("JimmyGod", 100.0f, 100.0f, 20.0f, JimmyGod::Graphics::Colors::Red);
}


void GameState::DebugUI()
{
	SimpleDraw::Render(mCamera);
	ImGui::Begin("AI Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %.2f", tilemap.fps);

	ImGui::Separator();

	if (ImGui::Button("Place Start"))
	{
		tilemap.placeStart = true;
		tilemap.placeEnd = false;
	}
	if (ImGui::Button("Place End"))
	{
		tilemap.placeStart = false;
		tilemap.placeEnd = true;
	}


	ImGui::DragInt("Start X", &tilemap.mDimention.startX, 0.5f, 0, tilemap.mColumns - 1);
	ImGui::DragInt("Start Y", &tilemap.mDimention.startY, 0.5f, 0, tilemap.mRows - 1);
	ImGui::DragInt("End X", &tilemap.mDimention.endX, 0.5f, 0, tilemap.mColumns - 1);
	ImGui::DragInt("End Y", &tilemap.mDimention.endY, 0.5f, 0, tilemap.mRows - 1);

	ImGui::Separator();

	for (size_t i = 0; i < tilemap.mTextureIds.size(); i++)
	{
		if (ImGui::ImageButton(SpriteRenderManager::Get()->GetSprite(tilemap.mTextureIds[i]), { static_cast<float>(tilemap.mTileSize),static_cast<float>(tilemap.mTileSize) }))
		{
			tilemap.mCurrentTile = i;
			tilemap.isChosenTile = true;
		}
		if (i % 3 != 2)
			ImGui::SameLine();
	}

	ImGui::Separator();

	static Coord start = { tilemap.mDimention.startX,tilemap.mDimention.startY };
	static Coord end = { tilemap.mDimention.endX,tilemap.mDimention.endY };

	if (ImGui::CollapsingHeader("Breadth First Search", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Generate BFS"))
		{
			tilemap.Clear();
			tilemap.mPathFind = PathFind::BFS;
			Coord start = { tilemap.mDimention.startX,tilemap.mDimention.startY };
			Coord end = { tilemap.mDimention.endX,tilemap.mDimention.endY };
			tilemap.mPath = tilemap.mBFS.Search(tilemap.mGraph, start, end, [this](AI::Coord coord)
			{
				int tile = tilemap.mTiles[tilemap.GetIndex(coord.x, coord.y)];
				if (tile == 0)
					return false;
				return true;
			}, 5.0f);
		}
		if (ImGui::Button("Reset BFS"))
		{
			tilemap.Clear();
		}
	}
	if (ImGui::CollapsingHeader("Depth First Search", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Generate DFS"))
		{
			tilemap.Clear();
			tilemap.mPathFind = PathFind::DFS;
			Coord start = { tilemap.mDimention.startX,tilemap.mDimention.startY };
			Coord end = { tilemap.mDimention.endX,tilemap.mDimention.endY };
			tilemap.mPath = tilemap.mDFS.Search(tilemap.mGraph, start, end, [this](AI::Coord coord)
			{
				int tile = tilemap.mTiles[tilemap.GetIndex(coord.x, coord.y)];
				if (tile == 0)
					return false;
				return true;
			});
		}
		if (ImGui::Button("Reset DFS"))
		{
			tilemap.Clear();
		}
	}

	if (ImGui::CollapsingHeader("Dijkstra Search", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Generate Dijkstra"))
		{
			tilemap.Clear();
			tilemap.mPathFind = PathFind::Dijkstra;
			Coord start = { tilemap.mDimention.startX,tilemap.mDimention.startY };
			Coord end = { tilemap.mDimention.endX,tilemap.mDimention.endY };

			auto funcBlocked = [this](AI::Coord coord)
			{
				int tile = tilemap.mTiles[tilemap.GetIndex(coord.x, coord.y)];
				if (tile == 0)
					return false;
				return true;
			};

			auto funcGetCost = [this](AI::Coord from, AI::Coord to)
			{
				return (from.x != to.x && from.y != to.y) ? 1.414f : 1.0f;
			};

			tilemap.mPath = tilemap.mDijkstra.Search(tilemap.mGraph, start, end, funcBlocked, funcGetCost);
		}
		if (ImGui::Button("Reset Dijkstra"))
		{
			tilemap.Clear();
		}
	}

	if (ImGui::CollapsingHeader("AStar Search", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::RadioButton("Euclidian", &tilemap.mAStarNum, 0);
		ImGui::RadioButton("Manhattan", &tilemap.mAStarNum, 1);
		ImGui::RadioButton("Chebyshev", &tilemap.mAStarNum, 2);

		switch (tilemap.mAStarNum)
		{
		case 0:
			tilemap.mAStarType = AStarTypes::Euclidian;
			break;
		case 1:
			tilemap.mAStarType = AStarTypes::Manhattan;
			break;
		case 2:
			tilemap.mAStarType = AStarTypes::Chebyshev;
			break;
		default:
			break;
		}

		if (ImGui::Button("Generate AStar"))
		{
			tilemap.Clear();
			tilemap.mPathFind = PathFind::AStar;
			Coord start = { tilemap.mDimention.startX,tilemap.mDimention.startY };
			Coord end = { tilemap.mDimention.endX,tilemap.mDimention.endY };
			auto funcBlocked = [this](AI::Coord coord)
			{
				int tile = tilemap.mTiles[tilemap.GetIndex(coord.x, coord.y)];
				if (tile == 0)
					return false;
				return true;
			};

			auto funcGetCost = [this](AI::Coord from, AI::Coord to)
			{
				return (from.x != to.x && from.y != to.y) ? 1.414f : 1.0f;
			};

			switch (tilemap.mAStarType)
			{
			case AStarTypes::Euclidian:
			{

				auto funcGetHeuristic = [this](Coord from, Coord to)
				{
					float xdiff = static_cast<float>(from.x - to.x);
					float ydiff = static_cast<float>(from.y - to.y);
					return sqrtf(xdiff * xdiff + ydiff * ydiff);
				};
				tilemap.mPath = tilemap.mAStar.Search(tilemap.mGraph, start, end, funcBlocked, funcGetCost, funcGetHeuristic);
				break;
			}
			case AStarTypes::Manhattan:
			{
				auto funcGetHeuristic = [this](Coord from, Coord to)
				{
					return abs(from.x - to.x) + abs(from.y - to.y);
				};
				tilemap.mPath = tilemap.mAStar.Search(tilemap.mGraph, start, end, funcBlocked, funcGetCost, funcGetHeuristic);
				break;
			}
			case AStarTypes::Chebyshev:
			{
				auto funcGetHeuristic = [this](Coord from, Coord to)
				{
					return JimmyGod::Math::Max((from.x - to.x), (from.y - to.y));
				};
				tilemap.mPath = tilemap.mAStar.Search(tilemap.mGraph, start, end, funcBlocked, funcGetCost, funcGetHeuristic);
				break;
			}
			default:
				break;
			}

		}
		if (ImGui::Button("Reset AStar"))
		{
			tilemap.Clear();
		}
	}

	ImGui::End();
}
