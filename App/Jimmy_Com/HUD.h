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
		HUD() = default;
		~HUD() = default;

		void Initialize();
		void Terminate();
		void Update(float deltaTime);

		void Render();

		void DisplayAllButtons()
		{
			for (auto& button : mButtons)
				button->SetDisplay(true);
		}
		void DisappearAllButtons()
		{
			for (auto& button : mButtons)
				button->SetDisplay(false);
		}

		std::unique_ptr<JimmyGod::Button>& GetButton(size_t index) { return mButtons[index]; }

	private:
		std::vector<std::unique_ptr<Button>> mButtons;
		JimmyGod::Graphics::TextureId mTextures;
	};
}
