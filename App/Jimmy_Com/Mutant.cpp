#include "Mutant.h"
#include "UIManager.h"
#include "GameManager.h"

using namespace JimmyCom;
using namespace JimmyGod;
using namespace JimmyGod::Math;

JimmyCom::Mutant::Mutant(const std::string& name, Flag flag) :
	CharacterModule(), Unit(name, flag)
{
	mHealth = 25.0f;
	mDamage = 25.0f;
	mDefence = 2.0f;
	mRange = 1.0f;
	mMaxHelath = mHealth;
	mUnitType = UnitType::Mutant;
	isDead = false;
}

Mutant& JimmyCom::Mutant::operator=(Mutant&& rhs) noexcept
{
	if (this == &rhs) return *this;
	CharacterModule::operator=(std::move(rhs));
	Unit::operator=(std::move(rhs));
	*this = std::move(rhs);
	return *this;
}

void JimmyCom::Mutant::Initialize(JimmyGod::GameWorld* gameWorld)
{
	ASSERT(gameWorld != nullptr, "The Game World does not exist!");
	gameWorld->Create("../../Assets/Templates/Mutant.json", mName);
	mGameObject = gameWorld->Find(mName).Get();
}

void JimmyCom::Mutant::Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path)
{
	ASSERT(gameWorld != nullptr, "The Game World does not exist!");
	gameWorld->Create(path, mName);
	mGameObject = gameWorld->Find(mName).Get();
}

void JimmyCom::Mutant::TakeDamage(float val)
{
	if (isDead)
		return;
	if (val - mDefence > 0.0f)
	{
		float tookDmg = val - mDefence;
		mHealth -= tookDmg;

		std::string str = std::to_string(static_cast<int>(tookDmg));
		UIManager::Get()->RenderText(str.c_str(), GetPosition(), 50.0f, 1.0f, Colors::Blue);
	}
	if (mHealth < 0.0f)
	{
		isDead = true;
		mAnimationProcess = true;
		SetStatus(Status::Dead);
		mGameObject->GetComponent<ModelComponent>()->PlayAnimation(3);
		mGameObject->GetComponent<ModelComponent>()->SetAnimationTime(0.0f);
		mGameObject->GetComponent<ModelComponent>()->GetAnimator().StopLoop(true);
	}
}

void JimmyCom::Mutant::Reset()
{
	isDead = false;
	mHealth = mMaxHelath;
	mAnimationProcess = true;
	mGameObject->GetComponent<ModelComponent>()->GetAnimator().StopLoop(false);
}

void JimmyCom::Mutant::Move(const JimmyGod::AI::Coord& pos)
{
	if (GridManager::Get() == nullptr) return;
	SetStatus(Status::Move);
	GridManager::Get()->GetGird().FindPath(mCurrentCoordinate, pos, mRange, GetAgentComponent().mPath, AI::PathFind::AStar);

	while (GetAgentComponent().mPath.size() > 2)
	{
		if (GameManager::Get()->IsExist(GridManager::Get()->GetGird().GetGraph().GetNode(GetAgentComponent().mPath[GetAgentComponent().mPath.size() - 1])->coordinate))
			GetAgentComponent().mPath.pop_back();
		else
			break;
	}

	if (GetAgentComponent().mPath.size() > 0) GetAgentComponent().ChangeState("Move");
}