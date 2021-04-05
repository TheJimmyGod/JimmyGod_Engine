#include "GameState.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <ImGui/Inc/imgui.h>
#include <ImGui/Inc/ImPlot.h>
#include <ImGui/Src/imgui_internal.h>


using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;
using namespace JimmyGod::ML;

AppLog mAppLog;

namespace
{
	struct ClusterResult
	{
		std::vector<Vector3> centroids;
		std::vector<size_t> assignment;

		std::vector<std::vector<Vector3>> centroidHistory;
	};

	ClusterResult KMeansCluster(const std::vector<Vector3>& data, size_t k, size_t maxIteration)
	{
		ClusterResult CR;
		CR.centroids.resize(k);

		for (auto& centroid : CR.centroids)
		{
			centroid.x = RandomFloat(0.f, 800.f);
			centroid.y = RandomFloat(0.f, 600.f);
		}

		CR.assignment.clear();
		CR.assignment.resize(data.size());
		for (size_t i = 0; i < maxIteration; ++i)
		{
			std::vector<Vector3> newCentroids;
			std::vector<size_t> clusterEntryCount;
			newCentroids.resize(k);
			clusterEntryCount.resize(k);

			CR.assignment.clear();
			CR.assignment.reserve(data.size());
			for (const auto& sample : data)
			{
				size_t closestClusterID = 0;
				float closestDistSqr = JimmyGod::Math::Sqr(sample.x - CR.centroids[0].x) +
					JimmyGod::Math::Sqr(sample.y - CR.centroids[0].y);
				for (size_t i = 0; i < CR.centroids.size(); ++i)
				{
					const float distSqr = JimmyGod::Math::Sqr(sample.x - CR.centroids[i].x) +
						JimmyGod::Math::Sqr(sample.y - CR.centroids[i].y);
					if (distSqr < closestDistSqr)
					{
						closestClusterID = i;
						closestDistSqr = distSqr;
					}
				}
				CR.assignment.push_back(closestClusterID);
				newCentroids[closestClusterID] += sample;
				clusterEntryCount[closestClusterID]++;
			}
			for (size_t i = 0; i < newCentroids.size(); ++i)
				newCentroids[i] /= static_cast<float>(clusterEntryCount[i]);
			bool pass = false;
			for (size_t i = 0; i < newCentroids.size(); i++)
			{
				if (newCentroids[i].x == CR.centroids[i].x || newCentroids[i].y == CR.centroids[i].y)
				{
					pass = true;
				}
				else
				{
					pass = false;
					break;
				}
			}

			if (pass) break;
			CR.centroids = std::move(newCentroids);
		}


		return CR;
	}
}

ClusterResult mClusterResult;

void GameState::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	graphicsSystem->SetClearColor(Colors::Black);

	mUnitTexture.Initialize(L"../../Assets/Images/scv_01.png");

	for (size_t i = 0; i < 50; ++i)
	{
		float px = RandomFloat(0.0f, 800.0f);
		float py = RandomFloat(0.0f, 600.0f);
		float vx = RandomFloat(-100.0f, 100.0f);
		float vy = RandomFloat(-100.0f, 100.0f);
		mUnits.emplace_back(Unit{ {px,py} , {vx,vy} });
	}
}

void GameState::Terminate()
{
	mUnitTexture.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::ESCAPE))
	{
		JimmyGod::MainApp().Quit();
		return;
	}

	if (!mDraw)
	{
		if (inputSystem->IsKeyDown(KeyCode::SPACE))
		{
			// Run KMeans
			// Show centroid & assignment
			std::vector<JimmyGod::Math::Vector3> mData;
			mData.reserve(mUnits.size());
			for (auto& unit : mUnits)
				mData.push_back({ unit.position.x,unit.position.y,0.0f });

			mClusterResult = KMeansCluster(mData, 5, 10);
			mDraw = true;
		}
		else
		{
			for (auto& unit : mUnits)
			{
				unit.position += unit.velocity * deltaTime;
				if (unit.position.x < 0.0f)
					unit.position.x += 800.0f;
				if (unit.position.x > 800.0f)
					unit.position.x -= 800.0f;
				if (unit.position.y < 0.0f)
					unit.position.y += 600.0f;
				if (unit.position.y > 600.0f)
					unit.position.y -= 600.0f;
			}
		}
	}
	else
	{
		if (inputSystem->IsKeyDown(KeyCode::SPACE))
		{
			mDraw = false;
		}
	}

}

void GameState::DebugUI()
{
	ImGui::SetNextWindowSize({ 800.0f,600.0f });
	ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoResize);

	const ImVec2 unitSize = ImVec2{ 32.f, 32.f };
	for (auto& unit : mUnits)
	{
		ImGui::SetCursorPos(unit.position - unitSize * 0.5f);
		ImGui::Image(mUnitTexture.GetRawData(), { 64.0f,64.0f });
	}

	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	ImVec2 winPos = ImGui::GetWindowPos();
	for (auto& u : mUnits)
	{
		drawList->AddCircle({ winPos + u.position }, 50.0f, 0xff0000ff);
	}

	
	if (mDraw)
	{
		const ImU32 clusterColors[] = { 0xFFFF5000 , 0xff00ff00, 0xFFFFFF00 };
		for (size_t i = 0; i < mClusterResult.centroids.size(); ++i)
		{
			ImVec2 center{ mClusterResult.centroids[i].x, mClusterResult.centroids[i].y };
			drawList->AddCircleFilled(winPos + center, 10.0f, clusterColors[i]);
		}
		for (size_t i = 0; i < mClusterResult.assignment.size(); i++)
		{
			size_t clusterID = mClusterResult.assignment[i];
			Vector3 centroid = mClusterResult.centroids[clusterID];
			ImVec2 center{ centroid.x,centroid.y };
			drawList->AddLine(winPos + center, winPos + mUnits[i].position, clusterColors[clusterID]);
		}
	}

	ImGui::End();
	mAppLog.Draw("Console");
}

