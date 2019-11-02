#pragma once

namespace JimmyGod::Core 
{
	class Window
	{
	public:
		Window() = default;
		//HINSTANCE = Window process ID
		void Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height);
		void Terminate();

		bool ProcessMessage();

		HWND GetWindowHandle() const { return mWindow; }

	private:
		HINSTANCE mInstance = nullptr;
		HWND mWindow = nullptr;
		std::string mAppName;
	};
}