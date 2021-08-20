#pragma once
#include "Flag.h"
#include "CharacterModule.h"
#include "GridManager.h"
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class Unit
{
public:
	Unit(std::string name, Flag flag) : mName(name), mFlag(flag) {};

	virtual ~Unit() = default;

	virtual void Attack(Unit& unit);
	virtual bool AttackUpdate(float deltaTime);
	virtual const AgentComponent& GetAgent() const = 0;
	virtual AgentComponent& GetAgent() = 0;
	virtual const JimmyGod::Math::Sphere& GetSphereCollider() const;
	virtual void Move(const JimmyGod::AI::Coord& pos);
	virtual void TakeDamage(float val);
	virtual void SetProcess(bool p) = 0;

public:
	const Flag& GetFlag() const { return mFlag; }
	const UnitType& GetUnitType() const { return mUnitType; }
	const Status& GetStatus() const { return mStatus; }
	const Unit& GetUnit() const { return *this; };
	Unit& GetUnit() { return *this; }
	const bool GetProcess() const { return isProcess; }
	void SetStatus(Status s) { mStatus = s; }
	float mTime = 0.0f;
	
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
	bool isDead = true;
	bool isProcess = false;
};