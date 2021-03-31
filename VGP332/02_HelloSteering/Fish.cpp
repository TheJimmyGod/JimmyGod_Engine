#include "Fish.h"

using namespace Steering;

Fish::Fish(AI::AIWorld& world)
	:Agent(world,UnitType::Mushroom)
{
}

void Fish::Load()
{
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	Position = (X::Math::Vector2(X::GetScreenWidth() * 0.5f, X::GetScreenHeight() * 0.5f));
	mSteeringModule->AddBehavior<AI::SeekBehavior>("Seek")->SetActive(true);
	//mSteeringModule->AddBehavior<AI::AviodObsBehavior>("Avoid")->SetActive(true);
	mSteeringModule->AddBehavior<AI::WallAvoidBehvior>("Wall")->SetActive(true);
	mFishSprite = X::LoadTexture("Mushroom.png");
	MaxSpeed = 200.0f;
	Mass = 1.0f;
	Radius = 32.0f;
}

void Fish::Unload()
{
	mFishSprite = 0;
	mSteeringModule.reset();
}

void Fish::Update(float deltaTime)
{
	// auto& records = mPerceptionModule->GetMemoryRecords();
	// for(records)
	// try until you find a good 




	Destination = { static_cast<float>(X::GetMouseScreenX()),static_cast<float>(X::GetMouseScreenY()) };
	auto force = mSteeringModule->Calculate();
	auto accelration = (force / Mass);
	Velocity += accelration * deltaTime;
	auto speed = X::Math::Magnitude(Velocity);
	if (speed > MaxSpeed)
	{
		Velocity = Velocity / speed * MaxSpeed;
	}


	Position += Velocity * deltaTime;
	

	if (speed > 0.0f)
	{
		Heading = X::Math::Normalize(Velocity);
	}

	if (Position.x < 0.0f)
		Position.x = (static_cast<float>(X::GetScreenWidth()));
	else if (Position.x > (static_cast<float>(X::GetScreenWidth())))
		Position.x = 0.0f;
	if (Position.y < 0.0f)
		Position.y = (static_cast<float>(X::GetScreenHeight()));
	else if (Position.y > (static_cast<float>(X::GetScreenHeight())))
		Position.y = 0.0f;
	mTimer += deltaTime;
}

void Fish::Render()
{
	X::DrawSprite(mFishSprite, Position);
}
