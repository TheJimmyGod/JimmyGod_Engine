#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace FighterGround
{
	class Energy
	{
	public:
		void Load();
		void Unload();
		void Update(float deltaTime);
		void Render();

		void Fire(const JimmyGod::Math::Vector2& pos, const JimmyGod::Math::Vector2& vel, bool ability = false);
		void Kill();
		bool IsActive() const { return mActive; }
		bool IsAbility() const { return mAbility; }

		JimmyGod::Math::Circle GetBoundary() const { return JimmyGod::Math::Circle{ mPos, 15.0f }; }
	private:
		JimmyGod::Math::Vector2 mPos = Vector2::Zero;
		JimmyGod::Math::Vector2 mVel = Vector2::Zero;
		JimmyGod::Graphics::TextureId mTexture[2];
		bool mActive = false;
		bool mAbility = false;
	};
}