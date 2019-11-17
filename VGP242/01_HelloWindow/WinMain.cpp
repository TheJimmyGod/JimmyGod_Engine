#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	JimmyGod::MainApp().AddState<JimmyGod::AppState>("DummyState");
	JimmyGod::MainApp().Run({ "Jimmy's head",1280,720 });
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