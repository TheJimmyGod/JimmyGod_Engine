#include "Precompiled.h"
#include "MeshIO.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

void MeshIO::Write(FILE * file, const SkinnedMesh & mesh)
{
	const uint32_t size = static_cast<uint32_t>(mesh.vertices.size());
	fprintf_s(file, "VertexCount: %d\n", size);
	for (auto& vertex : mesh.vertices)
	{
		fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
			vertex.position.x, vertex.position.y, vertex.position.z,
			vertex.normal.x, vertex.normal.y, vertex.normal.z,
			vertex.tangent.x, vertex.tangent.y, vertex.tangent.z,
			vertex.texCoord.x, vertex.texCoord.y,
			vertex.boneIndices[0], vertex.boneIndices[1], vertex.boneIndices[2], vertex.boneIndices[3],
			vertex.boneWeights[0], vertex.boneWeights[1], vertex.boneWeights[2], vertex.boneWeights[3]);
	}
	
	const uint32_t indicesSize = static_cast<uint32_t>(mesh.indices.size());
	fprintf_s(file, "IndexCount: %d\n", indicesSize);
	for (uint32_t i = 0; i < indicesSize; i += 3)
	{
		fprintf_s(file, "%i %i %i\n",
			mesh.indices[i + 0],
			mesh.indices[i + 1],
			mesh.indices[i + 2]);
	}
}

void MeshIO::Write(FILE * file, Material & material)
{
	fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		material.ambient.x, material.ambient.y, material.ambient.z, material.ambient.w,
		material.diffuse.x, material.diffuse.y, material.diffuse.z, material.diffuse.w,
		material.specular.x, material.specular.y, material.specular.z, material.specular.w,
		material.power);
}

void MeshIO::Read(FILE * file, SkinnedMesh & mesh)
{
	uint32_t numVertices = 0;
	fscanf_s(file, "VertexCount: %d\n", &numVertices);
	mesh.vertices.resize(numVertices);
	for (auto& v : mesh.vertices)
	{
		fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
			&v.position.x, &v.position.y, &v.position.z,
			&v.normal.x, &v.normal.y, &v.normal.z,
			&v.tangent.x, &v.tangent.y, &v.tangent.z,
			&v.texCoord.x, &v.texCoord.y,
			&v.boneIndices[0], &v.boneIndices[1], &v.boneIndices[2], &v.boneIndices[3],
			&v.boneWeights[0], &v.boneWeights[1], &v.boneWeights[2], &v.boneWeights[3]);
	}

	uint32_t numIndices = 0;
	fscanf_s(file, "IndexCount: %d\n", &numIndices);
	mesh.indices.resize(numIndices);

	for (uint32_t i = 0; i < numIndices; i += 3)
	{
		uint32_t a, b, c;
		fscanf_s(file, "%i %i %i\n", &a, &b, &c);
		mesh.indices[i + 0] = a;
		mesh.indices[i + 1] = b;
		mesh.indices[i + 2] = c;
	}
}

void MeshIO::Read(FILE * file, Material & material)
{
	
	fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		&material.ambient.x, &material.ambient.y, &material.ambient.z, &material.ambient.w,
		&material.diffuse.x, &material.diffuse.y, &material.diffuse.z, &material.diffuse.w,
		&material.specular.x, &material.specular.y, &material.specular.z, &material.specular.w,
		&material.power);
}
