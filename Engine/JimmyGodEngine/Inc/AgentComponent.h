#pragma once

#include "Component.h"

namespace JimmyGod
{
	class TransformComponent;
	class ColliderComponent;

	class AgentComponent final : public Component
	{
	public:
		META_CLASS_DECLARE

		void EnableDebug() { if (!isDebugUI) isDebugUI = true; else isDebugUI = false; }

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

	private:
		float mMovementSpeed = 2.0f;

		AI::Path mPath;
		bool isDebugUI = false;
		bool isActive = true;

		const TransformComponent* mTransformComponent = nullptr;
		const ColliderComponent* mColliderComponent = nullptr;
	};
}