#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

class GameState : public JimmyGod::AppState
{
public:
	enum class Direction
	{
		Back,
		Front,
		Right,
		Left
	};

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
	JimmyGod::GameWorld mWorld;
	using TransformBuffer = JimmyGod::Graphics::TypedConstantBuffer<TransformData>;
	TransformBuffer mTransformBuffer;
	JimmyGod::CameraService* mCamera = nullptr;
	float mTime = 0.0f;
	const float mGravity = -9.8f;
	bool isJump = false;
	bool isKicked = false;
	bool isThrew = false;
	bool isRope = false;
	bool isCloak = false;
	bool isSpark = false;

	Direction mDirection = Direction::Front;

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

	JimmyGod::Graphics::AnimationBulider mAnimationBuilder;
	JimmyGod::Graphics::Animation mAnimation;
	float mAnimationTimer = 0.0f;

	JimmyGod::Graphics::Model mModel;

	JimmyGod::Graphics::Material mMaterial;

	JimmyGod::Graphics::VertexShader mVertexShader;
	JimmyGod::Graphics::PixelShader mPixelShader;

	JimmyGod::Graphics::Sampler mSampler;

	std::vector<JimmyGod::Math::Matrix4> mBoneMatrix;
	struct BoneTransformData
	{
		JimmyGod::Math::Matrix4 BoneTransforms[211];
	};

	using BoneTransformBuffer = JimmyGod::Graphics::TypedConstantBuffer<BoneTransformData>;
	BoneTransformBuffer mBoneTransformBuffer;

	JimmyGod::Graphics::DirectionalLight mDirectionalLight;
	SettingsData mSettings;
	JimmyGod::Graphics::Animator mAnimator;
	bool showSkeleton = false;
	bool stopAnimation = false;
	bool triggerActive = false;

	JimmyGod::Math::Vector3 position = JimmyGod::Math::Vector3::Zero;
	JimmyGod::Math::Matrix4 rotation = JimmyGod::Math::Matrix4::Identity;
	JimmyGod::Math::Vector3 accelation = JimmyGod::Math::Vector3::Zero;
	JimmyGod::Math::Vector3 velocity = JimmyGod::Math::Vector3::Zero;

	JimmyGod::Graphics::Bone* mLeftShoulder = nullptr;
	JimmyGod::Graphics::Bone* mRightShoulder = nullptr;
	JimmyGod::Graphics::Bone* mNeck = nullptr;

	float mRotation = 0.0f;
	float fps = 0.0f;
	size_t currentAnimation = 0;
	JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
	std::vector<JimmyGod::Physics::Particle*> particles;
	JimmyGod::Cloth mCloak;
	JimmyGod::Rope mRope;
	JimmyGod::Spark mSpark;
	JimmyGod::Bomb mBomb;
};