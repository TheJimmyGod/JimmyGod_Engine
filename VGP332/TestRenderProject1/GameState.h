#pragma once
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class GameState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	Camera mCamera;
	JimmyGod::Graphics::TextureId mTexture;

	JimmyGod::AI::Graph mGraph;
	std::vector<int> mTiles;
	std::vector<JimmyGod::AI::Node> mNode;

	int mColumns{ 0 };
	int mRows{ 0 };

	int mTileSize{ 32 };
	int mCurrentTile{ 0 };

	int wayPoint = 0;
	int maximumWayPoint = 0;
};