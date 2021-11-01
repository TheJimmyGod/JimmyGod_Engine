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
		virtual void Move(const JimmyGod::AI::Coord& pos) = 0;
		virtual void TakeDamage(float val);

	public:
		const float GetDamage() const;
		const float GetCurrentHealth() const;
		const Flag& GetFlag() const;
		const UnitType& GetUnitType() const;
		const Status& GetStatus() const;
		const Unit& GetUnit() const;
		const std::string& GetName() const;

		const JimmyGod::Math::Vector3& GetDestination() const;
		const JimmyGod::AI::Coord& GetCoordinate() const;

		Unit& GetUnit();
		void SetStatus(Status s);
		void SetCoordinate(const AI::Coord& coord);
		void SetDestination(const Vector3& pos);

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

