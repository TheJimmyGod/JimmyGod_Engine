#pragma once

namespace JimmyGod
{
	class Rope
	{
	public:
		Rope() = default;
		~Rope() = default;

		void Initialize(const std::filesystem::path& path, float radius, uint32_t length);
		void Terminate();
		void Update(float deltaTime, const JimmyGod::Math::Vector3& pos = JimmyGod::Math::Vector3::Zero);
		void ShowRope(const JimmyGod::Math::Vector3& pos, const JimmyGod::Math::Vector3& dir, float radius, float power, uint32_t length, float bounce, float dist, float LifeTime);
		void Render(const JimmyGod::Graphics::Camera& camera);
		void DebugUI(bool debug = false);

		void Active(bool act) { IsDisplay = act; }
		bool IsActive() const { return IsDisplay; }
	private:
		bool IsDisplay = false;
		bool IsDebugUI = false;
		bool IsRopeEnded = false;

		std::vector<JimmyGod::Physics::Particle*> mParticles;
		JimmyGod::Physics::Fixed* mFixed;
		JimmyGod::Math::Vector3 mPosition = JimmyGod::Math::Vector3::Zero;
		JimmyGod::Math::Vector3 mVelocity = JimmyGod::Math::Vector3::Zero;
		JimmyGod::Physics::PhysicsWorld mPhysicsWorld;

		uint32_t mLength = 0;
		float mRadius = 0.0f;

		JimmyGod::Graphics::MeshPX mMesh;
		JimmyGod::Graphics::MeshBuffer mMeshBuffer;
		JimmyGod::Graphics::Sampler mSampler;
		JimmyGod::Graphics::Texture mTexture;
		JimmyGod::Graphics::ConstantBuffer mConstantBuffer;

		JimmyGod::Graphics::PixelShader mPixelShader;
		JimmyGod::Graphics::VertexShader mVertexShader;
		float mTime = 0.0f;
	};
}