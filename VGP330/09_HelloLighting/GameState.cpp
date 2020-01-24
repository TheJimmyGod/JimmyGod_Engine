#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,0.0f,-50.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
	mMesh = MeshBuilder::CreateSpherePN(15);
	mMeshBuffer.Initialize(mMesh);
	mConstant.Initialize(sizeof(Matrix4));
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f };
	mDirectionalLight.diffuse = { 0.7f };
	mDirectionalLight.specular = { 0.5f };

	mMaterial.ambient = { 0.3f };
	mMaterial.diffuse = { 0.7f };
	mMaterial.specular = { 0.5f };
	mMaterial.power = { 1.0f };

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Clamp);
	mMeshX = MeshBuilder::CreateSpherePX(1000, 12, 360, true);
	mDomeMeshBuffer.Initialize(mMeshX);
	mSpace.Initialize("../../Assets/Textures/Sky.jpg");


	mVertexShader.Initialize("../../Assets/Shaders/DoPhongLighting.fx", VertexPN::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoPhongLighting.fx");

	mDomeVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mDomePixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");
}

void GameState::Terminate()
{
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
	mSampler.Bind();
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
	//mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
	//mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);

	//SimpleDraw::AddDisplacement(25.0f);
	//SimpleDraw::AddBox(15, Colors::Aqua);
	SimpleDraw::AddSphere(200, 12, 720, Colors::White);

	//mRotation += deltaTime;
}

void GameState::Render()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();


	auto matWorld0 = Matrix4::RotationY(mRotation.y);
	auto matTranslation0 = Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f));
	auto matSpace = matTranslation0 * matWorld0;
	auto matWVP = Transpose(matSpace* matView * matProj);

	mDomeVertexShader.Bind();
	mDomePixelShader.Bind();
	mSampler.Bind();

	mSpace.Bind();
	mConstant.Update(&matWVP);
	mConstant.BindVS(0);
	mDomeMeshBuffer.Draw();

	auto matTrans = Matrix4::Translation({ 0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	auto matWorld = matRot * matTrans;


	mVertexShader.Bind();
	mPixelShader.Bind();

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mCamera.GetPosition();

	mTransformBuffer.Update(&transformData);
	mTransformBuffer.BindVS(0);

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	
	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	
	mMeshBuffer.Draw();
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light"))
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
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f,1.0f,100.0f);
	}
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	ImGui::End();
}
