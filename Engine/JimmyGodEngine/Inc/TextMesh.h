#pragma once

namespace JimmyGod
{
	class TextMesh
	{
	public:
		enum AnimateText : std::uint32_t
		{
			Up = static_cast<uint32_t>(0),
			Down = static_cast<uint32_t>(1),
			Right = static_cast<uint32_t>(2),
			Left = static_cast<uint32_t>(3),
			None = static_cast<uint32_t>(4)
		};
		TextMesh() {}
		TextMesh(const char* str, const JimmyGod::Math::Vector3& pos, float size, JimmyGod::Graphics::Color color, float lifeTime, uint32_t animate);
		~TextMesh() = default;

		void Initialize(const char* str, const JimmyGod::Math::Vector3& pos, float size, JimmyGod::Graphics::Color color, float lifeTime, uint32_t animate);
		void Update(const JimmyGod::Graphics::Camera& camera, float deltaTime);

		const float GetLifeTime() const { return mLifeTime; }
		bool GetInitialize() const { return mIntialized; }
	private:
		AnimateText mCurrentAnimate;
		std::string mString;
		JimmyGod::Math::Vector3 mPosition = JimmyGod::Math::Vector3::Zero;
		JimmyGod::Graphics::Color mColor = JimmyGod::Math::Vector4::Zero;
		float mLifeTime = 0.0f;
		float mMaxLifeTime = 0.0f;
		float mSize = 0.0f;
		float mLocation = 0.0f;

		bool mIntialized = false;
	};
}