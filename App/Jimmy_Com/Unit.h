#pragma once
#include "Flag.h"
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class Unit
{
public:
	Unit(std::string name, Flag flag , float health, float maxHealth, float dmg, float def, float range):
		mName(name), mFlag(flag), mHealth(health), mMaxHelath(maxHealth), mDamage(dmg), mDefence(def), mRange(range)
	{}

	virtual ~Unit() = default;

	void Initialize(JimmyGod::GameWorld* gameWorld) {};
	void Terminate() {};
	void Update(float deltaTime) {};
	void Render() {};

	virtual void Attack() {};
	virtual void TakeDamage(float val) {};
	virtual void Move(const JimmyGod::Math::Vector3& pos) {};
protected:
	float mHealth = 0.0f;
	float mMaxHelath = 0.0f;
	float mDamage = 0.0f;
	float mDefence = 0.0f;
	float mRange = 0.0f;
	Flag mFlag = Flag::Neutral;
	std::string mName;

	GameObject* gameObject = nullptr;
};