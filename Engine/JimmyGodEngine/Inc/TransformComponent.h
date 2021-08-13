#pragma once
#include "Component.h"

namespace JimmyGod
{
	class TransformComponent final : public Component
	{
	public:
		META_CLASS_DECLARE

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void EnableDebug() { if (!isDebugUI) isDebugUI = true; else isDebugUI = false; }
		void Active(bool active) { isActive = active; }
		void SetLookAt(const Math::Vector3& from, const Math::Vector3& to, const Math::Vector3& worldUp = Math::Vector3::YAxis);

		void SetPosition(Math::Vector3 vec);
		void Translate(Math::Vector3 vec);
		const Math::Vector3& GetPosition() const;

		void SetRotation(float radian);
		void SetRotation(const Quaternion& q);
		void SetRotation(const Vector3& v);
		const Math::Quaternion& GetRotation() const;
		//const Math::Matrix4& GetRotation() const;

		void SetScale(Math::Vector3 vec);
		const Math::Vector3& GetScale() const;

		const Math::Matrix4& GetTransform() const;

		Math::Vector3 Forward() const;
		Math::Vector3 Up() const;
		Math::Vector3 Right() const;

		Vector3 pos = Vector3::Zero;
		Vector3 scl = Vector3::One;
		Quaternion rot = Quaternion::Zero;
	private:
	
		Matrix4 mat = Matrix4::Identity;
		
		bool isDebugUI = false;
		bool isActive = false;
	};
}