#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	JimmyGod::AppConfig appConfig;
	appConfig.appName = "Hello Jimmy";
	appConfig.windowWidth = 1280;
	appConfig.windowHeight = 720;

	JimmyGod::MainApp().Run(appConfig);
	return 0;
}