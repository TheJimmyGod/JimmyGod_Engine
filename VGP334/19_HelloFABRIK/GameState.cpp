#include "GameState.h"

#include <ImGui/Inc/imgui.h>

using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;

namespace
{
	// TODO: Move this function into a new file/name: AnimationUtil and add it to the graphic library
	void RunFABRIK(std::vector<Vector3>& points, const Vector3& target, int maxIteration, float errorThreshold)
	{
		ASSERT(points.size() >= 2, "Not enough points to run FABRIK");

		// Record the origin
		Vector3 origin = points.front();

		//Record the current point distances, we want to maintain these values when running IK
		std::vector<float> lengths;
		lengths.reserve(points.size());

		for (size_t i = 0; i +1 < points.size(); i++)
		{
			lengths.push_back(Magnitude(points[i + 1] - points[i]));
		}
		
		// Loop until we are close enough, or have used up all iteration
		for (int i = 0; i < maxIteration; i++)
		{
			if (MagnitudeSqr(points.back() - target) < errorThreshold * errorThreshold)
			{
				break;
			}
			// Run forward pass
			points.back() = target;
			for (int i = (int)points.size() - 2; i >= 0; --i)
			{
				Vector3 direction = Normalize(points[i + 1] - points[i]);
				points[i] = points[i + 1] - (direction * lengths[i]);
			}
			
			// Run backward pass
			points.front() = origin;
			for (int i = 0; i + 1 < (int)points.size(); ++i)
			{
				Vector3 direction = Normalize(points[i+1] - points[i]);
				points[i+1] = (direction * lengths[i]) + points[i];
			}

		}
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(500.0f);
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	const int numBones = 5;
	const float boneLength = 1.0f;

	mPoints.reserve(numBones + 1);
	mPoints.push_back(Vector3::Zero);
	for (size_t i = 1; i < mPoints.capacity(); ++i)
	{
		mPoints.push_back(mPoints.back() + Vector3{ 0.0f, boneLength, 0.0f });
	}

	mTarget = mPoints.back();
}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;

	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mTarget.y += kMoveSpeed * deltaTime;
	}
	else if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mTarget.y -= kMoveSpeed * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mTarget.x += kMoveSpeed * deltaTime;
	}
	else if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mTarget.x -= kMoveSpeed * deltaTime;
	}

	RunFABRIK(mPoints, mTarget, 5, 0.01f);
}

void GameState::Render()
{
	for (size_t i = 0; i + 1 < mPoints.size(); i++)
	{
		SimpleDraw::AddBone(Matrix4::Translation(mPoints[i]));
		SimpleDraw::AddLine(mPoints[i], mPoints[i + 1], Colors::Aqua);
		//if (i + 1 != mPoints.size())
			//SimpleDraw::AddSphere(mPoints[i], 0.1f, Colors::Red,4,8);
	}
	SimpleDraw::AddAABB(mTarget, 0.2f, Colors::White);


	SimpleDraw::AddGroundPlane(30.0f,1.0f,Colors::Aqua);
	SimpleDraw::AddTransform(Matrix4::Identity);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	
}