#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "GameState.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	JimmyGod::MainApp().AddState<GameState>("GameState");
	JimmyGod::MainApp().Run({ "Fighter Ground, JimmyGod",450,258 });
	return 0;
}