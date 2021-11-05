#include "Unit.h"
#include "Flag.h"
#include "CharacterModule.h"
#include "GridManager.h"
#include "GameManager.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::AI;
using namespace JimmyCom;

Unit& JimmyCom::Unit::operator=(Unit&& rhs) noexcept
{
	if (this == &rhs)
		return *this;
	*this = std::move(rhs);
	return *this;
}

void Unit::Attack(Unit& unit)
{
	unit.TakeDamage(mDamage);
}


void Unit::TakeDamage(float val)
{
	if (isDead)
		return;
	if (val - mDefence > 0.0f)
		mHealth -= val - mDefence;
	if (mHealth < 0.0f)
	{
		isDead = true;
	}
}

const float JimmyCom::Unit::GetDamage() const
{
	return mDamage;
}

const float JimmyCom::Unit::GetCurrentHealth() const
{
	return mHealth;
}

const Flag& JimmyCom::Unit::GetFlag() const
{
	return mFlag;
}

const UnitType& JimmyCom::Unit::GetUnitType() const
{
	return mUnitType;
}

const Status& JimmyCom::Unit::GetStatus() const
{
	return mStatus;
}

const Unit& JimmyCom::Unit::GetUnit() const
{
	return *this;
}

const std::string& JimmyCom::Unit::GetName() const
{
	return mName;
}

Unit& JimmyCom::Unit::GetUnit()
{
	return *this;
}

void JimmyCom::Unit::SetStatus(Status s)
{
	mStatus = s;
}