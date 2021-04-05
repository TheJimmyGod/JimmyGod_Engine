#pragma once

#include "Input/Inc/InputTypes.h"
#include "Math/Inc/EngineMath.h"

namespace DirectX
{
	class CommonStates;
	class SpriteBatch;
}

namespace JimmyGod::Graphics
{
	class Texture;
	class SpriteRenderer
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static SpriteRenderer* Get();

	public:
		SpriteRenderer();
		~SpriteRenderer();

		SpriteRenderer(const SpriteRenderer&) = delete;
		SpriteRenderer& operator=(const SpriteRenderer&) = delete;

		void Initialize();
		void Terminate();

		void SetTransform(const Math::Matrix4& transform);

		void BeginRender();
		void EndRender();

		void Draw(const Texture& texture, const Math::Vector2& pos, float rotation = 0.0f, 
			JimmyGod::Input::Pivot pivot = JimmyGod::Input::Pivot::Center, 
			JimmyGod::Input::Flip flip = JimmyGod::Input::Flip::None);
		void Draw(const Texture& texture, const Math::Rect& sourceRect, const Math::Vector2& pos, float rotation = 0.0f, 
			JimmyGod::Input::Pivot pivot = JimmyGod::Input::Pivot::Center, 
			JimmyGod::Input::Flip flip = JimmyGod::Input::Flip::None);

	private:
		friend class Font;

		std::unique_ptr<DirectX::CommonStates> mCommonStates;
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;

		Math::Matrix4 mTransform;
	};
}