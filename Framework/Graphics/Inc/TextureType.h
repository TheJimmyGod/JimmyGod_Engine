#pragma once

enum class Pivot
{
	TopLeft,
	Top,
	TopRight,
	Left,
	Center,
	Right,
	BottomLeft,
	Bottom,
	BottomRight
};

enum class Flip
{
	None,
	Horizontal,
	Vertical,
	Both
};

namespace JimmyGod::Graphics
{
	using AudioId = size_t;
	using TextureId = size_t;
}