#include "Soldier.h"

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
using namespace Steering;
using namespace JimmyGod;
using namespace JimmyGod::AI;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;



Soldier::Soldier(JimmyGod::AI::AIWorld & world)
	:JimmyGod::AI::Agent(world, (uint32_t)1)
{
}

void Soldier::Load()
{
	mSteeringModule = std::make_unique<SteeringModule>(*this);

	mSteeringModule->AddBehavior<SeekBehavior>("Seek")->SetActive(false);
	mSteeringModule->AddBehavior<FleeBehavior>("Flee")->SetActive(false);
	mSteeringModule->AddBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	mSteeringModule->AddBehavior<PursuitBehavior>("Pursuit")->SetActive(false);
	mSteeringModule->AddBehavior<EvadeBehavior>("Evade")->SetActive(false);
	mSteeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(false);
	mSteeringModule->AddBehavior<AviodObsBehavior>("Avoid")->SetActive(false);
	mSteeringModule->AddBehavior<HideBehavior>("Hide")->SetActive(false);

	mSteeringModule->AddBehavior<SeparationBehavior>("Separation")->SetActive(false);
	mSteeringModule->AddBehavior<AlignmentBehavior>("Alignment")->SetActive(false);
	mSteeringModule->AddBehavior<CohesionBehavior>("Cohesion")->SetActive(false);

	mSoldierSprite = TextureManager::Get()->Load("zombie_idle.png");
	MaxSpeed = 200.0f;
	Mass = 1.0f;
	Radius = 10.0f;
}

void Soldier::Unload()
{
	mSoldierSprite = 0;
	mSteeringModule.reset();
}

void Soldier::Update(float deltaTime)
{
	auto GS = JimmyGod::Graphics::GraphicsSystem::Get();
	// Update neighbors (exclude itself)
	neighbors = world.GetNeighborhood({ Position, 100.0f }, (uint32_t)1);
	std::remove_if(neighbors.begin(), neighbors.end(), [this](auto neighbor)
	{
		return this == neighbor;
	});

	if (mTimer > 5.0f)
	{
		if (Velocity.x > MaxSpeed)
			Velocity.x = MaxSpeed;
		if (Velocity.y > MaxSpeed)
			Velocity.y = MaxSpeed;
		if ((Velocity.x) < (MaxSpeed * -1))
			Velocity.x = MaxSpeed * -1;
		if ((Velocity.y) < (MaxSpeed * -1))
			Velocity.y = MaxSpeed * -1;
		mTimer = 0.0f;
	}

	auto force = mSteeringModule->Calculate();
	auto accelration = (force / Mass);
	Velocity += accelration * deltaTime;
	auto speed = Magnitude(Velocity);
	if (speed > MaxSpeed)
		Velocity = Velocity / speed * MaxSpeed;


	Position += Velocity * deltaTime;


	if (speed > 0.0f)
		Heading = Normalize(Velocity);

	if (Position.x < 0.0f)
		Position.x = (static_cast<float>(GS->GetBackBufferWidth()));
	else if (Position.x > (static_cast<float>(GS->GetBackBufferWidth())))
		Position.x = 0.0f;
	if (Position.y < 0.0f)
		Position.y = (static_cast<float>(GS->GetBackBufferHeight()));
	else if (Position.y > (static_cast<float>(GS->GetBackBufferHeight())))
		Position.y = 0.0f;
	mTimer += deltaTime;
}

void Soldier::Render()
{
	const float angle = atan2(Heading.y, Heading.x) * Math::Constants::RadToDeg / 50.0f; // Texture frames

	SpriteRenderManager::Get()->DrawSprite(mSoldierSprite, Position,angle);
}
