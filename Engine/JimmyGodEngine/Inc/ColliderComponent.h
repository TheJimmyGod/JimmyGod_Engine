#pragma once
#include "Component.h"

namespace JimmyGod
{
	class TransformComponent;

	class ColliderComponent final : public Component
	{
	public:
		META_CLASS_DECLARE

		void EnableDebug() { if (!isDebugUI) isDebugUI = true; else isDebugUI = false; }

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void SetAABB(const Math::AABB& aabb);
		const Math::AABB& GetAABB() const;
		const Math::OBB& GetOBB() const;
		const Math::Sphere& GetSphere()const;

		void Active(bool active) { isActive = active; }

		bool CheckAABBCollider(const Math::AABB& a, const Math::AABB& b);

		const Vector3& GetMin() const;
		const Vector3& GetMax() const;

		Math::Vector3 center = Math::Vector3::Zero;
		Math::Vector3 extend = Math::Vector3::Zero;
		float radius = 0.0f;
	private:
		Math::AABB mAABB;
		const TransformComponent* mTransformComponent = nullptr;

		Math::Sphere mSphere;
		bool isDebugUI = false;
		bool isActive = false;
	};
}