#pragma once

#include "Unit.h"
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

	using LightBuffer = TypedConstantBuffer<JimmyGod::Graphics::DirectionalLight>;
	using MaterialBuffer = TypedConstantBuffer<JimmyGod::Graphics::Material>;
	using SettingsBuffer = TypedConstantBuffer<SettingsData>;
	using PostProcessingSettingsBuffer = TypedConstantBuffer<PostProcessSettingsData>;
	using DepthMapConstantBuffer = TypedConstantBuffer<JimmyGod::Math::Matrix4>;
	using ShadowConstantBuffer = TypedConstantBuffer<JimmyGod::Math::Matrix4>;

	JimmyGod::GameWorld mWorld;
	JimmyGod::CameraService* mCamera = nullptr;

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

	JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
	std::vector<JimmyGod::Physics::Particle*> particles;
	
private:
	bool isActive = false;
	float mTime = 0.0f;
	const float mGravity = -9.8f;
	float fps = 0.0f;

private:
	Unit* Johny = nullptr;

	JimmyGod::AI::Coord destiniation = {0,0};
	JimmyGod::AI::Coord current = { 0,0 };
};