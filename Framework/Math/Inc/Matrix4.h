#pragma once

namespace JimmyGod::Math 
{
	struct Matrix4
	{
		// row + column subscript
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;

		const static Matrix4 Identity;

		constexpr Matrix4 operator+(const Matrix4& m) const
		{
			return
			{
			_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14,
			_21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
			_31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34,
			_41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44
			};
		}

		constexpr Matrix4 operator-(const Matrix4& m) const
		{
			return
			{
			_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14,
			_21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24,
			_31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34,
			_41 - m._41, _42 - m._42, _43 - m._43, _44 - m._44
			};
		}

		constexpr Matrix4 operator/(float s) const
		{
			return
			{
			_11 / s, _12 / s, _13 / s, _14 / s,
			_21 / s, _22 / s, _23 / s, _24 / s,
			_31 / s, _32 / s, _33 / s, _34 / s,
			_41 / s, _42 / s, _43 / s, _44 / s
			};
		}

		constexpr Matrix4 operator*(const Matrix4& m) const
		{
			return 
			{ 
			(_11 * m._11) + (_12 * m._21) + (_13 * m._31) + (_14 * m._41),
			(_11 * m._12) + (_12 * m._22) + (_13 * m._32) + (_14 * m._42),
			(_11 * m._13) + (_12 * m._23) + (_13 * m._33) + (_14 * m._43),
			(_11 * m._14) + (_12 * m._24) + (_13 * m._34) + (_14 * m._44),

			(_21 * m._11) + (_22 * m._21) + (_23 * m._31) + (_24 * m._41),
			(_21 * m._12) + (_22 * m._22) + (_23 * m._32) + (_24 * m._42),
			(_21 * m._13) + (_22 * m._23) + (_23 * m._33) + (_24 * m._43),
			(_21 * m._14) + (_22 * m._24) + (_23 * m._34) + (_24 * m._44),

			(_31 * m._11) + (_32 * m._21) + (_33 * m._31) + (_34 * m._41),
			(_31 * m._12) + (_32 * m._22) + (_33 * m._32) + (_34 * m._42),
			(_31 * m._13) + (_32 * m._23) + (_33 * m._33) + (_34 * m._43),
			(_31 * m._14) + (_32 * m._24) + (_33 * m._34) + (_34 * m._44),

			(_41 * m._11) + (_42 * m._21) + (_43 * m._31) + (_44 * m._41),
			(_41 * m._12) + (_42 * m._22) + (_43 * m._32) + (_44 * m._42),
			(_41 * m._13) + (_42 * m._23) + (_43 * m._33) + (_44 * m._43),
			(_41 * m._14) + (_42 * m._24) + (_43 * m._34) + (_44 * m._44)
			};
		}

		constexpr Matrix4 operator*(float s) const
		{
			return
			{
			_11 * s, _12 * s, _13 * s, _14 * s,
			_21 * s, _22 * s, _23 * s, _24 * s,
			_31 * s, _32 * s, _33 * s, _34 * s,
			_41 * s, _42 * s, _43 * s, _44 * s
			};
		}

		static Matrix4 Translation(const Vector3& d)
		{
			Matrix4 mResult;
			mResult._11 = 1.0f; mResult._12 = 0.0f; mResult._13 = 0.0f; mResult._14 = 0.0f;
			mResult._21 = 0.0f; mResult._22 = 1.0f; mResult._23 = 0.0f; mResult._24 = 0.0f;
			mResult._31 = 0.0f; mResult._32 = 0.0f; mResult._33 = 1.0f; mResult._34 = 0.0f;
			mResult._41 = d.x; mResult._42 = d.y; mResult._43 = d.z; mResult._44 = 1.0f;
			return mResult;
		}
		static Matrix4 RotationX(float radian)
		{
			Matrix4 mResult;
			mResult._11 = 1.0f; mResult._12 = 0.0f; mResult._13 = 0.0f; mResult._14 = 0.0f;
			mResult._21 = 0.0f; mResult._22 = cosf(radian); mResult._23 = sinf(radian); mResult._24 = 0.0f;
			mResult._31 = 0.0f; mResult._32 = -sinf(radian); mResult._33 = cosf(radian); mResult._34 = 0.0f;
			mResult._41 = 0.0f; mResult._42 = 0.0f; mResult._43 = 0.0f; mResult._44 = 1.0f;
			return mResult;
		}
		static Matrix4 RotationY(float radian)
		{
			Matrix4 mResult;
			mResult._11 = cosf(radian); mResult._12 = 0.0f; mResult._13 = -sinf(radian); mResult._14 = 0.0f;
			mResult._21 = 0.0f; mResult._22 = 1.0f; mResult._23 = 0.0f; mResult._24 = 0.0f;
			mResult._31 = sinf(radian); mResult._32 = 0.0f; mResult._33 = cosf(radian); mResult._34 = 0.0f;
			mResult._41 = 0.0f; mResult._42 = 0.0f; mResult._43 = 0.0f; mResult._44 = 1.0f;
			return mResult;
		}
		static Matrix4 RotationZ(float radian)
		{
			Matrix4 mResult;
			mResult._11 = cosf(radian); mResult._12 = sinf(radian); mResult._13 = 0.0f; mResult._14 = 0.0f;
			mResult._21 = -sinf(radian); mResult._22 = cosf(radian); mResult._23 = 0.0f; mResult._24 = 0.0f;
			mResult._31 = 0.0f; mResult._32 = 0.0f; mResult._33 = 1.0f; mResult._34 = 0.0f;
			mResult._41 = 0.0f; mResult._42 = 0.0f; mResult._43 = 0.0f; mResult._44 = 1.0f;
			return mResult;
		}
		static Matrix4 RotationAxis(const Vector3& axis, float radian)
		{
			Matrix4 mResult;
			const float cos = cosf(radian);
			const float sin = sinf(radian);
			mResult._11 = (axis.x*(axis.x*(1 - cos) + cos));
			mResult._12 = (axis.y*(axis.x*(1 - cos) - axis.z * sin));
			mResult._13 = (axis.z*(axis.x*(1 - cos) + axis.y * sin));
			mResult._14 = 0.0f;
			mResult._21 = (axis.x*(axis.y*(1 - cos) + axis.z * sin));
			mResult._22 = (axis.y*(axis.y*(1 - cos) + cos));
			mResult._23 = (axis.z*(axis.y*(1 - cos) - axis.x * sin));
			mResult._24 = 0.0f;
			mResult._31 = (axis.x*(axis.z*(1 - cos) - axis.y * sin));
			mResult._32 = (axis.y*(axis.z*(1 - cos) + axis.x * sin));
			mResult._33 = (axis.z*(axis.z*(1 - cos) + cos));
			mResult._34 = 0.0f;
			mResult._41 = 0.0f; mResult._42 = 0.0f; mResult._43 = 0.0f; mResult._44 = 1.0f;

			return mResult;
		}
		static Matrix4 Scaling(float scale)
		{
			Matrix4 mResult;
			mResult._11 = scale; mResult._12 = 0.0f; mResult._13 = 0.0f; mResult._14 = 0.0f;
			mResult._21 = 0.0f; mResult._22 = scale; mResult._23 = 0.0f; mResult._24 = 0.0f;
			mResult._31 = 0.0f; mResult._32 = 0.0f; mResult._33 = scale; mResult._34 = 0.0f;
			mResult._41 = 0.0f; mResult._42 = 0.0f; mResult._43 = 0.0f; mResult._44 = 1.0f;
			return mResult;
		}
	};
}