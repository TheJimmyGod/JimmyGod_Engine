#pragma once

#include "Unit.h"
#include "CharacterModule.h"
#include "GridManager.h"

class Soldier: public CharacterModule, public Unit
{
public:
	Soldier(std::string name, Flag flag) : 
		CharacterModule(), Unit(name, flag)
	{
		mHealth = 100.0f;
		mDamage = 10.0f;
		mDefence = 1.0f;
		mRange = 5.0f;
		mMaxHelath = mHealth;
		mUnitType = UnitType::Soldier;
		isDead = false;
	}

	~Soldier() = default;

	AgentComponent& GetAgent() override { return *mGameObject->GetComponent<AgentComponent>(); }
	const AgentComponent& GetAgent() const override { return *mGameObject->GetComponent<AgentComponent>(); }

	void Initialize(JimmyGod::GameWorld* gameWorld) override
	{
		ASSERT(gameWorld != nullptr, "The Game World does not exist!");
		gameWorld->Create("../../Assets/Templates/Batman.json", mName);
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
		if(val - mDefence > 0.0f)
			mHealth -= val - mDefence;
		if (mHealth < 0.0f)
		{
			isDead = true;
			// TODO: Dead animation
			GetAgent().Dead();
			mModuleActive = false;
		}
	}
	void Attack(Unit& unit) override
	{
		if (Distance(GetAgent().GetPosition(), unit.GetAgent().GetPosition()) < mRange)
			unit.TakeDamage(mDamage);
		// TODO: Make bullet
	}
};