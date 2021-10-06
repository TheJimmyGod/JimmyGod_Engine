#pragma once
#include "Flag.h"
#include "CharacterModule.h"
#include "GridManager.h"
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace JimmyCom
{
	class Unit
	{
	public:
		Unit(std::string name, Flag flag) : mName(name), mFlag(flag) 
		{
		}

		Unit& operator= (const Unit& rhs) = default;
		Unit& operator= (Unit&& rhs) noexcept;

		virtual ~Unit() = default;

		virtual void Attack(Unit& unit);
		virtual bool AttackUpdate(float deltaTime);

		virtual const AgentComponent& GetAgent() const = 0;
		virtual AgentComponent& GetAgent() = 0;

		virtual const ModelComponent& GetModelComponent() const = 0;
		virtual ModelComponent& GetModelComponent() = 0;

		virtual const JimmyGod::Math::Sphere& GetSphereCollider() const;
		virtual void Move(const JimmyGod::AI::Coord& pos);
		virtual void TakeDamage(float val);

		virtual void SetProcess(bool p) = 0;

	public:
		const float GetDamage() const { return mDamage; }
		const float GetCurrentHealth() const { return mHealth; }
		const int GetArea() const { return GetAgent().mArea; }
		const Flag& GetFlag() const { return mFlag; }
		const UnitType& GetUnitType() const { return mUnitType; }
		const Status& GetStatus() const { return mStatus; }
		const Unit& GetUnit() const { return *this; }
		const std::string& GetName() const { return mName; }

		const JimmyGod::Math::Vector3& GetDestination() const { return mTargetPath; }
		const JimmyGod::Math::Vector3& GetPosition() const { return GetAgent().GetTransformComponent()->GetPosition(); }
		const JimmyGod::AI::Coord& GetCoordinate() const { return mCurrentCoordinate; }
		const JimmyGod::Math::Quaternion& GetRotation() const { return GetAgent().GetTransformComponent()->GetRotation(); }

		Unit& GetUnit() { return *this; }
		void SetStatus(Status s) { mStatus = s; }
		void SetCoordinate(const AI::Coord& coord) { mCurrentCoordinate = coord; }
		void SetDestination(const Vector3& pos) { mTargetPath = pos; }

		float mUpdateTime = 0.0f;
		JimmyGod::Math::Vector3 mTargetPath = JimmyGod::Math::Vector3::Zero;
	protected:
		int mLevel = 1;
		int mEXP = 0;
		int mChance = 2;

		float mHealth = 0.0f;
		float mMaxHelath = 0.0f;
		float mDamage = 0.0f;
		float mDefence = 0.0f;
		float mRange = 0.0f;

		Flag mFlag = Flag::Neutral;
		Status mStatus = Status::Standby;
		UnitType mUnitType = UnitType::Soldier;
		std::string mName;

		AI::Coord mCurrentCoordinate = { 0,0 };
		bool isDead = true;
	};
}

