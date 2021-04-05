#pragma once

#include "Input/Inc/InputTypes.h"
#include "Colors.h"

namespace JimmyGod::Graphics
{
	class SpriteRenderManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static SpriteRenderManager* Get();
	public:
		void Initialize();
		void Terminate();
		void Render();

		JimmyGod::Input::TextureId LoadTexture(const char* fileName);
		void ClearAllTextures();
		void DrawSprite(JimmyGod::Input::TextureId textureId, const Math::Vector2& position, 
			JimmyGod::Input::Pivot pivot = JimmyGod::Input::Pivot::Center, 
			JimmyGod::Input::Flip flip = JimmyGod::Input::Flip::None);
		void DrawSprite(JimmyGod::Input::TextureId textureId, const Math::Vector2& position, 
			float rotation, JimmyGod::Input::Pivot pivot = JimmyGod::Input::Pivot::Center, 
			JimmyGod::Input::Flip flip = JimmyGod::Input::Flip::None);
		void DrawSprite(JimmyGod::Input::TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position);
		void* GetSprite(JimmyGod::Input::TextureId textureId);
		void DrawScreenText(const char* str, float x, float y, float size, const Graphics::Color& color);
	};
}