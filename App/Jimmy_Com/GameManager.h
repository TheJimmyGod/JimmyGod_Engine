#pragma once
#include <iostream>
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

#include "Flag.h"
#include "Unit.h"
#include "Soldier.h"
#include "Mutant.h"
#include "GridManager.h"

class GameManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static GameManager* Get();

public:
	void Terminate();
	void Update(float deltaTime);
	void Spawn(const char* name, UnitType type, Flag flag);

	bool RayCast(const JimmyGod::Math::Vector3& mousePoint, 
		const JimmyGod::Math::Vector3& target, float maxDistance , Flag layerMask);
	const JimmyGod::GameObject& GetTarget(const JimmyGod::Math::Vector3& pos) const;
private:

	JimmyGod::GameWorld mWorld;

	Flag mCurrentState = Flag::Ally;
	bool GameOver = false;
	bool isProgressing = false;

	float mMouseX = 0.0f;
	float mMouseY = 0.0f;
	Ray mRay;

	
};