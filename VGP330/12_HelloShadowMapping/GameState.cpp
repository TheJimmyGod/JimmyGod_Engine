#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);

	mTankPos = { 0.0f, 3.5f, 0.0f };

	mDefaultCamera.SetPosition({ 0.0f,0.0f,-100.0f });
	mDefaultCamera.SetDirection({ 0.0f,0.0f,1.0f });
	mDefaultCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mLightCamera.SetDirection(Normalize({ 1.0f,-1.0f,1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);

	mActiveCamera = &mDefaultCamera;

	OBJLoader::Load("../../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(mTankMesh);

	mGroundMesh = MeshBuilder::CreatePlane(20.0f);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.5f };
	mDirectionalLight.diffuse = { 0.5f };
	mDirectionalLight.specular = { 0.5f };

	mMaterial.ambient = { 0.5f };
	mMaterial.diffuse = { 0.5f };
	mMaterial.specular = { 0.5f };
	mMaterial.power = { 60.0f };
	mSettings.bumpMapWeight = { 10.0f };
	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mGroundTexture.Initialize("../../Assets/Textures/Ground.jpg");
	mVertexShader.Initialize("../../Assets/Shaders/DoPhongShading.fx",Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoPhongShading.fx");
	
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(),
		graphicsSystem->GetBackBufferHeight(),RenderTarget::Format::RGBA_U8);
	mNDCMesh = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mNDCMesh);
	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcess.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSNoProcessing");
}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mScreenQuadBuffer.Terminate();
	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mTankMeshBuffer.Terminate();
	mSampler.Terminate();
	mBlendState.Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 100.5f;
	const float kTurnSpeed = 0.5f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mDefaultCamera.Walk(kMoveSpeed*deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mDefaultCamera.Walk(-kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mDefaultCamera.Strafe(-kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mDefaultCamera.Strafe(kMoveSpeed * deltaTime);
	}
	mCloudRotation += 0.0001f;
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
	DrawScene();
	mRenderTarget.EndRender();

	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);
}

void GameState::DebugUI()
{
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool lightCamera = mActiveCamera == &mLightCamera;
		if (ImGui::Checkbox("Use Light Camera", &lightCamera))
		{
			mActiveCamera = lightCamera ? &mLightCamera : &mDefaultCamera;
		}
	}
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionalLight.direction.x, 0.01f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionalLight.direction.y, 0.01f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionalLight.direction.z, 0.01f);

		if (directionChanged)
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.x);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.x);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.x);
	}
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f,1.0f,100.0f);
	}
	if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static bool specularMap = true;
		static bool normalMap = true;
		ImGui::SliderFloat("Displacement", &mSettings.bumpMapWeight, 0.0f, 100.0f);
		if (ImGui::Checkbox("Normal", &normalMap))
		{
			mSettings.normalMapWeight = normalMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Specular", &specularMap))
		{
			mSettings.specularWeight = specularMap ? 1.0f : 0.0f;
		}
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mTankRot.x, 0.01f);
	}
	ImGui::End();
}

void GameState::DrawScene()
{
	auto matView = mDefaultCamera.GetViewMatrix();
	auto matProj = mDefaultCamera.GetPerspectiveMatrix();
	auto matTrans = Matrix4::Translation({ -1.25f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mTankRot.x) * Matrix4::RotationY(mTankRot.y);
	auto matWorld = matRot * matTrans;
	
	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mDefaultCamera.GetPosition();

	mTransformBuffer.Update(&transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(&mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTankMeshBuffer.Draw();



	SimpleDraw::Render(*mActiveCamera);
}

void GameState::PostProcess()
{
	mPostProcessingVertexShader.Bind();
	mPostProcessingPixelShader.Bind();

	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}

// Blending : find color = Source color * SourceBlend + destinationColor * destinationBlend
// destinationColor = represent the pixel on the backbuffer