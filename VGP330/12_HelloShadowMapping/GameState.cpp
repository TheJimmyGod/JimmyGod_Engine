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

	mLightCamera.SetDirection(Normalize({1.0f,-1.0f,1.0f}));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);
	mLightCamera.SetPosition({ 15.0f,50.0f,10.0f });
	mActiveCamera = &mDefaultCamera;

	OBJLoader::Load("../../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(mTankMesh);

	mGroundMesh = MeshBuilder::CreatePlane(200.0f, 10, 10, false);
	mGroundMeshBuffer.Initialize(mGroundMesh);

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
	mMaterial.power = { 1.0f };
	mSettings.bumpMapWeight = { 10.0f };
	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mTankSpecualr.Initialize("../../Assets/Models/Tank/tank_specular.jpg");
	mTankDiffuse.Initialize("../../Assets/Models/Tank/tank_diffuse.jpg");
	mTankNormal.Initialize("../../Assets/Models/Tank/tank_normal.jpg");
	mTankDisplacementMap.Initialize("../../Assets/Models/Tank/tank_gloss.jpg");
	mTankAoMap.Initialize("../../Assets/Models/Tank/tank_ao.jpg");
	mGroundTexture.Initialize("../../Assets/Textures/Ground.jpg");
	mVertexShader.Initialize("../../Assets/Shaders/DoPhongShading.fx",Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoPhongShading.fx");
	

	constexpr uint32_t depthMapSize = 1024;
	mDepthVertexShader.Initialize("../../Assets/Shaders/DepthMap.fx",Vertex::Format);
	mDepthPixelShader.Initialize("../../Assets/Shaders/DepthMap.fx");
	mDepthMapConstantBuffer.Initialize();
	
	mShadowConstantBuffer.Initialize();

	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);

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
	mShadowConstantBuffer.Terminate();
	mTankDisplacementMap.Terminate();
	mTankAoMap.Terminate();
	mShadowConstantBuffer.Terminate();
	mDepthMapRenderTarget.Terminate();
	mDepthVertexShader.Terminate();
	mDepthPixelShader.Terminate();
	mDepthMapConstantBuffer.Terminate();
	mGroundMeshBuffer.Terminate();
	mGroundTexture.Terminate();
	mTankSpecualr.Terminate();
	mTankNormal.Terminate();
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
		mActiveCamera->Walk(kMoveSpeed*deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mActiveCamera->Walk(-kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mActiveCamera->Strafe(-kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mActiveCamera->Strafe(kMoveSpeed * deltaTime);
	}

	
	mLightCamera.SetDirection(mDirectionalLight.direction);
	mLightCamera.SetPosition(mTankPos - mLightCamera.GetDirection() * 50.0f);

}

void GameState::Render()
{
	mDepthMapRenderTarget.BeginRender();
	DrawDepthMap();
	mDepthMapRenderTarget.EndRender();

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

		ImGui::Image(
			mDepthMapRenderTarget.GetShaderResourceView(),
			{ 150.0f,150.0f },
			{ 0.0f,0.0f },
			{ 1.0f,1.0f },
			{ 1.0f,1.0f,1.0f,1.0f },
			{ 1.0f,1.0f,1.0f,1.0f }
		);

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
		static bool specularMap = false;
		static bool normalMap = false;
		static bool aoMap = false;
		static bool useShadow = false;
		ImGui::SliderFloat("Displacement", &mSettings.bumpMapWeight, 0.0f, 100.0f);
		if (ImGui::Checkbox("Normal", &normalMap))
		{
			mSettings.normalMapWeight = normalMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Specular", &specularMap))
		{
			mSettings.specularWeight = specularMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Ambient Occlusion Map", &aoMap))
		{
			mSettings.aoMapWeight = aoMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Shadow", &useShadow))
		{
			mSettings.useShadow = useShadow ? 1 : 0;
		}
		//ImGui::SliderFloat("Depth Bias", &mSettings.depthBias, 0.0f, 0.01f, "%.4f");
		//ImGui::SliderFloat("Brightness", &mSettings.brightness, 1.0f, 10.0f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mTankRot.x, 0.01f);
	}
	ImGui::End();
}

void GameState::DrawScene()
{
	auto matView = mActiveCamera->GetViewMatrix();
	auto matProj = mActiveCamera->GetPerspectiveMatrix();
	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();
	auto matTrans = Matrix4::Translation({ 0.0f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mTankRot.x) * Matrix4::RotationY(mTankRot.y);
	auto matWorld = matRot * matTrans;

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mActiveCamera->GetPosition();

	mTransformBuffer.Update(&transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);


	auto wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(&wvpLight);
	mShadowConstantBuffer.BindPS(4);

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(&mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mSampler.BindPS();
	mSampler.BindVS();
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTankDiffuse.BindPS(0);
	mTankSpecualr.BindPS(1);
	mTankDisplacementMap.BindVS(2);
	mTankNormal.BindPS(3);
	mTankAoMap.BindPS(4);
	mDepthMapRenderTarget.BindPS(5);
	mTankMeshBuffer.Draw();


	matWorld = Matrix4::Identity;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(&transformData);

	wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(&wvpLight);
	mShadowConstantBuffer.BindPS(4);

	mGroundTexture.BindPS(0);

	SettingsData settings;
	settings.bumpMapWeight = 0.0f;
	settings.normalMapWeight = 0.0f;
	settings.specularWeight = 0.0f;
	settings.aoMapWeight = 0.0f;
	settings.useShadow = 1;
	mSettingsBuffer.Update(&settings);
	mSettingsBuffer.BindPS(3);
	mSettingsBuffer.BindVS(3);

	mGroundMeshBuffer.Draw();

	SimpleDraw::Render(*mActiveCamera);
}

void GameState::DrawDepthMap()
{

	mDepthVertexShader.Bind();
	mDepthPixelShader.Bind();

	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();

	auto matTrans = Matrix4::Translation(mTankPos);
	auto matRot = Matrix4::RotationX(mTankRot.x) * Matrix4::RotationY(mTankRot.y);
	auto matWorld = matRot * matTrans;
	auto wvp = Transpose(matWorld * matViewLight * matProjLight);
	mDepthMapConstantBuffer.Update(&wvp);
	mDepthMapConstantBuffer.BindVS(0);

	mTankMeshBuffer.Draw();
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