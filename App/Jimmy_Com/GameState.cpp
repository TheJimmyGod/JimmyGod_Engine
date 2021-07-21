#include "GameState.h"

#include <ImGui/Inc/imgui.h>

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;

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
	mPhysicsWorld.Clear();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	auto& camera = mCamera->GetActiveCamera();
	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;

	fps = 1.0f / deltaTime;
	mWorld.Update(deltaTime);
	mPhysicsWorld.Update(deltaTime);

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		camera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		camera.Walk(kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		camera.Walk(-kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		camera.Strafe(kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		camera.Strafe(-kMoveSpeed * deltaTime);
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

	SimpleDraw::AddGroundPlane(100.0f, 2.0f, Colors::Magenta);
	SimpleDraw::Render(mCamera->GetActiveCamera());
}

void GameState::DebugUI()
{
	mWorld.DebugUI();
	static bool OBBcollision = true;

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("fps: %.2f", fps);
		if (ImGui::CollapsingHeader("Debug UI Option", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Checkbox("OBB Debug UI active/inactive", &OBBcollision))
				mWorld.Find("Jimmy").Get()->GetComponent<ColliderComponent>()->Active();
		}
	}
	ImGui::End();
}