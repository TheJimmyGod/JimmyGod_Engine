#include "Precompiled.h"
#include "Model.h"

#include "MeshIO.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;

// ../../Assets/Models/soldier.model
// ../../Assets/Models/soldier.skeleton
// ../../Assets/Models/soldier.animation

void ModelLoader::LoadModel(std::filesystem::path fileName, Model & model)
{
	fileName.replace_extension("Model");

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	uint32_t numMeshes = 0;
	fscanf_s(file, "MeshCount: %d\n", &numMeshes);

	model.meshData.resize(numMeshes);
	for (uint32_t i = 0; i < numMeshes; ++i)
	{
		MeshIO::Read(file, model.meshData[i].mesh);
	}

	//For homework, load model.materialData as well...
	// if read <none> for texture name, there is no te
	fclose(file);
	for (auto& data : model.meshData)
	{
		data.meshBuffer.Initialize(data.mesh);
	}
	for (auto& data : model.materialData)
	{
		if (!data.diffuseMap)
		{
			data.diffuseMap = std::make_unique<>;
			data.diffuseMap->Initialize();
		}
	}
}


void Model::Initialize(const std::filesystem::path & fileName)
{
	ModelLoader::LoadModel(fileName, *this);
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
		materialData[data.materialIndex].diffuseMap->BS
		data.meshBuffer.Draw();
	}
}
