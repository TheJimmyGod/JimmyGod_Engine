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

	mPath.clear();

	delete mAgent;
	mAgent = nullptr;
}

void JimmyGod::AgentComponent::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);

	if (ActiveSteering)
	{
		Vector3 force = Vector3{ mSteeringModule->Calculate().x,0.0f,mSteeringModule->Calculate().y };
		Vector3 accelration = (force / mAgent->Mass);
		auto newAccel = accelration * deltaTime;
		mAgent->Velocity += Vector2{newAccel.x,newAccel.z};

		auto speed = Magnitude(mAgent->Velocity);
		if (speed > mAgent->MaxSpeed)
			mAgent->Velocity = mAgent->Velocity / speed * mAgent->MaxSpeed;
		auto newVel = mAgent->Velocity * deltaTime;
		mTransformComponent->pos += Vector3{newVel.x,mTransformComponent->pos.y,newVel.y};

		if (speed > 0.0f)
			mAgent->Heading = Normalize(mAgent->Velocity);
	}
}
void JimmyGod::AgentComponent::DebugUI()
{
	if (isDebugUI == false) return;
	if (mPath.empty()) return;

	for (auto& path : mPath)
	{
		JimmyGod::Graphics::SimpleDraw::AddSphere(path,0.5f, Colors::Red,6,6);
	}
}

void JimmyGod::AgentComponent::AgentInitilize(AI::AIWorld& aiWorld, uint32_t num)
{
	mAgent = new AI::Agent(aiWorld, num);

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this->mAgent);
	mSteeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(false);
	mSteeringModule->AddBehavior<SeekBehavior>("Seek")->SetActive(false);
	mSteeringModule->AddBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	mSteeringModule->AddBehavior<AvoidObsBehavior>("Avoid")->SetActive(false);
	mSteeringModule->AddBehavior<WallAvoidBehvior>("Wall")->SetActive(false);
	mSteeringModule->AddBehavior<FleeBehavior>("Flee")->SetActive(false);

	ActiveSteering = true;
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