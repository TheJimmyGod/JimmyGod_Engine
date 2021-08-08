#pragma once
#include "Flag.h"
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class Unit
{
public:
	Unit(std::string name, Flag flag, float health, float dmg, float def, float range);

	virtual ~Unit() = default;

	void Initialize(JimmyGod::GameWorld* gameWorld, const std::filesystem::path& path);
	void Terminate();
	void Update(float deltaTime);
	void Render(const JimmyGod::Graphics::Camera& camera);
	void DebugUI();
	void Move(const JimmyGod::AI::Coord& pos);
	void PlayAnimation(int index) { mGameObject->GetComponent<ModelComponent>()->PlayAnimation(index); }

	const AgentComponent& GetAgent() const { return *mGameObject->GetComponent<AgentComponent>(); }
	AgentComponent& GetAgent() { return *mGameObject->GetComponent<AgentComponent>(); }

	virtual void Attack(Unit& unit) = 0;
	virtual void TakeDamage(float val) = 0;
	
protected:
	float mHealth = 0.0f;
	float mMaxHelath = 0.0f;
	float mDamage = 0.0f;
	float mDefence = 0.0f;
	float mRange = 0.0f;
	Flag mFlag = Flag::Neutral;
	std::string mName;
	bool isDead = true;

	JimmyGod::GameObject* mGameObject = nullptr;
private:
	struct TransformData
	{
		JimmyGod::Math::Matrix4 world;
		JimmyGod::Math::Matrix4 wvp;
		JimmyGod::Math::Vector3 viewPosition;
		float padding;
	};

	using TransformBuffer = TypedConstantBuffer<TransformData>;
	TransformBuffer mTransformBuffer;
};