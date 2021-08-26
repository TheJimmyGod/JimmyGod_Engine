#include "UIManager.h"
#include "GameManager.h"
#include "Flag.h"
#include "Unit.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

namespace
{
	std::unique_ptr<UIManager> sInstance;
}

void UIManager::StaticInitialize()
{
	sInstance = std::make_unique<UIManager>();
	sInstance->Initialize();
}

void UIManager::StaticTerminate()
{
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
	mSoldierSpark.Initialize("../../Assets/Textures/Sun.jpg", 20, 0.2f);
	mMutantSpark.Initialize("../../Assets/Textures/Neptune.jpg", 20, 0.2f);
}

void UIManager::Terminate()
{
	mSoldierSpark.Terminate();
	mMutantSpark.Terminate();
	for (auto& text : mTextmeshes)
	{
		text.reset();
	}
	mTextmeshes.clear();
}

void UIManager::Update(float deltaTime)
{
	mSoldierSpark.Update(deltaTime);
	mMutantSpark.Update(deltaTime);

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
	mSoldierSpark.Render(camera);
	mMutantSpark.Render(camera);
}

void UIManager::UpdateAnimation(Unit* unit, Unit* target, float lifeTime)
{
	if (unit == nullptr && target == nullptr)
	{
		mSparkPlay = false;
		mAnimationPlay = false;
		return;
	}
	else
	{
		float time = unit->mTime;
		auto rot = unit->GetAgent().GetTransformComponent().GetRotation();
		auto pos = unit->GetAgent().GetPosition();
		if (target == nullptr) return;
		auto targetPos = target->GetAgent().GetPosition();

		if ((time > 0.0f && time <= lifeTime) && !mAnimationPlay)
		{
			mAnimationPlay = true;
			mCurrentUnit = unit->GetUnitType();
			if (unit->GetUnitType() == UnitType::Soldier)
			{
				unit->GetAgent().GetModelComponent().SetAnimationSpeed(1.1f);
				unit->GetAgent().GetModelComponent().PlayAnimation(4);
				unit->GetAgent().GetModelComponent().SetAnimationTime(0.0f);
			}
			else
			{
				unit->GetAgent().GetModelComponent().SetAnimationSpeed(1.1f);
				unit->GetAgent().GetModelComponent().PlayAnimation(2);
				unit->GetAgent().GetModelComponent().SetAnimationTime(0.0f);
			}
		}
		if ((time > 0.0f && time <= lifeTime - 1.0f) && !mSparkPlay)
		{
			mSparkPlay = true;
			mHand = FindBone(unit->GetAgent().GetModelComponent().GetModel().mSkeleton, "RightHand");
			auto handPosition = pos + GetTranslation(unit->GetAgent().GetModelComponent().GetBoneMatrices()[mHand->index]
				* Matrix4::RotationQuaternion(rot)) * 0.04f;
			auto direction = targetPos - pos;

			PlayEffect(handPosition, direction, lifeTime);
		}
	}
}

void UIManager::RenderText(const char* text, const JimmyGod::Math::Vector3& pos, float size, float lifeTime, JimmyGod::Graphics::Color color)
{
	auto& texts = mTextmeshes.emplace_back(std::make_unique<TextMesh>());
	texts->Initialize(text, pos, size, color, lifeTime, static_cast<uint32_t>(0));
}

void UIManager::PlayEffect(const Vector3& pos, const Vector3& dir, float lifeTime)
{
	switch (mCurrentUnit)
	{
	case UnitType::Soldier:
		mSoldierSpark.ShowSpark(pos, dir, lifeTime);
		break;
	case UnitType::Mutant:
		mMutantSpark.ShowSpark(pos, dir, lifeTime);
		break;
	default:
		break;
	}

}
