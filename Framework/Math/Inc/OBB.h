#pragma once

namespace JimmyGod::Math
{
	struct OBB
	{
		Vector3 center = Vector3::Zero;
		Vector3 extend = Vector3::One;
		Quaternion rot = Quaternion::Identity;

		//OBB() : center(0.0f, 0.0f, 0.0f), extend(0.0f, 0.0f, 0.0f), rot(0.0f,0.0f,0.0f,0.0f) {}
		//OBB(const Vector3& center, const Vector3& extend, const Quaternion& rotation) : center(center), extend(extend), rot(rotation) {}
		//OBB(const Vector3& center, float radius, const Quaternion& rot) : center(center), extend(Vector3(radius, radius, radius)), rot(rot) {}

	};
}