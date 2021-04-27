#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

META_DERIVED_BEGIN(ColliderComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(center, "Center")
		META_FIELD(extend, "Extend")
	META_FIELD_END
META_CLASS_END

void JimmyGod::ColliderComponent::Initialize() // simliar Start() as Monobehavior in Unity
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void JimmyGod::ColliderComponent::Terminate()
{
}

void JimmyGod::ColliderComponent::Update(float deltaTime)
{
}

void JimmyGod::ColliderComponent::Render()
{

}

void JimmyGod::ColliderComponent::DebugUI()
{
	auto aabb = GetAABB();
	SimpleDraw::AddAABB({aabb.center, aabb.extend}, Colors::LightGreen);
}

void JimmyGod::ColliderComponent::SetAABB(const Math::AABB & aabb)
{
	center = aabb.center;
	extend = aabb.extend;
}

const Math::AABB & ColliderComponent::GetAABB() const
{
	if (!isActive)
		return Math::AABB{};
	auto translation = mTransformComponent->pos;

	// This is incorrect if we have orientation as well
	return {translation + center,extend};
}

const Math::OBB & JimmyGod::ColliderComponent::GetOBB() const
{
	if (!isActive)
		return Math::OBB();
	return Math::OBB(center,extend, Quaternion::Identity);
}

const Math::Sphere JimmyGod::ColliderComponent::GetSphere() const
{
	return Math::Sphere(center,mRadius);
}

void JimmyGod::ColliderComponent::Active()
{
	isActive = !isActive;
	if (!isActive)
		isDebugUI = false;
}

bool JimmyGod::ColliderComponent::CheckAABBCollider(const Math::AABB & a, const Math::AABB & b)
{
	Vector3 AABBForA1 = a.center + a.center + a.extend;
	Vector3 AABBForA2 = a.center+ a.center- a.extend;
	Vector3 AABBForB1 = b.center+ b.center+ b.extend;
	Vector3 AABBForB2 = b.center + b.center- b.extend;
	if (AABBForA1.x < AABBForB2.x)
		return false;
	if (AABBForA2.x > AABBForB1.x)
		return false;
	if (AABBForA1.y < AABBForB2.y)
		return false;
	if (AABBForA2.y > AABBForB1.y)
		return false;
	if (AABBForA1.z < AABBForB2.z)
		return false;
	if (AABBForA2.z > AABBForB1.z)
		return false;
	return true;
}

const Vector3 & ColliderComponent::GetMin() const
{
	return mAABB.Min();
}

const Vector3 & ColliderComponent::GetMax() const
{
	return mAABB.Max();
}
