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

	void ShowGraph();

private:
	AppLog mAppLog;

	int sampleCount = 100.0f;
	float b0Val = 0.0f;
	float b1Val = 0.5f;
	float b2Val = 1.0f;
	float minX = 0.0f;
	float maxX = 10.0f;
	float noise = 2.0f;
	int epoch = 10000;
	JimmyGod::ML::Dataset mDataset;
	JimmyGod::ML::LinearRegression mRegression;
	JimmyGod::ML::LogisticRegression mLogisticRegression;
};