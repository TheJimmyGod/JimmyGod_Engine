#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class GameState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void DrawDepthMap();
	void DrawScene();
	void PostProcess();

private:

	JimmyGod::Graphics::AnimationBulider mAnimationBuilder;
	JimmyGod::Graphics::Animation mAnimaion;
	float mAnimationTimer = 0.0f;

	JimmyGod::Graphics::Camera mDefaultCamera;
	JimmyGod::Graphics::Camera mDebugCamera;
	JimmyGod::Graphics::Camera mLightCamera;
	JimmyGod::Graphics::Camera* mActiveCamera = nullptr;

	JimmyGod::Math::Matrix4 mLightProjectMatrix;
	std::vector<JimmyGod::Math::Vector3> mViewFrustumVertices;

	JimmyGod::Graphics::Mesh mTankMesh;
	JimmyGod::Graphics::MeshBuffer mTankMeshBuffer;

	JimmyGod::Graphics::Mesh mGroundMesh;
	JimmyGod::Graphics::MeshBuffer mGroundMeshBuffer;

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

	using TransformBuffer = JimmyGod::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::DirectionalLight>;
	using MaterialBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::Material>;
	using SettingsBuffer = JimmyGod::Graphics::TypedConstantBuffer<SettingsData>;
	using PostProcessingSettingsBuffer = JimmyGod::Graphics::TypedConstantBuffer<PostProcessSettingsData>;
	using DepthMapConstantBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Math::Matrix4>;
	using ShadowConstantBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Math::Matrix4>;
	

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;
	PostProcessingSettingsBuffer mPostProcessingSettingsBuffer;

	JimmyGod::Graphics::DirectionalLight mDirectionalLight;
	JimmyGod::Graphics::Material mMaterial;

	JimmyGod::Graphics::VertexShader mVertexShader;
	JimmyGod::Graphics::PixelShader mPixelShader;

	JimmyGod::Graphics::Sampler mSampler;
	JimmyGod::Graphics::Texture mDiffuseMap;
	JimmyGod::Graphics::Texture mSpecularMap;
	JimmyGod::Graphics::Texture mDisplacementMap;
	JimmyGod::Graphics::Texture mNormalMap;
	JimmyGod::Graphics::Texture mAOMap;

	JimmyGod::Graphics::Texture mGroundDiffuseMap;

	std::vector<JimmyGod::Math::Vector3> mTankPositions;
	JimmyGod::Math::Vector3 mTankRotation = 0.0f;
	float mTankSpacing = 20.0f;

	JimmyGod::Math::Vector3 pos = { 10.0f,0.0f,10.0f };

	SettingsData mSettings;

	// Shadow
	JimmyGod::Graphics::RenderTarget mDepthMapRenderTarget;
	JimmyGod::Graphics::VertexShader mDepthMapVertexShader;
	JimmyGod::Graphics::PixelShader mDepthMapPixelShader;
	DepthMapConstantBuffer mDepthMapConstantBuffer;
	ShadowConstantBuffer mShadowConstantBuffer;

	// Post processing
	JimmyGod::Graphics::RenderTarget mRenderTarget;
	JimmyGod::Graphics::MeshPX mScreenQuad;
	JimmyGod::Graphics::MeshBuffer mScreenQuadBuffer;
	JimmyGod::Graphics::VertexShader mPostProcessingVertexShader;
	JimmyGod::Graphics::PixelShader mPostProcessingPixelShader;
	PostProcessSettingsData mPostProcessSettings;

	JimmyGod::Graphics::Model mModel;

	std::vector<JimmyGod::Math::Matrix4> mBoneMatrix;

	JimmyGod::Terrain mTerrain;
	struct BoneTransformData
	{
		JimmyGod::Math::Matrix4 BoneTransforms[128];
	};

	using BoneTransformBuffer = JimmyGod::Graphics::TypedConstantBuffer<BoneTransformData>;
	BoneTransformBuffer mBoneTransformBuffer;

	JimmyGod::Graphics::Animator mAnimator;
	bool showSkeleton = false;
};