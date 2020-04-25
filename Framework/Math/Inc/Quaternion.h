#pragma once

#include "Vector3.h"

namespace JimmyGod::Math
{
	struct Quaternion
	{
		float x, y, z, w;

		const static Quaternion Identity;
		const static Quaternion Zero;

		constexpr Quaternion() noexcept : Quaternion{ 0.0f } {}
		constexpr Quaternion(float f) noexcept : Quaternion{ f,f,f,f } {}
		constexpr Quaternion(float x, float y, float z, float w) noexcept : x{ x }, y{ y }, z{ z }, w{ w }{}

		constexpr Quaternion operator-() const { return{ -x,-y,-z,-w }; }
		constexpr Quaternion operator+ (Quaternion& q) const
		{
			return
			{
				x + q.x, y + q.y, z + q.z, w + q.w
			};
		}

		constexpr Quaternion operator+ (const Quaternion& q) const
		{
			return
			{
				x + q.x, y + q.y, z + q.z, w + q.w
			};
		}

		constexpr Quaternion operator- (Quaternion& q) const
		{
			return
			{
				x - q.x, y - q.y, z - q.z, w - q.w
			};
		}

		constexpr Quaternion operator- (const Quaternion& q) const
		{
			return
			{
				x - q.x, y - q.y, z - q.z, w - q.w
			};
		}

		constexpr Quaternion operator* (float s) const
		{
			return Quaternion(x * s, y * s, z * s, w * s);
		}

		constexpr Quaternion operator* (Quaternion& q) const
		{
			Quaternion result;

			result.w = w * q.w - x * q.x - y * q.y - z * q.z;
			result.x = w * q.x + x * q.w + y * q.z - z * q.y;
			result.y = w * q.y - x * q.z + y * q.w + z * q.x;
			result.z = w * q.z + x * q.y - y * q.x + z * q.w;

			return result;
		}

		constexpr Quaternion operator* (const Quaternion& q) const
		{
			Quaternion result;

			result.w = w * q.w - x * q.x - y * q.y - z * q.z;
			result.x = w * q.x + x * q.w + y * q.z - z * q.y;
			result.y = w * q.y - x * q.z + y * q.w + z * q.x;
			result.z = w * q.z + x * q.y - y * q.x + z * q.w;

			return result;
		}

		constexpr Quaternion operator= (const Quaternion& q) const
		{
			if (this == &q)
			{
				return *this;
			}
			else
			{
				return
				{
					q.x,q.y,q.z,q.w
				};
			}
		}

		constexpr Quaternion operator== (const Quaternion& q) const
		{
			return (x == q.x &&
				y == q.y &&
				z == q.z &&
				w == q.w);
		}

		constexpr Quaternion operator!= (const Quaternion& q) const
		{
			return !(x == q.x &&
				y == q.y &&
				z == q.z &&
				w == q.w);
		}

		constexpr Quaternion operator*= (Quaternion& q) const
		{
			Quaternion result;
			result.w = w * q.w - x * q.x - y * q.y - z * q.z;
			result.x = w * q.x + x * q.w + y * q.z - z * q.y;
			result.y = w * q.y - x * q.z + y * q.w + z * q.x;
			result.z = w * q.z + x * q.y - y * q.x + z * q.w;

			*this = q;
			return *this;
		}

		constexpr Quaternion operator*= (const Quaternion& q) const
		{
			Quaternion result;
			result.w = w * q.w - x * q.x - y * q.y - z * q.z;
			result.x = w * q.x + x * q.w + y * q.z - z * q.y;
			result.y = w * q.y - x * q.z + y * q.w + z * q.x;
			result.z = w * q.z + x * q.y - y * q.x + z * q.w;

			*this = q;
			return *this;
		}

		Quaternion& operator+= (const Quaternion& q)
		{
			x += q.x;
			y += q.y;
			z += q.z;
			w += q.w;
		}

		Quaternion& operator-= (const Quaternion& q)
		{
			x -= q.x;
			y -= q.y;
			z -= q.z;
			w -= q.w;
		}
	};
}