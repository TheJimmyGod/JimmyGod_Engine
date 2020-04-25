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

const Quaternion Quaternion::Zero{ 0.0f };
const Quaternion Quaternion::Identity{ 0.0f,0.0f,0.0f,1.0f };