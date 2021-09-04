#include "Mutant.h"
#include "GridManager.h"
#include "UIManager.h"

using namespace JimmyCom;
using namespace JimmyGod;
using namespace JimmyGod::Math;

JimmyCom::Mutant::Mutant(std::string name, Flag flag) :
	CharacterModule(), Unit(name, flag)
{
	mHealth = 150.0f;
	mDamage = 8.0f;
	mDefence = 2.0f;
	mRange = 1.0f;
	mMaxHelath = mHealth;
	mUnitType = UnitType::Mutant;
	isDead = false;
}

Mutant& JimmyCom::Mutant::operator=(Mutant&& rhs)
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
		mAnimationProcess = false;
		SetStatus(Status::Dead);
		GetAgent().GetModelComponent().PlayAnimation(3);
	}
}
