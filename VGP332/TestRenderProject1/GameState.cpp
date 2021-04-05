#include "GameState.h"
#include <ImGui/Inc/imgui.h>

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::AI;
using namespace JimmyGod::Math;
using namespace JimmyGod::Input;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);
	mTexture = SpriteRenderManager::Get()->LoadTexture("grass.png");

	mColumns = 25;
	mRows = 20;
	mTiles.resize(static_cast<size_t>(mColumns * mRows));
	mGraph.Resize(mColumns, mRows);
	mNode = mGraph.GetNodes();
}

void GameState::Terminate()
{
	mTiles.clear();
}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; x++)
		{
			const int index = x + (y * mColumns);
			Vector2 pos
			{
				static_cast<float>(x) * mTileSize,
				static_cast<float>(y) * mTileSize
			};
			SpriteRenderManager::Get()->DrawSprite(mTexture, pos, Pivot::TopLeft);
			SpriteRenderManager::Get()->DrawScreenText("T", pos.x,pos.y,20,JimmyGod::Graphics::Colors::Aqua);
		}
	}
}

void GameState::DebugUI()
{
	//ImGui::ShowDemoWindow();
}