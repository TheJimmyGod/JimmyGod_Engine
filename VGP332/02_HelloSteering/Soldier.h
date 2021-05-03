#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace Steering
{
	class AIWorld;

	class Soldier : public JimmyGod::AI::Agent
	{
	public:
		Soldier(JimmyGod::AI::AIWorld& world);
		~Soldier() override = default;

		void Load();
		void Unload();
		void Update(float deltaTime);
		void Render();

		JimmyGod::AI::SteeringModule* GetSteeringModule() const
		{
			return mSteeringModule.get();
		}

		void SetOrder(bool order) { isOrdered = order; }
		bool GetOrder() const { return isOrdered; }

	private:
		std::unique_ptr<JimmyGod::AI::SteeringModule> mSteeringModule = nullptr;
		JimmyGod::Graphics::TextureId mSoldierSprite;
		JimmyGod::Math::Circle mCirclePos;
		float mTimer = 0.0f;
		bool isOrdered = false;
		bool isStarted = false;

		SpriteAnimation mSmoke;
	};
}

