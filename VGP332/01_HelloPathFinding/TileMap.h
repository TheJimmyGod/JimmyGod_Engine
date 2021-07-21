#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "Cat.h"

using namespace JimmyGod;
using namespace JimmyGod::AI;


class TileMap
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();
	void DebugUI();

	bool funcBlokced(Coord coord);
	float funcGetCost(Coord from, Coord to);

	void SetAStarTypes(AStarTypes type) { mAStarType = type; }
	AStarTypes GetAStarTypes() const { return mAStarType; }

	void Clear();

	std::vector<int> GetTiles() const
	{
		return mTiles;
	}

	int GetIndex(int x, int y) const;
private:
	friend class GameState;

	std::vector<int> mTiles;
	Graph mGraph;

	BFS mBFS;
	DFS mDFS;
	Dijkstras mDijkstra;
	AStar mAStar;
	Path mPath;

	std::array<JimmyGod::Graphics::TextureId, 6> mTextureIds;

	std::vector<Node> mNode;

	AStarTypes mAStarType;
	int mAStarNum{ 0 };

	int mColumns{ 0 };
	int mRows{ 0 };

	int mTileSize{ 32 };
	int mHalfSize = mTileSize / 2;
	int mCurrentTile{ 0 };

	bool isChosenTile{ false };

	PathFind mPathFind = PathFind::BFS;

	float fps = 0.0f;

	struct Dimention
	{
		int startX{ 0 };
		int startY{ 0 };
		int endX{ 0 };
		int endY{ 0 };
	};

	Dimention mDimention;

	bool placeStart = false;
	bool placeEnd = false;
	bool arrived = false;
	Cat mCat;

	int wayPoint = 0;
	int maximumWayPoint = 0;
};