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

	mDataset = Datasets::MakeLogistic(sampleCount, b0Val, b1Val, b2Val, minX, maxX, noise);

	for (int e = 0; e < epoch; e++)
	{
		float error = mLogisticRegression.Fit(mDataset);
		mAppLog.AddLog("b0 = %f, b1 = %f, error = %f\n", mLogisticRegression.b0, mLogisticRegression.b1, mLogisticRegression.b2, error);
	}
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
	ImGui::SliderFloat("Leaning Rate", &mRegression.learningRate, 0.000001f, 0.001f, "%.7f");
	ImGui::SliderInt("Epochs", &epoch, 100, 10000);
	ImGui::SliderInt("Sample Count", &sampleCount, 10, 1000);
	ImGui::DragFloat("B0 Value", &b0Val, 0.1f);
	ImGui::DragFloat("B1 Value", &b1Val, 0.1f);
	ImGui::DragFloat("B2 Value", &b2Val, 0.1f);
	ImGui::DragFloat("Minimum X", &minX, 0.1f);
	ImGui::DragFloat("Maximum X", &maxX, 0.1f);
	ImGui::DragFloat("Noise", &noise, 0.1f);
	static bool Linear = false;
	static bool Logistic = false;

	if (ImGui::Checkbox("Linear Regression", &Linear))
	{
		if (Linear)
		{
			Logistic = false;
			mRegression.b0 = 0.0f;
			mRegression.b1 = 0.0f;
			mDataset = Datasets::MakeLinear(sampleCount, b0Val, b1Val, minX, maxX, noise);
		}
	}
	if (ImGui::Checkbox("Logistic Regression", &Logistic))
	{
		if (Logistic)
		{
			Linear = false;
			mLogisticRegression.b0 = 0.0f;
			mLogisticRegression.b1 = 0.0f;
			mLogisticRegression.b2 = 0.0f;
			mDataset = Datasets::MakeLogistic(sampleCount, b0Val, b1Val, b2Val, minX, maxX, noise);

			for (int e = 0; e < epoch; e++)
			{
				float error = mLogisticRegression.Fit(mDataset);
				mAppLog.AddLog("b0 = %f, b1 = %f, error = %f\n", mLogisticRegression.b0, mLogisticRegression.b1, mLogisticRegression.b2, error);
			}
		}
	}

	if (Linear)
	{
		if (ImPlot::BeginPlot("Linear Regression", "X", "y"))
		{
			float startX = minX;
			float endX = maxX;
			float startY = mRegression.Predict(startX);
			float endY = mRegression.Predict(endX);

			std::vector<ImVec2> line;
			line.push_back({ startX,startY });
			line.push_back({ endX,endY });

			ImPlot::PlotScatter("Dataset", mDataset.x0.data(), mDataset.y.data(), mDataset.x0.size());
			ImPlot::PlotLine("Model", line.data(), line.size());
			ImPlot::EndPlot();
		}

		if (ImGui::Button("Train"))
		{
			for (int e = 0; e < epoch; e++)
			{
				float error = mRegression.Fit(mDataset);
				mAppLog.AddLog("b0 = %f, b1 = %f, error = %f\n", mRegression.b0, mRegression.b1, error);
			}
		}
	}

	if (Logistic)
	{
		if (ImPlot::BeginPlot("Logistic Regression", "x0", "x1"))
		{
			std::vector<ImVec2> modelLine;
			int count = static_cast<int>(std::size(mDataset.x0));
			float m = mLogisticRegression.b1 / -mLogisticRegression.b2;
			float b = mLogisticRegression.b0 / -mLogisticRegression.b2; // Scope
			modelLine.push_back({ minX, minX * m + b});
			modelLine.push_back({ maxX, maxX * m + b});
			ImPlot::PlotLine("Model", modelLine.data(), (int)modelLine.size());
			std::vector<float> ax, ay, bx, by;
			for (int i = 0; i < count; ++i)
			{
				float p = mLogisticRegression.b0 + mLogisticRegression.b1 * mDataset.x0[i] + mLogisticRegression.b2 * mDataset.x1[i];
				if (p >= 0)
				{
					ax.push_back(mDataset.x0[i]);
					ay.push_back(mDataset.x1[i]);
				}
				else
				{
					bx.push_back(mDataset.x0[i]);
					by.push_back(mDataset.x1[i]);
				}
			}
			ImPlot::PlotScatter("Ones", ax.data(), ay.data(), (int)ax.size());
			ImPlot::PlotScatter("Zeros", bx.data(), by.data(), (int)bx.size());
			ImPlot::EndPlot();
		}

		if (ImGui::Button("Train"))
		{
			for (int e = 0; e < epoch; e++)
			{
				float error = mLogisticRegression.Fit(mDataset);
				mAppLog.AddLog("b0 = %f, b1 = %f, error = %f\n", mLogisticRegression.b0, mLogisticRegression.b1, mLogisticRegression.b2, error);
			}
		}
	}



	mAppLog.Draw("Console");
}

