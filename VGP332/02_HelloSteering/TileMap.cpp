#include "TileMap.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

void TileMap::Load()
{
	mTextureIds[0] = TextureManager::Get()->Load("tile2.png");
	mColumns = 50;
	mRows = 40;
	mTiles.resize(static_cast<size_t>(mColumns * mRows), 0);
	for (size_t i = 0; i < mTiles.size(); ++i)
	{
		mTiles[i] = 0;
	}

}

void TileMap::UnLoad()
{
	mTiles.clear();
}

void TileMap::Render()
{

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; x++)
		{
			const int index = GetIndex(x, y);
			Vector2 pos
			{
				static_cast<float>(x) * mTileSize,
				static_cast<float>(y) * mTileSize
			};
			SpriteRenderManager::Get()->DrawSprite(mTextureIds[mTiles[index]], pos, Pivot::TopLeft);
		}
	}
}