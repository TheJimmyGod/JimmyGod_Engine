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
		static MeshPX CreateSpherePX(float radius, int rings = 12, int slices = 36, bool isSpace = false);

		static MeshPN CreateCylinderPN(float height, float width);
		static MeshPN CreateSpherePN(float radius, int rings = 12, int slices = 36, bool isSpace = false);

		static Mesh CreateSphere(float radius, int rings = 12, int slices = 36, bool isSpace = false);

		static MeshPX CreateNDCQuad();
	};
}