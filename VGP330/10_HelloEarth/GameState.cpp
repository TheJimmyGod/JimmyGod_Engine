#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,0.0f,-100.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
	mMesh = MeshBuilder::CreateSphere(50.0f,64,64);
	mMeshBuffer.Initialize(mMesh);
	mConstant.Initialize(sizeof(Matrix4));
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
	mAlphaBlend.Initialize();
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mMeshX = MeshBuilder::CreateSpherePX(1000, 12, 360, true);
	mDomeMeshBuffer.Initialize(mMeshX);
	mSpace.Initialize("../../Assets/Textures/Space.jpg");
	mEarth.Initialize("../../Assets/Textures/JimmyEarth.jpg");
	mEarthSpecualr.Initialize("../../Assets/Textures/earth_spec.jpg");
	mEarthDisplacement.Initialize("../../Assets/Textures/earth_bump.jpg");
	mNightMap.Initialize("../../Assets/Textures/earth_lights.jpg");
	mEarthCould.Initialize("../../Assets/Textures/earth_clouds.jpg");

	mVertexShader.Initialize("../../Assets/Shaders/DoPhongShading.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoPhongShading.fx");
	mNormalMap.Initialize("../../Assets/Textures/earth_normal.jpg");
	mDomeVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mDomePixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");
}

void GameState::Terminate()
{
	mSettingsBuffer.Terminate();
	mNormalMap.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mMeshBuffer.Terminate();
	mDomeMeshBuffer.Terminate();
	mSpace.Terminate();
	mConstant.Terminate();
	mDomePixelShader.Terminate();
	mDomeVertexShader.Terminate();
	mSampler.Terminate();
	mEarth.Terminate();
	mEarthDisplacement.Terminate();
	mEarthSpecualr.Terminate();
	mNightMap.Terminate();
	mAlphaBlend.Terminate();
	mEarthCould.Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 100.5f;
	const float kTurnSpeed = 0.5f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(kMoveSpeed*deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(kMoveSpeed * deltaTime);
	}
	//mRotation += deltaTime;
}

void GameState::Render()
{

	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	auto matTranslation0 = Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f));
	auto matSpace = matTranslation0;
	auto matWVP = Transpose(matSpace* matView * matProj);

	mDomeVertexShader.Bind();
	mDomePixelShader.Bind();


	mSpace.BindVS(0);
	mSpace.BindPS(0);
	mConstant.Update(&matWVP);
	mConstant.BindVS(0);
	mConstant.BindPS(0);
	mDomeMeshBuffer.Draw();

	auto matTrans = Matrix4::Translation({ -1.25f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	auto matWorld = matRot * matTrans;
	auto matWorldCloud = Matrix4::Scaling(1.05f) * matRot * matTrans;


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

	mPixelShader.Bind();
	mVertexShader.Bind();

	mEarth.BindPS(0);
	mEarthSpecualr.BindPS(1);
	mEarthDisplacement.BindVS(2);
	mNormalMap.BindPS(3);
	mNightMap.BindPS(4);
	mSampler.BindVS();
	mSampler.BindPS();

	mAlphaBlend.UnBind();
	mMeshBuffer.Draw();

	transformData.world = Transpose(matWorldCloud);
	transformData.wvp = Transpose(matWorldCloud * matView * matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mEarthCould.BindPS(0);
	mEarthCould.BindVS(0);
	mAlphaBlend.Bind();
	mMeshBuffer.Draw();

	SimpleDraw::Render(mCamera);
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
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	ImGui::End();
}
