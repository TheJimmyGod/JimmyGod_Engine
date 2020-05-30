#pragma once

#include "AnimationClip.h"

namespace JimmyGod::Graphics
{
	struct AnimationSet
	{
		std::vector<std::unique_ptr<AnimationClip>> clips;
	};
}