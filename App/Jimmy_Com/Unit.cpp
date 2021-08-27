#include "Unit.h"
#include "Flag.h"
#include "CharacterModule.h"
#include "GridManager.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::AI;
using namespace JimmyCom;

void Unit::Attack(Unit& unit)
{
	unit.TakeDamage(mDamage);
}

bool Unit::AttackUpdate(float deltaTime)
{
	if (mTime <= 0.0f) return true;
	else
	{
		mTime -= deltaTime;
		return false;
	}
}

const Sphere& Unit::GetSphereCollider() const
{
	return GetAgent().GetColliderComponent().GetSphere();
}

void Unit::Move(const Coord& pos)
{
	if (GridManager::Get() == nullptr) return;
	GridManager::Get()->GetGird().FindPath(mCurrentCoordinate, pos, mRange, GetAgent().mPath);
	if (GetAgent().mPath.size() > 0) GetAgent().ChangeState("Move");
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
		GetAgent().Dead();
	}
}

