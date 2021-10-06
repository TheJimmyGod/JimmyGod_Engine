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
		float bumpMapWeight = 1.0f;
		float normalMapWeight = 1.0f;
		float aoMapWeight = 1.0f;
		float brightness = 1.0f;
		int useShadow = 1;
		float depthBias = 0.0f;
		float padding;
	};
	struct PostProcessSettingsData
	{
		float screenWidth = 0.0f;
		float screenHeight = 0.0f;
		float time = 0.0f;
		float padding;
	};


	using TransformBuffer = TypedConstantBuffer<TransformData>;
	using LightBuffer = TypedConstantBuffer<JimmyGod::Graphics::DirectionalLight>;
	using MaterialBuffer = TypedConstantBuffer<JimmyGod::Graphics::Material>;
	using SettingsBuffer = TypedConstantBuffer<SettingsData>;
	using PostProcessingSettingsBuffer = TypedConstantBuffer<PostProcessSettingsData>;
	using DepthMapConstantBuffer = TypedConstantBuffer<JimmyGod::Math::Matrix4>;
	using ShadowConstantBuffer = TypedConstantBuffer<JimmyGod::Math::Matrix4>;
	Camera mCamera;
	MeshBuffer mMeshBuffer;

	VertexShader mEarthVertexShader;
	PixelShader mEarthPixelShader;

	VertexShader mCloudVertexShader;
	PixelShader mCloudPixelShader;

	BlendState mBlendState;

	MeshPX mMeshX;
	
	MeshPX mNDCMesh;
	RenderTarget mRenderTarget;
	MeshBuffer mScreenQuadBuffer;
	VertexShader mPostProcessingVertexShader;
	PixelShader mPostProcessingPixelShader;

	TransformBuffer mTransformBuffer; 
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;
	LightBuffer mLightBuffer; 

	SettingsData mSettings;

	DirectionalLight mDirectionalLight;
	Material mMaterial;
	
	float mCloudRotation = 0.0f;

	Mesh mMesh;
	Texture mEarth;
	Texture mEarthDisplacement;
	Texture mEarthSpecualr;
	Texture mNormalMap;
	Texture mNightMap;
	Texture mEarthCould;
	Sampler mSampler;
	Vector3 mRotation = Vector3::Zero;
	Vector3 mVelocity = Vector3::Zero;
	Vector3 mAccelation = Vector3::Zero;

	// Space
	JimmyGod::SkyDome mSkyDome;

	// Planets
	JimmyGod::Planet mMoon;
	JimmyGod::Planet mMercury;
	JimmyGod::Planet mVenus;
	JimmyGod::Planet mMars;
	JimmyGod::Planet mJupiter;
	JimmyGod::Planet mSaturn;
	JimmyGod::Planet mUranos;
	JimmyGod::Planet mNeptune;
};