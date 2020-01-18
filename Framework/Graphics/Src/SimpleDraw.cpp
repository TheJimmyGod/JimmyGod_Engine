#include"Precompiled.h"
#include"SimpleDraw.h"

#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"

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
			mVertexCount = 0;
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

		void AddDisplacement(float Length)
		{
			SimpleDraw::AddLine(Vector3::Zero, Vector3::XAxis * Length, Colors::Red);
			SimpleDraw::AddLine(Vector3::Zero, Vector3::YAxis * Length, Colors::Blue);
			SimpleDraw::AddLine(Vector3::Zero, Vector3::ZAxis * Length, Colors::Green);
		}

		void AddBox(float Length, const Color & color)
		{
			SimpleDraw::AddLine(Vector3{ -Length,-Length,-Length }, Vector3{Length,-Length,-Length}, color);
			SimpleDraw::AddLine(Vector3{ -Length,-Length,-Length }, Vector3{ -Length,Length,-Length }, color);
			SimpleDraw::AddLine(Vector3{ -Length,-Length,-Length }, Vector3{ -Length,-Length,Length }, color);
			SimpleDraw::AddLine(Vector3{ -Length,Length,-Length }, Vector3{ Length,Length,-Length }, color);
			SimpleDraw::AddLine(Vector3{ -Length,Length,-Length }, Vector3{ -Length,Length,Length }, color);
			SimpleDraw::AddLine(Vector3{ Length,Length,-Length }, Vector3{  Length,Length,Length }, color);
			SimpleDraw::AddLine(Vector3{ Length,Length,-Length }, Vector3{ Length,-Length,-Length }, color);
			SimpleDraw::AddLine(Vector3{ Length,-Length,-Length }, Vector3{ Length,-Length,Length }, color);
			SimpleDraw::AddLine(Vector3{ Length,Length,Length }, Vector3{ -Length,Length,Length }, color);
			SimpleDraw::AddLine(Vector3{ Length,Length,Length }, Vector3{ Length,-Length,Length }, color);
			SimpleDraw::AddLine(Vector3{ Length,-Length,Length }, Vector3{ -Length,-Length,Length }, color);
			SimpleDraw::AddLine(Vector3{ -Length,-Length,Length }, Vector3{ -Length,Length,Length }, color);
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
			mConstantBuffer.Set(&transform);
			mConstantBuffer.Bind();

			mVertexShader.Bind();
			mPixelShader.Bind();

			mMeshBuffer.Update(mLineVertices.get(), mVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Draw();

			mVertexCount = 0;
		}
	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		unique_ptr<VertexPC[]> mLineVertices;
		uint32_t mVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
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
void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}