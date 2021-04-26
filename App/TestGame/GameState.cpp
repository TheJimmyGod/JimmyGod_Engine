#include "GameState.h"

#include <ImGui/Inc/imgui.h>

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera = mWorld.AddService<CameraService>();
	mWorld.Initialize(100);

	auto& camera = mCamera->GetActiveCamera();

	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(500.0f);
	camera.SetPosition({ 0.0f, 10.0f, -15.0f });
	camera.SetLookAt({ 0.0f, 7.0f, 0.0f });
	mWorld.Create("../../Assets/Templates/tallBox.json", "Jimmy");

	mModel.Initialize("../../Assets/Models/Batman/Idle.model");

	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", BoneVertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	mAnimator.Initialize(mModel);
	mAnimator.ComputeBindPose();
	mAnimator.PlayAnimation(0);
	mAnimator.BlendTo(1, 2.0f);
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.35f, 0.35f, 0.35f, 1.0f };
	mDirectionalLight.specular = { 0.25f, 0.25f, 0.25f, 1.0f };

	mMaterial.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mMaterial.diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
	mMaterial.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.power = 40.0f;

	mSettings.specularMapWeight = 1.0f;
	mSettings.bumpMapWeight = 0.0f;
	mSettings.normalMapWeight = 0.0f;
	mSettings.aoMapWeight = 1.0f;
	mSettings.brightness = 3.5f;
	mSettings.useShadow = 1;
	mSettings.depthBias = 0.0003f;

	JimmyGod::Physics::PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f, -9.8f, 0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;

	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.SingleOBB = mWorld.Find("Jimmy").Get()->GetComponent<ColliderComponent>()->GetOBB();
	
	mCloak.Initialize("../../Assets/Textures/BatTexture.png", 5, 8);
}

void GameState::Terminate()
{
	mWorld.Terminate();
	mSampler.Terminate();
	mAnimator.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mModel.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mCloak.Terminate();
	mPhysicsWorld.Clear();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	auto& camera = mCamera->GetActiveCamera();
	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;
	accelation = Vector3::Zero;

	mWorld.Update(deltaTime);
	if (!stopAnimation)
		mAnimator.Update(deltaTime);
	mPhysicsWorld.Update(deltaTime);
	mCloak.Update(deltaTime);
	mPhysicsWorld.SingleOBB = mWorld.Find("Jimmy").Get()->GetComponent<ColliderComponent>()->GetOBB();
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		camera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}
	if (mTime > 0.1f)
	{
		if(stopAnimation == false)
			mTime -= deltaTime;
		return;
	}
	else
	{
		isKicked = false;
		isThrew = false;
	}

	if (!isJump)
	{
		if (mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y > 0.1f)
		{
			mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y += mGravity * deltaTime;
			accelation.y += mGravity * deltaTime;
		}
	}
	else
	{
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y -= 1.2f * mGravity * deltaTime;
		accelation.y -= 1.2f * mGravity * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
		if (mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y > 5.0f)
			isJump = false;
	}
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		camera.Walk(kMoveSpeed * deltaTime);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.z += kMoveSpeed * deltaTime;
		accelation.z += kMoveSpeed * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		camera.Walk(-kMoveSpeed * deltaTime);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.z -= kMoveSpeed * deltaTime;
		accelation.z -= kMoveSpeed * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		camera.Strafe(kMoveSpeed * deltaTime);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.x += kMoveSpeed * deltaTime;
		accelation.x += kMoveSpeed * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		camera.Strafe(-kMoveSpeed * deltaTime);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.x -= kMoveSpeed * deltaTime;
		accelation.x -= kMoveSpeed * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
	}

	velocity += accelation * deltaTime;
	velocity = Normalize(velocity);

	if (mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y < 0.1f)
	{
		if (Magnitude(accelation) != 0.0f)
		{
			mAnimator.PlayAnimation(1);
			currentAnimation = 1;
		}
		else
		{
			mAnimator.PlayAnimation(0);
			currentAnimation = 0;
		}
		if (inputSystem->IsKeyPressed(KeyCode::Z))
		{
			isKicked = true;
			mAnimator.SetTime(0.0f);
			mAnimator.PlayAnimation(3);
			currentAnimation = 3;
			mTime += 1.75f;
		}

		if (inputSystem->IsKeyPressed(KeyCode::X))
		{
			isThrew = true;
			mAnimator.SetTime(0.0f);
			mAnimator.PlayAnimation(4);
			currentAnimation = 4;
			mTime += 2.5f;
		}

		if (inputSystem->IsKeyPressed(KeyCode::SPACE))
		{
			isJump = true;
			mAnimator.SetTime(0.0f);
			mAnimator.PlayAnimation(2);
		}
		if (inputSystem->IsKeyPressed(KeyCode::C))
		{
			isCloak = !isCloak;
			mPhysicsWorld.Clear();
			mCloak.Active(isCloak);
			if (mCloak.IsActive())
			{
				mCloak.ShowCloth(Vector3{ mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition().x - 0.5f,
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition().y + 1.0f,
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition().z + 2.0f });
			}
		}
	}
}

void GameState::Render()
{
	mPhysicsWorld.DebugDraw();
	auto matView = mCamera->GetActiveCamera().GetViewMatrix();
	auto matProj = mCamera->GetActiveCamera().GetPerspectiveMatrix();
	mWorld.Render();
	mSampler.BindVS();
	mSampler.BindPS();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(&mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mBoneTransformBuffer.BindVS(5);


	auto matWorld = Matrix4::Scaling(0.04f) * Matrix4::Translation(mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos);
	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mCamera->GetActiveCamera().GetPosition();
	mTransformBuffer.Update(&transformData);

	BoneTransformData boneTransformData{};

	for (size_t i = 0; i < mAnimator.GetBoneMatrices().size(); ++i)
	{
		boneTransformData.BoneTransforms[i] = Transpose(mModel.mSkeleton.bones[i]->offsetTransform * mAnimator.GetBoneMatrices()[i] /** Matrix4::Translation(pos)*/);
	}
	if (!showSkeleton)
		mModel.Render();
	else
	{
		for (auto& b : mModel.mSkeleton.bones)
		{
			DrawSkeleton(b.get(), mAnimator.GetBoneMatrices(), mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition(), 0.04f);
		}

	}
	mBoneTransformBuffer.Update(&boneTransformData);

	mCloak.Render(mCamera->GetActiveCamera(), Vector3{ mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition().x,
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition().y + 1.0f,
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition().z + 2.0f });

	SimpleDraw::AddGroundPlane(50.0f,Colors::Aqua);
	SimpleDraw::Render(mCamera->GetActiveCamera());
}

void GameState::DebugUI()
{
	mWorld.DebugUI();
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("fps: %.2f", fps);
		//static float mAniTime = 0.0f;
		//ImGui::SliderFloat("Duration", &mAniTime, 0.0f, mModel.mAnimationSet.clips[currentAnimation].get()->duration);
		//if (mAniTime > 0.1f && !stopAnimation)
		//{
		//	mAnimator.StopAnimation(true);
		//	mAnimator.SetTime(mAniTime);
		//}
		//else if (mAniTime < 0.1f && stopAnimation)
		//{
		//	mAnimator.StopAnimation(false);
		//}

		if (ImGui::Checkbox("Set Skeleton", &showSkeleton))
		{
		}
		//if (ImGui::Checkbox("Stop Animation", &stopAnimation))
		//{
		//	mAnimator.StopAnimation(stopAnimation);
		//}
	}
	ImGui::End();
}