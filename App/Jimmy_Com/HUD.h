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
		void Render();
		void RenderText();
	private:
		JimmyGod::Graphics::TextureId mIcons[4];
		// TODO: make new object for button
	};
}
