#pragma once

#include "TileMap.h"
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

using namespace JimmyGod;
using namespace std;
using namespace Math;

class GameState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	TileMap tilemap;
	JimmyGod::Graphics::Camera mCamera;
};