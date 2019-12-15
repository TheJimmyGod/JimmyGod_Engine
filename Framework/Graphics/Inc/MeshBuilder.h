#pragma once
#include "Mesh.h"
namespace JimmyGod::Graphics
{
	class MeshBuilder
	{
		static MeshPX CreatePlanePX(int height, int width);
		static MeshPX CreateCylinderPX(int height);
		static MeshPX CreateSpherePX(float radius, int ringhs = 16, int slices = 16);
	};
}