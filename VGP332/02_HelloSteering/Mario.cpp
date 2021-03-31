#include "Mario.h"

using namespace Steering;



Mario::Mario(AI::AIWorld & world)
	:Agent(world, UnitType::Qmark)
{
}

void Mario::Load()
{
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	//Position = (X::Math::Vector2(X::GetScreenWidth() * 0.5f, X::GetScreenHeight() * 0.5f));
	mSteeringModule->AddBehavior<AI::PursuitBehavior>("Pursuit")->SetActive(true);
	//mSteeringModule->AddBehavior<AI::AviodObsBehavior>("Avoid")->SetActive(true);

	mSteeringModule->AddBehavior<AI::SeparationBehavior>("Separation")->SetActive(true);
	mSteeringModule->AddBehavior<AI::AlignmentBehavior>("Alignment")->SetActive(true);
	//mSteeringModule->AddBehavior<AI::CohesionBehavior>("Cohesion")->SetActive(true);
	mMarioSprite = X::LoadTexture("qmark.png");
	MaxSpeed = 200.0f;
	Mass = 1.0f;
	Radius = 50.0f;
}

void Mario::Unload()
{
	mMarioSprite = 0;
}

void Mario::Update(float deltaTime)
{
	// Update neighbors (exclude itself)
	neighbors = world.GetNeighborhood({ Position, 100.0f }, UnitType::Qmark);
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

void Mario::Render()
{
	X::DrawSprite(mMarioSprite, Position);
}
