#include "Precompiled.h"
#include "Camera.h"
#include "Graphics/Inc/GraphicsSystem.h"
using namespace JimmyGod;
using namespace JimmyGod::Graphics;

void Camera::SetPosition(const Vector3 & position)
{
	mPosition = position;
}

void Camera::SetDirection(const Vector3 & direction)
{
	mDirection = Normalize(direction);
}

void Camera::SetLookAt(const Vector3 & target)
{
	mDirection = Normalize(target - mPosition);
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	const Vector3 right = Normalize(Cross(Vector3::YAxis, mDirection));
	mPosition += right * distance;
}

void Camera::Rise(float distance)
{
	mPosition += Vector3::YAxis * distance;
}

void Camera::Pitch(float radian)
{
	const Vector3 right = Normalize(Cross(Vector3::YAxis, mDirection));
	const Matrix4 matRot = Matrix4::RotationAxis(right, radian);
	const Vector3 newLook = TransformNormal(mDirection, matRot);

	// Check to prevent our new look direction from being colinear with the Y-axis
	const float dot = Dot(newLook, Vector3::YAxis);
	if (Abs(dot) < 0.995f)
		mDirection = newLook;
}

void Camera::Yaw(float radian)
{
	Matrix4 matRot = Matrix4::RotationY(radian);
	mDirection = TransformNormal(mDirection, matRot);
}

void Camera::SetFov(float fov)
{
	constexpr float kMinFov = 10.0f * Constants::DegToRad;
	constexpr float kMaxFov = 170.0f * Constants::DegToRad;
	mFov = Clamp(fov, kMinFov,kMaxFov);
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

void Camera::SetAspectRatio(float ratio)
{
	mAspectratio = ratio;
}

Matrix4 Camera::GetViewMatrix() const
{
	const Vector3 l = mDirection;
	const Vector3 r = Normalize(Cross(Vector3::YAxis, l));
	const Vector3 u = Normalize(Cross(l, r));
	const float dx = -Dot(r, mPosition);
	const float dy = -Dot(u, mPosition);
	const float dz = -Dot(l, mPosition);
	return {
		r.x,	u.x,	l.x,	0.0f,
		r.y,	u.y,	l.y,	0.0f,
		r.z,	u.z,	l.z,	0.0f,
		dx,		dy,		dz,		1.0f
	};
}

Matrix4 Camera::GetPerspectiveMatrix() const
{
	float aspectRatio = mAspectratio;
	if (aspectRatio == 0)
	{
		const auto width = GraphicsSystem::Get()->GetBackBufferWidth();
		const auto height = GraphicsSystem::Get()->GetBackBufferHeight();
		aspectRatio = static_cast<float>(width) / height;
	}
	const float h = 1.0f / tanf(mFov * 0.5f);
	const float w = h / aspectRatio;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
	const float d = zf / (zf - zn);

	return {
	w,		0.0f,	0.0f,	0.0f,
	0.0f,	h,		0.0f,	0.0f,
	0.0f,	0.0f,	d,		1.0f,
	0.0f,	0.0f,	-zn * d,	0.0f
	};
}

Matrix4 Camera::GetOrthographioMatrix(float width, float height) const
{
	const float w = width;
	const float h = height;
	const float zf = mFarPlane;
	const float zn = mNearPlane;

	return Math::Matrix4
	{
		2.0f / w,	 0.0f,	0.0f,	0.0f,
		0.0f,	2.0f / h,		0.0f,	0.0f,
		0.0f,	0.0f,	1.0f / (zf - zn),	0.0f,
		0.0f,	0.0f,	zn / (zn - zf),		1.0f
	};
}
