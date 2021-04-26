#include "GameState.h"

#include <ImGui/Inc/imgui.h>

using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;

namespace
{
	void SimpleDrawCamera(const Camera& camera)
	{
		auto defaultMatView = camera.GetViewMatrix();
		Vector3 cameraPosition = camera.GetPosition();
		Vector3 cameraRight = { defaultMatView._11, defaultMatView._21, defaultMatView._31 };
		Vector3 cameraUp = { defaultMatView._12, defaultMatView._22, defaultMatView._32 };
		Vector3 cameraLook = { defaultMatView._13, defaultMatView._23, defaultMatView._33 };

		SimpleDraw::AddLine(cameraPosition, cameraPosition + cameraRight, Colors::Red);
		SimpleDraw::AddLine(cameraPosition, cameraPosition + cameraUp, Colors::Green);
		SimpleDraw::AddLine(cameraPosition, cameraPosition + cameraLook, Colors::Blue);
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mDefaultCamera.SetNearPlane(0.1f);
	mDefaultCamera.SetFarPlane(500.0f);
	mDefaultCamera.SetPosition({ 0.0f, 10.0f, -5.0f });
	mDefaultCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDebugCamera.SetNearPlane(0.001f);
	mDebugCamera.SetFarPlane(10000.0f);
	mDebugCamera.SetPosition({ 0.0f, 2.0f, -5.0f });
	mDebugCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mLightCamera.SetDirection(Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);

	mActiveCamera = &mDefaultCamera;

	mModel.Initialize("../../Assets/Models/Mutant_Walking.model");
	
	mGroundMesh = MeshBuilder::CreatePlane(1500.0f, 16, 16, false);
	mGroundMeshBuffer.Initialize(mGroundMesh);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mPostProcessingSettingsBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };

	mMaterial.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
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

	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", BoneVertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);

	mGroundDiffuseMap.Initialize("../../Assets/Textures/grass_2048.jpg");

	auto graphicsSystem = GraphicsSystem::Get();

	constexpr uint32_t depthMapSize = 4096;
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);
	mDepthMapVertexShader.Initialize("../../Assets/Shaders/DepthMap.fx", BoneVertex::Format);
	mDepthMapPixelShader.Initialize("../../Assets/Shaders/DepthMap.fx");
	mDepthMapConstantBuffer.Initialize();
	mShadowConstantBuffer.Initialize();

	mRenderTarget.Initialize(
		graphicsSystem->GetBackBufferWidth(),
		graphicsSystem->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);

	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcess.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx", "PSNoProcessing");

	mTerrain.Initialize(20, 20, 1.0f);
	mTerrain.SetHeightScale(1.0f);
	//mTerrain.LoadHeightMap("../../Assets/HeightMaps/heightmap_200x200.raw");

	mBoneTransformBuffer.Initialize();
	mAnimator.Initialize(mModel);
	mAnimator.ComputeBindPose();
	mAnimator.PlayAnimation(0);

	mSkyDome.Intialize("../../Assets/Textures/Space.jpg", 3000, 12, 360, mActiveCamera->GetPosition());
}

void GameState::Terminate()
{
	mModel.Terminate();
	mBoneTransformBuffer.Terminate();
	mModel.Terminate();
	mTerrain.Terminate();
	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	mShadowConstantBuffer.Terminate();
	mDepthMapConstantBuffer.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapVertexShader.Terminate();
	mDepthMapRenderTarget.Terminate();
	mGroundDiffuseMap.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mPostProcessingSettingsBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mGroundMeshBuffer.Terminate();
	mSkyDome.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;

	if (inputSystem->IsKeyDown(KeyCode::W))
		mActiveCamera->Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mActiveCamera->Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mActiveCamera->Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mActiveCamera->Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mActiveCamera->Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mActiveCamera->Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	mPostProcessSettings.time += deltaTime;

	mLightCamera.SetDirection(mDirectionalLight.direction);
	//mLightCamera.SetPosition(mLightCamera.GetDirection() * -50.0f);

	mViewFrustumVertices =
	{
		// Near plane
		{ -1.0f, -1.0f, 0.0f },
		{ -1.0f,  1.0f, 0.0f },
		{  1.0f,  1.0f, 0.0f },
		{  1.0f, -1.0f, 0.0f },

		// Far plane
		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f,  1.0f, 1.0f },
		{  1.0f,  1.0f, 1.0f },
		{  1.0f, -1.0f, 1.0f },
	};
	auto defaultMatView = mDefaultCamera.GetViewMatrix();
	auto defaultMatProj = mDefaultCamera.GetPerspectiveMatrix();
	auto invViewProj = Inverse(defaultMatView * defaultMatProj);
	for (auto& vertex : mViewFrustumVertices)
	{
		vertex = TransformCoord(vertex, invViewProj);
	}

	auto lightLook = mLightCamera.GetDirection();
	auto lightSide = Normalize(Cross(Vector3::YAxis, lightLook));
	auto lightUp = Normalize(Cross(lightLook, lightSide));
	float minX = FLT_MAX, maxX = -FLT_MAX;
	float minY = FLT_MAX, maxY = -FLT_MAX;
	float minZ = FLT_MAX, maxZ = -FLT_MAX;
	for (auto& vertex : mViewFrustumVertices)
	{
		float projectX = Dot(lightSide, vertex);
		minX = Min(minX, projectX);
		maxX = Max(maxX, projectX);
		float projectY = Dot(lightUp, vertex);
		minY = Min(minY, projectY);
		maxY = Max(maxY, projectY);
		float projectZ = Dot(lightLook, vertex);
		minZ = Min(minZ, projectZ);
		maxZ = Max(maxZ, projectZ);
	}
	mLightCamera.SetPosition(
		lightSide * (minX + maxX) * 0.5f +
		lightUp * (minY + maxY) * 0.5f +
		lightLook * (minZ + maxZ) * 0.5f
	);
	mLightCamera.SetNearPlane(minZ - 300.0f);
	mLightCamera.SetFarPlane(maxZ);
	mLightProjectMatrix = mLightCamera.GetOrthographioMatrix(maxX - minX, maxY - minY);

	auto v0 = lightSide * minX + lightUp * minY + lightLook * minZ;
	auto v1 = lightSide * minX + lightUp * maxY + lightLook * minZ;
	auto v2 = lightSide * maxX + lightUp * maxY + lightLook * minZ;
	auto v3 = lightSide * maxX + lightUp * minY + lightLook * minZ;
	auto v4 = lightSide * minX + lightUp * minY + lightLook * maxZ;
	auto v5 = lightSide * minX + lightUp * maxY + lightLook * maxZ;
	auto v6 = lightSide * maxX + lightUp * maxY + lightLook * maxZ;
	auto v7 = lightSide * maxX + lightUp * minY + lightLook * maxZ;

	SimpleDraw::AddLine(v0, v1, Colors::Yellow);
	SimpleDraw::AddLine(v1, v2, Colors::Yellow);
	SimpleDraw::AddLine(v2, v3, Colors::Yellow);
	SimpleDraw::AddLine(v3, v0, Colors::Yellow);

	SimpleDraw::AddLine(v0, v4, Colors::Red);
	SimpleDraw::AddLine(v1, v5, Colors::Red);
	SimpleDraw::AddLine(v2, v6, Colors::Red);
	SimpleDraw::AddLine(v3, v7, Colors::Red);

	SimpleDraw::AddLine(v4, v5, Colors::Red);
	SimpleDraw::AddLine(v5, v6, Colors::Red);
	SimpleDraw::AddLine(v6, v7, Colors::Red);
	SimpleDraw::AddLine(v7, v4, Colors::Red);

	SimpleDrawCamera(mLightCamera);
	fps = 1 / deltaTime;
	if(!stopAnimation)
		mAnimator.Update(deltaTime);

	mSkyDome.Update(*mActiveCamera);
}

void GameState::Render()
{
	mSkyDome.Render(*mActiveCamera);
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
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("fps: %.2f", fps);
		static float mTime = 0.0f;
		static float mCurrentTime = mAnimator.GetTime();
		ImGui::SliderFloat("Duration", &mTime, 0.0f, mModel.mAnimationSet.clips[0].get()->duration);
		if (mTime > 0.1f && !stopAnimation)
		{
			mAnimator.StopAnimation(true);
			mAnimator.SetTime(mTime);
		}
		else if(mTime < 0.1f && stopAnimation)
		{
			mAnimator.StopAnimation(false);
		}

		if (ImGui::Checkbox("Set Skeleton", &showSkeleton))
		{}
		if (ImGui::Checkbox("Stop Animation", &stopAnimation))
		{
			mAnimator.StopAnimation(stopAnimation);
		}
	}
	ImGui::End();
}

void GameState::DrawDepthMap()
{
	mDepthMapVertexShader.Bind();
	mDepthMapPixelShader.Bind();

	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightProjectMatrix;// mLightCamera.GetPerspectiveMatrix();

	mDepthMapConstantBuffer.BindVS(0);

	auto matWorld = Matrix4::Scaling(0.01f);
	auto wvp = Transpose(matWorld * matViewLight * matProjLight);
	mDepthMapConstantBuffer.Update(&wvp);
	mModel.Render();
}

void GameState::DrawScene()
{
	auto matView = mActiveCamera->GetViewMatrix();
	auto matProj = mActiveCamera->GetPerspectiveMatrix();
	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightProjectMatrix;// mLightCamera.GetPerspectiveMatrix();

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(&mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mSampler.BindVS();
	mSampler.BindPS();

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mShadowConstantBuffer.BindVS(4);
	mBoneTransformBuffer.BindVS(5);

	auto matWorld = Matrix4::Scaling(0.01f) * Matrix4::Translation(startingPos);
	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mActiveCamera->GetPosition();
	mTransformBuffer.Update(&transformData);
	BoneTransformData boneTransformData{};

	for (size_t i = 0; i < mAnimator.GetBoneMatrices().size(); ++i)
	{
		boneTransformData.BoneTransforms[i] = Transpose(mModel.mSkeleton.bones[i]->offsetTransform * mAnimator.GetBoneMatrices()[i] /** Matrix4::Translation(pos)*/);
	}
	if(!showSkeleton)
		mModel.Render();
	else
	{
		for (auto& b : mModel.mSkeleton.bones)
		{
			DrawSkeleton(b.get(), mAnimator.GetBoneMatrices(),startingPos, 0.01f);
		}

	}
	mBoneTransformBuffer.Update(&boneTransformData);
	auto wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(&wvpLight);


	auto matWorld2 = Matrix4::Scaling(100.0f) * Matrix4::Translation(Vector3{0.0f,0.0f,0.0f});
	TransformData transformData2;
	transformData2.world = Transpose(matWorld2);
	transformData2.wvp = Transpose(matWorld2 * matView * matProj);
	transformData2.viewPosition = mActiveCamera->GetPosition();
	mTransformBuffer.Update(&transformData2);

	auto wvpLight2 = Transpose(matWorld2 * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(&wvpLight2);

	mGroundDiffuseMap.BindPS(0);

	SettingsData settings;
	settings.specularMapWeight = 0.0f;
	settings.bumpMapWeight = 0.0f;
	settings.normalMapWeight = 0.0f;
	settings.aoMapWeight = 0.0f;
	settings.useShadow = 1;
	mSettingsBuffer.Update(&settings);

	mGroundMeshBuffer.Draw();

	mTerrain.SetDirectionalLight(mDirectionalLight);
	mTerrain.Render(mDefaultCamera);

	SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[1], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[2], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[3], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[0], Colors::White);

	SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[4], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[5], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[6], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[7], Colors::White);

	SimpleDraw::AddLine(mViewFrustumVertices[4], mViewFrustumVertices[5], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[5], mViewFrustumVertices[6], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[6], mViewFrustumVertices[7], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[7], mViewFrustumVertices[4], Colors::White);

	SimpleDrawCamera(mDefaultCamera);

	SimpleDraw::Render(*mActiveCamera);
}

void GameState::PostProcess()
{
	mPostProcessingVertexShader.Bind();
	mPostProcessingPixelShader.Bind();

	mSampler.BindPS();

	auto graphicsSystem = GraphicsSystem::Get();
	mPostProcessSettings.screenWidth = (float)graphicsSystem->GetBackBufferWidth();
	mPostProcessSettings.screenHeight = (float)graphicsSystem->GetBackBufferHeight();
	mPostProcessingSettingsBuffer.Update(&mPostProcessSettings);
	mPostProcessingSettingsBuffer.BindVS(0);
	mPostProcessingSettingsBuffer.BindPS(0);

	mScreenQuadBuffer.Draw();
}
