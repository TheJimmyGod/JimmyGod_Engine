#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class RedState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};

class BlueState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};