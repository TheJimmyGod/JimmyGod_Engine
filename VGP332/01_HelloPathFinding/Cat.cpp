#include "Cat.h"

using namespace JimmyGod::AI;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;

void Cat::Load()
{
	mStateMachine = std::make_unique<StateMachine<Cat>>(*this);
	mCatSprite = SpriteRenderManager::Get()->LoadTexture("../../Assets/Images/mushroom.png");
	SetPosition(Vector2(11.0f * 32.0f, 9.0f * 32.0f));
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
	SpriteRenderManager::Get()->DrawSprite(mCatSprite, mPostion);
}
