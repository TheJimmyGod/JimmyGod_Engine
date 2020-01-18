#pragma once

#include "Colors.h"

namespace JimmyGod::Graphics { class Camera; }

namespace JimmyGod::Graphics::SimpleDraw
{
	void StaticInitialize(uint32_t maxVertexCount = 10000);
	void StaticTerminate();

	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	void AddBox(float Length, const Color& color);
	void AddSphere(float radius, int rings = 12, int slices = 36);
	void Render(const Camera& camera);
} // namespace JimmyGod::Graphics::SimpleDraw