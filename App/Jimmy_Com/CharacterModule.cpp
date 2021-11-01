#include "CharacterModule.h"
#include "Unit.h"
using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace JimmyCom;

void CharacterModule::Terminate()
{
	mTransformBuffer.Terminate();
}

void CharacterModule::Update(float deltaTime)
{
	if (mModuleActive == false) return;
	if (mTime > 0.0f)
	{
		mTime -= deltaTime;
		if(mTime <= 0.0f) mGameObject->GetComponent<ModelComponent>()->SetAnimationSpeed(1.0f);
		return;
	}
	if (!mAnimationProcess)
	{
		if (mGameObject->GetComponent<AgentComponent>()->GetSpeed() > 0.1f)
			mGameObject->GetComponent<ModelComponent>()->PlayAnimation(1);
		else
			mGameObject->GetComponent<ModelComponent>()->PlayAnimation(0);
	}
}

bool JimmyCom::CharacterModule::AnimateTime(float deltaTime)
{
	if (mUpdateTime <= 0.0f) return true;
	else
	{
		mUpdateTime -= deltaTime;
		return false;
	}
}

void CharacterModule::Render(const JimmyGod::Graphics::Camera& camera)
{
	if (mModuleActive == false) return;

	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetPerspectiveMatrix();
	auto matWorld = mGameObject->GetComponent<TransformComponent>()->GetTransform();
	mTransformBuffer.BindVS(0);
	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = camera.GetPosition();
	mTransformBuffer.Update(&transformData);
	mGameObject->GetComponent<ModelComponent>()->Render();
}

const CharacterModule& JimmyCom::CharacterModule::GetCharacterModule() const
{
	return *this;
}

CharacterModule& JimmyCom::CharacterModule::GetCharacterModule()
{
	return *this;
}

const TransformComponent& JimmyCom::CharacterModule::GetTransformComponent() const
{
	return *mGameObject->GetComponent<TransformComponent>();
}

TransformComponent& JimmyCom::CharacterModule::GetTransformComponent()
{
	return *mGameObject->GetComponent<TransformComponent>();
}

const ColliderComponent& JimmyCom::CharacterModule::GetColliderComponent() const
{
	return *mGameObject->GetComponent<ColliderComponent>();
}

ColliderComponent& JimmyCom::CharacterModule::GetColliderComponent()
{
	return *mGameObject->GetComponent<ColliderComponent>();
}

const ModelComponent& JimmyCom::CharacterModule::GetModelComponent() const
{
	return *mGameObject->GetComponent<ModelComponent>();
}

ModelComponent& JimmyCom::CharacterModule::GetModelComponent()
{
	return *mGameObject->GetComponent<ModelComponent>();
}

const AgentComponent& JimmyCom::CharacterModule::GetAgentComponent() const
{
	return *mGameObject->GetComponent<AgentComponent>();
}

AgentComponent& JimmyCom::CharacterModule::GetAgentComponent()
{
	return *mGameObject->GetComponent<AgentComponent>();
}

const JimmyGod::Math::Vector3& JimmyCom::CharacterModule::GetPosition() const
{
	return GetTransformComponent().GetPosition();
}

const JimmyGod::Math::Quaternion& JimmyCom::CharacterModule::GetRotation() const
{
	return GetTransformComponent().GetRotation();
}

const JimmyGod::Math::Sphere& JimmyCom::CharacterModule::GetSphereCollider() const
{
	return GetColliderComponent().GetSphere();
}
