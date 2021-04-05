#pragma once
#include "Colors.h"

interface IFW1Factory;
interface IFW1FontWrapper;

namespace JimmyGod::Graphics
{
	class Font
	{
	public:
		Font();
		~Font();

		void Initialize();
		void Terminate();

		void Draw(const wchar_t* str, float x, float y, float size, const Color& color);

	private:
		IFW1Factory* mFW1Factory;
		IFW1FontWrapper* mFontWrapper;
	};
}