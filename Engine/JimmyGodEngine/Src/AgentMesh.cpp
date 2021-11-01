#include "Precompiled.h"
#include "AgentMesh.h"

using namespace JimmyGod;
using namespace JimmyGod::AI;
using namespace JimmyGod::Math;

void JimmyGod::AgentMesh::Terminate()
{
	mSteeringModule.reset();
	mDecisionModule.reset();
}

void JimmyGod::AgentMesh::Update(float deltaTime)
{
	if (isInitialized)
	{
		Vector3 force = Vector3{ mSteeringModule->Calculate().x,0.0f,mSteeringModule->Calculate().y };
		Vector3 accelration = (force / Mass);
		auto newAccel = accelration * deltaTime;
		Velocity += Vector2{ newAccel.x,newAccel.z };

		auto speed = Magnitude(Velocity);
		if (speed > MaxSpeed)
			Velocity = Velocity / speed * MaxSpeed;
		auto newVel = Velocity * deltaTime;
		Position += Vector2{ newVel.x,newVel.y };
		Position3D += Vector3{ newVel.x, 0.0f, newVel.y };

		if (speed > 0.0f)
			Heading = Normalize(Velocity);


		mDecisionModule->Update();
	}
}

void JimmyGod::AgentMesh::AddStrategy(DecisionState state)
{
	if (state == DecisionState::Attack)
	{
		//mDecisionModule->AddStrategy<>();
	}

}

void JimmyGod::AgentMesh::Initialize()
{
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSteeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(false);
	mSteeringModule->AddBehavior<SeekBehavior>("Seek")->SetActive(false);
	mSteeringModule->AddBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	mSteeringModule->AddBehavior<AvoidObsBehavior>("Avoid")->SetActive(false);
	mSteeringModule->AddBehavior<WallAvoidBehvior>("Wall")->SetActive(false);
	mSteeringModule->AddBehavior<FleeBehavior>("Flee")->SetActive(false);
	mDecisionModule = std::make_unique<AI::DecisionModule<AgentMesh>>(*this);
	isInitialized = true;
}
