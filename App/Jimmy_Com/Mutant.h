#pragma once

#include "Unit.h"

class Mutant : public Unit
{
public:
	Mutant() : Unit(mName,mFlag,mHealth,mMaxHelath,mDamage,mDefence,mRange)
	{}

	~Mutant() = default;

	void TakeDamage(float val) override;
	void Move(const JimmyGod::Math::Vector3& pos) override;
	void Attack() override;
};