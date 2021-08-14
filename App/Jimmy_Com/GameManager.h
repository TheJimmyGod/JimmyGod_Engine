#pragma once
#include <iostream>
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "Flag.h"
#include "Unit.h"
#include "Soldier.h"
#include "Mutant.h"

class GameManager
{
public:
	static void StaticInitialize(size_t count);
	static void StaticTerminate();
	static GameManager* Get();

public:
	void Initialize(size_t count);
	void Terminate();
	void Update(float deltaTime);
	void Render();
	void DebugUI();
	void Spawn(const JimmyGod::Math::Vector3& pos, const char* name, UnitType type, Flag flag);

	bool RayCast(const JimmyGod::Math::Ray& mousePoint, 
		const JimmyGod::Math::Vector3& target, float maxDistance , Flag layerMask);
	const Unit* GetTarget(const JimmyGod::Math::Vector3& pos) const;

	Soldier* GetSoldier(size_t index) { return mSoliders[index]; }
	const size_t GetSoldierCount() const { return mSoliders.size(); }
	Mutant* GetMutant(size_t index) { return mMutants[index]; }
	const size_t GetMutantCount() const { return mMutants.size(); }

	const JimmyGod::GameWorld& GetGameWorld() const { return mWorld; }
private:

	JimmyGod::GameWorld mWorld;
	JimmyGod::CameraService* mCamera = nullptr;
	JimmyGod::RenderService* mRender = nullptr;

	std::vector<Soldier*> mSoliders;
	std::vector<Mutant*> mMutants;
	std::map<std::string, Unit*> mPending;
	Flag mCurrentState = Flag::Ally;
	bool GameOver = false;
	bool isProgressing = false;

	bool moveState = false;
	bool actionState = false;

	int mMouseX = 0;
	int mMouseY = 0;

	int mNextUnit;
	Ray mRay;	
};