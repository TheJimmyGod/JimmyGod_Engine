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

private:
	Camera mCamera;
	VertexShader mVertexShader;
	MeshBuffer mMeshBuffer;
	MeshBuffer mMeshDomeBuffer;
	PixelShader mPixelShader;
	ConstantBuffer mConstantBuffer;

	Sampler mSampler;
	Texture mSun;
	Texture mEarth;
	Texture mMoon;
	Texture mMercury;
	Texture mVenus;
	Texture mMars;
	Texture mJupiter;
	Texture mSaturn;
	Texture mUranos;
	Texture mNeptune;
	Texture mSpace;
	MeshPX mMesh;

	Vector3 mRotation = 0.0f;
	Vector3 mEarthRotation = 5.0f;

};