#pragma once
#include <iostream>
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "Flag.h"
#include "Unit.h"
#include "Soldier.h"
#include "Mutant.h"

namespace JimmyCom
{
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

		std::unique_ptr<Soldier>& GetSoldier(size_t index) { return mSoldiers[index]; }
		const size_t GetSoldierCount() const { return mSoldiers.size(); }
		std::unique_ptr<Mutant>& GetMutant(size_t index) { return mMutants[index]; }
		const size_t GetMutantCount() const { return mMutants.size(); }

		const Unit* SelectedUnit() const { return mUnit; }
		Unit* SelectedUnit() { return mUnit; }
		const Unit* SelectedTarget() const { return mTarget; }
		Unit* SelectedTarget() { return mTarget; }

		const JimmyGod::GameWorld& GetGameWorld() const { return mWorld; }
		JimmyGod::GameWorld& GetGameWorld() { return mWorld; }
	private:

		JimmyGod::GameWorld mWorld;
		JimmyGod::CameraService* mCamera = nullptr;
		JimmyGod::RenderService* mRender = nullptr;

		std::vector<std::unique_ptr<Soldier>> mSoldiers;
		std::vector<std::unique_ptr<Mutant>> mMutants;

		Flag mCurrentState = Flag::Ally;
		bool GameOver = false;

		int mMouseX = 0;
		int mMouseY = 0;

		int mNextUnit = 0;

		float mMaxDistance = 0.0f;
		Ray mRay;

		AI::Coord mDestination = { 0,0 };
		Quaternion mRotation = Quaternion::Zero;

		Unit* mUnit = nullptr;
		Unit* mTarget = nullptr;

		const JimmyGod::Graphics::Color GetColor(Flag flag) const;
		const JimmyGod::Graphics::Color GetColor_Standby(Flag flag) const;

		void ActionState(float deltaTime);
		void ControlState();
		Unit* TraceUnit(Flag flag);

		bool RayCast(const JimmyGod::Math::Ray& mousePoint,
			const Unit& collider, float& maxDistance, Flag layerMask);
		bool RayCast(const JimmyGod::Math::Ray& mousePoint,
			const JimmyGod::Math::Sphere& collider, float& maxDistance);
		bool TurnProcess();

		void Reset();
	};
}