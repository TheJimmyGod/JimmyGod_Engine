#include "Precompiled.h"
#include "App.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;

void App::ChangeState(const std::string & name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
		mNextState = iter->second.get();
}

void App::Run(AppConfig appConfig)
{
	mAppConfig = std::move(appConfig);

	// Setup our application window
	mWindow.Initialize(
		GetModuleHandle(NULL),
		mAppConfig.appName.c_str(),
		mAppConfig.windowWidth,
		mAppConfig.windowHeight
	);

	// Initialize input systems
	auto handle = mWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);

	// Initialize Graphics systems
	GraphicsSystem::StaticInitialize(handle, false);

	// Initialize the starting state
	mCurrentState = mAppStates.begin()->second.get();
	mCurrentState->Initialize();

	// OnInit

	mRunning = true;
	while (mWindow.IsActive() && mRunning)
	{
		mWindow.ProcessMessage();

		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}


		// TODO
		float deltaTime = 1.0f / 60.0f;
		mCurrentState->Update(deltaTime);
		
		auto graphicSystem = GraphicsSystem::Get();
		graphicSystem->BeginRender();
		mCurrentState->Render();
		graphicSystem->EndRender();
	}

	mCurrentState->Terminate();

	// Termiates engine systems

	InputSystem::StaticTerminate();
	GraphicsSystem::StaticTerminate();

	// Terminate window

	mWindow.Terminate();
}
