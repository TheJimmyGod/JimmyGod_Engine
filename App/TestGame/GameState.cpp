#include "GameState.h"

#include <ImGui/Inc/imgui.h>

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;

static bool ActiveBomb = true;
static bool ActiveRope = false;
static float BombLifeTime = 5.0f;
static float BombPower = 0.2f;
static float BombThrowDist = 7.0f;
static float BombBounce = 0.7f;
static float BombRadius = 0.5f;

static float RopeLifeTime = 3.0f;
static float RopePower = 1.0f;
static float RopeThrowDist = 4.0f;
static float RopeBounce = 0.4f;
static float RopeRadius = 0.1f;
static int RopeLength = 50;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

	mCamera = mWorld.AddService<CameraService>();
	mWorld.Initialize(100);

	auto& camera = mCamera->GetActiveCamera();

	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(1750.0f);
	camera.SetPosition({ 0.0f, 7.5f, -20.0f });
	camera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mWorld.Create("../../Assets/Templates/Batman.json", "Jimmy");

	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", BoneVertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 0.25f, 0.25f, 0.25f, 1.0f };

	mMaterial.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mMaterial.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
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
	mPhysicsWorld.AddOBB(mWorld.Find("Jimmy").Get()->GetComponent<ColliderComponent>()->GetOBB());

	mCloak.Initialize(*mWorld.Find("Jimmy").Get(),"../../Assets/Textures/BatTexture.png", 4, 7);
	mSpark.Initialize("../../Assets/Textures/Sun.jpg", 20 ,0.2f);
	mRope.Initialize("../../Assets/Textures/BatTexture.png", RopeRadius, static_cast<uint32_t>(RopeLength));
	mBomb.Initialize("../../Assets/Textures/BatTexture.png", BombRadius);
}

void GameState::Terminate()
{
	mWorld.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mBomb.Terminate();
	mCloak.Terminate();
	mSpark.Terminate();
	mPhysicsWorld.Clear();
	mRope.Terminate();
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
		mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->Update(deltaTime);
	}
	mBomb.Update(deltaTime);
	mPhysicsWorld.Update(deltaTime);
	mCloak.Update(deltaTime, true);
	mSpark.Update(deltaTime);
	mRope.Update(deltaTime, Vector3{ position.x + (3.0f * Normalize(velocity).x),position.y + 4.0f, position.z +(3.0f * Normalize(velocity).z) });
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		camera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	position = mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition();
	rotation = Matrix4::RotationQuaternion(mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetRotation());
	mRightToe = FindBone(mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->GetModel().mSkeleton, "mixamorig1:RightFoot");

	if (mCloak.IsActive())
		mCloak.SetPosition();

	if (mTime < 1.3f)
	{
		mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->SetAnimationSpeed(0.5f);
		if (isKicked == true)
		{
			mSpark.ShowSpark(position + GetTranslation(mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->GetBoneMatrices()[mRightToe->index] * rotation) * 0.04f, velocity,3.0f);
			isKicked = false;
		}

		if (isThrew == true)
		{
			mBomb.ShowBomb(position, velocity, BombLifeTime, BombRadius, BombBounce, BombPower, BombThrowDist);
			isThrew = false;
		}
		if (isRope == true)
		{
			mRope.ShowRope(position, velocity, RopeRadius, RopePower, static_cast<uint32_t>(RopeLength), RopeBounce, RopeThrowDist, RopeLifeTime);
			isRope = false;
		}
	}

	if (mTime > 0.1f && stopAnimation == false)
		return;

	if (!isJump)
	{
		if (mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y > 0.1f)
		{
			mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y += mGravity * deltaTime;
			accelation.y += mGravity * deltaTime;
			if (mCloak.IsActive())
				mCloak.SetVelocity(-accelation);
		}
	}
	else
	{
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y -= 1.2f * mGravity * deltaTime;
		accelation.y -= mGravity * deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
		if (mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y > 5.0f)
			isJump = false;
	}
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		camera.Walk(kMoveSpeed * deltaTime);

		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->SetRotation(180.0f);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.z += kMoveSpeed * deltaTime;
		accelation.z += deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		camera.Walk(-kMoveSpeed * deltaTime);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->SetRotation(0.0f);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.z -= kMoveSpeed * deltaTime;
		accelation.z -= deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		camera.Strafe(kMoveSpeed * deltaTime);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->SetRotation(-90.0f);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.x += kMoveSpeed * deltaTime;
		accelation.x += deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		camera.Strafe(-kMoveSpeed * deltaTime);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->SetRotation(90.0f);
		mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.x -= kMoveSpeed * deltaTime;
		accelation.x -= deltaTime;
		if (mCloak.IsActive())
			mCloak.SetVelocity(-accelation);
	}
	if (IsZero(accelation) == false)
		velocity = accelation * deltaTime;
	if (mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->pos.y < 0.1f)
	{
		if (Magnitude(accelation) != 0.0f)
		{
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->PlayAnimation(1);
			//mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->SetRotation(Quaternion::RotationFromTo(mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetPosition(), accelation));
		}
		else
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->PlayAnimation(0);
		if (inputSystem->IsKeyPressed(KeyCode::Z))
		{
			isKicked = true;
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->SetAnimationTime(0.0f);
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->PlayAnimation(3.0f);
			mTime = 1.75f;
			if (mCloak.IsActive())
				mCloak.SetVelocity(-accelation * deltaTime);
		}

		if (inputSystem->IsKeyPressed(KeyCode::X))
		{
			if (ActiveBomb)
			{
				mBomb.Initialize("../../Assets/Textures/BatTexture.png", BombRadius);
				isThrew = true;
			}
			else if (ActiveRope)
			{
				mRope.Initialize("../../Assets/Textures/BatTexture.png", RopeRadius,static_cast<uint32_t>(RopeLength));
				isRope = true;
			}
			
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->SetAnimationTime(0.0f);
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->SetAnimationSpeed(1.5f);
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->PlayAnimation(4);
			mTime = 1.75f;
		}

		if (inputSystem->IsKeyPressed(KeyCode::SPACE))
		{
			isJump = true;
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->SetAnimationTime(0.0f);
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->SetAnimationSpeed(0.5f);
			mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->PlayAnimation(2);
		}
		if (inputSystem->IsKeyPressed(KeyCode::C))
		{
			isCloak = !isCloak;
			mCloak.Active(isCloak);
			if (mCloak.IsActive())
				mCloak.ShowCloth();
			else
				mCloak.Clear();
		}
	}
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
	auto matWorld = mWorld.Find("Jimmy").Get()->GetComponent<TransformComponent>()->GetTransform();

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mCamera->GetActiveCamera().GetPosition();
	mTransformBuffer.Update(&transformData);

	mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->Render();

	mCloak.Render(mCamera->GetActiveCamera());
	mSpark.Render(mCamera->GetActiveCamera());
	mRope.Render(mCamera->GetActiveCamera());
	mBomb.Render(mCamera->GetActiveCamera());

	SimpleDraw::AddGroundPlane(100.0f, 2.0f, Colors::Magenta);
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
		if (ImGui::CollapsingHeader("Debug UI Option", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Checkbox("Set Skeleton", &showSkeleton))
				mWorld.Find("Jimmy").Get()->GetComponent<ModelComponent>()->EnableDebug();
			if (ImGui::Checkbox("Show DebugUI of particles", &showDebugUI))
			{}
			if (ImGui::Checkbox("OBB Debug UI active/inactive", &OBBcollision))
				mWorld.Find("Jimmy").Get()->GetComponent<ColliderComponent>()->Active(OBBcollision);
		}

		if (ImGui::CollapsingHeader("Batman Weapon Option", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::CollapsingHeader("BatBomb Option", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (mBomb.IsActive() == false)
				{
					if (ImGui::Checkbox("BatBomb", &ActiveBomb))
						if (ActiveBomb)
							ActiveRope = false;
					ImGui::DragFloat("BatBomb Bounce##BatBomb", &BombBounce, 0.1f, 0.1f, 2.0f);
					ImGui::DragFloat("BatBomb Life Time##BatBomb", &BombLifeTime, 1.0f, 0.1f, 7.0f);
					ImGui::DragFloat("BatBomb Power##BatBomb", &BombPower, 0.1f, 0.1f, 10.0f);
					ImGui::DragFloat("BatBomb Radius##BatBomb", &BombRadius, 0.1f, 0.1f, 10.0f);
					ImGui::DragFloat("BatBomb Thorw Distance##BatBomb", &BombThrowDist, 0.1f, 0.1f, 10.0f);
				}
			}
			if (ImGui::CollapsingHeader("BatRope Option", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (mRope.IsActive() == false)
				{
					if (ImGui::Checkbox("BatRope", &ActiveRope))
						if (ActiveRope)
							ActiveBomb = false;
					ImGui::DragFloat("BatRope Bounce##BatRope", &RopeBounce, 0.1f, 0.1f, 2.0f);
					ImGui::DragInt("BatRope Length##BatRope", &RopeLength, 1.0f, 1, 100);
					ImGui::DragFloat("BatRope Life Time##BatRope", &RopeLifeTime, 1.0f, 0.1f, 7.0f);
					ImGui::DragFloat("BatRope Power##BatRope", &RopePower, 0.1f, 0.1f, 2.0f);
					ImGui::DragFloat("BatRope Radius##BatRope", &RopeRadius, 0.1f, 0.1f, 10.0f);
					ImGui::DragFloat("BatRope Thorw Distance##BatRope", &RopeThrowDist, 0.1f, 0.1f, 10.0f);
				}
			}
		}

	}
	mCloak.DebugUI(showDebugUI);
	mSpark.DebugUI(showDebugUI);
	mBomb.DebugUI(showDebugUI);
	mRope.DebugUI(showDebugUI);
	ImGui::End();
}