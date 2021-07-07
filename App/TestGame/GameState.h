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
	struct TransformData
	{
		JimmyGod::Math::Matrix4 world;
		JimmyGod::Math::Matrix4 wvp;
		JimmyGod::Math::Vector3 viewPosition;
		float padding;
	};
	using TransformBuffer = JimmyGod::Graphics::TypedConstantBuffer<TransformData>;
	TransformBuffer mTransformBuffer;
	JimmyGod::GameWorld mWorld;
	JimmyGod::CameraService* mCamera = nullptr;

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
	using LightBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::DirectionalLight>;
	using MaterialBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::Material>;
	using SettingsBuffer = JimmyGod::Graphics::TypedConstantBuffer<SettingsData>;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;
	float mAnimationTimer = 0.0f;

	JimmyGod::Graphics::Material mMaterial;

	JimmyGod::Graphics::VertexShader mVertexShader;
	JimmyGod::Graphics::PixelShader mPixelShader;

	JimmyGod::Graphics::Sampler mSampler;

	JimmyGod::Graphics::DirectionalLight mDirectionalLight;
	SettingsData mSettings;
	JimmyGod::Graphics::Animator* mAnimator = nullptr;

	JimmyGod::Graphics::Bone* mLeftShoulder = nullptr;
	JimmyGod::Graphics::Bone* mRightShoulder = nullptr;
	JimmyGod::Graphics::Bone* mNeck = nullptr;
	JimmyGod::Graphics::Bone* mRightToe = nullptr;

	JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
	std::vector<JimmyGod::Physics::Particle*> particles;
	JimmyGod::Cloth mCloak;
	JimmyGod::Rope mRope;
	JimmyGod::Spark mSpark;
	JimmyGod::Bomb mBomb;
	
private:
	JimmyGod::Math::Vector3 position = JimmyGod::Math::Vector3::Zero;
	JimmyGod::Math::Matrix4 rotation = JimmyGod::Math::Matrix4::Identity;
	JimmyGod::Math::Vector3 accelation = JimmyGod::Math::Vector3::Zero;
	JimmyGod::Math::Vector3 velocity = JimmyGod::Math::Vector3::Zero;

	float mTime = 0.0f;
	const float mGravity = -9.8f;
	bool isJump = false;
	bool isKicked = false;
	bool isThrew = false;
	bool isRope = false;
	bool isCloak = false;
	bool isSpark = false;

	bool showSkeleton = false;
	bool stopAnimation = false;

	float fps = 0.0f;
};