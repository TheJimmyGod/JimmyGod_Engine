#include "Precompiled.h"
#include "App.h"

using namespace JimmyGod;

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

	// Initialize engine systems
	mRunning = true;
	while (mWindow.IsActive() && mRunning)
	{
		mWindow.ProcessMessage();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			Quit();
		}
		//Do Game Stuff
	}

	// Termiates engine systems

	// Terminate window

	mWindow.Terminate();
}
