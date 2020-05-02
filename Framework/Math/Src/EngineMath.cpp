#include "Precompiled.h"
#include "EngineMath.h"

using namespace JimmyGod::Math;

const Vector2 Vector2::Zero{ 0.0f };
const Vector2 Vector2::One{ 1.0f };
const Vector2 Vector2::XAxis{ Vector2{ 1.0f,0.0f} };
const Vector2 Vector2::YAxis{ Vector2{ 0.0f,1.0f} };

const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f };
const Vector3 Vector3::XAxis{ Vector3{ 1.0f,0.0f,0.0f } };
const Vector3 Vector3::YAxis{ Vector3{ 0.0f,1.0f,0.0f } };
const Vector3 Vector3::ZAxis{ Vector3{ 0.0f,0.0f,1.0f } };

const Vector4 Vector4::Zero{ 0.0f };
const Vector4 Vector4::One{ 1.0f };
const Vector4 Vector4::XAxis{ Vector4{ 1.0f,0.0f,0.0f,0.0f } };
const Vector4 Vector4::YAxis{ Vector4{ 0.0f,1.0f,0.0f,0.0f } };
const Vector4 Vector4::ZAxis{ Vector4{ 0.0f,0.0f,1.0f,0.0f } };

const Matrix4 Matrix4::Identity
{
	Matrix4{1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f}
};

Matrix4 Matrix4::RotationAxis(const Vector3& axis, float radian)
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

Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	return
	{
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.w * q.z),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.w * q.x),
		0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.w * q.y),
		(2.0f * q.y * q.z) - (2.0f * q.w * q.x),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	};
}

const Quaternion Quaternion::Zero{ 0.0f,0.0f,0.0f,0.0f };
const Quaternion Quaternion::Identity{ 0.0f,0.0f,0.0f,1.0f };

Quaternion Quaternion::RotationAxis(const Vector3& v, float radian)
{
	const float halfAngle = radian * 0.5f;
	const float s = sinf(halfAngle);
	const Vector3 normalize = Normalize(v);
	return { normalize.x * s, normalize.y * s, normalize.z * s, cosf(halfAngle) };
}
Quaternion Quaternion::RotationMatrix(const Matrix4& m)
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
		s = 0.5f / s;
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
Quaternion Quaternion::RotationLook(const Vector3& v)
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
Quaternion Quaternion::RotationFromTo(Vector3 from, Vector3 to)
{
	Vector3 up = Normalize(from);
	Vector3 forward = Normalize(to);

	float cosTheta = Dot(up, forward);
	Vector3 axis;
	if (cosTheta < -1.0f + 0.001f)
	{
		axis = Cross(Vector3{ 0.0f,0.0f,1.0f }, up);
		axis = Normalize(axis);
		return Quaternion::RotationAxis(axis, 180.0f);
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