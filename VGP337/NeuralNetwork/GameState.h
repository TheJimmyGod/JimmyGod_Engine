#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include <ImGui/Inc/Applog.h>

class GameState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

private:
	AppLog mAppLog;

	int x1 = 0;
	int x2 = 0;

	int mTimes = 1000;

	std::vector<size_t> mTopology;

	float mResult = 0.0f;
};