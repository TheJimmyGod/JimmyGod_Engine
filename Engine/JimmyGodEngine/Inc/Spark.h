#pragma once

namespace JimmyGod
{
	class Spark
	{
	public:
		Spark() = default;
		~Spark() = default;
		void Initialize(const std::filesystem::path& path, float radius);
		void Terminate();
		void Update(float deltaTime);
		void ShowSpark(const JimmyGod::Math::Vector3& pos, uint32_t amount, float endTime);
		void Render(const JimmyGod::Graphics::Camera& camera);
		void DebugUI(bool debug = false);

		void ChangeAmount(uint32_t amount);

		void Active(bool act) { IsDisplay = act; }
		bool IsActive() const { return IsDisplay; }
	private:
		bool IsDisplay = false;
		bool IsSummoned = false;
		bool IsDebugUI = false;

		size_t GetIndex(uint32_t x, uint32_t y, uint32_t column) { return (y*column) + x; }
		std::vector<JimmyGod::Physics::Particle*> mParticles;
		std::vector<JimmyGod::Graphics::MeshPX*> mMeshes;
		JimmyGod::Math::Vector3 mFoot = JimmyGod::Math::Vector3::Zero;

		JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
		float mRadius = 0;;
		uint32_t mAmount = 0;
		JimmyGod::Graphics::MeshPX mMesh;
		JimmyGod::Graphics::MeshBuffer mMeshBuffer;
		JimmyGod::Graphics::Sampler mSampler;
		JimmyGod::Graphics::Texture mTexture;
		JimmyGod::Graphics::ConstantBuffer mConstantBuffer;

		JimmyGod::Graphics::PixelShader mPixelShader;
		JimmyGod::Graphics::VertexShader mVertexShader;
		float mTime = 0.0f;
	private:
	};
}