#pragma once
#include "Common.h"
#include "Constants.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Plane.h"
#include "LineSegment.h"
#include "AABB.h"
#include "Circle.h"
#include "Rect.h"
#include "Ray.h"
#include "Sphere.h"
#include "OBB.h"

#include "Timer.h"
#include "MetaRegistration.h"

namespace JimmyGod::Math
{
	namespace
	{
		std::random_device sRandomDevice{};
		std::mt19937 sRandomEngine{ sRandomDevice() };
	}

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

	inline float Sin(float angle)
	{
		return sinf(angle);
	}

	inline float Cos(float angle)
	{
		return cosf(angle);
	}

	constexpr float Sqr(float value)
	{
		return value * value;
	}

	inline float Sqrt(float value)
	{
		return sqrtf(value);
	}

	constexpr float Dot(const Vector3& a, const Vector3& b)
	{
		return (a.x*b.x) + (a.y*+b.y) + (a.z*+b.z);
	}

	constexpr float Dot(const Vector2& a, const Vector2& b) 
	{ 
		return (a.x * b.x) + (a.y * b.y); 
	}

	constexpr float MagnitudeSqr(const Vector2& v)
	{
		return (v.x * v.x) + (v.y * v.y);
	}
	constexpr float MagnitudeSqr(const Vector3& v)
	{
		return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	}

	inline float DistanceSqr(const Vector2& a, const Vector2& b)
	{
		return MagnitudeSqr(a - b);
	}
	inline float DistanceSqr(const Vector3& a, const Vector3& b)
	{
		return MagnitudeSqr(a - b);
	}
	inline float Distance(const Vector2& a, const Vector2& b)
	{
		return Sqrt(MagnitudeSqr(a-b));
	}
	inline float Distance(const Vector3& a, const Vector3& b)
	{
		return Sqrt(DistanceSqr(a, b));
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

	constexpr Vector2 TransformNormal(const Vector2& v, const Matrix3& m)
	{
		return Vector2
		(
			v.x * m._11 + v.y * m._21,
			v.x * m._12 + v.y * m._22
		);
	}

	constexpr Vector2 TransformCoord(const Vector2& v, const Matrix3& m)
	{
		return Vector2
		(
			v.x * m._11 + v.y * m._21 + m._31,
			v.x * m._12 + v.y * m._22 + m._32
		);
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

	constexpr float MagnitudeXZSqr(const Vector3& v) 
	{ 
		return (v.x * v.x) + (v.z * v.z); 
	}

	inline float MagnitudeXZ(const Vector3& v) 
	{ 
		return Sqrt(MagnitudeXZSqr(v)); 
	}

	inline bool Compare(float a, float b, float epsilon = FLT_MIN) 
	{ 
		return Abs(a - b) <= epsilon; 
	}

	inline bool IsZero(float value) 
	{ 
		return Compare(value, 0.0f); 
	}
	inline bool IsZero(const Vector2& v) 
	{ 
		return IsZero(v.x) && IsZero(v.y); 
	}
	inline bool IsZero(const Vector3& v) 
	{ 
		return IsZero(v.x) && IsZero(v.y) && IsZero(v.z); 
	}

	inline Vector2 PerpendicularLH(const Vector2& v) 
	{ 
		return Vector2(-v.y, v.x); 
	}
	inline Vector2 PerpendicularRH(const Vector2& v) 
	{ 
		return Vector2(v.y, -v.x); 
	}

	inline Vector2 Rotate(const Vector2& v, float rad)
	{
		const float kCosAngle = cos(rad);
		const float kSinAngle = sin(rad);
		return Vector2
		(
			v.x * kCosAngle - v.y * kSinAngle,
			v.y * kCosAngle + v.x * kSinAngle
		);
	}

	inline Vector2 Project(const Vector2& v, const Vector2& n) 
	{ 
		return n * (Dot(v, n) / Dot(n, n)); 
	}
	inline Vector3 Project(const Vector3& v, const Vector3& n) 
	{
		return n * (Dot(v, n) / Dot(n, n)); 
	}
	inline Vector2 Reflect(const Vector2& v, const Vector2& normal) 
	{ 
		return v - (normal * Dot(v, normal) * 2.0f); 
	}
	inline Vector3 Reflect(const Vector3& v, const Vector3& normal) 
	{ 
		return v - (normal * Dot(v, normal) * 2.0f); 
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

	inline float Magnitude(const Vector2& v)
	{
		return sqrtf(MagnitudeSqr(v));
	}

	inline float Magnitude(const Vector3& v)
	{
		return sqrtf(MagnitudeSqr(v));
	}

	inline Vector2 Normalize(const Vector2& v) 
	{ 
		float magnitude = Magnitude(v);
		if (magnitude == 0)
			return Vector2();
		return v / magnitude; 
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

	inline Matrix3 Adjoint(const Matrix3& m)
	{
		return Matrix3
		(
			(m._22 * m._33 - m._23 * m._32),
			-(m._12 * m._33 - m._13 * m._32),
			(m._12 * m._23 - m._13 * m._22),

			-(m._21 * m._33 - m._23 * m._31),
			(m._11 * m._33 - m._13 * m._31),
			-(m._11 * m._23 - m._13 * m._21),

			(m._21 * m._32 - m._22 * m._31),
			-(m._11 * m._32 - m._12 * m._31),
			(m._11 * m._22 - m._12 * m._21)
		);
	}

	inline float Determinant(const Matrix3& m)
	{
		float det = 0.0f;
		det = (m._11 * (m._22 * m._33 - m._23 * m._32));
		det -= (m._12 * (m._21 * m._33 - m._23 * m._31));
		det += (m._13 * (m._21 * m._32 - m._22 * m._31));
		return det;
	}

	inline bool PointInCircle(const Vector2& point, const Circle& circle)
	{
		const Vector2 centerToPoint = point - circle.center;
		const float distSqr = Dot(centerToPoint, centerToPoint);
		const float radiusSqr = circle.radius * circle.radius;
		return distSqr < radiusSqr;
	}

	inline bool IsEmpty(const Rect& rect) 
	{ 
		return rect.right <= rect.left || rect.bottom <= rect.top; 
	}

	inline float RandomNormal(float mean, float stdev)
	{
		return std::normal_distribution<float>{mean, stdev}(sRandomEngine);
	}

	inline float RandomFloat(float min =0.0f, float max=1.0f)
	{
		return std::uniform_real_distribution<float>{min, max}(sRandomEngine);
	}

	inline int RandomInt(int min = 0, int max = std::numeric_limits<int>::max())
	{
		return std::uniform_int_distribution<>{ min, max }(sRandomEngine);
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

	bool IsContained(const Vector3& point, const AABB& aabb);
	bool IsContained(const Vector3& point, const OBB& obb);
	bool GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal);
	bool Intersect(const Ray& ray, const Plane& plane, float& distance);
	bool Intersect(const LineSegment& a, const LineSegment& b);
	bool Intersect(const Circle& c, const LineSegment& l, Vector2* closestPoint = nullptr);
	bool Intersect(const LineSegment& l, const Circle& c);

	//Linear Interpolations
	constexpr Quaternion Lerp(const Quaternion& from, const Quaternion& to, float time)
	{
		return (from * (1.0f - time) + (to * time));
	}

	//Spherical Linear Interpolations
	inline Quaternion Slerp(const Quaternion& from, const Quaternion& to, float time)
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
#pragma endregion
}