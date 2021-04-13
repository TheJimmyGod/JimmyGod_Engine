#pragma once

#include "TextureType.h"
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

		TextureId LoadTexture(const char* fileName);
		void ClearAllTextures();
		void DrawSprite(TextureId textureId, const Math::Vector2& position, 
			Pivot pivot = Pivot::Center, 
			Flip flip = Flip::None);
		void DrawSprite(TextureId textureId, const Math::Vector2& position, 
			float rotation, Pivot pivot = Pivot::Center, 
			Flip flip = Flip::None);
		void DrawSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position);
		void* GetSprite(TextureId textureId);
		void DrawScreenText(const char* str, float x, float y, float size, const Graphics::Color& color);
	};
}