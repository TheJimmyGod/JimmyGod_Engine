#pragma once
#include "Mesh.h"
namespace JimmyGod::Graphics
{
	class OBJLoader
	{
	public:
		static void Load(const std::filesystem::path& fileName, float scale, Mesh& mesh);
	};
}