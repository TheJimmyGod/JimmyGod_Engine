#include "Precompiled.h"
#include "EngineMath.h"

using namespace JimmyGod::Math;

const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f };
const Vector3 Vector3::XAxis{ Vector3{ 1.0f,0.0f,0.0f } };
const Vector3 Vector3::YAxis{ Vector3{ 0.0f,1.0f,0.0f } };
const Vector3 Vector3::ZAxis{ Vector3{ 0.0f,0.0f,1.0f } };

const Matrix4 Matrix4::Identity
{
	Matrix4{1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f}
};