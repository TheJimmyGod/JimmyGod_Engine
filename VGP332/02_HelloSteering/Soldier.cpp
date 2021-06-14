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
	auto Calculator = [this](const AI::Agent& agent, AI::MemoryRecord& m)
	{
		Vector2 pos = std::get<Vector2>(m.properties["lastSeenPosition"]);
		float dist = Distance(agent.Position, pos);
		m.importance = dist / agent.Radius;
		return m.importance;
	};

	mSteeringModule = std::make_unique<SteeringModule>(*this);
	mPerceptionModule = std::make_unique<PerceptionModule>(*this, Calculator);


	mSteeringModule->AddBehavior<SeekBehavior>("Seek")->SetActive(false);
	mSteeringModule->AddBehavior<FleeBehavior>("Flee")->SetActive(false);
	mSteeringModule->AddBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	mSteeringModule->AddBehavior<PursuitBehavior>("Pursuit")->SetActive(false);
	mSteeringModule->AddBehavior<EvadeBehavior>("Evade")->SetActive(false);
	mSteeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(false);
	mSteeringModule->AddBehavior<AvoidObsBehavior>("Avoid")->SetActive(true);
	mSteeringModule->AddBehavior<HideBehavior>("Hide")->SetActive(false);
	mSteeringModule->AddBehavior<WallAvoidBehvior>("Wall")->SetActive(true);

	mSteeringModule->AddBehavior<SeparationBehavior>("Separation")->SetActive(false);
	mSteeringModule->AddBehavior<AlignmentBehavior>("Alignment")->SetActive(false);
	mSteeringModule->AddBehavior<CohesionBehavior>("Cohesion")->SetActive(false);
	mSteeringModule->AddBehavior<EnforceNonPenetrationConstraint>("Enforce")->SetActive(false);

	mPerceptionModule->SetMemorySpan(0.25f);
	mPerceptionModule->AddSensor<VisualSensor>("VisualSense");

	SpriteAnimationInfo spriteInfo;
	spriteInfo.fileName = "SmokeSprite.png";
	spriteInfo.columns = 8;
	spriteInfo.rows = 8;
	spriteInfo.frameCount = 64;
	spriteInfo.framePerSecond = 64.0f;
	spriteInfo.looping = true;

	mSmoke.Load(spriteInfo);

	mSoldierSprite = TextureManager::Get()->Load("Enemy_Sprite.png");
	mEmojiSprite = TextureManager::Get()->Load("EmojiTexture.png");
	MaxSpeed = 200.0f;
	Mass = 1.0f;
	Radius = 20.0f;
}

void Soldier::Unload()
{
	mSoldierSprite = 0;
	mEmojiSprite = 0;
	mSteeringModule.reset();
}

void Soldier::Update(float deltaTime)
{
	// auto& records = mPerceptionModule->GetMemoryRecords();
// for(auto& r : records)
// try until you find a good

	neighbors = world.GetNeighborhood({ Position, 100.0f }, (uint32_t)1);
	[[maybe_unused]] auto n = std::remove_if(neighbors.begin(), neighbors.end(), [this](auto neighbor)
	{
		return this == neighbor;
	});

	if (isDebug)
	{
		mPerceptionModule->Update(deltaTime);
		auto& records = mPerceptionModule->GetMemoryRecords();

		for (auto& r : records)
		{
			const auto& val = std::get<JimmyGod::Math::Vector2>(r.properties.find("lastSeenPosition")->second);
			if (r.importance < 3)
			{
				isPercepted = true;
				mEmojiTimer = 0.3f;
				LastSeenPos = val;
			}
			else
			{
				LastSeenPos = Vector2::Zero;
			}
		}
		if (!IsZero(LastSeenPos))
		{
			if (Distance(Position, LastSeenPos) < mPerceptionModule->GetSensor<VisualSensor>("VisualSense")->viewRange)
				JimmyGod::Graphics::SimpleDraw::AddScreenLine(Position, LastSeenPos, JimmyGod::Graphics::Colors::Yellow);
		}
		if (mEmojiTimer > 0.0f)
			mEmojiTimer -= deltaTime;
		else
		{
			LastSeenPos = Vector2::Zero;
			isPercepted = false;
		}
	}


	auto GS = JimmyGod::Graphics::GraphicsSystem::Get();
	// Update neighbors (exclude itself)


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


	mSmoke.Update(deltaTime);
	mSmoke.SetPosition(Position);
	if (speed > 120.0f)
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
	mTimer += deltaTime;

}

void Soldier::Render()
{
	mSmoke.Render();
	const float angle = atan2(Heading.y, Heading.x) * Math::Constants::RadToDeg / 60.0f; // Texture frames

	SpriteRenderManager::Get()->DrawSprite(mSoldierSprite, Position,angle);
	if (isPercepted && isDebug)
		SpriteRenderManager::Get()->DrawSprite(mEmojiSprite, Vector2{ Position.x + Radius /2.0f, Position.y - Radius});
}
