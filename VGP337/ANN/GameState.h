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

	JimmyGod::ML::GeneticAlgorithm mGA;

	float mCrossoverRate = 0.0f;
	float mMutationRate = 0.0f;

	float mRadius = 0.0f;

	float mBestDistance = 0.0f;

	int kMaxCityCount = 5;

	bool mInitialized = false;
	bool mFound = false;
	bool mDraw = false;
};