#pragma once
#include "SpriteAnimationInfo.h"
#include "TextureType.h"

using namespace JimmyGod::Math;

namespace JimmyGod::Graphics
{
	class SpriteAnimation
	{
	public:
		SpriteAnimation() = default;
		~SpriteAnimation() = default;
		SpriteAnimation(const SpriteAnimation&) = delete;
		SpriteAnimation& operator=(const SpriteAnimation&) = delete;

		void Load(JimmyGod::Graphics::SpriteAnimationInfo info);
		void Unload();
		void Update(float deltaTime);
		//void Render(const JimmyGod::Graphics::Camera& camera);
		void Render();
		
		const JimmyGod::Math::Vector2& GetPosition()
		{
			return mPos;
		}

		void SetPosition(const JimmyGod::Math::Vector2& pos)
		{
			mPos = pos;
		}

		void Play();
		void Stop();
	private:
		JimmyGod::Graphics::SpriteAnimationInfo mInfo;
		JimmyGod::Graphics::TextureId mTextureID;
		JimmyGod::Math::Vector2 mPos = Vector2::Zero;
		float mTimer{ 0.0f };
		bool mActive{ false };
	};
}