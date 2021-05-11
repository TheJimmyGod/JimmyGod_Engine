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

	SpriteAnimationInfo spriteInfo;
	spriteInfo.fileName = "SmokeSprite.png";
	spriteInfo.columns = 8;
	spriteInfo.rows = 8;
	spriteInfo.frameCount = 64;
	spriteInfo.framePerSecond = 64.0f;
	spriteInfo.looping = true;
	mSmoke.Load(spriteInfo);

	mSteeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(false);
	mSteeringModule->AddBehavior<SeekBehavior>("Seek")->SetActive(false);
	mSteeringModule->AddBehavior<FleeBehavior>("Flee")->SetActive(false);
	mSteeringModule->AddBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	mSteeringModule->AddBehavior<AvoidObsBehavior>("Avoid")->SetActive(false);
	mSteeringModule->AddBehavior<WallAvoidBehvior>("Wall")->SetActive(false);
	mPlayerSprite = TextureManager::Get()->Load("survivor_handgun.png");
	MaxSpeed = 200.0f;
	Mass = 1.0f;
	Radius = 30.0f;
}

void Player::Unload()
{
	mPlayerSprite = 0;
	mSteeringModule.reset();
	mSmoke.Unload();
}

void Player::Update(float deltaTime)
{
	// auto& records = mPerceptionModule->GetMemoryRecords();
	// for(auto& r : records)
	// try until you find a good 
	auto GS = JimmyGod::Graphics::GraphicsSystem::Get();
	auto Input = JimmyGod::Input::InputSystem::Get();
	auto force = mSteeringModule->Calculate();
	auto accelration = (force / Mass);
	Velocity += accelration * deltaTime;


	auto speed = Magnitude(Velocity);
	if (speed > MaxSpeed)
		Velocity = Velocity / speed * MaxSpeed;

	mSmoke.Update(deltaTime);
	mSmoke.SetPosition(Position);

	if (speed > 100.0f)
	{
		if (isStarted == false)
		{
			isStarted = true;
			mSmoke.Play();
		}
	}
	else
	{
		isStarted = false;
		mSmoke.Stop();
	}
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
}

void Player::Render()
{
	mSmoke.Render();
	const float angle = atan2(Heading.y, Heading.x) * Math::Constants::RadToDeg / 50.0f;
	SpriteRenderManager::Get()->DrawSprite(mPlayerSprite, Position,angle);
}
