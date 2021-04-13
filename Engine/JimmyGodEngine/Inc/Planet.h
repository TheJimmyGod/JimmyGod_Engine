#pragma once

namespace JimmyGod
{
	class Planet
	{
	public:
		Planet() = default;
		~Planet() = default;
		void Initialize(const std::filesystem::path& path, const JimmyGod::Math::Vector3& pos, float radius, float rings, float slices);
		void Terminate();
		void Update(float deltaTime);
		void Render(const JimmyGod::Graphics::Camera& camera, const JimmyGod::Math::Matrix4& mat = Math::Matrix4::Identity);

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
			float bumpMapWeight = 10.0f;
			float normalMapWeight = 1.0f;
			float padding;
		};

		JimmyGod::Graphics::Mesh mMesh;
		JimmyGod::Graphics::ConstantBuffer mConstantBuffer;
		JimmyGod::Graphics::VertexShader mVertexShader;
		JimmyGod::Graphics::PixelShader mPixelShader;
		JimmyGod::Graphics::MeshBuffer mMeshBuffer;
		JimmyGod::Graphics::Texture mTexture;
		JimmyGod::Graphics::Sampler mSampler;

		JimmyGod::Math::Vector3 mPos;
		JimmyGod::Math::Vector3 mRot;

		using TransformBuffer = JimmyGod::Graphics::TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;
		using SettingsBuffer = JimmyGod::Graphics::TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;

		SettingsData mSettings;
	};
}