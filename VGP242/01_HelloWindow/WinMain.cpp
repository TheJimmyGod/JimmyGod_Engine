#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	JimmyGod::AppConfig appConfig;
	appConfig.appName = "Jimmy's window";
	appConfig.windowWidth = 1280;
	appConfig.windowHeight = 720;

	JimmyGod::MainApp().Run(appConfig);
	return 0;
}

// Interface : a list of public pure virture function declarations

// struct IMoviePlayer
// {
//		virtual void Play() = 0;
// };
//	class VLC : public IMoviePlayer
//	{
//public:
//	void Play() override;
//	};

// COM -> 1) Interface based API
//	   -> 2) Handle memory management