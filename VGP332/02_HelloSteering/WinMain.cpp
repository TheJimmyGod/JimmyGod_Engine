#include "Fish.h"
#include <XEngine.h>
#include <AI.h>
#include "AIWorld.h"
#include "Mario.h"
#include "TileMap.h"

using namespace AI;
using namespace Steering;

AI::AIWorld::Settings aiSetting;
AI::AIWorld world;
Fish fish(world);
Mario mario(world);
TileMap tileMap;
//
std::vector<std::unique_ptr<Mario>> marios;
std::vector<std::unique_ptr<Fish>> fishes;
void GameInit()
{
	tileMap.Load();
	world.AddObstacles({ {100.0f,500.0f},100.0f });
	world.AddObstacles({ {300.0f,500.0f},80.0f });
	//world.AddObstacles({ {800.0f,300.0f},100.0f });
	//world.AddObstacles({ {1000.0f,100.0f},100.0f });
	//world.AddObstacles({ {900.0f,400.0f},100.0f });

	world.AddWalls({ 800,300 });

	AI::AIWorld::Settings settings;
	settings.partitionGridSize = 100.0f;
	settings.worldSize.x = static_cast<float>(X::GetScreenWidth());
	settings.worldSize.y = static_cast<float>(X::GetScreenHeight());

	world.Initialize(settings);

	auto afish = fishes.emplace_back(std::make_unique<Fish>(world)).get();
	afish->Load();
	for (int i = 0; i < 10; i++)
	{
		auto amario = &marios.emplace_back(std::make_unique<Mario>(world));
		amario->get()->Load();
		amario->get()->Position = { X::RandomFloat(250,750), X::RandomFloat(100,500) };
		amario->get()->threat = afish;
	}

	//fish.Load();
	//mario.Load();
}

void GameCleanup()
{
	auto afish = fishes.emplace_back(std::make_unique<Fish>(world)).get();
	for (auto& entity : marios)
		entity->Unload();

	for (auto& entity : fishes)
		entity->Unload();
	tileMap.UnLoad();
	//fish.Unload();
	//mario.Unload();
}

bool GameLoop(float deltaTime)
{
	world.Update();
	world.DebugDraw();
	//tileMap.Render();


	for (auto& entity : marios)
		entity->Update(deltaTime);
	for (auto& entity : marios)
		entity->Render();

	for (auto& entity : fishes)
		entity->Update(deltaTime);
	for (auto& entity : fishes)
		entity->Render();

	//fish.Update(deltaTime);
	//fish.Render();

	//mario.Update(deltaTime);
	//mario.Render();
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}