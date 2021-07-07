#include "Precompiled.h"
#include "TransformComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

META_DERIVED_BEGIN(TransformComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(pos, "Position")
		META_FIELD(rot, "Rotation")
		META_FIELD(scl, "Scale")
	META_FIELD_END
META_CLASS_END

void TransformComponent::Initialize()
{
}

void TransformComponent::Terminate()
{
	
}

void TransformComponent::Update(float deltaTime)
{

}

void TransformComponent::Render()
{
}

void TransformComponent::DebugUI()
{
	auto mTransform = GetTransform();
	SimpleDraw::AddTransform(mTransform);
}

void TransformComponent::SetLookAt(const Math::Vector3 & from, const Math::Vector3 & to, const Math::Vector3 & worldUp)
{
	Math::Vector3 forward = Normalize(from - to);
	Math::Vector3 right = Cross(Normalize(worldUp), forward);
	Math::Vector3 up = Cross(forward, right);

	mat._11 = right.x; mat._12 = right.y; mat._13 = right.z;
	mat._21 = up.x; mat._22 = up.y; mat._23 = up.z;
	mat._31 = forward.x; mat._32 = forward.y; mat._33 = forward.z;
	mat._41 = from.x; mat._42 = from.y; mat._43 = from.z;
}

void TransformComponent::SetPosition(Math::Vector3 vec)
{
	pos.x = vec.x;
	pos.y = vec.y;
	pos.z = vec.z;
}

void TransformComponent::Translate(Math::Vector3 vec)
{	
	mat = Matrix4::Translation(vec);
}

const Vector3 & TransformComponent::GetPosition() const
{
	return pos;
}

void TransformComponent::SetRotation(float radian)
{
	//rot = Quaternion::RotationAxis(GetPosition(), radian * Constants::DegToRad);

	rotation = Matrix4::RotationAxis(GetPosition().YAxis, radian * Constants::DegToRad);
}

const Math::Matrix4 & TransformComponent::GetRotation() const
{
	return rotation;
}

void TransformComponent::SetScale(Math::Vector3 vec)
{
	scl.x = vec.x; scl.y = vec.y; scl.z = vec.z;
}

const Math::Vector3 & TransformComponent::GetScale() const
{
	return scl;
}

const Math::Matrix4& TransformComponent::GetTransform() const
{
	//return Math::Matrix4::Transform(GetPosition(), GetRotation(), GetScale());

	return Matrix4::Scaling(GetScale()) * rotation * Matrix4::Translation(GetPosition());
}

Math::Vector3 TransformComponent::Forward() const
{
	return Math::GetForward(mat);
}

Math::Vector3 TransformComponent::Up() const
{
	return Math::GetUp(mat);
}

Math::Vector3 TransformComponent::Right() const
{
	return Math::GetRight(mat);
}