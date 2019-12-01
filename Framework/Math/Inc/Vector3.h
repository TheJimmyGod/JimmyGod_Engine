#pragma once

namespace JimmyGod::Math 
{
	struct Vector3
	{
		float x, y, z;

		constexpr Vector3() noexcept : Vector3{ 0.0f } {}
		constexpr Vector3(float f) noexcept : Vector3{ f,f,f } {}
		constexpr Vector3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z }{}
		// Declaration
		const static Vector3 Zero;
		const static Vector3 One;
		const static Vector3 XAxis;
		const static Vector3 YAxis;
		const static Vector3 ZAxis;

		constexpr Vector3 operator-() const { return{ -x,-y,-z }; }
		constexpr Vector3 operator+(const Vector3& v) const { return { x + v.x, y + v.y,z + v.z }; }
		constexpr Vector3 operator-(const Vector3& v) const { return { x - v.x, y - v.y,z - v.z }; }
		constexpr Vector3 operator*(float s) const { return { x * s, y * s,z * s }; }
		constexpr Vector3 operator/(float s) const { if (s == 0.0f) { return Zero; } return { x / s, y / s,z / s }; }

		Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this;}
		Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this;}
		Vector3& operator*=(float s) { x *= s; y *= s; z *= s;  return *this;}
		Vector3& operator/=(float s) { if (s == 0.0f) { x /= 1; y /= 1; z /= 1; } else { x /= s; y /= s; z /= s; } return *this;}
	};
}


