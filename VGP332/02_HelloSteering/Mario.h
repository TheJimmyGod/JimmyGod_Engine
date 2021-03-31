#pragma once

#include <AI.h>
#include <XEngine.h>
//#include <Agent.h>
#include "UnitType.h"
namespace Steering
{
	class AIWorld;

	class Mario : public AI::Agent
	{
	public:
		Mario(AI::AIWorld& world);
		~Mario() override = default;

		void Load();
		void Unload();
		void Update(float deltaTime);
		void Render();

	private:


		std::unique_ptr<AI::SteeringModule> mSteeringModule = nullptr;
		X::TextureId mMarioSprite;
		X::Math::Circle mCirclePos;
		float mTimer;
	};
}

