#include "Precompiled.h"
#include "RenderService.h"

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

using namespace JimmyGod;
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;

META_DERIVED_BEGIN(RenderService, Service)
	META_NO_FIELD
META_CLASS_END

void JimmyGod::RenderService::Initialize()
{
	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", BoneVertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

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

	//constexpr uint32_t depthMapSize = 4096;
	//mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);
	//mDepthMapVertexShader.Initialize("../../Assets/Shaders/DepthMap.fx", Vertex::Format);
	//mDepthMapPixelShader.Initialize("../../Assets/Shaders/DepthMap.fx");
	//mDepthMapConstantBuffer.Initialize();
	//mShadowConstantBuffer.Initialize();
}

void JimmyGod::RenderService::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mSettingsBuffer.Terminate();

	//mShadowConstantBuffer.Terminate();
	//mDepthMapConstantBuffer.Terminate();
	//mDepthMapPixelShader.Terminate();
	//mDepthMapVertexShader.Terminate();
	//mDepthMapRenderTarget.Terminate();
}

void JimmyGod::RenderService::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;
	/*
	mLightCamera->SetDirection(mDirectionalLight.direction);
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
	auto defaultMatView = mDefaultCamera->GetViewMatrix();
	auto defaultMatProj = mDefaultCamera->GetPerspectiveMatrix();
	auto invViewProj = Inverse(defaultMatView * defaultMatProj);
	for (auto& vertex : mViewFrustumVertices)
	{
		vertex = TransformCoord(vertex, invViewProj);
	}

	auto lightLook = mLightCamera->GetDirection();
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
	mLightCamera->SetPosition(
		lightSide * (minX + maxX) * 0.5f +
		lightUp * (minY + maxY) * 0.5f +
		lightLook * (minZ + maxZ) * 0.5f
	);
	mLightCamera->SetNearPlane(minZ - 300.0f);
	mLightCamera->SetFarPlane(maxZ);
	mLightProjectMatrix = mLightCamera->GetOrthographioMatrix(maxX - minX, maxY - minY);
	*/
}

void JimmyGod::RenderService::Render()
{
	//mDepthMapRenderTarget.BeginRender();
	//DrawDepthMap();
	//mDepthMapRenderTarget.EndRender();

	mSampler.BindVS();
	mSampler.BindPS();

	mVertexShader.Bind();
	mPixelShader.Bind();

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(&mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

}

void JimmyGod::RenderService::DrawDepthMap()
{
	mDepthMapVertexShader.Bind();
	mDepthMapPixelShader.Bind();

	auto matViewLight = mLightCamera->GetViewMatrix();
	auto matProjLight = mLightProjectMatrix;// mLightCamera.GetPerspectiveMatrix();

	mDepthMapConstantBuffer.BindVS(0);

	//for (auto& go : mGameObjects)
	//{
	//	auto matTrans = Matrix4::Translation(go->GetComponent<TransformComponent>()->pos);
	//	auto matRot = Matrix4::RotationQuaternion(go->GetComponent<TransformComponent>()->rot);
	//	auto matWorld = matRot * matTrans;
	//	auto wvp = Transpose(matWorld * matViewLight * matProjLight);
	//	mDepthMapConstantBuffer.Update(&wvp);
	//}
}
