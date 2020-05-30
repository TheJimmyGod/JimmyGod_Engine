#pragma once

#include "Common.h"

namespace JimmyGod::Graphics
{
	struct Bone
	{
		std::string name;
		int index;

		Bone* parent = nullptr;
		int parentIndex = -1;

		std::vector<Bone*> children;
		std::vector<int> childIndice;

		Math::Matrix4 toParentTransform;
		Math::Matrix4 offsetTransform;
	};
}