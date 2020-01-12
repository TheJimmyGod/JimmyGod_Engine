#pragma once

#include "Colors.h"

namespace JimmyGod::Graphics { class Camera; }

namespace JimmyGod::Graphics::SimpleDraw
{
	void StaticInitialize(uint32_t maxVertexCount = 10000);
	void StaticTerminate();

	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	//void AddBox();
	//void AddSphere();
	void Render(const Camera& camera);
} // namespace JimmyGod::Graphics::SimpleDraw