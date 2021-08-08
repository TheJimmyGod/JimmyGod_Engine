#pragma once

#include "Unit.h"

class Mutant : public Unit
{
public:
	Mutant(std::string name, Flag flag, float health, float dmg, float def, float range) :
		Unit(name, flag, health, dmg, def, range)
	{}

	~Mutant() = default;

	void TakeDamage(float val) override;
	void Attack(Unit& unit) override;
};