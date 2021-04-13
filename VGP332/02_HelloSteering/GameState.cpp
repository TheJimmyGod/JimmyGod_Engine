#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace JimmyGod;
using namespace Steering;
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;
using namespace JimmyGod::AI;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetFov(60.0f * JimmyGod::Math::Constants::DegToRad);
	mCamera.SetNearPlane(0.01f);
	mCamera.SetFarPlane(10000.0f);
	mTilemap.Load();
	auto GS = JimmyGod::Graphics::GraphicsSystem::Get();
	mAI_Setting.partitionGridSize = { 100.0f };
	mAI_Setting.worldSize.x = static_cast<float>(GS->GetBackBufferWidth());
	mAI_Setting.worldSize.y = static_cast<float>(GS->GetBackBufferHeight());

	mWorld.AddObstacles({ {100.0f,500.0f},100.0f });
	mWorld.AddObstacles({ {700.0f,200.0f},120.0f });

	mWorld.Initialize(mAI_Setting);

	mPlayer = std::make_unique<Player>(mWorld);
	mPlayer.get()->Load();

	for (int i = 0; i < 12; ++i)
	{
		auto soldier = &mSolider.emplace_back(std::make_unique<Soldier>(mWorld));
		soldier->get()->Load();
		soldier->get()->Position = { RandomFloat(5.0f,static_cast<float>(GS->GetBackBufferWidth())), 
			RandomFloat(5.0f,static_cast<float>(GS->GetBackBufferHeight())) };
		soldier->get()->threat = mPlayer.get();
	}
}

void GameState::Terminate()
{
	mTilemap.UnLoad();
	mPlayer.get()->Unload();
	for (auto& entity : mSolider)
		entity->Unload();
}

void GameState::Update(float deltaTime)
{
	mWorld.Update();


	mPlayer.get()->Update(deltaTime);

	for (auto& entity : mSolider)
	{
		entity->Update(deltaTime);
	}
	Processing(deltaTime);
}

void GameState::Render()
{
	mTilemap.Render();
	mWorld.DebugDraw();
	mPlayer.get()->Render();
	for (auto& entity : mSolider)
	{
		entity->Render();
	}
	if(mOrder == Order::None)
		SpriteRenderManager::Get()->DrawScreenText("Order completed!", 100.0f, 100.0f, 20.0f, JimmyGod::Graphics::Colors::Red);
}

void GameState::Processing(float deltaTime)
{
	if (isProcessing == false)
		return;
	switch (mOrder)
	{
	case Order::None:
		break;
	case Order::Gathering:
	{
		size_t NumberOfArrive = 0;
		for (auto& entity : mSolider)
		{
			if (Distance(entity.get()->Position, entity.get()->Destination) < 5.0f)
			{
				ClearSingleEntity(entity.get());
			}
		}
		for (auto& entity : mSolider)
		{
			if (entity.get()->GetOrder() == false)
				NumberOfArrive++;
		}
		if (NumberOfArrive >= mSolider.size() || mTimer > 7.5f)
		{
			mOrder = Order::None;
			isProcessing = false;
			isCommand = false;
			Clear();
		}
		break;
	}
	case Order::Moving:
		break;
	case Order::Attacking:
		break;
	case Order::Wandering:
		break;
	default:
		break;
	}

	mTimer += deltaTime;
}

void GameState::ClearSingleEntity(Soldier* soldier)
{
	soldier->GetSteeringModule()->GetBehavior<SeekBehavior>("Seek")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<FleeBehavior>("Flee")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<PursuitBehavior>("Pursuit")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<AviodObsBehavior>("Avoid")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<EvadeBehavior>("Evade")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<SeparationBehavior>("Separation")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<AlignmentBehavior>("Alignment")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<CohesionBehavior>("Cohesion")->SetActive(false);
	soldier->Destination = Vector2::Zero;
	soldier->Velocity = Vector2::Zero;
	soldier->SetOrder(false);
}

void GameState::Clear()
{
	for (auto& entity : mSolider)
	{
		entity.get()->GetSteeringModule()->GetBehavior<SeekBehavior>("Seek")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<FleeBehavior>("Flee")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<PursuitBehavior>("Pursuit")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<AviodObsBehavior>("Avoid")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<ArriveBehavior>("Arrive")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<EvadeBehavior>("Evade")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<SeparationBehavior>("Separation")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<AlignmentBehavior>("Alignment")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<CohesionBehavior>("Cohesion")->SetActive(false);
		entity.get()->Destination = Vector2::Zero;
		entity.get()->Velocity = Vector2::Zero;
		entity.get()->SetOrder(false);
	}
}

void GameState::DebugUI()
{
	auto GS = GraphicsSystem::Get();
	SimpleDraw::Render(mCamera);
	ImGui::Begin("AI Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Command", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!isCommand)
		{
			if (ImGui::Button("Gathering!"))
			{
				isCommand = true;
				mOrder = Order::Gathering;
				for (auto& entity : mSolider)
				{
					entity.get()->SetOrder(true);
				}
				previous = Vector2::Zero;
				nearest = Vector2::Zero;
				float lastDistance = 0.0f;
				float minDistance = std::numeric_limits<float>::infinity();
				for (auto& entity : mSolider)
				{
					if (previous.x == 0 && previous.y == 0)
					{
						previous = entity.get()->Position;
						continue;
					}
					else
					{
						lastDistance = Distance(previous, entity.get()->Position);
						if (lastDistance < minDistance)
						{
							minDistance = lastDistance;
							nearest = entity.get()->Position;
						}
						else
						{
							previous = entity.get()->Position;
						}
					}
				}

				if (nearest.x < 100.0f)
				{
					nearest.x += 150.0f;
				}
				if (nearest.y < 100.0f)
				{
					nearest.y += 150.0f;
				}
				if (nearest.x > static_cast<float>(GS->GetBackBufferWidth()) - 150.0f)
				{
					nearest.x -= 150.0f;
				}
				if (nearest.y > static_cast<float>(GS->GetBackBufferHeight()) - 150.0f)
				{
					nearest.y -= 150.0f;
				}
				float offset = 64.0f;
				int Index = 0;
				int MaxIndex = mSolider.size() / 3;
				bool FirstFormation = true;
				bool SecondFormation = false;
				bool ThirdFormation = false;
				for (auto& entity : mSolider)
				{
					if (FirstFormation)
					{
						entity.get()->Destination = Vector2{ nearest.x + (offset * Index), nearest.y };
						entity.get()->MaxSpeed = 200.0f;
						Index++;
						if (Index == MaxIndex)
						{
							FirstFormation = false;
							SecondFormation = true;
							Index = 0;
							continue;
						}
					}
					if (SecondFormation)
					{
						entity.get()->Destination = Vector2{ nearest.x + (offset * Index), nearest.y + offset };
						entity.get()->MaxSpeed = 200.0f;
						Index++;
						if (Index == MaxIndex)
						{
							FirstFormation = false;
							SecondFormation = false;
							ThirdFormation = true;
							Index = 0;
							continue;
						}
					}
					if (ThirdFormation)
					{
						entity.get()->Destination = Vector2{ nearest.x + (offset * Index), nearest.y + offset * 2 };
						entity.get()->MaxSpeed = 200.0f;
						Index++;
					}
					
					
				}
				mTimer = 0.0f;
				isProcessing = true;
				for (auto& entity : mSolider)
				{
					//entity.get()->GetSteeringModule()->GetBehavior<AviodObsBehavior>("Avoid")->SetActive(true);
					entity.get()->GetSteeringModule()->GetBehavior<SeekBehavior>("Seek")->SetActive(true);
					entity.get()->GetSteeringModule()->GetBehavior<SeparationBehavior>("Separation")->SetActive(true);
					entity.get()->GetSteeringModule()->GetBehavior<AlignmentBehavior>("Alignment")->SetActive(true);
					entity.get()->GetSteeringModule()->GetBehavior<CohesionBehavior>("Cohesion")->SetActive(true);
				}
			}
		}

	}
	ImGui::End();
}
