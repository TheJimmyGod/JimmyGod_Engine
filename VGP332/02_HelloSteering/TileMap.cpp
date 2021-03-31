#include "TileMap.h"

using namespace Steering;

void TileMap::Load()
{
	mTextureIds[0] = X::LoadTexture("grass.png");
	mColumns = 50;
	mRows = 40;
	mTiles.resize(static_cast<size_t>(mColumns * mRows), 0);
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
			X::Math::Vector2 pos
			{
				static_cast<float>(x) * mTileSize,
				static_cast<float>(y) * mTileSize
			};
			X::DrawSprite(mTextureIds[mTiles[index]], pos, X::Pivot::TopLeft);
		}
	}
}