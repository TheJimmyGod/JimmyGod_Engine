#pragma once

namespace JimmyGod
{
	class Spark
	{
	public:
		Spark() = default;
		~Spark() = default;
		void Initialize(const std::filesystem::path& path, uint32_t amount, float radius, float spread = 0.5f);
		void Terminate();
		void Update(float deltaTime);
		void ShowSpark(const JimmyGod::Math::Vector3& pos, const JimmyGod::Math::Vector3& dir, float endTime);
		void Render(const JimmyGod::Graphics::Camera& camera);
		void DebugUI(bool debug = false);
		void Clear() 
		{
			mParticles.clear();
			mPhysicsWorld.Clear(true);
		}

		void Active(bool act) { IsDisplay = act; }
		bool IsActive() const { return IsDisplay; }
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

		bool IsDisplay = false;
		bool IsSummoned = false;
		bool IsDebugUI = false;

		std::vector<JimmyGod::Physics::Particle*> mParticles;
		JimmyGod::Math::Vector3 mFoot = JimmyGod::Math::Vector3::Zero;
		
		JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
		float mRadius = 0.0f;
		float mSpread = 0.0f;
		uint32_t mAmount = 0;
		JimmyGod::Graphics::MeshPX mMesh;
		JimmyGod::Graphics::MeshBuffer mMeshBuffer;
		JimmyGod::Graphics::Sampler mSampler;
		JimmyGod::Graphics::Texture mTexture;
		JimmyGod::Graphics::Texture mBloomTexture;
		JimmyGod::Graphics::ConstantBuffer mConstantBuffer;

		JimmyGod::Graphics::PixelShader mPixelShader;
		JimmyGod::Graphics::VertexShader mVertexShader;

		JimmyGod::Graphics::PixelShader mGlowPixelShader;
		JimmyGod::Graphics::VertexShader mGlowVertexShader;

		float mTime = 0.0f;
	};
}