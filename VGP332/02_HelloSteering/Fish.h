#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

using namespace JimmyGod::AI;

//#include <Agent.h>
#include "UnitType.h"
namespace Steering
{
	class AIWorld;

	class Fish : public Agent
	{
	public:
		Fish(AIWorld& world);
		~Fish() override = default;

		void Load();
		void Unload();
		void Update(float deltaTime);
		void Render();

	private:
		
		
		std::unique_ptr<SteeringModule> mSteeringModule = nullptr;
		JimmyGod::Input::TextureId mFishSprite;
		JimmyGod::Math::Circle mCirclePos;
		float mTimer;
	};
}

