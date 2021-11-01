#include "Precompiled.h"
#include "AgentComponent.h"

#include "GameObject.h"
#include "AgentMesh.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::AI;

META_DERIVED_BEGIN(AgentComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(mMovementSpeed,"Speed")
		META_FIELD(mMass, "Weight")
		META_FIELD(mArea,"Area")
	META_FIELD_END
META_CLASS_END

void JimmyGod::AgentComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mColliderComponent = GetOwner().GetComponent<ColliderComponent>();

	mStateMachine = std::make_unique<StateMachine<AgentComponent>>(*this);
	mStateMachine->AddState<Idle>("Idle");
	mStateMachine->AddState<Move>("Move");
	mStateMachine->ChangeState("Idle");
}

void JimmyGod::AgentComponent::Terminate()
{
	mStateMachine.reset();

	mPath.clear();
	delete mAgentMesh;
	mAgentMesh = nullptr;
}

void JimmyGod::AgentComponent::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
	if (mAgentMesh && mAgentMesh->GetInitialized())
		mAgentMesh->Update(deltaTime);
}
void JimmyGod::AgentComponent::DebugUI()
{
	if (isDebugUI == false) return;
	if (mPath.empty()) return;

	for (auto& path : mPath)
	{
		JimmyGod::Graphics::SimpleDraw::AddSphere(path,0.75f, Colors::Red,24,24);
	}
}

void JimmyGod::AgentComponent::Initialize_AgentMesh(AI::AIWorld& aiWorld, uint32_t num)
{
	mAgentMesh = new AgentMesh(aiWorld,num);
}

void JimmyGod::AgentComponent::ChangeState(std::string stateName)
{
	mStateMachine->ChangeState(stateName);
}

void JimmyGod::AgentComponent::Dead()
{
	isActive = false;
}

void JimmyGod::AgentComponent::Stop()
{
	mSpeed = 0.0f;
	mPath.clear();
	ChangeState("Idle");
}

void JimmyGod::AgentComponent::Movement(const Vector3& pos, float deltaTime)
{
	mSpeed = Magnitude(pos);

	Quaternion rotation = Quaternion::RotationLook(mTransformComponent->pos - pos);
	mTransformComponent->SetRotation(Slerp(GetTransformComponent()->GetRotation(), rotation, deltaTime * 10.0f));
	mTransformComponent->SetPosition(pos);
}

const float JimmyGod::AgentComponent::GetSpeed() const
{
	return mSpeed;
}