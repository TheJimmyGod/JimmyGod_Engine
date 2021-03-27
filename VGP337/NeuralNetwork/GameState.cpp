#include "GameState.h"

#include <ImGui/Inc/imgui.h>
#include <ImGui/Inc/ImPlot.h>
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;
using namespace JimmyGod::ML;

void GameState::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	graphicsSystem->SetClearColor(Colors::Black);

	mAppLog.AddLog("Generating data ...\n");


}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;

	if (inputSystem->IsKeyDown(KeyCode::ESCAPE))
	{
		JimmyGod::MainApp().Quit();
		return;
	}
	if (inputSystem->IsKeyPressed(KeyCode::SPACE))
	{
		mAppLog.AddLog("Bam!\n");
	}
}

void GameState::DebugUI()
{
	ImGui::SetNextWindowSize({ 800.0f,600.0f });
	ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoResize);

	std::vector<std::vector<float>> mInputValue(4);
	std::vector<std::vector<float>> mTargetValue(4);


	mInputValue[0].push_back(0.0f);
	mInputValue[0].push_back(0.0f);

	mInputValue[1].push_back(1.0f);
	mInputValue[1].push_back(0.0f);

	mInputValue[2].push_back(0.0f);
	mInputValue[2].push_back(1.0f);

	mInputValue[3].push_back(1.0f);
	mInputValue[3].push_back(1.0f);

	mTargetValue[0].push_back(-1.0f);
	mTargetValue[1].push_back(1.0f);
	mTargetValue[2].push_back(1.0f);
	mTargetValue[3].push_back(-1.0f);

	ImGui::DragInt("X1 Value", &x1, 1);
	ImGui::DragInt("X2 Value", &x2, 1);

	ImGui::DragInt("Times", &mTimes, 1);

	if (ImGui::Button("Train"))
	{
		mAppLog.Clear();
		mTopology.clear();
		mTopology.push_back(x1);
		mTopology.push_back(x2);
		mTopology.push_back(1);

		JimmyGod::ML::NeuralNetwork mNeuralNetwork(mTopology);
		for (int i = 0; i < mTimes; ++i)
		{
			size_t rang = static_cast<size_t>(JimmyGod::Math::RandomInt(0, 3));

			mNeuralNetwork.FeedForward(mInputValue[rang]);
			std::vector<float> result = mNeuralNetwork.GetResults();
			float error = result[0] - mTargetValue[rang][0];
			mNeuralNetwork.BackPropagate(mTargetValue[rang]);


			mAppLog.AddLog("\nTrain times: %d\n",mTimes);
			mAppLog.AddLog("Prediction: %d, %d\n", x1,x2);
			mAppLog.AddLog("Error: %f\n", error);
		}
	}

	ImGui::End();

	mAppLog.Draw("Console");
}

