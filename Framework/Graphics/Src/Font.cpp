#include "Precompiled.h"
#include "Font.h"

#include "GraphicsSystem.h"
#include <FW1FontWrapper/Source/FW1FontWrapper.h>

using namespace JimmyGod::Graphics;

namespace
{
	inline uint32_t ToColor(const Color& color)
	{
		uint8_t r = (uint8_t)(color.x * 255);
		uint8_t g = (uint8_t)(color.y * 255);
		uint8_t b = (uint8_t)(color.z * 255);
		return 0xff000000 | (b << 16) | (g << 8) | r;
	}
}

//----------------------------------------------------------------------------------------------------

Font::Font()
	: mFW1Factory(nullptr)
	, mFontWrapper(nullptr)
{
}

Font::~Font()
{
}

void Font::Initialize()
{
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	FW1CreateFactory(FW1_VERSION, &mFW1Factory);
	auto hr = mFW1Factory->CreateFontWrapper(device, L"Consolas", &mFontWrapper);
	ASSERT(SUCCEEDED(hr), "[Font] Failed to initialize FW1FontWrapper. Error = %x", hr);
}


void Font::Terminate()
{
	SafeRelease(mFontWrapper);
	SafeRelease(mFW1Factory);
}

void Font::Draw(const wchar_t* str, float x, float y, float size, const Color& color)
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	mFontWrapper->DrawString(context, str, size, x, y, ToColor(color), FW1_RESTORESTATE);
}