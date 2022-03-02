#pragma once

namespace JimmyGod
{
	class Button
	{
	public:
		Button(const char* fileName_on, const char* fileName_off, const JimmyGod::Math::Vector2& pos, float radius);
		~Button() = default;

		void Update(float deltaTime);
		void Render();

		void AddListener(std::function<void()> func);

		void Invoke() const;

		void Click(bool c) { onClick = c; }
		const bool IsClicked() const { return onClick; }

		void SetPosition(const JimmyGod::Math::Vector2& pos) { mPosition = pos; }
		void SetDisplay(bool d) { IsDisplay = d; }

		const JimmyGod::Math::Circle& GetCollider() const { return mCollider; }
		const JimmyGod::Math::Vector2& GetPosition() const { return mPosition; }

	private:
		bool IsDisplay = false;
		bool onClick = false;
		bool onPoint = false;
		std::function<void()> mFunc;

		JimmyGod::Math::Vector2 mPosition = JimmyGod::Math::Vector2::Zero;
		JimmyGod::Math::Circle mCollider;

		JimmyGod::Graphics::TextureId mTexture[2];
	};
}