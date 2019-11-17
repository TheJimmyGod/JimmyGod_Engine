#include "TestState.h"

using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;


void RedState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Red);
}

void RedState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
		JimmyGod::MainApp().ChangeState("BlueState");
}

void BlueState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Blue);
}

void BlueState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
		JimmyGod::MainApp().ChangeState("RedState");
}
