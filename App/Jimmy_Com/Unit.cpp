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

bool Unit::AttackUpdate(float deltaTime)
{
	if (mUpdateTime <= 0.0f) return true;
	else
	{
		mUpdateTime -= deltaTime;
		return false;
	}
}

const Sphere& Unit::GetSphereCollider() const
{
	return GetAgent().GetColliderComponent()->GetSphere();
}

void Unit::Move(const Coord& pos)
{
	if (GridManager::Get() == nullptr) return;
	GridManager::Get()->GetGird().FindPath(mCurrentCoordinate, pos, mRange, GetAgent().mPath, AI::PathFind::AStar);

	//if (GetAgent().mPath.empty())
	//{
	//	while (GetAgent().mPath.empty())
	//	{
	//		auto x = RandomInt(pos.x - 2, pos.x + 2);
	//		auto y = RandomInt(pos.y - 2, pos.y + 2);
	//		auto coord = Coord{ x,y };
	//		if (GridManager::Get()->GetGird().GetGraph().GetNode(coord)->GetWalkable() == false) continue;
	//		
	//		GridManager::Get()->GetGird().FindPath(mCurrentCoordinate, Coord{ x,y }, mRange, GetAgent().mPath, AI::PathFind::AStar);
	//	}
	//}

	while (GetAgent().mPath.size() > 2)
	{
		if (GameManager::Get()->IsExist(GridManager::Get()->GetGird().GetGraph().GetNode(GetAgent().mPath[GetAgent().mPath.size() - 1])->coordinate))
			GetAgent().mPath.pop_back();
		else
			break;
	}

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