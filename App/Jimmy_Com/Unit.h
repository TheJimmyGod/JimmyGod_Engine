#pragma once
#include "Flag.h"
#include "CharacterModule.h"
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class Unit
{
public:
	Unit(std::string name, Flag flag) : mName(name), mFlag(flag) {};

	virtual ~Unit() = default;

	virtual const AgentComponent& GetAgent() const = 0;
	virtual AgentComponent& GetAgent() = 0;

	virtual const JimmyGod::Math::Sphere& GetSphereCollider() const = 0;

	virtual void Move(const JimmyGod::AI::Coord& pos) = 0;
	virtual void Attack(Unit& unit) = 0;
	virtual void TakeDamage(float val) = 0;

	const Flag& GetFlag() const { return mFlag; }
	const Unit& GetUnit() const { return *this; };
	Unit& GetUnit() { return *this; }

	bool GetAct() const { return isActed; }
	void SetAct(bool act) { isActed = act; }
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
	std::string mName;
	bool isDead = true;
	bool isActed = false;
};