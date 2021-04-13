#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace Steering
{
	class AIWorld;

	class Player : public JimmyGod::AI::Agent
	{
	public:
		Player(JimmyGod::AI::AIWorld& world);
		~Player() override = default;

		void Load();
		void Unload();
		void Update(float deltaTime);
		void Render();

	private:
		std::unique_ptr<JimmyGod::AI::SteeringModule> mSteeringModule = nullptr;
		JimmyGod::Graphics::TextureId mPlayerSprite;
		JimmyGod::Math::Circle mCirclePos;
		float mTimer;
	};
}

