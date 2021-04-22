#pragma once

namespace JimmyGod::Graphics
{
	struct SpriteAnimationInfo
	{
		std::string fileName;
		int rows{ 0 };
		int columns{ 0 };
		int frameCount{ 0 };
		float framePerSecond{ 0.0f };
		bool looping{ false };
	};
}