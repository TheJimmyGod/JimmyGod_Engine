#pragma once

#include "Particle.h"

namespace JimmyGod::Physics
{
	class Constraint
	{
	public:
		virtual ~Constraint() = default;

		virtual void Apply() const = 0;
		virtual void DebugDraw() const {}
	};

	class Fixed : public Constraint
	{
	public:
		Fixed(Particle* p);
		Fixed(Particle* p, const Math::Vector3& position);

		void Apply() const override;
		void DebugDraw() const override;

		void SetPosition(const Math::Vector3& position);
	protected:
		Particle* mParticle;
		Vector3 mPosition;
	};

	class Spring : public Constraint
	{
	public:
		Spring(Particle* a, Particle* b, float restLength = 0.0f);

		void Apply() const override;
		void DebugDraw() const override;
	protected:
		Particle* mParticleA;
		Particle* mParticleB;
		float mRestLength;
	};

	//class Cloth : public Constraint
	//{
	//public:
	//	Cloth();

	//	void Initialize(const std::filesystem::path& path,
	//		uint32_t width, uint32_t height);
	//	void Terminate();
	//	void Update(float deltaTime);
	//	void Render(const JimmyGod::Graphics::Camera& camera);

	//protected:
	//	std::vector<Particle*> mParticles;
	//	float mRestLength;
	//private:
	//	bool IsDisplay = false;

	//	JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
	//	uint32_t mWidth;
	//	uint32_t mHeight;
	//	JimmyGod::Graphics::MeshPX mMesh;
	//	JimmyGod::Graphics::MeshBuffer mMeshBuffer;
	//	JimmyGod::Graphics::Sampler mSampler;
	//	JimmyGod::Graphics::Texture mTexture;
	//	JimmyGod::Graphics::ConstantBuffer mConstantBuffer;

	//	JimmyGod::Graphics::PixelShader mPixelShader;
	//	JimmyGod::Graphics::VertexShader mVertexShader;
	//};
}