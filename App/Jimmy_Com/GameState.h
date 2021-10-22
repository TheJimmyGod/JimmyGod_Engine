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

	void Setup();

private:
	JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
	std::vector<JimmyGod::Physics::Particle*> particles;
	JimmyGod::Terrain mTerrain;
	JimmyGod::Spark mSpark;

	bool mInitialize = false;
	bool SparkEffect = false;
	bool mAttack = false;
private:
	float fps = 0.0f;
};