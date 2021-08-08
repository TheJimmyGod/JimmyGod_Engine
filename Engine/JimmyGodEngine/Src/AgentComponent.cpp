#include "Precompiled.h"
#include "AgentComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "ModelComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::AI;

META_DERIVED_BEGIN(AgentComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(mMovementSpeed,"Speed")
		META_FIELD(mMass, "Weight")
		META_FIELD(mRadius, "Radius")
		META_FIELD(mArea,"Area")
	META_FIELD_END
META_CLASS_END

void JimmyGod::AgentComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mColliderComponent = GetOwner().GetComponent<ColliderComponent>();
	mModelComponent = GetOwner().GetComponent<ModelComponent>();

	mStateMachine = std::make_unique<StateMachine<AgentComponent>>(*this);
	mStateMachine->AddState<Idle>("Idle");
	mStateMachine->AddState<Move>("Move");
	mStateMachine->ChangeState("Idle");
}
void JimmyGod::AgentComponent::Terminate()
{
	mStateMachine.reset();
}
void JimmyGod::AgentComponent::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
}
void JimmyGod::AgentComponent::DebugUI()
{
	if (isDebugUI == false) return;
	if (mPath.empty()) return;

	for (auto& path : mPath)
	{
		JimmyGod::Graphics::SimpleDraw::AddSphere(path,0.25f, Colors::Red,6,6);
	}
}

void JimmyGod::AgentComponent::Movement(const Vector3& pos)
{
	mSpeed = Magnitude(pos);

	mTransformComponent->rot = Quaternion::RotationLook(GetTransformComponent().pos - pos);
	mTransformComponent->pos = pos;
}

const JimmyGod::Math::Vector3& JimmyGod::AgentComponent::GetPosition() const
{
	return mTransformComponent->GetPosition();
}

const float JimmyGod::AgentComponent::GetSpeed() const
{
	return mSpeed;
}