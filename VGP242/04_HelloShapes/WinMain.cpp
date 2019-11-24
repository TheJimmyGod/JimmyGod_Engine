#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "GameState.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	JimmyGod::MainApp().AddState<GameState>("GameState");
	JimmyGod::MainApp().AddState<HeartState>("HeartState");
	JimmyGod::MainApp().AddState<TriForceState>("TriForceState");
	JimmyGod::MainApp().Run({"Hello Shapes, JimmyGod",1280,720});
	return 0;
}