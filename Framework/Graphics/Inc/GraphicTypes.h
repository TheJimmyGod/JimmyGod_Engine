#pragma once

#include "Colors.h"

namespace JimmyGod::Graphics
{
	struct Vertex
	{
		JimmyGod::Math::Vector3 position;
		JimmyGod::Graphics::Color color;
	};

	struct Mesh
	{
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;
	};
}
