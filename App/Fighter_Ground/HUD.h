#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace FighterGround
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
		void Update(size_t energy, size_t hp);
		void Render();
	private:
		JimmyGod::Graphics::TextureId mEnergy[5];
		JimmyGod::Graphics::TextureId mHealth[3];

		int mEnergyRate = 0;
		int mHealthRate = 0;
	};
}