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
	void DrawDepthMap();
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

	// Shadow

	using DepthMapConstantBuffer = JimmyGod::Graphics::TypedConstantBuffer<Math::Matrix4>;
	using ShadowConstantBuffer = JimmyGod::Graphics::TypedConstantBuffer<Math::Matrix4>;
	RenderTarget mDepthMapRenderTarget;
	VertexShader mDepthVertexShader;
	PixelShader mDepthPixelShader;
	DepthMapConstantBuffer mDepthMapConstantBuffer;
	ShadowConstantBuffer mShadowConstantBuffer;
	struct TransformData
	{
		JimmyGod::Math::Matrix4 world;
		JimmyGod::Math::Matrix4 wvp;
		JimmyGod::Math::Vector3 viewPosition;
		float padding;
	};

	struct SettingsData
	{
		float specularMapWeight = 1.0f;
		float bumpMapWeight = 10.0f;
		float normalMapWeight = 1.0f;
		float aoMapWeight = 1.0f;
		float brightness = 1.0f;
		float depthBias = 0.0003f;
		int useShadow = 1;
		float padding;
	};

	struct ShadowData
	{
		Matrix4 wvpLight;
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

	Mesh mGroundMesh;
	MeshBuffer mGroundMeshBuffer;

	Mesh mTankMesh;
	MeshBuffer mTankMeshBuffer;

	Texture mTankSpecualr;
	Texture mTankDiffuse;
	Texture mTankNormal;
	Texture mTankAoMap;
	Texture mTankDisplacementMap;
	Texture mGroundTexture;
	Sampler mSampler;
	Vector3 mRotation = 0.0f;
	Vector3 mTankRot = 0.0f;
	Vector3 mTankPos = 0.0f;
};