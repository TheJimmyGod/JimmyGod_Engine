#include "Precompiled.h"
#include "App.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;

void App::Run(AppConfig appConfig)
{
	mAppConfig = appConfig;

	// Setup our application window
	mWindow.Initialize(
		GetModuleHandle(NULL),
		appConfig.appName.c_str(),
		appConfig.windowWidth,
		appConfig.windowHeight
	);

	// Initialize input systems
	auto handle = mWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);

	// Initialize Graphics systems
	GraphicsSystem::StaticInitialize(handle, false);

	// OnInit

	mRunning = true;
	while (mWindow.IsActive() && mRunning)
	{
		mWindow.ProcessMessage();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			Quit();
		}
		// Do Game Stuff
		// OnGameLoop
	}

	// OnCleanUp

	// Termiates engine systems

	InputSystem::StaticTerminate();
	GraphicsSystem::StaticTerminate();

	// Terminate window

	mWindow.Terminate();
}
