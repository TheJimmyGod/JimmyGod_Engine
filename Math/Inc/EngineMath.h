#pragma once

#include "Common.h"

#include "Vector3.h"
#include "Matrix4.h"

namespace JimmyGod::Math
{
	constexpr float Dot(const Vector3& a, const Vector3& b)
	{
		return (a.x*b.x) + (a.y*+b.y) + (a.z*+b.z);
	}

	constexpr Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3
		(
			((a.y*b.z) - (a.z*b.y)),
			((a.z*b.x) - (a.x*b.z)),
			((a.x*b.y) - (a.y*b.x))
		);
	}

	constexpr Matrix4 Transpose(const Matrix4& m)
	{
		Matrix4 mResult = m;

		mResult._12 = m._21;
		mResult._13 = m._31;
		mResult._14 = m._41;
		mResult._21 = m._12;
		mResult._23 = m._32;
		mResult._24 = m._42;
		mResult._31 = m._13;
		mResult._32 = m._23;
		mResult._34 = m._43;
		mResult._41= m._14;
		mResult._42 = m._24;
		mResult._43 = m._34;
		

		return mResult;
	}

	constexpr Matrix4 Determinant(const Matrix4& m)
	{
		return {};
	}

	constexpr Matrix4 Inverse(const Matrix4& m)
	{
		Matrix4 mResult = m;
		float factor1 = mResult._22 * mResult._33 - mResult._23 * mResult._32;
		float factor2 = mResult._13 * mResult._32 - mResult._12 * mResult._33;
		float factor3 = mResult._12 * mResult._23 - mResult._13 * mResult._22;
		float det = mResult._11 * factor1 + mResult._21*factor2 + mResult._31*factor3;
		if (det == 0.0f)
		{
			return {};
		}

		float inverseDet = 1.0f / det;

		mResult._11 = inverseDet * factor1;
		mResult._12 = inverseDet * factor2;
		mResult._13 = inverseDet * factor3;

		return {};
	}

	constexpr Matrix4 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		Matrix4 mResult = m;

		return {};
	}

	constexpr Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		Matrix4 mResult = m;
		Vector3 mVecResult;

		mVecResult.x = m._11*v.x + m._21*v.y + m._31*v.z + m._41;
		mVecResult.y = m._12*v.x + m._22*v.y + m._32*v.z + m._42;
		mVecResult.z = m._13*v.x + m._23*v.y + m._33*v.z + m._43;

		return mVecResult;
	}

	constexpr float MagnitudeSqr(const Vector3& v)
	{
		return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	}

	inline float Magnitude(const Vector3& v)
	{
		return sqrt(MagnitudeSqr(v));
	}

	inline Vector3 Normalize(const Vector3& v)
	{
		float magnitude = Magnitude(v);
		if (magnitude == 0)
		{
			return Vector3();
		}
		return Vector3(v.x / magnitude, v.y / magnitude, v.z / magnitude);
	}

}