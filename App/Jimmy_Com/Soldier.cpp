#include "Soldier.h"
#include "UIManager.h"

using namespace JimmyCom;
using namespace JimmyGod;
using namespace JimmyGod::Math;

JimmyCom::Soldier::Soldier(const std::string& name, Flag flag) :
	CharacterModule(), Unit(name, flag)
{
	mHealth = 20.0f;
	mDamage = 15.0f;
	mDefence = 1.0f;
	mRange = 5.0f;
	mMaxHelath = mHealth;
	mUnitType = UnitType::Soldier;
	isDead = false;
}

Soldier& JimmyCom::Soldier::operator=(Soldier&& rhs)
{
	if (this == &rhs) return *this;
	CharacterModule::operator=(std::move(rhs));
	Unit::operator=(std::move(rhs));
	*this = std::move(rhs);
	return *this;
}

AgentComponent& JimmyCom::Soldier::GetAgent()
{
	return *mGameObject->GetComponent<AgentComponent>();
}

const AgentComponent& JimmyCom::Soldier::GetAgent() const
{
	return *mGameObject->GetComponent<AgentComponent>();
}

const ModelComponent& JimmyCom::Soldier::GetModelComponent() const
{
	return *mGameObject->GetComponent<ModelComponent>();
}

ModelComponent& JimmyCom::Soldier::GetModelComponent()
{
	return *mGameObject->GetComponent<ModelComponent>();
}

void JimmyCom::Soldier::Initialize(JimmyGod::GameWorld* gameWorld)
{
	ASSERT(gameWorld != nullptr, "The Game World does not exist!");
	gameWorld->Create("../../Assets/Templates/Batman.json", mName);
	mGameObject = gameWorld->Find(mName).Get();
}

void JimmyCom::Soldier::Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path)
{
	ASSERT(gameWorld != nullptr, "The Game World does not exist!");
	gameWorld->Create(path, mName);
	mGameObject = gameWorld->Find(mName).Get();
}

void JimmyCom::Soldier::SetProcess(bool p)
{
	mAnimationProcess = p;
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
