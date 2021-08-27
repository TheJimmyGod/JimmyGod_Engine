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
		void Initialize();
		void Terminate();
		void Update(float deltaTime);
		void Render(const JimmyGod::Graphics::Camera& camera);

		void UpdateAnimation(Unit* unit, Unit* Target, float lifeTime);
		void RenderText(const char* text, const JimmyGod::Math::Vector3& pos, float size, float lifeTime, JimmyGod::Graphics::Color color);
		//void PlayEffect(const JimmyGod::Math::Vector3& pos, const JimmyGod::Math::Vector3& dir, float lifeTime);

	private:
		std::vector<JimmyGod::Spark*> mSoldierSpark;
		JimmyGod::Spark mMutantSpark;

		Unit* mUnit = nullptr;
		Unit* mTarget = nullptr;
		Bone* mHand = nullptr;

		UnitType mCurrentUnit = UnitType::Soldier;

		std::vector<std::unique_ptr<JimmyGod::TextMesh>> mTextmeshes;

		bool mSparkPlay = false;
		bool mAnimationPlay = false;

		float mSparkTime = 0.0f;
		float mAnimationTime = 0.0f;

		float TextLifeTime = 0.0f;

		float mTime = 0.0f;
		size_t mIndex = 0;
	};
}

