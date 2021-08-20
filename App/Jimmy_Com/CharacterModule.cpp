#include "CharacterModule.h"
#include "Unit.h"
using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

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
