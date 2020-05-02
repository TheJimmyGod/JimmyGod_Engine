#include "Precompiled.h"
#include "MeshIO.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

void MeshIO::Write(FILE * file, const Mesh & mesh)
{
	const uint32_t size = mesh.vertices.size();
	fprintf_s(file, "%d vertices\n", &size);


	if ((mesh.vertices[0].position.x ||
		mesh.vertices[0].position.y ||
		mesh.vertices[0].position.z) != 0.0f)
	{
		for (uint32_t i = 0; i < size; i++)
		{
			fprintf_s(file, "v %f %f %f\n", mesh.vertices[i].position.x, mesh.vertices[i].position.y, mesh.vertices[i].position.z);
		}
	}

	if ((mesh.vertices[0].normal.x ||
		mesh.vertices[0].normal.y ||
		mesh.vertices[0].normal.z) != 0.0f)
	{
		for (uint32_t i = 0; i < size; i++)
		{
			fprintf_s(file, "vn %f %f %f\n", mesh.vertices[i].normal.x, mesh.vertices[i].normal.y, mesh.vertices[i].normal.z);
		}
	}

	if ((mesh.vertices[0].tangent.x ||
		mesh.vertices[0].tangent.y ||
		mesh.vertices[0].tangent.z) != 0.0f)
	{
		for (uint32_t i = 0; i < size; i++)
		{
			fprintf_s(file, "t %f %f %f\n", mesh.vertices[i].tangent.x, mesh.vertices[i].tangent.y, mesh.vertices[i].tangent.z);
		}
	}

	if ((mesh.vertices[0].texcoord.x ||
		mesh.vertices[0].texcoord.y ) != 0.0f)
	{
		for (uint32_t i = 0; i < size; i++)
		{
			fprintf_s(file, "vt %f %f %f\n", mesh.vertices[i].texcoord.x, mesh.vertices[i].texcoord.y, nullptr);
		}
	}

	fprintf_s(file, "%d indices\n", mesh.indices.size());
	for (uint32_t i = 0; i + 2 < mesh.indices.size(); ++i)
	{
		fprintf_s(file, "f %d/%d/%d\n", mesh.indices[i], mesh.indices[i + 1], mesh.indices[i + 2]);
	}
}

void MeshIO::Read(FILE * file, Mesh & mesh)
{
	Vector3 vertex;
	Vector3 normal;
	Vector2 coord;
	uint32_t numVertices = 0;
	uint32_t numNormal = 0;
	uint32_t numCoord = 0;

	fscanf_s(file, "%d vertices\n", &numVertices);
	fscanf_s(file, "%d vertex normals\n", &numNormal);
	fscanf_s(file, "%d texture coords\n", &numCoord);

	mesh.vertices.resize(numVertices);

	for (uint32_t i = 0; i < numVertices; i++)
	{
		fscanf_s(file, "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
		mesh.vertices[i].position = vertex;
	}

	for (uint32_t i = 0; i < numNormal; i++)
	{
		fscanf_s(file, "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
		mesh.vertices[i].normal = normal;
	}

	for (uint32_t i = 0; i < numCoord; i++)
	{
		fscanf_s(file, "vt %f %f %f\n", &coord.x, &coord.y);
		mesh.vertices[i].texcoord = coord;
	}

	for (uint32_t i = 0; i + 2 < mesh.indices.size(); i += 3)
	{
		uint32_t a, b, c;
		fscanf_s(file, "f %d/%d/%d\n", &a, &b, &c);
		mesh.indices[i] = a;
		mesh.indices[i + 1] = b;
		mesh.indices[i + 2] = c;
	}


	fclose(file);
}
