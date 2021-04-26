#pragma once

namespace JimmyGod
{
	class Cloth
	{
	public:
		Cloth() = default;
		~Cloth() = default;
	void Initialize(const std::filesystem::path& path, uint32_t width, uint32_t height);
	void Terminate();
	void Update(float deltaTime);
	void ShowCloth(const JimmyGod::Math::Vector3& pos);
	void Render(const JimmyGod::Graphics::Camera& camera, const JimmyGod::Math::Vector3& pos = Vector3::Zero);
	void SetVelocity(const JimmyGod::Math::Vector3& vel);

	void Active(bool act) { IsDisplay = act; }
	bool IsActive() const { return IsDisplay; }
private:
	bool IsDisplay = false;
	size_t GetIndex(uint32_t x, uint32_t y, uint32_t column) { return (y*column) + x; }
	std::vector<JimmyGod::Physics::Particle*> mParticles;

	JimmyGod::Physics::PhysicsWorld mPhysicsWorld;
	uint32_t mWidth;
	uint32_t mHeight;
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