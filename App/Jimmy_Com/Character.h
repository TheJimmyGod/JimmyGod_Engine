#pragma once
#include "Unit.h"
#include "CharacterModule.h"
#include "Soldier.h"
#include "Mutant.h"
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace JimmyCom
{
	class Character
	{
	public:
		Character(std::string name, Flag flag) : mName(name), mFlag(flag) {}

		//Character Module
		virtual void Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path) = 0;
		virtual void Initialize(JimmyGod::GameWorld* gameWorld) = 0;
		virtual void Terminate() {}
		virtual void Update(float deltaTime) {}
		virtual void Render(const JimmyGod::Graphics::Camera& camera) {}
		// Unit
		virtual bool AttackUpdate(float deltaTime) { return false; }
		virtual void Attack(Character& unit) {}
		virtual void Move(const JimmyGod::AI::Coord& pos) {}
		virtual void TakeDamage(float val) {}

		const Flag& GetFlag() const { return mFlag; }
		const UnitType& GetUnitType() const { return mUnitType; }
		const Status& GetStatus() const { return mStatus; }
		const std::string& GetName() const { return mName; }

		void SetStatus(Status s) { mStatus = s; }
		void SetPosition(const Vector3& pos) { mGameObject->GetComponent<TransformComponent>()->SetPosition(pos); }
		void SetRotation(const Quaternion& q) { mGameObject->GetComponent<TransformComponent>()->SetRotation(q); }
		void SetCoordinate(const AI::Coord& coord) { mCurrentCoordinate = coord; }

		const JimmyGod::Math::Vector3& GetPosition() const { return mGameObject->GetComponent<TransformComponent>()->GetPosition(); }
		const JimmyGod::AI::Coord& GetCoordinate() const { return mCurrentCoordinate; }
		const JimmyGod::Math::Quaternion& GetRotation() const { return mGameObject->GetComponent<TransformComponent>()->GetRotation(); }

		const JimmyGod::AgentComponent& GetAgentComponent() const { return *mGameObject->GetComponent<AgentComponent>(); }
		const JimmyGod::AnimatorComponent& GetAnimatorComponent() const {return *mGameObject->GetComponent<AnimatorComponent>(); }
		JimmyGod::AnimatorComponent& GetAnimatorComponent() { return *mGameObject->GetComponent<AnimatorComponent>(); }

		const JimmyGod::ModelComponent& GetModelComponent() const { return *mGameObject->GetComponent<ModelComponent>(); }
		JimmyGod::ModelComponent& GetModelComponent() { return *mGameObject->GetComponent<ModelComponent>(); }

		const JimmyGod::Math::Sphere& GetCollider() const { return mGameObject->GetComponent<ColliderComponent>()->GetSphere(); }

		bool mAnimationProcess = false;
		float mUpdateTime = 0.0f;
	protected:
		friend class Soldier;
		friend class Mutant;

		JimmyGod::GameObject* mGameObject = nullptr;
		AI::Coord mCurrentCoordinate = { 0,0 };
		Flag mFlag = Flag::Neutral;
		Status mStatus = Status::Standby;
		UnitType mUnitType = UnitType::Soldier;
		std::string mName;

	};
}