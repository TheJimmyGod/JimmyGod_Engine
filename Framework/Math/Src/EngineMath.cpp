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

Matrix3 Matrix3::Inverse(const Matrix3& m)
{
	const float determinant = Determinant(m);
	const float invDet = 1.0f / determinant;
	return Adjoint(m) * invDet;
}

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

bool JimmyGod::Math::IsContained(const Vector3 & point, const AABB & aabb)
{
	auto min = aabb.Min();
	auto max = aabb.Max();

	if(point.x < min.x || point.x > max.x ||
		point.y < min.y || point.y > max.y ||
		point.z < min.z || point.z > max.z)
		return false;
	return true;
}

bool JimmyGod::Math::IsContained(const Vector3 & point, const OBB & obb)
{
	// Compute the world to local matrices
	Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
	Math::Matrix4 matRot = Math::Matrix4::RotationQuaternion(obb.rot);
	Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
	Math::Matrix4 toWorld = matScale * matRot * matTrans;
	Math::Matrix4 toLocal = Inverse(toWorld);

	// Transform the point into the OBB's local space
	Vector3 localPoint = TransformCoord(point, toLocal);

	// Test against local AABB
	return IsContained(localPoint, AABB{Vector3::Zero, Vector3::One});
}

bool JimmyGod::Math::GetContactPoint(const Ray & ray, const OBB & obb, Vector3 & point, Vector3 & normal)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center);
	Matrix4 matRot = Matrix4::RotationQuaternion(obb.rot);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.origin, matWorldInv);
	Vector3 dir = TransformNormal(ray.direction, matWorldInv);
	Ray localRay{ org, dir };

	Plane planes[] =
	{
		{ {  0.0f,  0.0f, -1.0f }, obb.extend.z },
		{ {  0.0f,  0.0f,  1.0f }, obb.extend.z },
		{ {  0.0f, -1.0f,  0.0f }, obb.extend.y },
		{ {  0.0f,  1.0f,  0.0f }, obb.extend.y },
		{ { -1.0f,  0.0f,  0.0f }, obb.extend.x },
		{ {  1.0f,  0.0f,  0.0f }, obb.extend.x }
	};

	uint32_t numIntersections = 0;
	for (uint32_t i = 0; i < 6; ++i)
	{
		const float d = Dot(org, planes[i].n);
		if (d > planes[i].d)
		{
			float distance = 0.0f;
			if (Intersect(localRay, planes[i], distance) && distance >= 0.0f)
			{
				Vector3 pt = org + (dir * distance);
				if (abs(pt.x) <= obb.extend.x + 0.01f &&
					abs(pt.y) <= obb.extend.y + 0.01f &&
					abs(pt.z) <= obb.extend.z + 0.01f)
				{
					point = pt;
					normal = planes[i].n;
					++numIntersections;
				}
			}
		}
	}

	if (numIntersections == 0)
	{
		return false;
	}

	point = TransformCoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);
	return true;

}

bool JimmyGod::Math::Intersect(const Ray & ray, const Plane & plane, float & distance)
{
	const float orgDotN = Dot(ray.origin, plane.n);
	const float dirDotN = Dot(ray.direction, plane.n);

	// Check if ray is parallel to the plane
	if (Abs(dirDotN) < 0.0001f)
	{
		if (Abs(orgDotN - plane.d) < 0.0001f)
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}
	// Compute distance
	distance = (plane.d - orgDotN) / dirDotN;
	return true;
}

bool JimmyGod::Math::Intersect(const LineSegment& a, const LineSegment& b)
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

	float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
	float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
	float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

	// First check for special cases
	if (denom == 0.0f)
	{
		if (ua == 0.0f && ub == 0.0f)
		{
			// The line segments are the same
			return true;
		}
		else
		{
			// The line segments are parallel
			return false;
		}
	}

	ua /= denom;
	ub /= denom;

	if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
	{
		return false;
	}

	return true;
}

bool JimmyGod::Math::Intersect(const Circle& c, const LineSegment& l, Vector2* closestPoint)
{
	Vector2 startToCenter = c.center - l.from;
	Vector2 startToEnd = l.to - l.from;
	float len = Magnitude(startToEnd);
	Vector2 dir = startToEnd / len;

	// Find the closest point to the line segment
	float projection = Dot(startToCenter, dir);
	Vector2 point;
	if (projection > len)
	{
		point = l.to;
	}
	else if (projection < 0.0f)
	{
		point = l.from;
	}
	else
	{
		point = l.from + (dir * projection);
	}

	// Check if the closest point is within the circle
	if (!PointInCircle(point, c))
		return false;

	if (closestPoint)
		*closestPoint = point;

	return true;
}

bool JimmyGod::Math::Intersect(const Ray& ray, const Sphere& sphere, float& distance)
{
	Vector3 originToCenter = sphere.center - ray.origin;
	float originToCenterDist = Magnitude(originToCenter);
	// Check if sphere is behind the origin
	const float d = Dot(originToCenter, ray.direction);
	if (d < 0.0f)
	{
		// Check if origin is inside sphere
		if (originToCenterDist > sphere.radius)
		{
			// No intersection
			return false;
		}
		else
		{
			Vector3 centerOnRay = ray.origin + ray.direction * d;
			float side = sqrt(Sqr(sphere.radius) - MagnitudeSqr(sphere.center - centerOnRay));
			distance = side - Magnitude(centerOnRay - ray.origin);
			return true;
		}
	}
	// Center of sphere projects on the ray
	Vector3 centerOnRay = ray.GetPoint(d);
	// Check if projected center is inside sphere
	float centerToRayDist = Magnitude(sphere.center - centerOnRay);
	if (centerToRayDist > sphere.radius)
	{
		// No intersection
		return false;
	}
	float side = sqrt(Sqr(sphere.radius) - Sqr(centerToRayDist));
	if (originToCenterDist > sphere.radius)
	{
		// Origin is outside sphere
		distance = Magnitude(centerOnRay - ray.origin) - side;
	}
	else
	{
		// Origin is inside sphere
		distance = Magnitude(centerOnRay - ray.origin) + side;
	}
	return true;
}


bool JimmyGod::Math::Intersect(const LineSegment& l, const Circle& c)
{
	return Intersect(c, l);
}

Matrix4 JimmyGod::Math::Matrix4::Transform(const Vector3& translation, const Quaternion& rotation, const Vector3& scale)
{
	Math::Matrix4 transform = Math::Matrix4::RotationQuaternion(rotation);
	transform._11 *= scale.x;
	transform._22 *= scale.y;
	transform._33 *= scale.z;
	transform._41 = translation.x;
	transform._42 = translation.y;
	transform._43 = translation.z;
	return transform;
}

Matrix4 JimmyGod::Math::Matrix4::Transform(const Vector3& translation, const Matrix4& rotation, const Vector3& scale)
{
	Math::Matrix4 transform = rotation;
	transform._11 *= scale.x;
	transform._22 *= scale.y;
	transform._33 *= scale.z;
	transform._41 = translation.x;
	transform._42 = translation.y;
	transform._43 = translation.z;
	return transform;
}