#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

static bool ActiveRadialBlur = false;
static bool ActiveGaussianBlur = false;
static bool ActiveGreyscale = false;
static bool ActiveNegative = false;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);

	mCamera.SetPosition({ 0.0f,0.0f,-100.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
	mMesh = MeshBuilder::CreateSphere(50.0f,64,64);
	mMeshBuffer.Initialize(mMesh);
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f,1.0f };

	mMaterial.ambient = { 1.0f };
	mMaterial.diffuse = { 1.0f };
	mMaterial.specular = { 1.0f };
	mMaterial.power = { 10.0f };
	mSettings.bumpMapWeight = { 0.2f };
	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);

	std::filesystem::path assets = "../../Assets/Shaders/Earth.fx";
	mBlendState.Initialize(BlendState::Mode::AlphaPremultiplied);

	mEarth.Initialize("../../Assets/Textures/JimmyEarth.jpg");
	mEarthSpecualr.Initialize("../../Assets/Textures/earth_spec.jpg");
	mEarthDisplacement.Initialize("../../Assets/Textures/earth_bump.jpg");
	mNightMap.Initialize("../../Assets/Textures/earth_lights.jpg");
	mEarthCould.Initialize("../../Assets/Textures/earth_clouds.jpg");


	mEarthVertexShader.Initialize(assets, "VSEarth",Vertex::Format);
	mEarthPixelShader.Initialize(assets, "PSEarth");

	mCloudVertexShader.Initialize(assets, "VSCloud", Vertex::Format);
	mCloudPixelShader.Initialize(assets, "PSCloud");

	mNormalMap.Initialize("../../Assets/Textures/earth_normal.jpg");

	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(),
		graphicsSystem->GetBackBufferHeight(),RenderTarget::Format::RGBA_U8);
	mNDCMesh = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mNDCMesh);
	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcess.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSNoProcessing");

	// Space
	mSkyDome.Intialize("../../Assets/Textures/Space.jpg", 1000, 12, 360, {0.0f,0.0f,0.0f});

	// Moon
	mMoon.Initialize("../../Assets/Textures/Moon.jpg", Vector3{ 95.5f,0.0f,0.0f }, 15.0f, 64.0f, 64.0f);

	// Mercury
	mMercury.Initialize("../../Assets/Textures/Mercury.jpg", Vector3{ 130.0f,0.0f,0.0f }, 24.0f, 64.0f, 64.0f);

	// Venus
	mVenus.Initialize("../../Assets/Textures/Venus.jpg", Vector3{ 170.0f,0.0f,0.0f }, 28.0f, 64.0f, 64.0f);

	// Mars
	mMars.Initialize("../../Assets/Textures/Mars.jpg", Vector3{ 200.0f,0.0f,0.0f }, 32.0f, 64.0f, 64.0f);

	// Jupitor
	mJupiter.Initialize("../../Assets/Textures/Jupiter.jpg", Vector3{ 280.0f,0.0f,0.0f }, 62.0f, 64.0f, 64.0f);

	// Saturn
	mSaturn.Initialize("../../Assets/Textures/Saturn.jpg", Vector3{ 360.0f,0.0f,0.0f }, 27.0f, 64.0f, 64.0f);

	// Uranos
	mUranos.Initialize("../../Assets/Textures/Uranos.jpg", Vector3{ 420.0f,0.0f,0.0f }, 25.0f, 64.0f, 64.0f);

	// Neptune
	mNeptune.Initialize("../../Assets/Textures/Neptune.jpg", Vector3{ 480.0f,0.0f,0.0f }, 23.0f, 64.0f, 64.0f);
}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mScreenQuadBuffer.Terminate();
	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mNormalMap.Terminate();
	mEarthVertexShader.Terminate();
	mEarthPixelShader.Terminate();
	mCloudPixelShader.Terminate();
	mCloudVertexShader.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mMeshBuffer.Terminate();
	mSampler.Terminate();
	mEarth.Terminate();
	mEarthDisplacement.Terminate();
	mEarthSpecualr.Terminate();
	mNightMap.Terminate();
	mBlendState.Terminate();
	mEarthCould.Terminate();
	mSkyDome.Terminate();
	mMoon.Terminate();
	mMercury.Terminate();
	mVenus.Terminate();
	mMars.Terminate();
	mJupiter.Terminate();
	mSaturn.Terminate();
	mUranos.Terminate();
	mNeptune.Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 100.5f;
	const float kTurnSpeed = 0.5f;

	mAccelation = Vector3::Zero;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(kMoveSpeed*deltaTime);
		mAccelation += kMoveSpeed;
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-kMoveSpeed * deltaTime);
		mAccelation -= kMoveSpeed;
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(kMoveSpeed * deltaTime);
	}
	mCloudRotation += 0.0001f;

	mVelocity += mAccelation * deltaTime;
	auto Speed = Magnitude(mVelocity);
	if (ActiveRadialBlur)
	{
		if (Speed > 30.0f && Speed < 500.0f)
			mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSRadialBlur");
		else
			mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSNoProcessing");
	}


	mSkyDome.Update(mCamera);
	mMoon.Update(deltaTime);
	mMercury.Update(deltaTime);
	mVenus.Update(deltaTime);
	mMars.Update(deltaTime);
	mJupiter.Update(deltaTime);
	mSaturn.Update(deltaTime);
	mUranos.Update(deltaTime);
	mNeptune.Update(deltaTime);
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
	DrawScene();
	mRenderTarget.EndRender();
	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);

	std::string str1 = (ActiveRadialBlur) ? "Active Radial Blur" : "Inactive Radial Blur";
	SpriteRenderManager::Get()->DrawScreenText(str1.c_str(), 1050.0f, 600.0f, 15.0f, (!ActiveRadialBlur) ? Colors::WhiteSmoke : Colors::Magenta);

	std::string str2 = (ActiveGaussianBlur) ? "Active Gaussian Blur" : "Inactive Gaussian Blur";
	SpriteRenderManager::Get()->DrawScreenText(str2.c_str(), 1050.0f, 620.0f, 15.0f, (!ActiveGaussianBlur) ? Colors::WhiteSmoke : Colors::Magenta);

	std::string str3 = (ActiveGreyscale) ? "Active Greyscale" : "Inactive Greyscale";
	SpriteRenderManager::Get()->DrawScreenText(str3.c_str(), 1050.0f, 640.0f, 15.0f, (!ActiveGreyscale) ? Colors::WhiteSmoke : Colors::Magenta);

	std::string str4 = (ActiveNegative) ? "Active Negative" : "Inactive Negative";
	SpriteRenderManager::Get()->DrawScreenText(str4.c_str(), 1050.0f, 660.0f, 15.0f, (!ActiveNegative) ? Colors::WhiteSmoke : Colors::Magenta);
}

void GameState::DebugUI()
{
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
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
		ImGui::SliderFloat("Displacement", &mSettings.bumpMapWeight, 0.2f, 100.0f);
		if (ImGui::Checkbox("Normal", &normalMap))
		{
			mSettings.normalMapWeight = normalMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Specular", &specularMap))
		{
			mSettings.specularWeight = specularMap ? 1.0f : 0.0f;
		}

		if (ImGui::Button("Radial Blur"))
		{
			if (ActiveGaussianBlur || ActiveGreyscale || ActiveNegative)
			{
				ActiveRadialBlur = false;
			}
			else
			{
				ActiveRadialBlur = !ActiveRadialBlur;
				if (!ActiveRadialBlur)
					mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSNoProcessing");
			}

		}
		if (ImGui::Button("Gaussian"))
		{
			if (ActiveRadialBlur || ActiveGreyscale || ActiveNegative)
			{
				ActiveGaussianBlur = false;
			}
			else
			{
				ActiveGaussianBlur = !ActiveGaussianBlur;
				if (!ActiveGaussianBlur)
					mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSNoProcessing");
				else
					mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSGaussian");
			}
		}
		if (ImGui::Button("GreyScale"))
		{
			if (ActiveRadialBlur || ActiveGaussianBlur || ActiveNegative)
			{
				ActiveGreyscale = false;
			}
			else
			{
				ActiveGreyscale = !ActiveGreyscale;
				if (!ActiveGreyscale)
					mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSNoProcessing");
				else
					mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSGreyScale");
			}

		}
		if (ImGui::Button("Negative"))
		{
			if (ActiveRadialBlur || ActiveGreyscale || ActiveGaussianBlur)
			{
				ActiveNegative = false;
			}
			else
			{
				ActiveNegative = !ActiveNegative;
				if (!ActiveNegative)
					mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSNoProcessing");
				else
					mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSNegative");
			}
		}
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	ImGui::End();
}

void GameState::DrawScene()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	auto matTrans = Matrix4::Translation({ -1.25f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	auto matWorld = matRot * matTrans;

	mSkyDome.Render(mCamera);

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mCamera.GetPosition();

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

	mEarthPixelShader.Bind();
	mEarthVertexShader.Bind();

	mEarth.BindPS(0);
	mEarthSpecualr.BindPS(1);
	mEarthDisplacement.BindVS(2);
	mNormalMap.BindPS(3);
	mNightMap.BindPS(4);

	BlendState::ClearState();

	mMeshBuffer.Draw();

	// --- Cloud
	matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y + mCloudRotation);
	matWorld = matRot * matTrans;

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mCloudPixelShader.Bind();
	mCloudVertexShader.Bind();

	mEarthCould.BindPS(5);
	mEarthCould.BindVS(5);
	mBlendState.Set();
	mMeshBuffer.Draw();

	mMoon.Render(mCamera, 1.5f, 0.15f, matWorld);
	mMercury.Render(mCamera, 2.1f, 0.2f, matWorld);
	mVenus.Render(mCamera, 2.6f, 0.3f, matWorld);
	mMars.Render(mCamera, 3.1f, 0.4f, matWorld);
	mJupiter.Render(mCamera, 3.6f, 0.5f, matWorld);
	mSaturn.Render(mCamera, 4.1f, 0.3f, matWorld);
	mUranos.Render(mCamera, 4.6f, 0.2f, matWorld);
	mNeptune.Render(mCamera, 5.1f, 0.2f, matWorld);
	SimpleDraw::Render(mCamera);
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