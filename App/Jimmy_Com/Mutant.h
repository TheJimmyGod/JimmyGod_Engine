#pragma once

#include "Unit.h"
#include "CharacterModule.h"
#include "GridManager.h"
#include "UIManager.h"

class Mutant : public CharacterModule, public Unit
{
public:
	Mutant(std::string name, Flag flag) :
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

	~Mutant() = default;

	AgentComponent& GetAgent() override { return *mGameObject->GetComponent<AgentComponent>(); }
	const AgentComponent& GetAgent() const override { return *mGameObject->GetComponent<AgentComponent>(); }

	void Initialize(JimmyGod::GameWorld* gameWorld) override
	{
		ASSERT(gameWorld != nullptr, "The Game World does not exist!");
		gameWorld->Create("../../Assets/Templates/Mutant.json", mName);
		mGameObject = gameWorld->Find(mName).Get();
	}

	void Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path) override
	{
		ASSERT(gameWorld != nullptr, "The Game World does not exist!");
		gameWorld->Create(path, mName);
		mGameObject = gameWorld->Find(mName).Get();
	}

	void SetProcess(bool p) override { isProcess = p; mAnimationProcess = p; }

	void TakeDamage(float val) override
	{
		if (isDead)
			return;
		if (val - mDefence > 0.0f)
		{
			float tookDmg = val - mDefence;
			mHealth -= tookDmg;
			
			std::string str = std::to_string(static_cast<int>(tookDmg));
			UIManager::Get()->RenderText(str.c_str(), GetAgent().GetPosition(), 50.0f, 1.0f, Colors::Blue);
		}
		if (mHealth < 0.0f)
		{
			isDead = true;
			// TODO: Dead animation
			GetAgent().GetModelComponent().PlayAnimation(3);
		}
	}
};