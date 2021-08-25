#pragma once
#include "TextureType.h"
#include "Colors.h"
using namespace JimmyGod::Math;

namespace JimmyGod::Graphics
{
	
	// This camera assumes YAxis as up direction. You cannot look striaght up nor straight down
	class Camera
	{
	public:
		void SetPosition(const Vector3& position);
		void SetDirection(const Vector3& direction);
		void SetLookAt(const Vector3& target);

		// 3 Degrees of freedom for translations
		void Walk(float distance);
		void Strafe(float distance);
		void Rise(float distance);
		// 2 Degrees of freedom for rotations
		void Pitch(float radian);
		void Yaw(float radian);

		void SetFov(float fov);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);
		void SetAspectRatio(float ratio);
		const Vector3& GetPosition() const { return mPosition; }
		const Vector3& GetDirection() const { return mDirection; }

		Vector2 ConvertToScreenPosition(const Vector2& worldPos) const;
		Vector2 ConvertToWorldPosition(const Vector2& screenPos) const;
		Vector2 ConvertTo2DSpace(const Vector3& worldPos) const;
		void SetViewPosition(const Vector2& pos) { mViewPosition = pos; }

		void Draw(TextureId textureId, const Vector2& worldPos) const;
		void Write(const char* text, const Vector2& worldPos, float size, const Color& color) const;

		Matrix4 GetViewMatrix() const;
		Matrix4 GetPerspectiveMatrix() const;

		Matrix4 GetOrthographioMatrix(float width, float height) const;
		Math::Ray ScreenPointToWorldRay(int screenX, int screenY) const;
	private:
		Vector3 mPosition = Vector3::Zero;
		Vector3 mDirection = Vector3::ZAxis;
		Vector2 mViewPosition = Vector2::Zero;
		float mAspectratio = 0.0f;
		float mFov = 60.0f * Constants::DegToRad;
		float mNearPlane = 1.0f;
		float mFarPlane = 10000.0f;
	};
} // namespace JimmyGod::Graphics

// Fov = field of view