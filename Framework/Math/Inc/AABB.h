#pragma once
namespace JimmyGod::Math
{
	struct AABB
	{
		Vector3 center;
		Vector3 extend;

		AABB() : center(0.0f, 0.0f, 0.0f), extend(0.0f, 0.0f, 0.0f) {}
		AABB(const Vector3& center, const Vector3& extend) : center(center), extend(extend) {}
		AABB(const Vector3& center, float radius) : center(center), extend(Vector3(radius, radius, radius)) {}

		constexpr Vector3 Min() const { return center - extend; }
		constexpr Vector3 Max() const { return center + extend; }
	};
}
