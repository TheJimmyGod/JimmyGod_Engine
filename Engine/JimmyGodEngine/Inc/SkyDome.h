#pragma once

namespace JimmyGod
{
	class SkyDome
	{
	public:	
		SkyDome() = default;
		~SkyDome() = default;
		void Intialize(const std::filesystem::path& path, float radius, uint32_t rings, uint32_t slices, const Vector3& pos);
		void Terminate();
		void Update(const JimmyGod::Graphics::Camera& camera);
		void Render(const JimmyGod::Graphics::Camera& camera);

	private:
		JimmyGod::Graphics::ConstantBuffer mConstantBuffer;
		JimmyGod::Graphics::MeshBuffer mMeshBuffer;
		JimmyGod::Graphics::Texture mTexture;
		JimmyGod::Graphics::MeshPX mMesh;

		JimmyGod::Graphics::Sampler mSampler;
		JimmyGod::Graphics::VertexShader mVertexShader;
		JimmyGod::Graphics::PixelShader mPixelShader;

		JimmyGod::Math::Vector3 mPos;
	};
}