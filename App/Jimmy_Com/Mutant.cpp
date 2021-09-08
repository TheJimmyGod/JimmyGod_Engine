#include "Mutant.h"
#include "UIManager.h"

using namespace JimmyCom;
using namespace JimmyGod;
using namespace JimmyGod::Math;

JimmyCom::Mutant::Mutant(std::string name, Flag flag) :
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

AgentComponent& JimmyCom::Mutant::GetAgent()
{
	return *mGameObject->GetComponent<AgentComponent>();
}

const AgentComponent& JimmyCom::Mutant::GetAgent() const
{
	return *mGameObject->GetComponent<AgentComponent>();
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

void JimmyCom::Mutant::SetProcess(bool p)
{
	mAnimationProcess = p;
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
