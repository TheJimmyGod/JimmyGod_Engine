#include "Precompiled.h"
#include "SpriteRenderManager.h"
#include "SpriteRenderer.h"
#include "Font.h"
#include "TextureManager.h"
#include "Texture.h"

#pragma comment(lib,"FW1FontWrapper.lib")

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;
using namespace JimmyGod::Input;

namespace
{
	struct SpriteCommand
	{
		SpriteCommand(JimmyGod::Input::TextureId inTextureId, const Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		SpriteCommand(JimmyGod::Input::TextureId inTextureId, const Vector2& inPosiiton, float inRotation, JimmyGod::Input::Pivot pivot, JimmyGod::Input::Flip flip)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
			, pivot(pivot)
			, flip(flip)
		{}

		SpriteCommand(JimmyGod::Input::TextureId inTextureId, const Rect& inSourceRect, const Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect(inSourceRect)
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		JimmyGod::Input::TextureId textureId = 0;
		Rect sourceRect{};
		Vector2 position{ 0.0f };
		float rotation{ 0.0f };
		Pivot pivot = Pivot::Center;
		Flip flip = Flip::None;
	};

	struct TextCommand
	{
		TextCommand(std::wstring inStr, float inSize, float inX, float inY, const Color& inColor)
			: str(std::move(inStr))
			, size(inSize)
			, x(inX)
			, y(inY)
			, color(inColor)
		{}

		std::wstring str;
		float size, x, y;
		Color color;
	};
	std::vector<SpriteCommand> mySpriteCommands;
	std::vector<TextCommand> myTextCommands;

	Font myFont;
	std::unique_ptr<SpriteRenderManager> sInstance;
}

void SpriteRenderManager::StaticInitialize()
{
	sInstance = std::make_unique<SpriteRenderManager>();
	sInstance->Initialize();
}

void SpriteRenderManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

SpriteRenderManager * SpriteRenderManager::Get()
{
	return sInstance.get();
}

void SpriteRenderManager::Initialize()
{
	myFont.Initialize();
}

void SpriteRenderManager::Terminate()
{
	myFont.Terminate();
}

void SpriteRenderManager::Render()
{
	//Sprite

	JimmyGod::Input::TextureId id = 0;
	JimmyGod::Graphics::Texture* texture = nullptr;

	SpriteRenderer::Get()->BeginRender();
	for (const auto& command : mySpriteCommands)
	{
		if (id != command.textureId)
		{
			texture = JimmyGod::Graphics::TextureManager::Get()->GetTexture(command.textureId);
			id = command.textureId;
		}
		if (texture)
		{
			if (Math::IsEmpty(command.sourceRect))
			{
				SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.pivot, command.flip);
			}
			else
			{
				SpriteRenderer::Get()->Draw(*texture, command.sourceRect, command.position, command.rotation, command.pivot, command.flip);
			}
		}
	}
	mySpriteCommands.clear();
	SpriteRenderer::Get()->EndRender();

	// Text
	for (const auto& command : myTextCommands)
	{
		myFont.Draw(command.str.c_str(), command.x, command.y, command.size, command.color);
	}
	myTextCommands.clear();
}

JimmyGod::Input::TextureId JimmyGod::Graphics::SpriteRenderManager::LoadTexture(const char* fileName)
{
	return TextureManager::Get()->Load(fileName);
}

void SpriteRenderManager::ClearAllTextures()
{
	TextureManager::Get()->Clear();
}

void SpriteRenderManager::DrawSprite(JimmyGod::Input::TextureId textureId, const Math::Vector2 & position, JimmyGod::Input::Pivot pivot, JimmyGod::Input::Flip flip)
{
	mySpriteCommands.emplace_back(textureId, position, 0.0f, pivot, flip);
}

void SpriteRenderManager::DrawSprite(JimmyGod::Input::TextureId textureId, const Math::Vector2 & position, 
	float rotation, JimmyGod::Input::Pivot pivot, JimmyGod::Input::Flip flip)
{
	mySpriteCommands.emplace_back(textureId, position, rotation, pivot, flip);
}

void SpriteRenderManager::DrawSprite(TextureId textureId, const Math::Rect & sourceRect, const Math::Vector2 & position)
{
	mySpriteCommands.emplace_back(textureId, sourceRect, position, 0.0f);
}

void * SpriteRenderManager::GetSprite(JimmyGod::Input::TextureId textureId)
{
	JimmyGod::Graphics::Texture* texture = JimmyGod::Graphics::TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetShaderResourceView() : nullptr;
}

void SpriteRenderManager::DrawScreenText(const char * str, float x, float y, float size, const Graphics::Color & color)
{
	int strLenght = static_cast<int>(strlen(str));
	int convertResult = MultiByteToWideChar(CP_UTF8, 0, str, strLenght, NULL, 0);
	std::wstring wideString(convertResult, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, strLenght, &wideString[0], convertResult);
	myTextCommands.emplace_back(std::move(wideString), size, x, y, color);
}
