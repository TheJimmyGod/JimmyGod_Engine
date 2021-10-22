#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace JimmyCom
{
	class HUD
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static HUD* Get();
	public:

		enum State
		{
			None,
			GamePlay,
			End
		};

		HUD() = default;
		~HUD() = default;

		void Initialize();
		void Terminate();
		void Update(float deltaTime);

		void Render();

		void DisplayMainMenu();

		void DisplayAllButtons();
		void DisappearAllButtons();

		std::unique_ptr<JimmyGod::Button>& GetButton(size_t index) { return mButtons[index]; }

		const State& GetState() const { return mGameState; }


	private:
		std::vector<std::unique_ptr<Button>> mButtons;
		std::vector<std::unique_ptr<Button>> mMainMenuButtons;

		JimmyGod::Graphics::TextureId mMainMenuSprite;

		bool isDisplay = false;
		float mXaxis = 2000.0f;
		float mCurrentxAxis = 0.0f;

		State mGameState = State::None;
	};
}
