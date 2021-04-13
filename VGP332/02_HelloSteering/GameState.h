#pragma once

#include "TileMap.h"
#include "Player.h"
#include "Soldier.h"
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

using namespace JimmyGod;



class GameState : public JimmyGod::AppState
{
public:
	enum class Order
	{
		None,
		Gathering,
		Moving,
		Wandering,
		Attacking
	};
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

	void Processing(float deltaTime);
	void Clear();
	void ClearSingleEntity(Steering::Soldier* soldier);
private:
	TileMap mTilemap;
	JimmyGod::Graphics::Camera mCamera;

	AI::AIWorld::Settings mAI_Setting;
	AI::AIWorld mWorld;

	std::vector<std::unique_ptr<Steering::Soldier>> mSolider;
	std::unique_ptr<Steering::Player> mPlayer;

	Math::Vector2 nearest = Math::Vector2::Zero;
	Math::Vector2 previous = Math::Vector2::Zero;

	Order mOrder = Order::None;
	bool isCommand = false;
	bool isProcessing = false;
	float mTimer = 0.0f;
};