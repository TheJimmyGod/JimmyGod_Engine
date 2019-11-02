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

		void ProcessMessage();

		HWND GetWindowHandle() const { return mWindow; }
		bool IsActive() const { return mActive; }

	private:
		HINSTANCE mInstance = nullptr;
		HWND mWindow = nullptr;
		std::string mAppName;
		bool mActive{ false };
	};
}