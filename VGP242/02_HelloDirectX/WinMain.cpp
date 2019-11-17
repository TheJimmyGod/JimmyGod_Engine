#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "TestState.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	JimmyGod::MainApp().AddState<RedState>("RedState");
	JimmyGod::MainApp().AddState<BlueState>("BlueState");

	JimmyGod::MainApp().Run({"Jimmy's face",1280,720});
	return 0;
}