#include "UIManager.h"

#include "../App/Jimmy_Com/HUD.h"
#include "GameManager.h"
#include "Flag.h"
#include "Unit.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;
using namespace JimmyCom;

namespace
{
	std::unique_ptr<UIManager> sInstance;
}

void UIManager::StaticInitialize()
{
	sInstance = std::make_unique<UIManager>();
	sInstance->Initialize();

	JimmyCom::HUD::StaticInitialize();
}

void UIManager::StaticTerminate()
{
	JimmyCom::HUD::StaticTerminate();
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

UIManager* UIManager::Get()
{
	return sInstance.get();
}

void UIManager::Initialize()
{
	for (size_t i = 0; i < 15; i++)
	{
		JimmyGod::Spark* spark = new Spark();
		spark->Initialize("../../Assets/Textures/Sun.jpg", 4, 0.11f, 0.05f);
		mSoldierSpark.emplace_back(spark);
	}
	mMutantSpark.Initialize("../../Assets/Textures/Neptune.jpg", 20, 0.11f);
}

void UIManager::Terminate()
{
	for (auto& spark : mSoldierSpark) spark->Terminate();
	mMutantSpark.Terminate();
	for (auto& text : mTextmeshes) text.reset();
	mTextmeshes.clear();
}

void UIManager::Update(float deltaTime)
{
	for(auto& spark : mSoldierSpark) spark->Update(deltaTime);
	mMutantSpark.Update(deltaTime);
	JimmyCom::HUD::Get()->Update(deltaTime);
	for (auto& text : mTextmeshes)
	{
		text->Update(GameManager::Get()->GetGameWorld().GetService<CameraService>()->GetActiveCamera(), deltaTime);
		if (text->GetLifeTime() <= 0.0f && text->GetInitialize())
		{
			text.reset();
			auto it = std::find(mTextmeshes.begin(), mTextmeshes.end(), text);
			mTextmeshes.erase(it);
			break;
		}
	}
}

void UIManager::Render(const Camera& camera)
{
	for(auto& spark : mSoldierSpark) spark->Render(camera);
	mMutantSpark.Render(camera);
	JimmyCom::HUD::Get()->Render();
}

void JimmyCom::UIManager::ShowButtons()
{
	JimmyCom::HUD::Get()->DisplayAllButtons();
}

void JimmyCom::UIManager::HideButtons()
{
	JimmyCom::HUD::Get()->DisappearAllButtons();
}

void UIManager::UpdateAnimation(Unit* unit, Unit* target, float lifeTime)
{
	if (unit == nullptr && target == nullptr)
	{
		mSparkPlay = false;
		mAnimationPlay = false;
		mIndex = 0;
		mTime = 0.0f;
		return;
	}
	else
	{
		float time = unit->mUpdateTime;
		auto rot = unit->GetRotation();
		auto pos = unit->GetPosition();
		if (target == nullptr) return;
		auto targetPos = target->GetPosition();

		if ((time > 0.0f && time <= lifeTime) && !mAnimationPlay)
		{
			mAnimationPlay = true;
			mCurrentUnit = unit->GetUnitType();
			switch (mCurrentUnit)
			{
			case JimmyCom::UnitType::Soldier:
			{
				unit->GetModelComponent().SetAnimationSpeed(1.1f);
				unit->GetModelComponent().PlayAnimation(4);
				unit->GetModelComponent().SetAnimationTime(0.0f);
			}
				break;
			case JimmyCom::UnitType::Mutant:
			{
				unit->GetModelComponent().SetAnimationSpeed(1.1f);
				unit->GetModelComponent().PlayAnimation(2);
				unit->GetModelComponent().SetAnimationTime(0.0f);
			}
				break;
			default: break;
			}
		}
		switch (mCurrentUnit)
		{
		case JimmyCom::UnitType::Soldier:
		{
			if ((time > 0.0f && time <= lifeTime - 1.0f) && !mSparkPlay)
			{
				mSparkPlay = true;
				mHand = FindBone(unit->GetModelComponent().GetModel().mSkeleton, "RightHand");
			}
			if (mSparkPlay)
			{
				if (JimmyGod::Core::TimeUtil::GetTime() > mTime)
				{
					if (mIndex >= 14) mIndex = 0;
					mTime = JimmyGod::Core::TimeUtil::GetTime() + 0.05f;
					auto handPosition = pos + GetTranslation(unit->GetModelComponent().GetBoneMatrices()[mHand->index]
						* Matrix4::RotationQuaternion(rot)) * 0.04f;
					mSoldierSpark[mIndex]->ShowSpark(handPosition, Normalize(targetPos - pos), lifeTime - 0.8f);
					mIndex++;
				}
			}
		}
			break;
		case JimmyCom::UnitType::Mutant:
		{
			if ((time > 0.0f && time <= lifeTime - 1.0f) && !mSparkPlay)
			{
				mSparkPlay = true;
				mHand = FindBone(unit->GetModelComponent().GetModel().mSkeleton, "RightHand");
				auto handPosition = pos + GetTranslation(unit->GetModelComponent().GetBoneMatrices()[mHand->index]
					* Matrix4::RotationQuaternion(rot)) * 0.04f;
				mMutantSpark.ShowSpark(handPosition, Normalize(targetPos - pos), lifeTime - 0.8f);
			}
		}
			break;
		default:
			break;
		}
	}
}

void UIManager::RenderText(const char* text, const JimmyGod::Math::Vector3& pos, float size, float lifeTime, JimmyGod::Graphics::Color color)
{
	auto& texts = mTextmeshes.emplace_back(std::make_unique<TextMesh>());
	texts->Initialize(text, pos, size, color, static_cast<uint32_t>(0), lifeTime);
}
