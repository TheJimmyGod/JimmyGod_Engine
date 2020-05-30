#pragma once

#include "Mesh.h"
#include "MeshBuffer.h"
#include "Material.h"
#include "Texture.h"
#include "Skeleton.h"
#include "AnimationSet.h"

namespace JimmyGod::Graphics
{
	class Model;
	class ModelLoader
	{
	public:
		static void LoadModel(std::filesystem::path fileName, Model& model);
		static void LoadSkeleton(std::filesystem::path fileName, Skeleton& skeleton);
		static void LoadAnimationSet(std::filesystem::path fileName, AnimationSet& animationSet);
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
			SkinnedMesh mesh;
			uint32_t materialIndex = 0;
			MeshBuffer meshBuffer;
		};
		struct MaterialData
		{
			Material material;
			std::string diffuseMapName;
			std::unique_ptr<Texture> diffuseMap;
		};
		std::vector<MeshData> meshData;
		std::vector<MaterialData> materialData;
		Skeleton mSkeleton;
		AnimationSet mAnimationSet;
	};
}