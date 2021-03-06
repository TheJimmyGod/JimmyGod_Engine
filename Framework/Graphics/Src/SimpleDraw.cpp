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
using namespace JimmyGod::Math;
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
			m2DLineVertices = make_unique<VertexPC[]>(maxVertexCount);
			mFillVertices = make_unique<VertexPC[]>(maxVertexCount * 3);

			mVertexCount = 0;
			m2DVertexCount = 0;
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

		void AddAABB(const Math::Vector3& center, float radius, const Color& color)
		{
			// Check if we have enough space
			if (mVertexCount + 24 <= mMaxVertexCount)
			{
				float minX = center.x - radius;
				float minY = center.y - radius;
				float minZ = center.z - radius;
				float maxX = center.x + radius;
				float maxY = center.y + radius;
				float maxZ = center.z + radius;
				
				// Add lines
				mLineVertices[mVertexCount++] = { Math::Vector3(minX, minY, minZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(minX, minY, maxZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(minX, minY, maxZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, minY, maxZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, minY, maxZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, minY, minZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, minY, minZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(minX, minY, minZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(minX, minY, minZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(minX, maxY, minZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(minX, minY, maxZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(minX, maxY, maxZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, minY, maxZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, maxZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, minY, minZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, minZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(minX, maxY, minZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(minX, maxY, maxZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(minX, maxY, maxZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, maxZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, maxZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, minZ), color };

				mLineVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, minZ), color };
				mLineVertices[mVertexCount++] = { Math::Vector3(minX, maxY, minZ), color };
			}
		}

		void AddAABB(const Math::AABB& aabb, const Color& color)
		{
			if (mVertexCount + 24 <= mMaxVertexCount)
			{
				float minX = aabb.center.x - aabb.extend.x;
				float minY = aabb.center.y - aabb.extend.y;
				float minZ = aabb.center.z - aabb.extend.z;
				float maxX = aabb.center.x + aabb.extend.x;
				float maxY = aabb.center.y + aabb.extend.y;
				float maxZ = aabb.center.z + aabb.extend.z;


				AddLine(Math::Vector3(minX, minY, minZ), Math::Vector3(minX, minY, maxZ), color);
				AddLine(Math::Vector3(minX, minY, maxZ), Math::Vector3(maxX, minY, maxZ), color);
				AddLine(Math::Vector3(maxX, minY, maxZ), Math::Vector3(maxX, minY, minZ), color);
				AddLine(Math::Vector3(maxX, minY, minZ), Math::Vector3(minX, minY, minZ), color);
				AddLine(Math::Vector3(minX, minY, minZ), Math::Vector3(minX, maxY, minZ), color);
				AddLine(Math::Vector3(minX, minY, maxZ), Math::Vector3(minX, maxY, maxZ), color);
				AddLine(Math::Vector3(maxX, minY, maxZ), Math::Vector3(maxX, maxY, maxZ), color);
				AddLine(Math::Vector3(maxX, minY, minZ), Math::Vector3(maxX, maxY, minZ), color);
				AddLine(Math::Vector3(minX, maxY, minZ), Math::Vector3(minX, maxY, maxZ), color);
				AddLine(Math::Vector3(minX, maxY, maxZ), Math::Vector3(maxX, maxY, maxZ), color);
				AddLine(Math::Vector3(maxX, maxY, maxZ), Math::Vector3(maxX, maxY, minZ), color);
				AddLine(Math::Vector3(maxX, maxY, minZ), Math::Vector3(minX, maxY, minZ), color);
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

		void AddSphere(const Math::Vector3& sphere, float radius, const Color & color, uint32_t rings, uint32_t slices)
		{
			float x = sphere.x;
			float y = sphere.y;
			float z = sphere.z;

			//if (x == y == z == 0)
			//{
			//	return;
			//}

			float r = radius;
			const uint32_t kLines = (4 * slices * rings) - (2 * slices);
			// Check if we have enough space
			if (mVertexCount + kLines <= mMaxVertexCount)
			{
				// Add lines
				const float kTheta = Constants::Pi / (float)rings;
				const float kPhi = Constants::TwoPi / (float)slices;
				for (uint32_t j = 0; j < slices; ++j)
				{
					for (uint32_t i = 0; i < rings; ++i)
					{
						const float a = i * kTheta;
						const float b = a + kTheta;
						const float ay = radius * cos(a);
						const float by = radius * cos(b);

						const float theta = j * kPhi;
						const float phi = theta + kPhi;

						const float ar = sqrt(radius * radius - ay * ay);
						const float br = sqrt(radius * radius - by * by);

						const float x0 = x + (ar * sin(theta));
						const float y0 = y + (ay);
						const float z0 = z + (ar * cos(theta));

						const float x1 = x + (br * sin(theta));
						const float y1 = y + (by);
						const float z1 = z + (br * cos(theta));

						const float x2 = x + (br * sin(phi));
						const float y2 = y + (by);
						const float z2 = z + (br * cos(phi));

						mLineVertices[mVertexCount++] = { Math::Vector3(x0, y0, z0), color };
						mLineVertices[mVertexCount++] = { Math::Vector3(x1, y1, z1), color };

						if (i < rings - 1)
						{
							mLineVertices[mVertexCount++] = { Math::Vector3(x1, y1, z1), color };
							mLineVertices[mVertexCount++] = { Math::Vector3(x2, y2, z2), color };
						}
					}
				}
			}
		}

		void AddScreenCircle(const Math::Circle& circle, const Color& color)
		{
			// Check if we have enough space
			if (m2DVertexCount + 32 <= mMaxVertexCount)
			{
				float x = circle.center.x;
				float y = circle.center.y;
				float r = circle.radius;

				// Add line
				const float kAngle = Math::Constants::Pi / 8.0f;
				for (uint32_t i = 0; i < 16; ++i)
				{
					const float alpha = i * kAngle;
					const float beta = alpha + kAngle;
					const float x0 = x + (r * sin(alpha));
					const float y0 = y + (r * cos(alpha));
					const float x1 = x + (r * sin(beta));
					const float y1 = y + (r * cos(beta));
					m2DLineVertices[m2DVertexCount++] = { Math::Vector3(x0, y0, 0.0f), color };
					m2DLineVertices[m2DVertexCount++] = { Math::Vector3(x1, y1, 0.0f), color };
				}
			}

			ASSERT(m2DVertexCount < mMaxVertexCount, "[SimpleDraw] Too many vertices!");
		}

		void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color)
		{
			// Check if we have enough space
			if (m2DVertexCount + 2 <= mMaxVertexCount)
			{
				// Add line
				m2DLineVertices[m2DVertexCount++] = { Math::Vector3(v0.x, v0.y, 0.0f), color };
				m2DLineVertices[m2DVertexCount++] = { Math::Vector3(v1.x, v1.y, 0.0f), color };
			}

			ASSERT(m2DVertexCount < mMaxVertexCount, "[SimpleDraw] Too many vertices!");
		}

		void AddOBB(const Math::OBB& obb, const Color& color)
		{
			Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
			Math::Matrix4 matRot = Math::Matrix4::RotationQuaternion(obb.rot);
			Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
			Math::Matrix4 toWorld = matScale * matRot * matTrans;

			Math::Vector3 points[] =
			{
				// Front quad
				Math::Vector3(-1.0f,-1.0f,-1.0f),
				Math::Vector3(-1.0f,+1.0f,-1.0f),
				Math::Vector3(+1.0f,+1.0f,-1.0f),
				Math::Vector3(+1.0f,-1.0f,-1.0f),
				// Back quad
				Math::Vector3(-1.0f,-1.0f,+1.0f),
				Math::Vector3(-1.0f,+1.0f,+1.0f),
				Math::Vector3(+1.0f,+1.0f,+1.0f),
				Math::Vector3(+1.0f,-1.0f,+1.0f)
			};

			for (auto& p : points)
			{
				p = Math::TransformCoord(p, toWorld);
			}

			AddLine(points[0], points[1], color);
			AddLine(points[1], points[2], color);
			AddLine(points[2], points[3], color);
			AddLine(points[3], points[0], color);

			AddLine(points[0], points[4], color);
			AddLine(points[1], points[5], color);
			AddLine(points[2], points[6], color);
			AddLine(points[3], points[7], color);

			AddLine(points[4], points[5], color);
			AddLine(points[5], points[6], color);
			AddLine(points[6], points[7], color);
			AddLine(points[7], points[4], color);
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

			//Draw 2D Lines
			auto system = GraphicsSystem::Get();
			const uint32_t w = system->GetBackBufferWidth();
			const uint32_t h = system->GetBackBufferHeight();
			Math::Matrix4 screenToNDC
			{
				2.0f / w, 0.0f,0.0f,0.0f,
				0.0f, -2.0f / h, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				-1.0f, 1.0f, 0.0f,1.0f
			};
			auto transposeNDC = Math::Transpose(screenToNDC); // moved to local variable
			mConstantBuffer.Update(&transposeNDC);
			mConstantBuffer.BindVS(0);

			mMeshBuffer.Update(m2DLineVertices.get(), m2DVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Draw();

			mFillVertexCount = 0;
			mVertexCount = 0;
			m2DVertexCount = 0;
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
		uint32_t m2DVertexCount = 0;
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
void SimpleDraw::AddSphere(const Math::Vector3& sphere, float radius, const Color & color, uint32_t rings, uint32_t slices)
{
	sInstance->AddSphere(sphere, radius, color, rings, slices);
}
void SimpleDraw::AddAABB(const Math::Vector3& center, float radius, const Color& color)
{
	sInstance->AddAABB(center,radius,color);
}
void SimpleDraw::AddAABB(const Math::AABB & aabb, const Color & color)
{
	sInstance->AddAABB(aabb, color);
}
void SimpleDraw::AddOBB(const Math::OBB & obb, const Color & color)
{
	sInstance->AddOBB(obb, color);
}
void SimpleDraw::AddTransform(const Math::Matrix4 & transform)
{
	auto r = Math::GetRight(transform);
	auto u = Math::GetUp(transform);
	auto l = Math::GetForward(transform);
	auto p = Math::GetTranslation(transform);
	sInstance->AddLine(p, p + r, Colors::Red);
	sInstance->AddLine(p, p + u, Colors::Green);
	sInstance->AddLine(p, p + l, Colors::Blue);
}
void SimpleDraw::AddBone(const Math::Matrix4 & transform)
{
	auto r = Math::GetRight(transform);
	auto u = Math::GetUp(transform);
	auto l = Math::GetLook(transform);
	auto p = Math::GetTranslation(transform);
	AddSphere(p, 0.025f, Colors::BlueViolet, 5, 6);
	sInstance->AddLine(p, p + r * 0.1f, Colors::Red);
	sInstance->AddLine(p, p + u * 0.1f, Colors::Green);
	sInstance->AddLine(p, p + l * 0.1f, Colors::Blue);
}

void SimpleDraw::AddGroundPlane(float size, float yAxis, const Color & color)
{
	AddGroundPlane(size, yAxis, 1.0f, color);
}

void SimpleDraw::AddGroundPlane(float size, float yAxis, float cellSize, const Color & color)
{
	const float halfSize = size * 0.5f;
	for (float i = -halfSize; i <= halfSize; i += cellSize)
	{
		sInstance->AddLine({i,yAxis, -halfSize}, {i,yAxis,halfSize}, color);
		sInstance->AddLine({-halfSize,yAxis,i}, {halfSize,yAxis,i}, color);
	}
}

void SimpleDraw::AddScreenCircle(const Math::Circle& circle, const Color& color)
{
	sInstance->AddScreenCircle(circle, color);
}

void SimpleDraw::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color)
{
	sInstance->AddScreenLine(v0, v1, color);
}

void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}