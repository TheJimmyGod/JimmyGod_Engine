#pragma once

#include "Unit.h"

class Soldier: public Unit
{
public:
	Soldier(std::string name, Flag flag, float health, float dmg, float def, float range) : 
		Unit(name, flag, health, dmg, def, range)
	{
		isDead = false;
	}

	~Soldier() = default;

	void TakeDamage(float val) override
	{
		mHealth -= val;
		if (mHealth < 0.0f)
		{
			isDead = true;
			// TODO: Dead animation
			mGameObject->GetComponent<AgentComponent>()->Dead();
		}
	}
	void Attack(Unit& unit) override
	{
		if (Distance(GetAgent().GetPosition(), unit.GetAgent().GetPosition()) < mRange)
			unit.TakeDamage(mDamage);
	}
};