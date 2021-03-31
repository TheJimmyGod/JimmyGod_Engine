#pragma once

#include <AI.h>
#include <XEngine.h>
//#include <Agent.h>
#include "UnitType.h"
namespace Steering
{
	class AIWorld;

	class Fish : public AI::Agent
	{
	public:
		Fish(AI::AIWorld& world);
		~Fish() override = default;

		void Load();
		void Unload();
		void Update(float deltaTime);
		void Render();

	private:
		
		
		std::unique_ptr<AI::SteeringModule> mSteeringModule = nullptr;
		X::TextureId mFishSprite;
		X::Math::Circle mCirclePos;
		float mTimer;
	};
}

