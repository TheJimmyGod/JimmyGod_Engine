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

		void SetPerception(bool perception) { mPerceptionMode = perception; }
		void ChangeState(std::string stateName) { mStateMachine->ChangeState(stateName); }

		JimmyGod::AI::SteeringModule* GetSteeringModule() const
		{
			return mSteeringModule.get();
		}

		JimmyGod::AI::PerceptionModule* GetPerceptionModule() const
		{
			return mPerceptionModule.get();
		}

		void SetLastPos(const JimmyGod::Math::Vector2& pos)
		{
			mLastSeenPos = pos;
		}

		const JimmyGod::Math::Vector2& GetLastPos() const
		{
			return mLastSeenPos;
		}

		void SetViewRange(float val)
		{
			mViewRange = val;
		}

		float GetViewRange() const
		{
			return mViewRange;
		}

	private:
		std::unique_ptr<JimmyGod::AI::SteeringModule> mSteeringModule = nullptr;
		std::unique_ptr<JimmyGod::AI::PerceptionModule> mPerceptionModule = nullptr;
		std::unique_ptr<JimmyGod::AI::StateMachine<Steering::Player>> mStateMachine = nullptr;
		JimmyGod::Graphics::TextureId mPlayerSprite;
		JimmyGod::Math::Circle mCirclePos;
		float mTimer = 0.0f;
		float mViewRange = 0.0f;

		JimmyGod::Math::Vector2 mLastSeenPos;
		SpriteAnimation mSmoke;
		bool isStarted = false;
		bool mPerceptionMode = false;
	};
}

class NoneState : public JimmyGod::AI::State<Steering::Player>
{
public:
	void Enter(Steering::Player& agent)
	{
	}
	void Update(Steering::Player& agent, float deltaTime)
	{
	}
	void Exit(Steering::Player& agent)
	{
	}
private:

};

class FreeState : public JimmyGod::AI::State<Steering::Player>
{
public:
	void Enter(Steering::Player& agent)
	{
		agent.GetSteeringModule()->GetBehavior<JimmyGod::AI::WanderBehavior>("Wander")->SetActive(true);
	}
	void Update(Steering::Player& agent, float deltaTime)
	{
		auto& records = agent.GetPerceptionModule()->GetMemoryRecords();
		for (auto& r : records)
		{
			const auto& val = std::get<JimmyGod::Math::Vector2>(r.properties.find("lastSeenPosition")->second);
			if (r.importance >= 50)
			{
				agent.SetLastPos(val);
				agent.ChangeState("RunAway");
			}
		}
	}
	void Exit(Steering::Player& agent)
	{
		agent.GetSteeringModule()->GetBehavior<JimmyGod::AI::WanderBehavior>("Wander")->SetActive(false);
	}
private:
	
};

class RunAwayState : public JimmyGod::AI::State<Steering::Player>
{
public:
	void Enter(Steering::Player& agent)
	{
		agent.Destination = agent.GetLastPos();
		agent.GetSteeringModule()->GetBehavior<JimmyGod::AI::FleeBehavior>("Flee")->SetActive(true);
	}
	void Update(Steering::Player& agent, float deltaTime)
	{

		JimmyGod::Graphics::SimpleDraw::AddScreenLine(agent.Position, agent.GetLastPos(), JimmyGod::Graphics::Colors::PaleVioletRed);

		if (JimmyGod::Math::Distance(agent.Position, agent.GetLastPos()) > agent.GetViewRange() / 2.0f)
		{
			agent.SetLastPos(JimmyGod::Math::Vector2::Zero);
			agent.ChangeState("Free");
		}
		else
			JimmyGod::Graphics::SpriteRenderManager::Get()->DrawScreenText(
				"Enemy has been found!", agent.Position.x + 20.0f, agent.Position.y + 20.0f, 12.0f,
				Colors::White
			);
	}
	void Exit(Steering::Player& agent)
	{
		agent.GetSteeringModule()->GetBehavior<JimmyGod::AI::FleeBehavior>("Flee")->SetActive(false);
	}
private:
};
