#include "Precompiled.h"
#include "Cloth.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Physics;
using namespace JimmyGod::Math;

void Cloth::Initialize(const std::filesystem::path & path, uint32_t width, uint32_t height)
{
	PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f,-9.8f,0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;
	mPhysicsWorld.Initialize(settings);

	mWidth = width; mHeight = height;
	mMesh = JimmyGod::Graphics::MeshBuilder::CreatePlanePX(mHeight, mWidth);
	mMeshBuffer.Initialize(mMesh, true);
	std::filesystem::path texturePath = L"../../Assets/Shaders/DoTexturing.fx";
	mVertexShader.Initialize(texturePath, JimmyGod::Graphics::VertexPX::Format);
	mPixelShader.Initialize(texturePath);

	mSampler.Initialize(JimmyGod::Graphics::Sampler::Filter::Anisotropic,
		JimmyGod::Graphics::Sampler::AddressMode::Border);
	mTexture.Initialize(path);
	mConstantBuffer.Initialize(sizeof(Matrix4));
}

void Cloth::Terminate()
{
	mConstantBuffer.Terminate();
	mSampler.Terminate();
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mPhysicsWorld.Clear();
}

void Cloth::Update(float deltaTime, int dir)
{
	if (!IsDisplay)return;
	mPhysicsWorld.Update(deltaTime);
	
	if (IsDisplay && (mMesh.vertices.size() == mParticles.size()))
	{
		for (size_t i = 0; i < mParticles.size(); ++i)
		{
			mMesh.vertices[i].position = mParticles[i]->position;
		}
	}
	for (size_t i = 0; i < mFixed.size(); i++)
	{

		switch (dir)
		{
		case 0: // Back
		{
			if (i == 0)
			{
				mFixedParticles[i]->SetPosition(Vector3{ mLeft.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mLeft.y + mHeight) - 2.5f,mLeft.z - 3.25f});
				mParticles[mFixed[i]]->SetPosition(Vector3{ mLeft.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mLeft.y + mHeight) - 2.5f, mLeft.z - 3.25f });
			}
			else if (i == mFixed.size() - 1)
			{
				mFixedParticles[i]->SetPosition(Vector3{ mRight.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mRight.y + mHeight) - 2.5f,mRight.z - 3.25f });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mRight.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mRight.y + mHeight) - 2.5f, mRight.z - 3.25f });
			}
			else
			{
				mFixedParticles[i]->SetPosition(Vector3{ mPosition.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mPosition.y + mHeight) - 2.5f,mPosition.z - 3.25f });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mPosition.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mPosition.y + mHeight) - 2.5f, mPosition.z - 3.25f });
			}
		}
			break;
		case 1: // Front
		{
			if (i == 0)
			{
				mFixedParticles[i]->SetPosition(Vector3{ mLeft.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mLeft.y + mHeight) - 2.5f,mLeft.z });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mLeft.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mLeft.y + mHeight) - 2.5f, mLeft.z });
			}
			else if (i == mFixed.size() - 1)
			{
				mFixedParticles[i]->SetPosition(Vector3{ mRight.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mRight.y + mHeight) - 2.5f,mRight.z });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mRight.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mRight.y + mHeight) - 2.5f, mRight.z });
			}
			else
			{
				mFixedParticles[i]->SetPosition(Vector3{ mPosition.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mPosition.y + mHeight) - 2.5f,mPosition.z });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mPosition.x + (static_cast<float>(i) - 2.25f), static_cast<float>(mPosition.y + mHeight) - 2.5f, mPosition.z });
			}
		}
			break;
		case 2: // Right
		{
			if (i == 0)
			{
				mFixedParticles[i]->SetPosition(Vector3{ mLeft.x - 2.5f, static_cast<float>(mLeft.y + mHeight) - 2.5f,mLeft.z + (static_cast<float>(i) - 4.0f) });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mLeft.x - 2.5f, static_cast<float>(mLeft.y + mHeight) - 2.5f, mLeft.z + (static_cast<float>(i) - 4.0f) });
			}
			else if (i == mFixed.size() - 1)
			{
				mFixedParticles[i]->SetPosition(Vector3{ mRight.x - 2.5f, static_cast<float>(mRight.y + mHeight) - 2.5f,mRight.z + (static_cast<float>(i) - 4.0f) });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mRight.x - 2.5f, static_cast<float>(mRight.y + mHeight) - 2.5f, mRight.z + (static_cast<float>(i) - 4.0f) });
			}
			else
			{
				mFixedParticles[i]->SetPosition(Vector3{ mPosition.x - 2.5f, static_cast<float>(mPosition.y + mHeight) - 2.5f,mPosition.z + (static_cast<float>(i) - 3.5f) });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mPosition.x - 2.5f, static_cast<float>(mPosition.y + mHeight) - 2.5f, mPosition.z + (static_cast<float>(i) - 3.5f) });
			}
		}
			break;
		case 3: // Left
		{
			if (i == 0)
			{
				mFixedParticles[i]->SetPosition(Vector3{ mLeft.x + 0.5f, static_cast<float>(mLeft.y + mHeight) - 2.5f,mLeft.z + (static_cast<float>(i) - 4.0f) });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mLeft.x + 0.5f, static_cast<float>(mLeft.y + mHeight) - 2.5f, mLeft.z + (static_cast<float>(i) - 4.0f) });
			}
			else if (i == mFixed.size() - 1)
			{
				mFixedParticles[i]->SetPosition(Vector3{ mRight.x + 0.5f, static_cast<float>(mRight.y + mHeight) - 2.5f,mRight.z + (static_cast<float>(i) - 4.0f) });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mRight.x + 0.5f, static_cast<float>(mRight.y + mHeight) - 2.5f, mRight.z + (static_cast<float>(i) - 4.0f) });
			}
			else
			{
				mFixedParticles[i]->SetPosition(Vector3{ mPosition.x + 0.5f, static_cast<float>(mPosition.y + mHeight) - 2.5f,mPosition.z + (static_cast<float>(i) - 3.5f) });
				mParticles[mFixed[i]]->SetPosition(Vector3{ mPosition.x + 0.5f, static_cast<float>(mPosition.y + mHeight) - 2.5f, mPosition.z + (static_cast<float>(i) - 3.5f) });
			}
		}
			break;
		default:
			break;
		}
	}
}

void Cloth::ShowCloth(const JimmyGod::Math::Vector3 & pos)
{
	IsDisplay = true;
	Vector3 offset = { 0.5f * mWidth + pos.x, 0.5f * mHeight + pos.y, pos.z };
	mParticles.clear();

	mPhysicsWorld.Clear(true);
	mFixedParticles.clear();
	mFixed.clear();
	mOffset = offset;
	for (uint32_t y = 0; y < mHeight; y++)
	{
		for (uint32_t x = 0; x < mWidth; x++)
		{
			auto p = new Particle({ Vector3{ -offset.x + (static_cast<float>(x)), offset.y - (static_cast<float>(y)), offset.z } });
			p->SetVelocity(Vector3{ RandomFloat(1.0f, 1.5f) });
			p->radius = 0.1f;
			p->bounce = 0.4f;
			mParticles.push_back(p);
			mPhysicsWorld.AddParticle(p);

		}
	}

	for (uint32_t y = 0; y < mHeight; y++)
	{
		for (uint32_t x = 0; x < mWidth; x++)
		{
			if (y == 0 || x == mWidth)
			{
				auto f = new Fixed(mParticles[GetIndex(x, y, mWidth)]);
				mPhysicsWorld.AddConstraint(f);
				mFixed.push_back(x + y);
				mFixedParticles.push_back(f);
			}
			if (x + 1 < mWidth)
			{
				auto c = new Spring(mParticles[GetIndex(x, y, mWidth)], mParticles[GetIndex(x + 1, y, mWidth)]);
				mPhysicsWorld.AddConstraint(c);
			}
			if (y + 1 < mHeight)
			{
				auto c = new Spring(mParticles[GetIndex(x, y, mWidth)], mParticles[GetIndex(x, y + 1, mWidth)]);
				mPhysicsWorld.AddConstraint(c);
			}
		}
	}
}

void Cloth::Render(const JimmyGod::Graphics::Camera & camera, const Matrix4& rotation)
{
	if (!IsDisplay) return;

	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();

	mConstantBuffer.BindVS();
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS();
	mTexture.BindPS();
	auto matrixViewProjection = JimmyGod::Math::Transpose(view * projection);
	mConstantBuffer.Update(&matrixViewProjection);
	mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
	if(IsDebugUI == false)
		mMeshBuffer.Draw();
}

void Cloth::DebugUI(bool debug)
{
	IsDebugUI = debug;

	if (IsDebugUI == true)
	{
		mPhysicsWorld.DebugDraw();
	}
}

void Cloth::SetVelocity(const Vector3 & vel)
{
	for (auto& p : mParticles)
	{
		p->SetVelocity(vel);
	}
}

void Cloth::SetPosition(const JimmyGod::Math::Vector3 & pos, const JimmyGod::Math::Vector3& Left,
	const JimmyGod::Math::Vector3& Right)
{
	mPosition = pos;
	mLeft = Left;
	mRight = Right;
}
