#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace std;
using namespace Math;

class GameState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void DrawScene();
	void PostProcess();
private:
	Camera mDefaultCamera;
	Camera mLightCamera;
	Camera* mActiveCamera = nullptr;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

	BlendState mBlendState;
	
	MeshPX mNDCMesh;
	RenderTarget mRenderTarget;
	MeshBuffer mScreenQuadBuffer;
	VertexShader mPostProcessingVertexShader;
	PixelShader mPostProcessingPixelShader;

	struct TransformData
	{
		JimmyGod::Math::Matrix4 world;
		JimmyGod::Math::Matrix4 wvp;
		JimmyGod::Math::Vector3 viewPosition;
		float padding;
	};

	struct SettingsData
	{
		float specularWeight = 1.0f;
		float bumpMapWeight = 10.0f;
		float normalMapWeight = 1.0f;
		float padding;
	};

	using TransformBuffer = JimmyGod::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::DirectionalLight>;
	using MaterialBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::Material>;
	using SettingsBuffer = JimmyGod::Graphics::TypedConstantBuffer<SettingsData>;

	TransformBuffer mTransformBuffer; 
	LightBuffer mLightBuffer; 
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;

	SettingsData mSettings;

	DirectionalLight mDirectionalLight;
	Material mMaterial;
	
	float mCloudRotation = 0.0f;

	Mesh mGroundMesh;
	MeshBuffer mGroundMeshBuffer;

	Mesh mTankMesh;
	MeshBuffer mTankMeshBuffer;

	Texture mGroundTexture;
	Sampler mSampler;
	Vector3 mRotation = 0.0f;
	Vector3 mTankRot = 0.0f;
	Vector3 mTankPos = 0.0f;
};