#include "Unit.h"
#include "Flag.h"
#include "CharacterModule.h"
#include "GridManager.h"

void Unit::Attack(Unit& unit)
{
	unit.TakeDamage(mDamage);
}

bool Unit::AttackUpdate(float deltaTime)
{
	if (mTime <= 0.0f)
		return true;
	else
	{
		mTime -= deltaTime;
		return false;
	}
}

const JimmyGod::Math::Sphere& Unit::GetSphereCollider() const
{
	return GetAgent().GetColliderComponent().GetSphere();
}

void Unit::Move(const JimmyGod::AI::Coord& pos)
{
	if (GridManager::Get() == nullptr) return;
	JimmyGod::AI::Coord destiniation = pos;
	JimmyGod::AI::Coord current = GridManager::Get()->GetGraph().GetNode(GetAgent().GetPosition())->coordinate;
	GridManager::Get()->GetGird().FindPath(current, destiniation, GetAgent().mArea, GetAgent().mPath);
	if (GetAgent().mPath.size() > 0)
		GetAgent().ChangeState("Move");
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

