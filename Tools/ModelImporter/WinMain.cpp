#include <Graphics/Inc/Graphics.h>
#include <Math/Inc/EngineMath.h>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <cstdio>

using namespace JimmyGod::Graphics;

struct Arguments
{
	const char* inputFileName = nullptr;
	const char* outputFileName = nullptr;
	float scale = 1.0f;
	bool animOnly = false;
};

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
	//We need at least 3 arguments
	if (argc < 3)
		return std::nullopt;

	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];

	for (int i = 0; i + 2 < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 's':
				args.scale = static_cast<float>(atof(argv[i + 1]));
				break;
			}
		}
	}
	return args;
}

void PrintUsage()
{
	printf(
		"== ModelImporter Help ==\n"
		"\n"
		"Usage:\n"
		"    ModelImporter.exe [Options] <InputFile> <OutputFile>\n"
		"\n"
		"Options:\n"
		"     -s      Scale applied to the model."
		"\n"
	);
}

inline Color Convert(const aiColor3D& c)
{
	return{ c.r,c.g,c.b,1.0f };
}

inline Vector3 Convert(const aiVector3D& v)
{
	return { v.x,v.y,v.z };
}

inline Quaternion Convert(const aiQuaternion& q)
{
	return { q.x,q.y,q.z,q.w };
}

inline Matrix4 Convert(const aiMatrix4x4& m)
{
	Matrix4 mat = *(reinterpret_cast<const Matrix4*>(&m));
	return Transpose(mat);
}

void ExportEmbeddedTexture(const aiTexture& texture, const Arguments& args,
	const std::string& fileName)
{
	printf("Extracting embedded texture...\n");
	std::string fullFileName = args.outputFileName;
	fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
	fullFileName += fileName;

	FILE* file = nullptr;
	fopen_s(&file, fullFileName.c_str(), "wb");
	size_t written = fwrite(texture.pcData, 1, texture.mWidth, file);
	ASSERT(written == texture.mWidth, "Error: Failed to extract embedded texture!");
	fclose(file);
}

std::string FindTexture(const aiScene* scene, const aiMaterial* inputMaterial,
	aiTextureType textureType, const Arguments& args, const char* suffix)
{
	std::filesystem::path textureName;
	const uint32_t textureCount = inputMaterial->GetTextureCount(textureType);
	if (textureCount > 0)
	{
		aiString texturePath;
		if (inputMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
		{
			
			if (auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str()); embeddedTexture)
			{
				// embeddedFilePath "E:/dev/ ... / monster.tga"
				std::filesystem::path embeddedFilePath = texturePath.C_Str();
				// fileName = "../../Assets/Models/Tank/tank.fbx"
				std::string fileName = args.inputFileName;
				// fileName = "../../Assets/Models/Tank/tank"
				fileName.erase(fileName.length() - 4); // remove ".fbx" extension
				// fileName = "../../Assets/Models/Tank/tank_diffuse"
				fileName += suffix;
				// fileName = "../../Assets/Models/Tank/tank_diffuse.tga"
				fileName += embeddedFilePath.extension().u8string();

				ExportEmbeddedTexture(*embeddedTexture, args, fileName);

				printf("Exporting embedded texture to %s\n", fileName.c_str());
				textureName = fileName;
			}
		}
		else
		{
			std::filesystem::path filePath = texturePath.C_Str();
			std::string fileName = filePath.filename().u8string();
			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
	}
	return textureName.filename().u8string().c_str();
}

void SaveModel(const Arguments& args, Model& model)
{
	std::filesystem::path fileName = args.inputFileName;
	fileName.replace_extension("model");

	printf("Saving model: %s...\n", fileName.u8string().c_str());

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "w");
	uint32_t numMeshes = model.meshData.size();

	for (uint32_t i = 0; i < numMeshes; i++)
	{
		MeshIO::Write(file, model.meshData[i].mesh);
	}

	//For homework, save out model.materialData as well...
	// if diffuseMapName is empty string, write <none>
	fclose(file);
	//for (auto& data : model.meshData)
	//{
	//	data.meshBuffer.Initialize(data.mesh);
	//}

	// Homework, this is basically the oppostie of LoadModel
	// You will need MeshIO::Write;
}


// Description:
// Imports a 3D model file and save data out into a file
int main(int argc, char* argv[])
{
	const auto argsOpt = ParseArgs(argc, argv);
	if (!argsOpt.has_value())
	{
		PrintUsage();
		return -1;
	}

	const auto& args = argsOpt.value();

	// Create an instance of the importer class to do the parsing for us
	Assimp::Importer importer;
	
	// Try to import the model into a scene.
	const aiScene* scene = importer.ReadFile(args.inputFileName,
		aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	// scene
	// +- mesh[_][_][_][_][_]
	// +- material[_][_][_][_][_]
	// +- animation[_][_][_][_][_]
	// 
	// +- mRootNode (scene graph)
	//		+- Node
	//			+- Node
	//			+- Node
	//				+- Node ...
	//		+- Node
	//			+- Node ...
	//		+- Node

	Model model;

	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");

		const uint32_t numMeshes = scene->mNumMeshes;
		model.meshData.resize(numMeshes);

		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];
			const uint32_t numVertices = inputMesh->mNumVertices;
			const uint32_t numFaces = inputMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			printf("Reading material index...\n");

			model.meshData[meshIndex].materialIndex = inputMesh->mMaterialIndex;

			printf("Reading vertices...\n");


			std::vector<Vertex> vertices;
			vertices.reserve(numVertices);

			const aiVector3D* positions = inputMesh->mVertices;
			const aiVector3D* normals = inputMesh->mNormals;
			const aiVector3D* tangents = inputMesh->mTangents;
			const aiVector3D* texCoords = inputMesh->HasTextureCoords(0) ? inputMesh->mTextureCoords[0] : nullptr;

			// // For homework, add data to vertices
			if (positions)
			{
				for (uint32_t i = 0; i < numVertices; i++)
				{
					vertices.push_back({ positions[i].x * args.scale,
						positions[i].y * args.scale,
						positions[i].z * args.scale });
				}
			}
			if (normals)
			{
				for (uint32_t i = 0; i < numVertices; i++)
				{
					vertices[i].normal = { normals[i].x, normals[i].y,normals[i].z };
				}
			}
			if (tangents)
			{
				for (uint32_t i = 0; i < numVertices; i++)
				{
					vertices[i].tangent = { tangents[i].x, tangents[i].y,tangents[i].z };
				}
			}
			if (texCoords)
			{
				for (uint32_t i = 0; i < numVertices; i++)
				{
					vertices[i].texcoord = { texCoords[i].x, texCoords[i].y};
				}
			}

			printf("Reading indices...\n");

			std::vector<uint32_t> indices;
			indices.reserve(numIndices);

			const aiFace* faces = inputMesh->mFaces;
			for (uint32_t i = 0; i < numFaces; ++i)
			{
				indices.push_back(faces[i].mIndices[0]);
				indices.push_back(faces[i].mIndices[1]);
				indices.push_back(faces[i].mIndices[2]);
			}

			Mesh mesh;
			mesh.vertices = std::move(vertices);
			mesh.indices = std::move(indices);
			model.meshData[meshIndex].mesh = std::move(mesh);
		}
	}

	// Look for material data
	if (scene->HasMaterials())
	{
		printf("Reading materials...\n");

		const uint32_t numMaterials = scene->mNumMaterials;
		model.materialData.resize(numMaterials);
		for (uint32_t i = 0; i < numMaterials; ++i)
		{
			const aiMaterial* inputMaterial = scene->mMaterials[i];

			aiColor3D ambientColor(0.0f, 0.0f, 0.0f);
			inputMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
			aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
			inputMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			aiColor3D specularColor(0.0f, 0.0f, 0.0f);
			inputMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
			ai_real specularPower;
			inputMaterial->Get(AI_MATKEY_SHININESS, specularPower);

			auto& material = model.materialData[i];
			material.material.ambient = Convert(ambientColor);
			material.material.diffuse = Convert(diffuseColor);
			material.material.specular = Convert(specularColor);
			material.material.power = specularPower;
			material.diffuseMapName = FindTexture(scene, inputMaterial, aiTextureType_DIFFUSE, args, "_diffuse");
		}
	}

	SaveModel(args, model); // ../../Assets/Models/<name>.model

	printf("All done!\n");
	return 0;
}

// Add HelloFBX, use this to export a model that you can show in the demo project