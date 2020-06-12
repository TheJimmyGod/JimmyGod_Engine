#include "Precompiled.h"
#include "Model.h"

#include "AnimationIO.h"
#include "Skeleton.h"
#include "SkeletonIO.h"
#include "MeshIO.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;

// ../../Assets/Models/soldier.model
// ../../Assets/Models/soldier.skeleton
// ../../Assets/Models/soldier.animation

void ModelLoader::LoadModel(std::filesystem::path fileName, Model & model)
{
	fileName.replace_extension("model");

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	uint32_t numMeshes = 0;
	fscanf_s(file, "MeshCount: %d\n", &numMeshes);

	model.meshData.resize(numMeshes);
	for (uint32_t i = 0; i < numMeshes; ++i)
	{
		fscanf_s(file, "MaterialIndex: %d\n", &model.meshData[i].materialIndex);
		MeshIO::Read(file, model.meshData[i].mesh);
	}

	//For homework, load model.materialData as well...
	// if read <none> for texture name, there is no te
	fclose(file);

	fileName.replace_extension("Material");
	fopen_s(&file, fileName.u8string().c_str(), "r");
	uint32_t numMaterials = 0;

	fscanf_s(file, "MaterialCount: %d\n", &numMaterials);
	model.materialData.resize(numMaterials);

	for (uint32_t i = 0; i < numMaterials; ++i)
	{
		char name[128];
		fscanf_s(file, "DiffuseMapName: %s\n", &name, sizeof(name));
		model.materialData[i].diffuseMapName = name;
		MeshIO::Read(file, model.materialData[i].material);
	}



	fclose(file);

	for (auto& data : model.meshData)
	{
		data.meshBuffer.Initialize(data.mesh);
	}


	for (auto& data : model.materialData)
	{
		if (!data.diffuseMapName.empty())
		{
			std::string fullFileName = fileName.u8string().c_str();
			fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
			data.diffuseMap = std::make_unique<Texture>();
			data.diffuseMap.get()->Initialize(fullFileName + data.diffuseMapName);
		}
	}
}

void ModelLoader::LoadSkeleton(std::filesystem::path fileName, Skeleton & skeleton)
{
	fileName.replace_extension("skeleton");
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	uint32_t size;
	fscanf_s(file, "BoneCount: %d\n", &size);
	skeleton.bones.resize(size);

	SkeletonIO::Read(file, skeleton);

	fclose(file);
	for (auto& bone : skeleton.bones)
	{
		if (bone->parentIndex != -1)
		{
			bone->parent = skeleton.bones[bone->parentIndex].get();
		}
		else
		{
			skeleton.root = bone.get();
		}

		bone->children.reserve(bone->childIndice.size());

		for (auto childIndex : bone->childIndice)
		{
			bone->children.push_back(skeleton.bones[childIndex].get());
		}
	}
}

void ModelLoader::LoadAnimationSet(std::filesystem::path fileName, AnimationSet & animationSet)
{
	fileName.replace_extension("animset");

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	uint32_t clipCount;
	fscanf_s(file, "ClipCount: %d\n", &clipCount);
	animationSet.clips.resize(clipCount);

	for (auto& clip : animationSet.clips)
	{
		clip = std::make_unique<AnimationClip>();
		AnimationIO::Read(file, *clip);
	}

	fclose(file);
}


void Model::Initialize(const std::filesystem::path & fileName)
{
	ModelLoader::LoadModel(fileName, *this);
	ModelLoader::LoadSkeleton(fileName, mSkeleton);
	ModelLoader::LoadAnimationSet(fileName, mAnimationSet);
}

void Model::Terminate()
{
	for (auto& data : meshData)
		data.meshBuffer.Terminate();
}

void Model::Render()
{
	for (size_t i = 0; i < meshData.size(); ++i)
	{
		auto& data = meshData[i];
		materialData[data.materialIndex].diffuseMap->BindVS();
		materialData[data.materialIndex].diffuseMap->BindPS();
		data.meshBuffer.Draw();
	}
}
