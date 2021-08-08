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
	VertexShader mGourandShadingVertexShader;
	PixelShader mGourandShadingPixelShader;

	VertexShader mPhongShadingVertexShader;
	PixelShader mPhongShadingPixelShader;

	VertexShader mDomeVertexShader;
	PixelShader mDomePixelShader;

	TransformBuffer mTransformBuffer; 
	LightBuffer mLightBuffer; 
	MaterialBuffer mMaterialBuffer;
	
	DirectionalLight mDirectionalLight;
	Material mMaterial;
	
	ConstantBuffer mConstant;

	MeshBuffer mDomeMeshBuffer;
	MeshPN mMesh;
	MeshPX mMeshX;
	Texture mSpace;
	Sampler mSampler;
	Vector3 mRotation = 0.0f;
};