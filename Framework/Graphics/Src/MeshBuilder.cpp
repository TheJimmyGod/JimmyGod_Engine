#include "Precompiled.h"
#include "MeshBuilder.h"
#include "VertexTypes.h"
#include "Mesh.h"
using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;
MeshPX MeshBuilder::CreatePlanePX(int height, int width)
{
	MeshPX mMesh;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			mMesh.vertices.emplace_back(Vector3{ x,y,0.0f }, 0.0f, 0.0f);
		}

	}

	return mMesh;
}

MeshPX MeshBuilder::CreateCylinderPX(int height)
{
	MeshPX mMesh;
	for (int y = 0; y < height; ++y)
		for (int theta = 0; theta < TwoPi; theta += ...)
			mMesh.vertices.emplace_back(Vector3{ sin(theta),y,cos(theta) });

	return MeshPX();
}

MeshPX MeshBuilder::CreateSpherePX(float radius, int ringhs, int slices)
{
	MeshPX mMesh;
	return MeshPX();
}
