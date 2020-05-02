#pragma once

#include "Mesh.h"
#include "MeshBuffer.h"
#include "Material.h"
#include "Texture.h"

namespace JimmyGod::Graphics
{
	class Model;
	class ModelLoader
	{
	public:
		static void LoadModel(std::filesystem::path fileName, Model& model);
	};
	class Model
	{
	public:
		void Initialize(const std::filesystem::path& fileName);
		void Terminate();

		void Render();
	public:
		struct MeshData
		{
			Mesh mesh;
			uint32_t materialIndex = 0;
			MeshBuffer meshBuffer;
		};
		struct MaterialData
		{
			Material material;
			std::string diffuseMapName;
			Texture diffuseMap;
		};
		std::vector<MeshData> meshData;
		std::vector<MaterialData> materialData;
	};
}