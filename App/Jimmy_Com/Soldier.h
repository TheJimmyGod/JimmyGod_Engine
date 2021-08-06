#pragma once

#include "Unit.h"

class Soldier: public Unit
{
public:
	Soldier() : Unit(mName, mFlag, mHealth, mMaxHelath, mDamage, mDefence, mRange)
	{}

	~Soldier() = default;

	void TakeDamage(float val) override;
	void Move(const JimmyGod::Math::Vector3& pos) override;
	void Attack() override;
};