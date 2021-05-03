#pragma once

#include "Colors.h"

namespace JimmyGod::Graphics { class Camera; }

namespace JimmyGod::Graphics::SimpleDraw
{
	void StaticInitialize(uint32_t maxVertexCount = 1024 * 1024);
	void StaticTerminate();

	void AddAABB(const Math::Vector3& center, float radius, const Color& color);
	void AddAABB(const Math::AABB& aabb, const Color& color);
	void AddOBB(const Math::OBB& obb, const Color& color);
	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	void AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color);
	void AddDisplacement(float Length);
	void AddBox(float Length, const Color& color);
	void AddSphere(const Math::Vector3& sphere, float radius, const Color & color, uint32_t rings, uint32_t slices);
	void AddTransform(const Math::Matrix4& transform);
	void AddBone(const Math::Matrix4& transform);
	void AddGroundPlane(float size, const Color& color);
	void AddGroundPlane(float size, float cellSize, const Color& color);
	void AddScreenCircle(const Math::Circle& circle, const Color& color);
	void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color);
	void Render(const Camera& camera);
} // namespace JimmyGod::Graphics::SimpleDraw