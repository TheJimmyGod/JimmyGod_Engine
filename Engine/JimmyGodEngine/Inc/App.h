#pragma once

namespace JimmyGod
{
	class AppState;

	struct AppConfig
	{
		std::string appName = "JimmyGod";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
	};
	class App
	{
	public:
		//std::string msg = "Pater is awesome";
		//std::string_view sv = msg; -> string_view stores 2 pointers of stirng
		template<class StateType, class = std::void_t<std::is_base_of<AppState,StateType>>>

		void AddState(std::string name)
		{
			mAppStates.emplace(std::move(name), std::make_unique<StateType>());
		}

		void ChangeState(std::string name);

		void Run(AppConfig appConfig);
		void Quit() { mRunning = false; };
	private:
		AppConfig mAppConfig;
		Core::Window mWindow;
		bool mRunning = false;

		std::map<std::string, std::unique_ptr<AppState>> mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;
	};
} // namespace JimmyGod