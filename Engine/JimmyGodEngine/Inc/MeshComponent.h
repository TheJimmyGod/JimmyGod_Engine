#pragma once
#include "Component.h"

namespace JimmyGod
{
	class TransformComponent;
	class ColliderComponent;

	class MeshComponent final : public Component
	{
	public:
		META_CLASS_DECLARE

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void Bind();

		void SetDebug(bool debug) { isDebugUI = debug; }

		std::string mPath;
		std::string mDiffusePath;
		std::string mSpecularPath;
		std::string mDisplacementPath;
		std::string mNormalPath;
		std::string mAOPath;

	private:
		const TransformComponent* mTransformComponent = nullptr;
		const ColliderComponent* mColliderComponent = nullptr;

		JimmyGod::Graphics::Mesh mMesh;
		JimmyGod::Graphics::MeshBuffer mMeshBuffer;

		JimmyGod::Graphics::VertexShader mVertexShader;
		JimmyGod::Graphics::PixelShader mPixelShader;

		JimmyGod::Graphics::Sampler mSampler;
		JimmyGod::Graphics::Texture mDiffuseMap;
		JimmyGod::Graphics::Texture mSpecularMap;
		JimmyGod::Graphics::Texture mDisplacementMap;
		JimmyGod::Graphics::Texture mNormalMap;
		JimmyGod::Graphics::Texture mAOMap;

		bool isDebugUI = false;
	};
}