#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "Flag.h"
#include "Unit.h"

namespace JimmyCom
{
	class UIManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static UIManager* Get();

	public:
		enum Order : std::uint32_t
		{
			None = static_cast<uint32_t>(0),
			Move = static_cast<uint32_t>(1),
			Attack = static_cast<uint32_t>(2),
			Standby = static_cast<uint32_t>(3)
		};

		void Initialize();
		void Terminate();
		void Update(float deltaTime);
		void Render(const JimmyGod::Graphics::Camera& camera);
		void DebugUI();

		void HUD_Render();

		void SetOrder(uint32_t order) { mOrder = Order{ order }; }
		const Order& GetOrder() const { return mOrder; }

		void ShowButtons();
		void HideButtons();

		void EnableDebugUI() { mDebugUI = !mDebugUI; }

		void UpdateAnimation(CharacterModule* unit, CharacterModule* target, UnitType type, float updateTime, float lifeTime);
		void RenderText(const char* text, const JimmyGod::Math::Vector3& pos, float size, float lifeTime, JimmyGod::Graphics::Color color);

	private:
		std::vector<JimmyGod::Spark*> mSoldierSpark;
		JimmyGod::Spark mMutantSpark;

		CharacterModule* mUnit = nullptr;
		CharacterModule* mTarget = nullptr;
		Bone* mHand = nullptr;

		UnitType mCurrentUnit = UnitType::Soldier;

		Order mOrder = Order::None;

		std::vector<std::unique_ptr<JimmyGod::TextMesh>> mTextmeshes;

		bool mSparkPlay = false;
		bool mAnimationPlay = false;

		bool mDebugUI = false;

		float mSparkTime = 0.0f;
		float mAnimationTime = 0.0f;

		float TextLifeTime = 0.0f;

		float mTime = 0.0f;
		size_t mIndex = 0;
	};
}

