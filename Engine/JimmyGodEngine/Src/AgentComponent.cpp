#include "Precompiled.h"
#include "AgentComponent.h"

#include "GameObject.h"
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
	mSteeringModule.reset();
	mDecisionModule.reset();
	mPath.clear();
}

void JimmyGod::AgentComponent::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
	if (isInitialized)
	{
		Vector3 force = Vector3{ mSteeringModule->Calculate().x,0.0f,mSteeringModule->Calculate().y };
		Vector3 accelration = (force / mAgentMesh->Mass);
		auto newAccel = accelration * deltaTime;
		mAgentMesh->Velocity += Vector2{ newAccel.x,newAccel.z };

		auto speed = Magnitude(mAgentMesh->Velocity);
		if (speed > mAgentMesh->MaxSpeed)
			mAgentMesh->Velocity = mAgentMesh->Velocity / speed * mAgentMesh->MaxSpeed;
		auto newVel = mAgentMesh->Velocity * deltaTime;
		mAgentMesh->Position += Vector2{ newVel.x,newVel.y };
		mTransformComponent->pos += Vector3{ newVel.x, 0.0f, newVel.y };

		if (speed > 0.0f)
			mAgentMesh->Heading = Normalize(mAgentMesh->Velocity);


		mDecisionModule->Update();
	}
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
	mAgentMesh = new Agent(aiWorld,num);

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this->mAgentMesh);
	mSteeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(false);
	mSteeringModule->AddBehavior<SeekBehavior>("Seek")->SetActive(false);
	mSteeringModule->AddBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	mSteeringModule->AddBehavior<AvoidObsBehavior>("Avoid")->SetActive(false);
	mSteeringModule->AddBehavior<WallAvoidBehvior>("Wall")->SetActive(false);
	mSteeringModule->AddBehavior<FleeBehavior>("Flee")->SetActive(false);
	mDecisionModule = std::make_unique<AI::DecisionModule<AgentComponent>>(*this);

	isInitialized = true;
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

void JimmyGod::AgentComponent::AddStrategy(DecisionState state)
{
	if (state == DecisionState::Attack)
	{
		//mDecisionModule->AddStrategy<>();
	}
}

const float JimmyGod::AgentComponent::GetSpeed() const
{
	return mSpeed;
}