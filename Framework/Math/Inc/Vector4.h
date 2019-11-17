#pragma once
namespace JimmyGod::Math 
{
	struct Vector4
	{
		float x, y, z, w;
		constexpr Vector4() noexcept : Vector4{ 0.0f } {}
		constexpr Vector4(float f) noexcept : Vector4{ f,f,f,f } {}
		constexpr Vector4(float x, float y, float z, float w) noexcept : x{ x }, y{ y }, z{ z }, w{w}{}
		// Declaration
		const static Vector4 Zero;
		const static Vector4 One;
		const static Vector4 XAxis;
		const static Vector4 YAxis;
		const static Vector4 ZAxis;

		constexpr Vector4 operator-() const { return{ -x,-y,-z,-w }; }
		constexpr Vector4 operator+(const Vector4& v) const { return { x + v.x, y + v.y,z + v.z, w +v.w }; }
		constexpr Vector4 operator-(const Vector4& v) const { return { x - v.x, y - v.y,z - v.z, w -v.w }; }
		constexpr Vector4 operator*(float s) const { return { x * s, y * s,z * s, w*s }; }
		constexpr Vector4 operator/(float s) const { if (s == 0.0f) { return Zero; } return { x / s, y / s,z / s,w/s }; }

		Vector4& operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; }
		Vector4& operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
		Vector4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; }
		Vector4& operator/=(float s) { if (s == 0.0f) { x /= 1; y /= 1; z /= 1; } else { x /= s; y /= s; z /= s; w /= s; } }
	};
}