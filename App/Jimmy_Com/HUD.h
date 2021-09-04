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

		void DisplayAllButtons();
		void DisappearAllButtons();

		std::unique_ptr<JimmyGod::Button>& GetButton(size_t index) { return mButtons[index]; }

	private:
		std::vector<std::unique_ptr<Button>> mButtons;
		JimmyGod::Graphics::TextureId mTextures;

		bool isDisplay = false;
	};
}
