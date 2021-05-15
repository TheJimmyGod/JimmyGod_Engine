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

		void SetDebug(bool debug) { isDebug = debug; }

	private:
		std::unique_ptr<JimmyGod::AI::SteeringModule> mSteeringModule = nullptr;
		std::unique_ptr<JimmyGod::AI::PerceptionModule> mPerceptionModule = nullptr;
		JimmyGod::Graphics::TextureId mSoldierSprite;
		JimmyGod::Graphics::TextureId mEmojiSprite;
		JimmyGod::Math::Circle mCirclePos;
		JimmyGod::Math::Vector2 PerceptedPos = JimmyGod::Math::Vector2::Zero;
		JimmyGod::Math::Vector2 LastSeenPos = JimmyGod::Math::Vector2::Zero;
		float mTimer = 0.0f;
		float mEmojiTimer = 0.0f;
		bool isOrdered = false;
		bool isStarted = false;
		bool isPercepted = false;
		bool isDebug = false;
		SpriteAnimation mSmoke;
	};
}

