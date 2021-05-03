#pragma once
#include "Spark.h"
namespace JimmyGod
{
	class Bomb
	{
	public:
		Bomb() = default;
		~Bomb() = default;
		void Initialize(const std::filesystem::path& path, float radius);
		void Terminate();
		void Update(float deltaTime);
		void ShowBomb(const JimmyGod::Math::Vector3& pos, const JimmyGod::Math::Vector3& dir,float endTime, float radius, float bounce, float power, float throwDist);
		void Render(const JimmyGod::Graphics::Camera& camera);
		void DebugUI(bool debug = false);

		void Active(bool act) { IsDisplay = act; }
		bool IsActive() const { return IsDisplay; }
	private:
		bool IsDisplay = false;
		bool IsSummoned = false;
		bool IsDebugUI = false;

		JimmyGod::Physics::Particle* mParticle;

		JimmyGod::Math::Vector3 mPosition = JimmyGod::Math::Vector3::Zero;

		JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
		float mRadius = 0;;

		JimmyGod::Graphics::MeshPX mMesh;

		JimmyGod::Graphics::MeshBuffer mMeshBuffer;
		JimmyGod::Graphics::Sampler mSampler;
		JimmyGod::Graphics::Texture mTexture;
		JimmyGod::Graphics::ConstantBuffer mConstantBuffer;

		JimmyGod::Graphics::PixelShader mPixelShader;
		JimmyGod::Graphics::VertexShader mVertexShader;
		JimmyGod::Spark mSpark;
		float mTime = 0.0f;
		float mSec = 0.12f;
	};
}