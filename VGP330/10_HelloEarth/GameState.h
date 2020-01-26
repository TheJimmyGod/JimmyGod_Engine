#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace std;
using namespace Math;

class GameState : public JimmyGod::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;
private:
	Camera mCamera;
	MeshBuffer mMeshBuffer;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

	ConstantBuffer mConstant;
	VertexShader mDomeVertexShader;
	PixelShader mDomePixelShader;
	MeshBuffer mDomeMeshBuffer;
	MeshPX mMeshX;
	Texture mSpace;

	struct TransformData
	{
		JimmyGod::Math::Matrix4 world;
		JimmyGod::Math::Matrix4 wvp;
		JimmyGod::Math::Vector3 viewPosition;
		float padding;
	};

	using TransformBuffer = JimmyGod::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::DirectionalLight>;
	using MaterialBuffer = JimmyGod::Graphics::TypedConstantBuffer<JimmyGod::Graphics::Material>;

	TransformBuffer mTransformBuffer; 
	LightBuffer mLightBuffer; 
	MaterialBuffer mMaterialBuffer;
	
	DirectionalLight mDirectionalLight;
	Material mMaterial;
	

	Mesh mMesh;
	Texture mEarth;
	Texture mEarthDisplacement;
	Texture mEarthSpecualr;
	Sampler mSampler;
	Vector3 mRotation = 0.0f;
};