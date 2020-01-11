#pragma once
#include "Mesh.h"
namespace JimmyGod::Graphics
{
	class MeshBuilder
	{
	public:
		static MeshPX CreateCubePX();
		static MeshPX CreatePlanePX(float height, float width);
		static MeshPX CreateCylinderPX(float height, float width);
		static MeshPX CreateSpherePX(float radius, int rings, int slices, bool isSpace);
	};
}