#include "Cat.h"

using namespace FSM;

void Cat::Load()
{
	mStateMachine = std::make_unique<AI::StateMachine<Cat>>(*this);
	mCatSprite = X::LoadTexture("mushroom.png");
	SetPosition(X::Math::Vector2(11.0f * 32.0f, 9.0f * 32.0f));
	mStateMachine->AddState<IdleState>("Idle");
	mStateMachine->AddState<MeowState>("Meow");
	mStateMachine->ChangeState("Idle");
}

void Cat::Unload()
{
	mCatSprite = 0;

	mStateMachine.reset();
}

void Cat::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
}

void Cat::ChangeState(std::string stateName)
{
	mStateMachine->ChangeState(stateName);
}

void Cat::Render()
{
	X::DrawSprite(mCatSprite, mPostion);
}
