#include"Precompiled.h"
#include"SimpleDraw.h"

#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"
#include "GraphicsSystem.h"
using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace std;
namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount)
		{
			mVertexShader.Initialize("../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
			mPixelShader.Initialize("../../Assets/Shaders/SimpleDraw.fx");
			mConstantBuffer.Initialize(sizeof(Math::Matrix4));
			mMeshBuffer.Initialize<VertexPC>(nullptr,maxVertexCount,true);
			mLineVertices = make_unique<VertexPC[]>(maxVertexCount);
			mFillVertices = make_unique<VertexPC[]>(maxVertexCount * 3.0f);
			mVertexCount = 0;
			mFillVertexCount = 0;
			mMaxVertexCount = maxVertexCount;
		}
		void Terminate()
		{
			mVertexShader.Terminate();
			mPixelShader.Terminate();
			mConstantBuffer.Terminate();
			mMeshBuffer.Terminate();
		}
		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
		{
			if (mVertexCount + 2 < mMaxVertexCount)
			{
				mLineVertices[mVertexCount++] = VertexPC{ v0,color };
				mLineVertices[mVertexCount++] = VertexPC{ v1,color };
			}
		}

		void AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color)
		{
			if (mFillVertexCount + 3 < mMaxVertexCount)
			{
				mFillVertices[mFillVertexCount++] = VertexPC{ v0,color };
				mFillVertices[mFillVertexCount++] = VertexPC{ v1,color };
				mFillVertices[mFillVertexCount++] = VertexPC{ v2,color };
			}
		}

		void AddDisplacement(float Length)
		{
			AddLine(Vector3::Zero, Vector3::XAxis * Length, Colors::Red);
			AddLine(Vector3::Zero, Vector3::YAxis * Length, Colors::Blue);
			AddLine(Vector3::Zero, Vector3::ZAxis * Length, Colors::Green);
		}

		void AddBox(float Length, const Color & color)
		{
			AddLine(Vector3{ -Length,-Length,-Length }, Vector3{Length,-Length,-Length}, color);
			AddLine(Vector3{ -Length,-Length,-Length }, Vector3{ -Length,Length,-Length }, color);
			AddLine(Vector3{ -Length,-Length,-Length }, Vector3{ -Length,-Length,Length }, color);
			AddLine(Vector3{ -Length,Length,-Length }, Vector3{ Length,Length,-Length }, color);
			AddLine(Vector3{ -Length,Length,-Length }, Vector3{ -Length,Length,Length }, color);
			AddLine(Vector3{ Length,Length,-Length }, Vector3{  Length,Length,Length }, color);
			AddLine(Vector3{ Length,Length,-Length }, Vector3{ Length,-Length,-Length }, color);
			AddLine(Vector3{ Length,-Length,-Length }, Vector3{ Length,-Length,Length }, color);
			AddLine(Vector3{ Length,Length,Length }, Vector3{ -Length,Length,Length }, color);
			AddLine(Vector3{ Length,Length,Length }, Vector3{ Length,-Length,Length }, color);
			AddLine(Vector3{ Length,-Length,Length }, Vector3{ -Length,-Length,Length }, color);
			AddLine(Vector3{ -Length,-Length,Length }, Vector3{ -Length,Length,Length }, color);
		}

		void AddSphere(float radius, int rings, int slices, const Color & color)
		{
			float r = radius;
			for (float phi = 0; phi < Constants::Pi; phi += (Constants::Pi) / (rings))
			{
				for (float theta = 0; theta < Constants::TwoPi; theta += (Constants::TwoPi) / (slices))
				{
					auto vec = Vector3{
								sinf(phi)*cosf(theta)*r,
								cosf(phi)*r,
								sinf(theta) * sinf(phi) *r
					};

					mLineVertices[mVertexCount++] = VertexPC{ vec,color };
				}

			}
		}

		void Render(const Camera& camera)
		{
			auto matView = camera.GetViewMatrix();
			auto matProj = camera.GetPerspectiveMatrix();
			auto transform = Math::Transpose(matView * matProj);
			mConstantBuffer.Update(&transform);
			mConstantBuffer.BindVS(0);

			mVertexShader.Bind();
			mPixelShader.Bind();

			mMeshBuffer.Update(mLineVertices.get(), mVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Draw();

			mMeshBuffer.Update(mFillVertices.get(), mFillVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
			mMeshBuffer.Draw();

			auto system = GraphicsSystem::Get();
			Math::Matrix4 screenToNDC;
			const uint32_t screenW = system->GetBackBufferWidth();
			const uint32_t screenH = system->GetBackBufferHeight();
			screenToNDC._11 = 2.0f / screenW;
			screenToNDC._22 = 2.0f / screenH;
			mConstantBuffer.Update(&Math::Transpose(screenToNDC));

			//mMeshBuffer.Update(m2DLineVertices.get(), mVertexCount);
			//mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			//mMeshBuffer.Draw();

			mFillVertexCount = 0;
			mVertexCount = 0;
		}
	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		unique_ptr<VertexPC[]> mLineVertices;
		unique_ptr<VertexPC[]> mFillVertices;
		unique_ptr<VertexPC[]> m2DLineVertices;
		uint32_t mVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
		uint32_t mFillVertexCount = 0;
	};

	unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}
void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}
void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	sInstance->AddLine(v0, v1, color);
}

void JimmyGod::Graphics::SimpleDraw::AddFace(const Math::Vector3 & v0, const Math::Vector3 & v1, const Math::Vector3 & v2, const Color & color)
{
	sInstance->AddFace(v0, v1, v2, color);
}

void SimpleDraw::AddDisplacement(float Length)
{
	sInstance->AddDisplacement(Length);
}

void SimpleDraw::AddBox(float Length, const Color & color)
{
	sInstance->AddBox(Length, color);
}
void SimpleDraw::AddSphere(float radius, int rings, int slices, const Color & color)
{
	sInstance->AddSphere(radius, rings, slices, color);
}

void SimpleDraw::AddTransform(const Math::Matrix4 & transform)
{
	auto r = Math::GetRight(transform);
	auto u = Math::GetUp(transform);
	auto l = Math::GetLook(transform);
	auto p = Math::GetTranslation(transform);

	//sInstance->AddLine(p, p + r, Colors::Red);
	//sInstance->AddLine(p, p + u, Colors::Green);
	//sInstance->AddLine(p, p + l, Colors::Blue);
}
void SimpleDraw::AddBone(const Math::Matrix4 & transform)
{
	auto r = Math::GetRight(transform);
	auto u = Math::GetUp(transform);
	auto l = Math::GetLook(transform);
	auto p = Math::GetTranslation(transform);
	//AddSphere(GetTranslation, 0.025f, Colors::BlueViolet,5,6)
	//sInstance->AddLine(p, p + r * 0.1f, Colors::Red);
	//sInstance->AddLine(p, p + u * 0.1f, Colors::Green);
	//sInstance->AddLine(p, p + l * 0.1f, Colors::Blue);
}
void SimpleDraw::AddGroundPlane(float size, const Color & color)
{
	const float halfSize = size * 0.5f;
	for (float i = 0; i < halfSize; i += 1.0f)
	{
		sInstance->AddLine({i,0.0f, -halfSize}, {i,0.0f,halfSize}, color);
		sInstance->AddLine({-halfSize,0.0f,i}, {halfSize,0.0f,i}, color);
	}
}
void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}