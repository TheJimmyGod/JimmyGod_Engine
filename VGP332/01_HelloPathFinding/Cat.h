#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace FSM
{
	class Cat
	{
	public:
		void Load();
		void Unload();
		void Update(float deltaTime);
		void ChangeState(std::string stateName);
		void Render();

		bool GetMeow() const { return isMeow; }
		JimmyGod::Math::Vector2 GetPosition() const { return mPostion; }

		void SetPosition(JimmyGod::Math::Vector2 pos) { mPostion = pos; }
		void SetMeow(bool meow) { isMeow = meow; }
	private:
		bool isMeow = false;
		JimmyGod::Input::TextureId mCatSprite;
		JimmyGod::Math::Vector2 mPostion;
		std::unique_ptr<JimmyGod::AI::StateMachine<Cat>> mStateMachine;
	};

	class MeowState : public JimmyGod::AI::State<Cat>
	{
	public:
		void Enter(Cat& agent)
		{
			//mCatMeowSound = X::LoadSound("Meow.wav");
		}
		void Update(Cat& agent, float deltaTime)
		{
			agent.SetMeow(true);
			//X::PlaySoundOneShot(mCatMeowSound);
			agent.ChangeState("Idle");
		}
		void Exit(Cat& agent)
		{

		}
	private:
		//X::SoundId mCatMeowSound;
	};

	class IdleState : public JimmyGod::AI::State<Cat>
	{
	public:
		void Enter(Cat& agent)
		{

		}
		void Update(Cat& agent, float deltaTime)
		{

		}
		void Exit(Cat& agent)
		{

		}
	private:
	};
}