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
	camera.SetFarPlane(1750.0f);
	camera.SetPosition({ 0.0f, 7.5f, -20.0f });
	camera.SetLookAt({ 0.0f, 0.0f, 0.0f });
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
	//mPhysicsWorld.SingleOBB = mWorld.Find("Jimmy").Get()->GetComponent<ColliderComponent>()->GetOBB();
	//mPhysicsWorld.AddOBB(mPhysicsWorld.SingleOBB);

	mCloak.Initialize("../../Assets/Textures/BatTexture.png", 4, 7);
	mSpark.Initialize("../../Assets/Textures/Sun.jpg", 20 ,0.2f);
	mBomb.Initialize("../../Assets/Textures/BatTexture.png", 0.5f);
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
	mBomb.Terminate();
	mCloak.Terminate();
	mSpark.Terminate();
	mPhysicsWorld.Clear();

	mRainBuffer.Terminate();
	mRainTexture.Terminate();

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	auto& camera = mCamera->GetActiveCamera();
	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;
	fps = 1.0f / deltaTime;
	accelation = Vector3::Zero;

	mWorld.Update(deltaTime);
	if (!stopAnimation)
	{
		mTime -= deltaTime;
		mAnimator.Update(deltaTime);
	}
	mBomb.Update(deltaTime);
	mPhysicsWorld.Update(deltaTime);
	mCloak.Update(deltaTime, static_cast<int>(mDirection), true);
	mSpark.Update(deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		camera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	position = Vector3{ mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition().x,
mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition().y + 0.4f,
mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition().z - 0.5f };

	if (mCloak.IsActive())
		mCloak.SetPosition(position + GetTranslation(mModel.mSkeleton.bones[4]->offsetTransform * mAnimator.GetBoneMatrices()[4]) * 0.04f,
			position + GetTranslation(mModel.mSkeleton.bones[6]->offsetTransform * mAnimator.GetBoneMatrices()[6]) * 0.04f,
			position + GetTranslation(mModel.mSkeleton.bones[25]->offsetTransform * mAnimator.GetBoneMatrices()[25]) * 0.04f);

	if (mTime < 1.3f)
	{
		if (isKicked == true)
		{
			mSpark.ShowSpark(position + GetTranslation(mModel.mSkeleton.bones[50]->offsetTransform * mAnimator.GetBoneMatrices()[50]) * 0.04f, velocity,3.0f);
			isKicked = false;
		}
	}
	if (mTime < 1.1f)
	{
		if (isThrew == true)
		{
			mBomb.ShowBomb(position, velocity, 5.0f);
			isThrew = false;
		}
	}
	if (mTime > 0.1f && stopAnimation == false)
		return;

	if (!isJump)
	{
		if (mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y > 0.1f)
		{
			mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y += mGravity * deltaTime;
			accelation.y += kMoveSpeed * deltaTime;
			accelation.z -= kMoveSpeed* deltaTime;
			if (mCloak.IsActive())
				mCloak.SetVelocity(-accelation);
		}
	}
	else
	{
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y -= 1.2f * mGravity * deltaTime;
		accelation.y -= mGravity * deltaTime;
		accelation.z -= kMoveSpeed * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
		if (mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y > 5.0f)
			isJump = false;
	}
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mDirection = Direction::Back;
		camera.Walk(kMoveSpeed * deltaTime);
		rotation = Matrix4::RotationAxis(mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.YAxis, 180.0f * Constants::DegToRad);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.z += kMoveSpeed * deltaTime;
		accelation.z += kMoveSpeed * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation / 3.0f);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mDirection = Direction::Front;
		camera.Walk(-kMoveSpeed * deltaTime);
		rotation = Matrix4::RotationAxis(mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.YAxis, 0.0f * Constants::DegToRad);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.z -= kMoveSpeed * deltaTime;
		accelation.z -= kMoveSpeed * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mDirection = Direction::Right;
		camera.Strafe(kMoveSpeed * deltaTime);
		rotation = Matrix4::RotationAxis(mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.YAxis, -90.0f * Constants::DegToRad);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.x += kMoveSpeed * deltaTime;
		accelation.x += kMoveSpeed * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);

	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mDirection = Direction::Left;
		camera.Strafe(-kMoveSpeed * deltaTime);
		rotation = Matrix4::RotationAxis(mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.YAxis, 90.0f * Constants::DegToRad);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.x -= kMoveSpeed * deltaTime;
		accelation.x -= kMoveSpeed * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);

	}
	if(IsZero(accelation) == false)
		velocity = accelation;
	

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
			mTime = 1.75f;
		}

		if (inputSystem->IsKeyPressed(KeyCode::X))
		{
			isThrew = true;
			mAnimator.SetTime(0.0f);
			mAnimator.PlayAnimation(4);
			currentAnimation = 4;
			mTime = 2.5f;
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
			mCloak.Active(isCloak);
			if (mCloak.IsActive())
				mCloak.ShowCloth(position + GetTranslation(mModel.mSkeleton.bones[4]->offsetTransform * mAnimator.GetBoneMatrices()[4]) * 0.04f);
		}
	}
	//mRotation += deltaTime;
	//std::string str = std::to_string(mRotation);
	//SpriteRenderManager::Get()->DrawScreenText(str.c_str(), 1000.0f, 400.0f, 20.0f, Colors::Aqua);
}

void GameState::Render()
{
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

	auto matWorld = Matrix4::Scaling(0.04f) * rotation * Matrix4::Translation(mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition());
	//auto matWorld =  Matrix4::Translation(mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition()) * rotation * Matrix4::Scaling(0.04f);

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
	mCloak.Render(mCamera->GetActiveCamera());
	mSpark.Render(mCamera->GetActiveCamera());
	mBomb.Render(mCamera->GetActiveCamera());

	SimpleDraw::AddGroundPlane(30.0f,Colors::DarkMagenta);
	SimpleDraw::Render(mCamera->GetActiveCamera());
}

void GameState::DebugUI()
{
	mWorld.DebugUI();
	static bool showDebugUI = false;
	static bool OBBcollision = true;
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("fps: %.2f", fps);

		if (ImGui::Checkbox("Set Skeleton", &showSkeleton))
		{
		}
		if (ImGui::Checkbox("Show DebugUI of particles", &showDebugUI))
		{
		}
		if (ImGui::Checkbox("OBB Debug UI active/inactive", &OBBcollision))
		{
			mWorld.Find("Jimmy").Get()->GetComponent<ColliderComponent>()->Active();
		}
	}
	if (isCloak == true)
	{
		mCloak.DebugUI(showDebugUI);
	}
	mSpark.DebugUI(showDebugUI);
	mBomb.DebugUI(showDebugUI);
	ImGui::End();
}