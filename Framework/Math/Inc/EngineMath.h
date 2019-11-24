#pragma once

#include "Common.h"

#include "Vector3.h"
#include "Matrix4.h"

#include "Vector4.h"

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

	constexpr float Determinant(const Matrix4& m)
	{
		float factor1 = 0.0f;
		float factor2 = 0.0f;
		float factor3 = 0.0f;
		float factor4 = 0.0f;
		float det = 0.0f;

		factor1 = m._11*((m._22*((m._33*m._44) - (m._34*m._43))) - ((m._23*((m._32*m._44) - (m._34*m._42)))) + ((m._24*((m._32*m._43) - (m._33*m._42)))));
		factor2 = m._12*((m._21*((m._33*m._44) - (m._34*m._43))) - ((m._23*((m._31*m._44) - (m._34*m._41)))) + ((m._24*((m._31*m._43) - (m._33*m._41)))));
		factor3 = m._13*((m._21*((m._32*m._44) - (m._34*m._42))) - ((m._22*((m._31*m._44) - (m._34*m._41)))) + ((m._24*((m._31*m._42) - (m._32*m._41)))));
		factor4 = m._14*((m._21*((m._32*m._43) - (m._33*m._42))) - ((m._22*((m._31*m._43) - (m._33*m._41)))) + ((m._23*((m._31*m._42) - (m._32*m._41)))));

		det = factor1 - factor2 + factor3 - factor4;

		return det;
	}

	constexpr Matrix4 Inverse(const Matrix4& m)
	{
		float det = Determinant(m);
		float inverseDet = 1.0f / det;
		Matrix4 mResult{
		inverseDet*(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
		inverseDet *-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
		inverseDet*(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
		inverseDet *-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

		inverseDet *-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
		inverseDet*(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
		inverseDet *-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
		inverseDet*(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

		inverseDet*(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
		inverseDet *-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
		inverseDet*(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
		inverseDet *-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

		inverseDet *-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
		inverseDet*(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
		inverseDet *-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
		inverseDet*(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22))) };
		
		return mResult;
	}

	constexpr Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		Vector3 mVecResult;

		mVecResult.x = m._11*v.x + m._21*v.y + m._31*v.z + m._41;
		mVecResult.y = m._12*v.x + m._22*v.y + m._32*v.z + m._42;
		mVecResult.z = m._13*v.x + m._23*v.y + m._33*v.z + m._43;

		return mVecResult;
	}

	constexpr Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		Vector3 mVecResult;

		mVecResult.x = m._11*v.x + m._21*v.y + m._31*v.z;
		mVecResult.y = m._12*v.x + m._22*v.y + m._32*v.z;
		mVecResult.z = m._13*v.x + m._23*v.y + m._33*v.z;

		return mVecResult;
	}

	constexpr float MagnitudeSqr(const Vector3& v)
	{
		return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	}

	inline float Magnitude(const Vector3& v)
	{
		return sqrtf(MagnitudeSqr(v));
	}

	inline Vector3 Normalize(const Vector3& v) // Shunk a vector to make the vector's length 1
	{
		float magnitude = Magnitude(v);
		if (magnitude == 0)
		{
			return Vector3();
		}
		return Vector3(v.x / magnitude, v.y / magnitude, v.z / magnitude);
	}

}