#pragma once
#include "Common.h"
#include "Constants.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

namespace JimmyGod::Math
{
	template<class T>
	constexpr T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template<class T>
	constexpr T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template<class T>
	constexpr T Clamp(T value, T min, T max)
	{
		return Max(min, Min(max, value));
	}

	template<class T>
	constexpr T Lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}

	constexpr float Abs(float value)
	{
		return value >= 0.0f ? value : -value;
	}

	constexpr float Spr(float value)
	{
		return value * value;
	}

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
		const float w = (m._14*v.x) + (m._24*v.y) + (m._34*v.z) + (1.0f * m._44);
		mVecResult.x = ((m._11*v.x) + (m._21*v.y) + (m._31*v.z) + (1.0f * m._41)) / w;
		mVecResult.y = ((m._12*v.x) + (m._22*v.y) + (m._32*v.z) + (1.0f * m._42)) / w;
		mVecResult.z = ((m._13*v.x) + (m._23*v.y) + (m._33*v.z) + (1.0f * m._43)) / w;

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

	inline Vector3 GetRight(const Matrix4& m)
	{
		return Vector3{ m._11, m._12, m._13 };
	}

	inline Vector3 GetForward(const Matrix4& m)
	{
		return Vector3{ m._31, m._32, m._33 };
	}

	inline Vector3 GetUp(const Matrix4& m)
	{
		return Vector3{ m._21,m._22,m._23 };
	}

	inline Vector3 GetTranslation(const Matrix4& m)
	{
		return Vector3{ m._41, m._42, m._43 };
	}

	inline Vector3 GetLook(const Matrix4& m)
	{
		return Vector3{ m._14,m._24, m._34 };
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
#pragma region Quaternion Helper Functions

	constexpr float Dot(const Quaternion& from, const Quaternion& to)
	{
		return (from.x * to.x) + (from.y * to.y) + (from.z * to.z) + (from.w * to.w);
	}

	constexpr float MagnitudeSqr(const Quaternion& q)
	{
		return (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
	}

	inline float Magnitude(const Quaternion& q)
	{
		return sqrtf(MagnitudeSqr(q));
	}

	inline Quaternion Normalize(const Quaternion& q)
	{
		const float length = Magnitude(q);
		if (length == 0)
			return Quaternion();
		if (length == 1.0f)
		{
			return q;
		}
		const float invNorm = 1.0f / length;
		return { q * invNorm };
	}

	//Linear Interpolations
	constexpr Quaternion Lerp(const Quaternion& from, const Quaternion& to, float time)
	{
		return (from * (1.0f - time) + (to * time));
	}

	//Spherical Linear Interpolations
	inline Quaternion Slerp(Quaternion& from, Quaternion& to, float time)
	{
		const float Threshold = 0.9995f;
		float n1;
		float n2;
		//Angle between quaternions
		float angle = Dot(from, to);
		bool negative = false;

		// If the dot product is negative, slerp won't take
		// the shorter path. Note that q0 and q1 are equivalent when
		// the negation is applied to all four components. 
		//Fix by negating one of them so *-1 if their dot product is < 0
		if (angle < 0.0f)
		{
			negative = true;
			angle = -angle;
		}
		if (angle > Threshold)
		{
			n2 = 1 - time;
			n1 = negative ? -time : time;
		}
		else
		{
			float thetha = acosf(angle);
			float inverseSinThetha = 1.0f / sinf(thetha);
			n2 = sinf((1.0f - time) * thetha) * inverseSinThetha;
			if (negative)
			{
				n1 = -sinf(time * thetha) * inverseSinThetha;
			}
			else
			{
				n1 = sinf(time * thetha) * inverseSinThetha;
			}
		}
		Quaternion quaternion;
		//Lerps xyz
		quaternion.x = (n2 * from.x) + (n1 * to.x);
		quaternion.y = (n2 * from.y) + (n1 * to.y);
		quaternion.z = (n2 * from.z) + (n1 * to.z);
		quaternion.w = (n2 * from.w) + (n1 * to.w);
		return Normalize(quaternion);
	}

	static Matrix4 RotationAxis(const Vector3& axis, float radian)
	{
		const Vector3 u = Normalize(axis);
		const float x = u.x;
		const float y = u.y;
		const float z = u.z;
		const float s = sinf(radian);
		const float c = cosf(radian);

		return {
			c + (x * x * (1.0f - c)),
			x * y * (1.0f - c) + z * s,
			x * z * (1.0f - c) - y * s,
			0.0f,

			x * y * (1.0f - c) - z * s,
			c + (y * y * (1.0f - c)),
			y * z * (1.0f - c) + x * s,
			0.0f,

			x * z * (1.0f - c) + y * s,
			y * z * (1.0f - c) - x * s,
			c + (z * z * (1.0f - c)),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		};
	}

	inline Matrix4 RotationQuaternion(const Quaternion& q)
	{
		Matrix4 mat = Matrix4::Identity;

		mat._11 = 1.0f - (2.0f *(q.y * q.y)) - (2.0f * (q.z * q.z));
		mat._12 = (2.0f * q.x * q.y) - (2.0f * q.w * q.z);
		mat._13 = (2.0f * q.x * q.z) + (2.0f * q.w * q.y);

		mat._21 = (2.0f * q.x * q.y) + (2.0f * q.w * q.z);
		mat._22 = 1.0f - (2.0f * (q.x * q.x)) - (2.0f * (q.z * q.z));
		mat._23 = (2.0f * q.y * q.z) - (2.0f * q.w * q.x);

		mat._31 = (2.0f * q.x * q.z) - (2.0f * q.w * q.y);
		mat._32 = (2.0f * q.y * q.z) + (2.0f * q.w * q.x);
		mat._33 = 1.0f - (2.0f * (q.x * q.x)) - (2.0f * (q.y * q.y));

		return mat;
	}

	static Quaternion RotationAxisAngle(const Vector3& v, float radian)
	{
		const float halfAngle = radian * 0.5f;
		const float s = sinf(halfAngle);
		const Vector3 normalize = Normalize(v);
		return { normalize.x * s, normalize.y * s, normalize.z * s, cosf(halfAngle) };
	}

	inline Quaternion RotationMatrix(const Matrix4& m)
	{
		Quaternion result;

		float trace = m._11 + m._22 + m._33;
		float s;


		if (trace >= 0.0f)
		{
			s = sqrtf(trace + 1.0f);
			result.w = 0.5f * s;
			s = 0.5f / s;
			result.x = (m._23 - m._32) * s;
			result.y = (m._31 - m._13) * s;
			result.z = (m._12 - m._21) * s;
		}
		else if ((m._11 > m._22) && (m._11 > m._33))
		{
			s = sqrtf(1.0f + m._11 - m._22 - m._33);
			result.x = s * 0.5f;
			s = 0.5f / s;
			result.y = (m._21 + m._12) * s;
			result.z = (m._31 + m._13) * s;
			result.w = (m._32 - m._23) * s;
		}
		else if (m._22 > m._33)
		{
			s = sqrtf(1.0f + m._22 - m._11 - m._33);
			result.y = s * 0.5f;
			s = 0.5 / s;
			result.x = (m._21 + m._12)*s;
			result.z = (m._32 + m._23)*s;
			result.w = (m._13 - m._31)*s;
		}
		else
		{
			s = sqrtf(1.0f + m._33 - m._11 - m._22);
			result.z = s * 0.5f;
			s = 0.5f / s;
			result.x = (m._13 + m._31)*s;
			result.y = (m._32 + m._23)*s;
			result.w = (m._21 - m._12)*s;
		}

		return result;
	}

	inline Quaternion RotationLook(const Vector3& v)
	{
		Vector3 up = Vector3::YAxis;
		Vector3 forward = Normalize(v);
		Vector3 orth = Normalize(Cross(up, forward));
		Vector3 right = Cross(forward, orth);

		float num;
		float average;

		float m11 = orth.x;
		float m12 = orth.y;
		float m13 = orth.z;
		float m21 = right.x;
		float m22 = right.y;
		float m23 = right.z;
		float m31 = forward.x;
		float m32 = forward.y;
		float m33 = forward.z;

		Quaternion result;

		float intencity = (m11 + m22) + m33;

		if (intencity > 0.0f)
		{
			num = sqrtf(intencity + 1.0f);

			result.w = (num) * 0.5f;
			average = 0.5f / num;
			result.x = (m23 - m32) * average;
			result.y = (m31 - m13) * average;
			result.z = (m12 - m21) * average;

			return result;
		}
		if ((m11 >= m22) && (m11 >= m33))
		{
			num = sqrtf(((1.0f + m11) - m22) - m33);
			average = 0.5f / num;
			result.x = 0.5f * num;
			result.y = (m12 + m21) * average;
			result.z = (m13 + m31) * average;
			result.w = (m23 + m32) * average;

			return result;
		}
		if (m22 > m33)
		{
			num = sqrtf(((1.0f + m22) - m11) - m33);
			average = 0.5f / num;
			result.x = (m21 + m12) * average;
			result.y = 0.5f * num;
			result.z = (m32 + m23) * average;
			result.w = (m31 - m13) * average;

			return result;
		}

		num = sqrtf(((1.0f + m33) - m11) - m22);
		average = 0.5f / num;
		result.x = (m31 + m13) * average;
		result.y = (m32 + m23) * average;
		result.z = 0.5f * num;
		result.w = (m12 - m21) * average;
		return result;
	}

	inline Quaternion RotationFromTo(const Vector3& from, const Vector3& to)
	{
		Vector3 up = Normalize(from);
		Vector3 forward = Normalize(to);

		float cosTheta = Dot(up, forward);
		Vector3 axis;
		if (cosTheta < -1.0f + 0.001f)
		{
			axis = Cross(Vector3{ 0.0f,0.0f,1.0f }, up);
			axis = Normalize(axis);
			return RotationAxisAngle(axis, 180.0f);
		}

		axis = Cross(up, forward);
		float s = sqrtf((1.0f + cosTheta)*2.0f);
		float invs = 1.0f / s;

		return Quaternion(
			axis.x * invs,
			axis.y * invs,
			axis.z * invs,
			s * 0.5f
		);
	}
#pragma endregion
}