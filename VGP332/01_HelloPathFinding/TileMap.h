#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "Cat.h"

using namespace JimmyGod::AI;

enum class AStarTypes
{
	Euclidian,
	Manhattan,
	Chebyshev
};

class TileMap
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();
	void DebugUI();

	void SetAStarTypes(AStarTypes type) { mAStarType = type; }
	AStarTypes GetAStarTypes() const { return mAStarType; }
private:
	int GetIndex(int x, int y) const;

	Graph mGraph;

	BFS mBFS;
	Path mPath;

	DFS mDFS;
	Path mPathDFS;

	Dijkstras mDijkstra;
	Path mPathDijkstra;

	AStar mAStar;
	Path mPathAStar;

	std::array<JimmyGod::Input::TextureId, 6> mTextureIds;
	std::vector<int> mTiles;
	std::vector<Graph::Node> mNode;

	AStarTypes mAStarType;
	int mAStarNum{ 0 };

	int mColumns{ 0 };
	int mRows{ 0 };

	int mTileSize{ 32 };
	int mCurrentTile{ 0 };

	bool isChosenTile{ false };
	bool isDisplayed{ false };
	bool isDisplayedDFS{ false };
	bool isDisplayedDijkstar{ false };
	bool isDisplayedAStar{ false };
	float fps = 0.0f;

	int startX{0};
	int startY{0};
	int endX{0};
	int endY{0};

	bool placeStart = false;
	bool placeEnd = false;
	bool arrived = false;
	Cat mCat;

	int wayPoint = 0;
	int maximumWayPoint = 0;
};