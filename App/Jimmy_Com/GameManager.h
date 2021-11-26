#pragma once
#include <iostream>
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "Flag.h"
#include "Unit.h"
#include "Soldier.h"
#include "Mutant.h"
#include "Environment.h"
#include "Building.h"
#include "Grass.h"

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
		void Spawn(JimmyGod::Math::Vector3&& pos, const char* name, UnitType type, Flag flag);

		std::unique_ptr<Soldier>& GetSoldier(size_t index) { return mSoldiers[index]; }
		const size_t GetSoldierCount() const { return mSoldiers.size(); }
		std::unique_ptr<Mutant>& GetMutant(size_t index) { return mMutants[index]; }
		const size_t GetMutantCount() const { return mMutants.size(); }

		template<typename ...Arg>
		void AddSoldier(Arg&& ... arg);

		template<typename ...Arg>
		void AddMutant(Arg&& ... arg);

		template<typename ...Arg>
		void AddBuilding(Arg&& ... arg);

		template<typename ...Arg>
		void AddGrass(Arg&& ... arg);

		const CharacterModule* SelectedUnit() const { return mUnitCM; }
		CharacterModule* SelectedUnit() { return mUnitCM; }
		const UnitType& SelectedUnitType() const {
			if (mUnit == nullptr)
				return UnitType::None;
			return mUnit->GetUnitType(); 
		}
		const CharacterModule* SelectedTarget() const { return mTargetCM; }
		CharacterModule* SelectedTarget() { return mTargetCM; }

		const JimmyGod::GameWorld& GetGameWorld() const { return mWorld; }
		JimmyGod::GameWorld& GetGameWorld() { return mWorld; }

		Unit* TraceClosestUnit(Flag flag);
		Unit* TraceEnemy();
		bool IsExist(const AI::Coord& coord) const;
	private:

		JimmyGod::GameWorld mWorld;
		JimmyGod::CameraService* mCamera = nullptr;
		JimmyGod::RenderService* mRender = nullptr;

		std::vector<std::unique_ptr<Soldier>> mSoldiers;
		std::vector<std::unique_ptr<Mutant>> mMutants;
		std::vector<std::unique_ptr<Building>> mBuildings;
		std::vector<std::unique_ptr<Grass>> mGrasses;
		Flag mCurrentState = Flag::Ally;
		bool GameOver = false;
		bool mProcessing_AI = false;

		float mTurnOver = 0.0f;

		int mMouseX = 0;
		int mMouseY = 0;

		int mNextUnit = 0;

		int mEnvironmentIndex = 0;

		float mMaxDistance = 0.0f;
		Ray mRay;

		Quaternion mRotation = Quaternion::Zero;
		Quaternion mRotationTarget = Quaternion::Zero;

		Unit* mUnit = nullptr;
		CharacterModule* mUnitCM = nullptr;
		Unit* mTarget = nullptr;
		CharacterModule* mTargetCM = nullptr;

		const JimmyGod::Graphics::Color GetColor(Flag flag) const;
		const JimmyGod::Graphics::Color GetColor_Standby(Flag flag) const;

		bool mCameraSwitched = false;

		void ActionState(float deltaTime);
		void ControlState(float deltaTime);
		void AIDecisionState(float deltaTime);

		void BeginAttack();

		Unit* TraceUnit(Flag flag);
		
		bool RayCast(const JimmyGod::Math::Ray& mousePoint, 
			const CharacterModule& collider, float& maxDistance, Flag unitFlag, Flag layerMask);
		bool RayCast(const JimmyGod::Math::Ray& mousePoint,
			const JimmyGod::Math::Sphere& collider, float& maxDistance);
		bool TurnProcess();
		void CameraSwtiching(float deltaTime);
		void Reset();
	};

	template<typename ...Arg>
	inline void GameManager::AddSoldier(Arg && ...arg)
	{
		mSoldiers.emplace_back(std::forward<Arg>(arg)...);
	}
	template<typename ...Arg>
	inline void GameManager::AddMutant(Arg && ...arg)
	{
		mMutants.emplace_back(std::forward<Arg>(arg)...);
	}
	template<typename ...Arg>
	inline void GameManager::AddBuilding(Arg && ...arg)
	{
		mBuildings.emplace_back(std::forward<Arg>(arg)...);
	}
	template<typename ...Arg>
	inline void GameManager::AddGrass(Arg && ...arg)
	{
		mGrasses.emplace_back(std::forward<Arg>(arg)...);
	}
}
