#pragma once

namespace JimmyGod
{
	struct AppConfig
	{
		std::string appName = "JimmyGod";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
	};
	class App
	{
	public:
		void Run(AppConfig appConfig);
		void Quit() { mRunning = false; };
	private:
		AppConfig mAppConfig;
		Core::Window mWindow;
		bool mRunning = false;
	};
}