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

	mWorld.AddObstacles({ {300.0f,500.0f},100.0f });
	mWorld.AddObstacles({ {700.0f,200.0f},120.0f });
	mWorld.AddObstacles({ {1000.0f,300.0f},80.0f });
	mWorld.Initialize(mAI_Setting);

	mPlayer = std::make_unique<Player>(mWorld);
	mPlayer->Load();
	
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
	auto input = InputSystem::Get();
	auto GS = GraphicsSystem::Get();
	mWorld.Update();

	if (input->IsMousePressed(MouseButton::RBUTTON))
	{
		mPlayer->Destination = Vector2{ static_cast<float>(input->GetMouseScreenX()) ,
			static_cast<float>(input->GetMouseScreenY()) };
	}

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
		mNumberOfArrive = 0;
		for (auto& entity : mSolider)
			if (Distance(entity.get()->Position, entity.get()->Destination) < 5.0f)
				ClearSingleEntity(entity.get());

		for (auto& entity : mSolider)
			if (entity.get()->GetOrder() == false)
				mNumberOfArrive++;

		if (mNumberOfArrive >= mSolider.size() || mTimer > 7.5f)
			Clear();
		break;
	}
	case Order::Moving:
	{
		mNumberOfArrive = 0;
		for (auto& entity : mSolider)
		{
			if (entity == mSolider[mGeneral])
			{
				if (Distance(mSolider[mGeneral]->Position, nearest) < 20.0f)
				{
					ClearSingleEntity(entity.get());
					isArrived = true;
				}
			}
			else
			{
				if (isArrived)
				{
					float distance = Distance(entity->Position, entity->threat->Position);
					entity->MaxSpeed = 200.0f;
					if (Distance(entity->Position, entity->threat->Position) < 75.0f)
						ClearSingleEntity(entity.get());
				}
			}
		}
		for (auto& entity : mSolider)
			if (entity.get()->GetOrder() == false)
				mNumberOfArrive++;
		if (mNumberOfArrive >= mSolider.size())
			Clear();
	}
		break;
	case Order::Ambush:
	{
		if (isHided == false)
		{
			mHideTimer += deltaTime;
			if (mHideTimer > 10.0f)
			{
				for (auto& entity : mSolider)
				{
					entity->GetSteeringModule()->GetBehavior<HideBehavior>("Hide")->SetActive(false);
					entity->GetSteeringModule()->GetBehavior<AI::SeparationBehavior>("Separation")->SetActive(true);
					entity->Velocity = Vector2::Zero;
				}
				isHided = true;
				mHideTimer = 0.0f;
			}
		}
		else
		{
			for (auto& entity : mSolider)
			{
				entity->GetSteeringModule()->GetBehavior<PursuitBehavior>("Pursuit")->SetActive(true);
				float distance = Distance(entity->Position, entity->threat->Position);
				if (Distance(entity->Position, entity->threat->Position) < 30.0f)
					ClearSingleEntity(entity.get());
			}
			mHideTimer += deltaTime;
			if (mHideTimer >= 10.0f)
				Clear();
		}
		break;
	}
	case Order::Wandering:
	{
		for (auto& entity : mSolider)
		{
			float distance = Distance(entity->Position, entity->threat->Position);
			if (distance < 175.0f)
			{
				entity->GetSteeringModule()->GetBehavior<AI::EvadeBehavior>("Evade")->SetActive(true);
				entity->GetSteeringModule()->GetBehavior<AI::WanderBehavior>("Wander")->SetActive(false);
			}
			else
			{
				entity->GetSteeringModule()->GetBehavior<AI::EvadeBehavior>("Evade")->SetActive(false);
				entity->GetSteeringModule()->GetBehavior<AI::WanderBehavior>("Wander")->SetActive(true);
			}
		}

	}
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
	soldier->GetSteeringModule()->GetBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<EvadeBehavior>("Evade")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<WanderBehavior>("Wander")->SetActive(false);
	soldier->GetSteeringModule()->GetBehavior<HideBehavior>("Hide")->SetActive(false);
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
		entity.get()->GetSteeringModule()->GetBehavior<ArriveBehavior>("Arrive")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<EvadeBehavior>("Evade")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<WanderBehavior>("Wander")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<HideBehavior>("Hide")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<SeparationBehavior>("Separation")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<AlignmentBehavior>("Alignment")->SetActive(false);
		entity.get()->GetSteeringModule()->GetBehavior<CohesionBehavior>("Cohesion")->SetActive(false);
		entity.get()->threat = mPlayer.get();
		entity.get()->Destination = Vector2::Zero;
		entity.get()->Velocity = Vector2::Zero;
		entity.get()->SetOrder(false);
	}
	mOrder = Order::None;
	nearest = Vector2::Zero;
	previous = Vector2::Zero;
	isHided = false;
	isArrived = false;
	isProcessing = false;
}

void GameState::DebugUI()
{
	auto GS = GraphicsSystem::Get();
	SimpleDraw::Render(mCamera);
	ImGui::Begin("AI Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	static bool mActive = false;
	if (ImGui::Checkbox("Debug UI", &mActive)) { isDisplaying = mActive; }
	if (ImGui::CollapsingHeader("Player Option", ImGuiTreeNodeFlags_DefaultOpen))
	{

		static bool mPlayerArrive = false;
		static bool mPlayerAvoid = false;
		static bool mPlayerSeek = false;
		static bool mPlayerWander = false;

		if (!mPlayerSeek && !mPlayerWander)
			mPlayer->Velocity = Vector2::Zero;

		if (ImGui::Checkbox("Player Arrive", &mPlayerArrive))
		{
			mPlayer->Velocity = Vector2::Zero;
			mPlayer->GetSteeringModule()->GetBehavior<AI::ArriveBehavior>("Arrive")->SetActive(mPlayerArrive);
			if (mPlayerArrive)
			{
				mPlayerSeek = true;
				mPlayerWander = false;
				mPlayer->GetSteeringModule()->GetBehavior<AI::WanderBehavior>("Wander")->SetActive(mPlayerWander);
				mPlayer->GetSteeringModule()->GetBehavior<AI::SeekBehavior>("Seek")->SetActive(mPlayerSeek);
			}
			else
			{
				mPlayerSeek = false;
			}
		}
		if (ImGui::Checkbox("Player Seek", &mPlayerSeek))
		{
			mPlayer->Velocity = Vector2::Zero;
			mPlayer->GetSteeringModule()->GetBehavior<AI::SeekBehavior>("Seek")->SetActive(mPlayerSeek);
			if (mPlayerSeek)
			{
				mPlayerWander = false;
				mPlayerArrive = false;
				mPlayer->GetSteeringModule()->GetBehavior<AI::WanderBehavior>("Wander")->SetActive(mPlayerWander);
				mPlayer->GetSteeringModule()->GetBehavior<AI::ArriveBehavior>("Arrive")->SetActive(mPlayerArrive);
			}
		}
		if (ImGui::Checkbox("Player Wander", &mPlayerWander))
		{
			mPlayer->Velocity = Vector2::Zero;
			mPlayer->GetSteeringModule()->GetBehavior<AI::WanderBehavior>("Wander")->SetActive(mPlayerWander);
			if (mPlayerWander)
			{
				mPlayerSeek = false;
				mPlayerArrive = false;
				mPlayer->GetSteeringModule()->GetBehavior<AI::SeekBehavior>("Seek")->SetActive(mPlayerSeek);
				mPlayer->GetSteeringModule()->GetBehavior<AI::ArriveBehavior>("Arrive")->SetActive(mPlayerArrive);
			}
		}
		if (ImGui::Checkbox("Player Obstacle Avoidance", &mPlayerAvoid))
		{
			mPlayer->GetSteeringModule()->GetBehavior<AI::AviodObsBehavior>("Avoid")->SetActive(mPlayerAvoid);
		}
	}
	ImGui::NewLine();

	if (ImGui::CollapsingHeader("Enemy Command Option", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static bool mEnemyAvoid = false;
		if (ImGui::Checkbox("Enemy Obstacle Avoidance", &mEnemyAvoid))
		{
			for (auto& entity : mSolider)
				entity->GetSteeringModule()->GetBehavior<AI::AviodObsBehavior>("Avoid")->SetActive(mEnemyAvoid);
		}
		if (ImGui::Button("Gathering!"))
		{
			Clear();
			mOrder = Order::Gathering;
			for (auto& entity : mSolider)
			{
				entity.get()->MaxSpeed = 200.0f;
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
					bool isCloseAtObstacles = false;
					for (auto& obs : mWorld.GetObstacles())
					{
						if (Distance(entity->Position, obs.center) < obs.radius + 50.0f)
							isCloseAtObstacles = true;
					}
					if (isCloseAtObstacles) continue;
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
				nearest.x += 350.0f;
			if (nearest.y < 100.0f)
				nearest.y += 350.0f;
			if (nearest.x > static_cast<float>(GS->GetBackBufferWidth()) - 150.0f)
				nearest.x -= 350.0f;
			if (nearest.y > static_cast<float>(GS->GetBackBufferHeight()) - 150.0f)
				nearest.y -= 350.0f;
			int Index = 0;
			int MaxIndex = mSolider.size() / 3;
			bool FirstFormation = true;
			bool SecondFormation = false;
			bool ThirdFormation = false;
			for (auto& entity : mSolider)
			{
				if (FirstFormation)
				{
					entity.get()->Destination = Vector2{ nearest.x + (mOffset * Index), nearest.y };
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
					entity.get()->Destination = Vector2{ nearest.x + (mOffset * Index), nearest.y + mOffset };
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
					entity.get()->Destination = Vector2{ nearest.x + (mOffset * Index), nearest.y + mOffset * 2 };
					Index++;
				}
			}
			mTimer = 0.0f;
			isProcessing = true;
			for (auto& entity : mSolider)
			{
				entity.get()->GetSteeringModule()->GetBehavior<SeekBehavior>("Seek")->SetActive(true);
				entity.get()->GetSteeringModule()->GetBehavior<SeekBehavior>("Arrive")->SetActive(true);
				entity.get()->GetSteeringModule()->GetBehavior<SeparationBehavior>("Separation")->SetActive(true);
				entity.get()->GetSteeringModule()->GetBehavior<CohesionBehavior>("Cohesion")->SetActive(true);
			}
		}
		if (ImGui::Button("Moving!"))
		{
			Clear();
			mOrder = Order::Moving;
			for (auto& entity : mSolider)
			{
				entity->MaxSpeed = 50.0f;
				entity.get()->SetOrder(true);
			}

			size_t size = mSolider.size();
			mGeneral = static_cast<size_t>(RandomInt(0, size - 1));
			mSolider[mGeneral]->MaxSpeed = 500.0f;
			for (auto& entity : mSolider)
				entity->threat = mSolider[mGeneral].get();

			Circle obstacle;
			for (auto& obs : mWorld.GetObstacles())
			{
				if (Distance(nearest, obs.center) < obs.radius + 150.0f)
					obstacle = Circle{obs.center,obs.radius};
			}
			nearest = Vector2{ RandomFloat(150.0f, (static_cast<float>(GS->GetBackBufferWidth()) - 150.0f))
, RandomFloat(150.0f, static_cast<float>(GS->GetBackBufferHeight()) - 150.0f) };
			
			bool isCheck = false;
			while (isCheck == false)
			{
				nearest = Vector2{ RandomFloat(150.0f, (static_cast<float>(GS->GetBackBufferWidth()) - 150.0f))
				, RandomFloat(150.0f, static_cast<float>(GS->GetBackBufferHeight()) - 150.0f) };
				bool isCloseAtObstacles = false;
				float dis = Distance(nearest, obstacle.center);
				if (dis < obstacle.radius)
					isCloseAtObstacles = true;
				for (auto& obs : mWorld.GetObstacles())
				{
					dis = Distance(nearest, obs.center);
					if (dis < obs.radius)
						isCloseAtObstacles = true;
				}
				if (isCloseAtObstacles)
				{
					continue;
				}
				if (Distance(nearest, previous) > 350.0f)
				{
					isCheck = true;
				}
			}

			if (nearest.x < 100.0f)
				nearest.x += 350.0f;
			if (nearest.y < 100.0f)
				nearest.y += 350.0f;
			if (nearest.x > static_cast<float>(GS->GetBackBufferWidth()) - 150.0f)
				nearest.x -= 350.0f;
			if (nearest.y > static_cast<float>(GS->GetBackBufferHeight()) - 150.0f)
				nearest.y -= 350.0f;

			previous = nearest;
			mSolider[mGeneral]->Destination = nearest;
			mSolider[mGeneral].get()->GetSteeringModule()->GetBehavior<ArriveBehavior>("Seek")->SetActive(true);
			mSolider[mGeneral].get()->GetSteeringModule()->GetBehavior<ArriveBehavior>("Arrive")->SetActive(true);
			mTimer = 0.0f;
			isProcessing = true;

			for (auto& entity : mSolider)
			{
				if (entity == mSolider[mGeneral])
					continue;
				entity.get()->Destination = entity->threat->Destination;
				entity.get()->MaxSpeed = 1.0f;
				entity.get()->GetSteeringModule()->GetBehavior<PursuitBehavior>("Pursuit")->SetActive(true);
				entity.get()->GetSteeringModule()->GetBehavior<SeparationBehavior>("Separation")->SetActive(true);
				entity.get()->GetSteeringModule()->GetBehavior<AlignmentBehavior>("Alignment")->SetActive(true);
			}
		}
		if (ImGui::Button("Wandering!"))
		{
			Clear();
			mOrder = Order::Wandering;
			for (auto& entity : mSolider)
			{
				entity.get()->MaxSpeed = 50.0f;
				entity.get()->SetOrder(true);
				entity.get()->threat = mPlayer.get();
			}
			mTimer = 0.0f;
			isProcessing = true;

			for (auto& entity : mSolider)
			{
				entity->GetSteeringModule()->GetBehavior<AI::EvadeBehavior>("Evade")->SetActive(true);
				entity->GetSteeringModule()->GetBehavior<AI::SeparationBehavior>("Separation")->SetActive(true);
			}
		}
		if (ImGui::Button("Ambush!"))
		{
			Clear();
			isHided = false;
			mOrder = Order::Ambush;
			for (auto& entity : mSolider)
			{
				entity.get()->MaxSpeed = 200.0f;
				entity.get()->SetOrder(true);
				entity.get()->threat = mPlayer.get();
			}

			mTimer = 0.0f;
			mHideTimer = 0.0f;
			isProcessing = true;

			for (auto& entity : mSolider)
			{
				entity->GetSteeringModule()->GetBehavior<AI::HideBehavior>("Hide")->SetActive(true);
				entity->GetSteeringModule()->GetBehavior<AI::AlignmentBehavior>("Alignment")->SetActive(true);
			}
		}
	}

	if (isDisplaying)
	{
		for (auto& entity : mSolider)
		{
			if (entity->Destination.x != 0.0f && entity->Destination.y != 0.0f)
			{
				if (entity->GetSteeringModule()->GetBehavior<EvadeBehavior>("Evade")->IsActive() ||
					entity->GetSteeringModule()->GetBehavior<PursuitBehavior>("Pursuit")->IsActive())
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(entity->Position, entity->threat->Position, JimmyGod::Graphics::Colors::Red);
					JimmyGod::Graphics::SimpleDraw::AddScreenCircle(JimmyGod::Math::Circle{ entity->threat->Position,12.5f }, JimmyGod::Graphics::Colors::Red);
				}
				else
				{
					JimmyGod::Graphics::SimpleDraw::AddScreenLine(entity->Position, entity->Destination, JimmyGod::Graphics::Colors::Red);
					JimmyGod::Graphics::SimpleDraw::AddScreenCircle(JimmyGod::Math::Circle{ entity->Destination,12.5f }, JimmyGod::Graphics::Colors::Red);
				}
			}
		}
		if (mPlayer->Destination.x != 0.0f && mPlayer->Destination.y != 0.0f)
		{
			JimmyGod::Graphics::SimpleDraw::AddScreenLine(mPlayer->Position, mPlayer->Destination, JimmyGod::Graphics::Colors::White);
			JimmyGod::Graphics::SimpleDraw::AddScreenCircle(JimmyGod::Math::Circle{ mPlayer->Destination,20.0f }, JimmyGod::Graphics::Colors::Gold);
		}

	}

	ImGui::End();
}
