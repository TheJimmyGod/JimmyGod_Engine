#include "Soldier.h"
#include "GameManager.h"
#include "UIManager.h"

using namespace JimmyCom;
using namespace JimmyGod;
using namespace JimmyGod::Math;

JimmyCom::Soldier::Soldier(const std::string& name, Flag flag, const JimmyGod::Math::Vector3& pos, JimmyGod::GameWorld* gameWorld) :
	CharacterModule(), Unit(name, flag)
{
	mHealth = 20.0f;
	mDamage = 15.0f;
	mDefence = 1.0f;
	mRange = 5.0f;
	mMaxHelath = mHealth;
	mUnitType = UnitType::Soldier;
	isDead = false;

	ASSERT(gameWorld != nullptr, "The Game World does not exist!");
	gameWorld->Create("../../Assets/Templates/Batman.json", mName);
	mGameObject = gameWorld->Find(mName).Get();

	mGameObject->GetComponent<TransformComponent>()->SetPosition(pos);
	mGameObject->GetComponent<AgentComponent>()->SetCurrentCoord(GridManager::Get()->GetGird().GetGraph().GetNode(pos)->coordinate);

	LOG("Soldier - L-value!");
}

JimmyCom::Soldier::Soldier(const std::string& name, Flag flag, JimmyGod::Math::Vector3&& pos, JimmyGod::GameWorld* gameWorld) :
	CharacterModule(), Unit(name, flag)
{
	mHealth = 20.0f;
	mDamage = 15.0f;
	mDefence = 1.0f;
	mRange = 5.0f;
	mMaxHelath = mHealth;
	mUnitType = UnitType::Soldier;
	isDead = false;

	ASSERT(gameWorld != nullptr, "The Game World does not exist!");
	gameWorld->Create("../../Assets/Templates/Batman.json", mName);
	mGameObject = gameWorld->Find(mName).Get();

	mGameObject->GetComponent<TransformComponent>()->SetPosition(pos);
	mGameObject->GetComponent<AgentComponent>()->SetCurrentCoord(GridManager::Get()->GetGird().GetGraph().GetNode(pos)->coordinate);

	LOG("Soldier - R-value!");
}

Soldier& JimmyCom::Soldier::operator=(Soldier&& rhs)
{
	if (this == &rhs) return *this;
	CharacterModule::operator=(std::move(rhs));
	Unit::operator=(std::move(rhs));
	*this = std::move(rhs);
	return *this;
}

void JimmyCom::Soldier::TakeDamage(float val)
{
	if (isDead)
		return;
	if (val - mDefence > 0.0f)
	{
		float tookDmg = val - mDefence;
		mHealth -= tookDmg;

		std::string str = std::to_string(static_cast<int>(tookDmg));
		UIManager::Get()->RenderText(str.c_str(), GetPosition(), 50.0f, 1.0f, Colors::Red);
	}
	if (mHealth < 0.0f)
	{
		isDead = true;
		mAnimationProcess = true;
		SetStatus(Status::Dead);
		mGameObject->GetComponent<ModelComponent>()->PlayAnimation(5);
		mGameObject->GetComponent<ModelComponent>()->SetAnimationTime(0.0f);
		mGameObject->GetComponent<ModelComponent>()->GetAnimator().StopLoop(true);
	}
}

void JimmyCom::Soldier::Reset()
{
	isDead = false;
	mHealth = mMaxHelath;
	mAnimationProcess = true;
	mGameObject->GetComponent<ModelComponent>()->GetAnimator().StopLoop(false);
}

void JimmyCom::Soldier::Move(const AI::Coord& curr, const AI::Coord& dest)
{
	if (GridManager::Get() == nullptr) return;
	SetStatus(Status::Move);

	auto& agent = GetAgentComponent();
	GridManager::Get()->GetGird().FindPath(curr, dest, GetAgentComponent().mPath);

	while (GetAgentComponent().mPath.size() > 2)
	{
		if (GameManager::Get()->IsExist(GridManager::Get()->GetGird().GetGraph().GetNode(GetAgentComponent().mPath[GetAgentComponent().mPath.size() - 1])->coordinate))
			GetAgentComponent().mPath.pop_back();
		else
			break;
	}

	if (GetAgentComponent().mPath.size() > 0) GetAgentComponent().ChangeState("Move");
}
