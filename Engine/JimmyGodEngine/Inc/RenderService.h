#pragma once

#include "Service.h"

namespace JimmyGod
{
	class RenderService : public Service
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void Render() override;

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

		using LightBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::DirectionalLight>;
		using MaterialBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::Material>;
		using SettingsBuffer = JimmyGod::Graphics::TypedConstantBuffer<SettingsData>;
		using PostProcessingSettingsBuffer = JimmyGod::Graphics::TypedConstantBuffer<PostProcessSettingsData>;
		using DepthMapConstantBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Math::Matrix4>;
		using ShadowConstantBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Math::Matrix4>;

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
	};
}