#include "Player.h"

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
using namespace JimmyGod;
using namespace JimmyGod::AI;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace Steering;

Player::Player(JimmyGod::AI::AIWorld& world)
	:JimmyGod::AI::Agent(world, (uint32_t)0)
{
	
}

void Player::Load()
{
	auto GS = JimmyGod::Graphics::GraphicsSystem::Get();
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	Position = (Vector2(GS->GetBackBufferWidth() * 0.5f, GS->GetBackBufferHeight() * 0.5f));

	mSteeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(true);
	mSteeringModule->AddBehavior<AviodObsBehavior>("Avoid")->SetActive(true);

	mPlayerSprite = TextureManager::Get()->Load("survivor_handgun.png");
	MaxSpeed = 200.0f;
	Mass = 1.0f;
	Radius = 12.0f;
}

void Player::Unload()
{
	mPlayerSprite = 0;
	mSteeringModule.reset();
}

void Player::Update(float deltaTime)
{
	// auto& records = mPerceptionModule->GetMemoryRecords();
	// for(records)
	// try until you find a good 
	auto GS = JimmyGod::Graphics::GraphicsSystem::Get();
	Destination = { static_cast<float>(InputSystem::Get()->GetMouseScreenX()),
		static_cast<float>(InputSystem::Get()->GetMouseScreenY()) };
	auto force = mSteeringModule->Calculate();
	auto accelration = (force / Mass);
	Velocity += accelration * deltaTime;
	auto speed = Magnitude(Velocity);
	if (speed > MaxSpeed)
	{
		Velocity = Velocity / speed * MaxSpeed;
	}


	Position += Velocity * deltaTime;
	

	if (speed > 0.0f)
	{
		Heading = Normalize(Velocity);
	}

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

void Player::Render()
{
	const float angle = atan2(Heading.y, Heading.x) * Math::Constants::RadToDeg / 50.0f;
	SpriteRenderManager::Get()->DrawSprite(mPlayerSprite, Position,angle);
}
