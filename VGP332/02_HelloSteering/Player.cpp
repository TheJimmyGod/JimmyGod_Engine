#include "Player.h"

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
using namespace JimmyGod;
using namespace JimmyGod::AI;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace Steering;

Player::Player(JimmyGod::AI::AIWorld& world)
	:JimmyGod::AI::Agent(world, (uint32_t)1)
{
	
}

void Player::Load()
{
	auto Calculator = [this](const AI::Agent& agent, AI::MemoryRecord& m)
	{
		const float maxRange = mPerceptionModule->GetSensor<VisualSensor>("VisualSense")->viewRange;
		Vector2 pos = std::get<Vector2>(m.properties["lastSeenPosition"]);
		float distToAgent = Distance(agent.Position, pos);
		float distPercent = Math::Clamp(distToAgent / maxRange, 0.0f, 1.0f);
		m.importance = (1 - distPercent) * 100.0f;
		return m.importance;
	};

	mPerceptionModule = std::make_unique<PerceptionModule>(*this, Calculator);

	mPerceptionModule->SetMemorySpan(0.25f);
	mPerceptionModule->AddSensor<VisualSensor>("VisualSense");
	mPerceptionModule->GetSensor<VisualSensor>("VisualSense")->viewRange = 450.0f;
	SetViewRange(mPerceptionModule->GetSensor<VisualSensor>("VisualSense")->viewRange);
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);

	auto GS = JimmyGod::Graphics::GraphicsSystem::Get();
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
	auto fleeBehavior = mSteeringModule->AddBehavior<FleeBehavior>("Flee");
	fleeBehavior->panicDistance = GetViewRange();
	fleeBehavior->SetActive(false);
	mSteeringModule->AddBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	mSteeringModule->AddBehavior<AvoidObsBehavior>("Avoid")->SetActive(false);
	mSteeringModule->AddBehavior<WallAvoidBehvior>("Wall")->SetActive(false);

	mStateMachine = std::make_unique<AI::StateMachine<Steering::Player>>(*this);
	mStateMachine->AddState<NoneState>("None");
	mStateMachine->AddState<FreeState>("Free");
	mStateMachine->AddState<RunAwayState>("RunAway");

	mStateMachine->ChangeState("None");
	mPlayerSprite = TextureManager::Get()->Load("Player_Sprite.png");
	MaxSpeed = 200.0f;
	Mass = 1.0f;
	Radius = 20.0f;
}

void Player::Unload()
{
	mPlayerSprite = 0;
	mSteeringModule.reset();
	mSmoke.Unload();

	mPerceptionModule.reset();
	
}

void Player::Update(float deltaTime)
{
	auto GS = JimmyGod::Graphics::GraphicsSystem::Get();
	auto Input = JimmyGod::Input::InputSystem::Get();
	auto force = mSteeringModule->Calculate();
	auto accelration = (force / Mass);
	Velocity += accelration * deltaTime;

	if (mPerceptionMode)
	{
		neighbors = world.GetNeighborhood({ Position, 100.0f }, (uint32_t)1);
		[[maybe_unused]] auto n = std::remove_if(neighbors.begin(), neighbors.end(), [this](auto neighbor)
		{
			return this == neighbor;
		});

		mStateMachine->Update(deltaTime);
		mPerceptionModule->Update(deltaTime);
	}

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
