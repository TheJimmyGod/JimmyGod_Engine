#pragma once

#include "Unit.h"
#include "CharacterModule.h"
#include "Soldier.h"
#include "Mutant.h"
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

	JimmyGod::GameWorld mWorld;
	JimmyGod::CameraService* mCamera = nullptr;
	JimmyGod::RenderService* mRender = nullptr;

	JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
	std::vector<JimmyGod::Physics::Particle*> particles;
	
private:
	bool isActive = false;
	float mTime = 0.0f;
	const float mGravity = -9.8f;
	float fps = 0.0f;
	float dist = 0.0f;

private:
	Soldier* Johny = nullptr;
	Mutant* Zone = nullptr;

	JimmyGod::AI::Coord destiniation = {0,0};
	JimmyGod::AI::Coord current = { 0,0 };
	Flag mTurn = Flag::Ally;

	Unit* unit = nullptr;
	Unit* target = nullptr;

};