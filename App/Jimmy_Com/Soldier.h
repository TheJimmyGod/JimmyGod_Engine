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
		isDead = false;
	}

	~Soldier() = default;

	AgentComponent& GetAgent() override { return *mGameObject->GetComponent<AgentComponent>(); }
	const AgentComponent& GetAgent() const override { return *mGameObject->GetComponent<AgentComponent>(); }

	const JimmyGod::Math::Sphere& GetSphereCollider() const override
	{
		return mGameObject->GetComponent<ColliderComponent>()->GetSphere();
	}

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
		GetAgent().GetTransformComponent().SetRotation(GetAgent().GetPosition() - unit.GetAgent().GetPosition());
		mTime = 1.75f;
		mGameObject->GetComponent<ModelComponent>()->PlayAnimation(3);
		if (Distance(GetAgent().GetPosition(), unit.GetAgent().GetPosition()) < mRange)
			unit.TakeDamage(mDamage);
	}

	void Move(const JimmyGod::AI::Coord& pos) override
	{
		if (GridManager::Get() == nullptr) return;
		JimmyGod::AI::Coord destiniation = pos;
		JimmyGod::AI::Coord current = GridManager::Get()->GetGraph().GetNode(GetAgent().GetPosition())->coordinate;
		GridManager::Get()->GetGird().FindPath(current, destiniation, GetAgent().mArea, GetAgent().mPath);
		if (GetAgent().mPath.size() > 0)
			GetAgent().ChangeState("Move");
	}
};