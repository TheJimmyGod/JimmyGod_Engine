#pragma once

namespace JimmyGod::Math
{
	struct Sphere
	{
		Vector3 center = Vector3::Zero;
		float radius = 1.0f;

		Sphere() : center(0.0f, 0.0f, 0.0f), radius(1.0f) {}
		Sphere(const Vector3& center, float rad) : center(center), radius(rad) {}

	};
}