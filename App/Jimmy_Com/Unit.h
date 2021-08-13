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

	virtual void Move(const JimmyGod::AI::Coord& pos) = 0;
	virtual void Attack(Unit& unit) = 0;
	virtual void TakeDamage(float val) = 0;

	const Flag& GetFlag() const { return mFlag; }
	
protected:
	int mLevel = 1;
	int mEXP = 0;

	float mHealth = 0.0f;
	float mMaxHelath = 0.0f;
	float mDamage = 0.0f;
	float mDefence = 0.0f;
	float mRange = 0.0f;
	Flag mFlag = Flag::Neutral;
	std::string mName;
	bool isDead = true;
};