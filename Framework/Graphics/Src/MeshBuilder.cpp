#include "Precompiled.h"
#include "MeshBuilder.h"
#include "VertexTypes.h"
#include "Mesh.h"
using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;
MeshPX MeshBuilder::CreateCubePX()
{
	MeshPX mMesh;
	//Front
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,-1.0f}, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,-1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,-1.0f}, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,-1.0f}, 1.0f,1.0f });
	//Right
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,-1.0f }, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,-1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,1.0f}, 1.0f,1.0f });
	//Back
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,1.0f }, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,1.0f}, 1.0f,1.0f });
	//Left
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,1.0f}, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,-1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,-1.0f}, 1.0f,1.0f });
	//Up
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,1.0f}, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,-1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,-1.0f}, 1.0f,1.0f });
	//Bottom
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,-1.0f}, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,-1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,1.0f}, 1.0f,1.0f });

	//Front
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(2);
	//Right
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(6);
	//Back
	mMesh.indices.push_back(8);
	mMesh.indices.push_back(9);
	mMesh.indices.push_back(10);
	mMesh.indices.push_back(9);
	mMesh.indices.push_back(11);
	mMesh.indices.push_back(10);
	//Left
	mMesh.indices.push_back(12);
	mMesh.indices.push_back(13);
	mMesh.indices.push_back(14);
	mMesh.indices.push_back(13);
	mMesh.indices.push_back(15);
	mMesh.indices.push_back(14);
	//Top
	mMesh.indices.push_back(16);
	mMesh.indices.push_back(17);
	mMesh.indices.push_back(18);
	mMesh.indices.push_back(17);
	mMesh.indices.push_back(19);
	mMesh.indices.push_back(18);
	//Bottom
	mMesh.indices.push_back(21);
	mMesh.indices.push_back(20);
	mMesh.indices.push_back(22);
	mMesh.indices.push_back(21);
	mMesh.indices.push_back(22);
	mMesh.indices.push_back(23);
	return mMesh;
}
MeshPX MeshBuilder::CreatePlanePX(float height, float width)
{
	ASSERT((height > 1.0f && width > 1.0f), "Must be more than 1.0 for height and width");
	MeshPX mMesh;
	const float OffsetX = (width) * 0.5f;
	const float OffsetY = (height) * 0.5f;
	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			mMesh.vertices.emplace_back(VertexPX{ Vector3{ -OffsetX + (static_cast<float>(x)), OffsetY - (static_cast<float>(y)),0.0f },
				static_cast<float>((x) /width ), static_cast<float>((y) /height) });
		}
	}

	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mMesh.indices.push_back(y*width + x);
			mMesh.indices.push_back((y + 1) * width + x + 1);
			mMesh.indices.push_back((y + 1) * width + x);

			mMesh.indices.push_back(y * width + x);
			mMesh.indices.push_back(y * width + x + 1);
			mMesh.indices.push_back((y + 1)* width + x + 1);
		}
	}
	return mMesh;
}

MeshPX MeshBuilder::CreateCylinderPX(float height, float width)
{
	MeshPX mMesh;
	const float radius = width / 2.0f;
	for (float y = 0.0f; y < height; ++y)
	{
		for (float theta = 0.0f; theta < Constants::TwoPi; theta += (Constants::TwoPi) / (width))
		{
			mMesh.vertices.emplace_back(VertexPX{ Vector3{
				-sinf(theta)* radius,
				static_cast<float>(-y),
				cosf(theta)* radius },
				theta / Constants::TwoPi,y / height });
		}
	}

	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			if ((y == 0) && (x != 0) && (x != width - 1))
			{
				mMesh.indices.push_back(0);
				mMesh.indices.push_back(0 + x + 1);
				mMesh.indices.push_back(0 + x);
			}
			if ((y == height - 1) && (x != 0) && (x != width - 1))
			{
				mMesh.indices.push_back(y*width);
				mMesh.indices.push_back(y * width + x);
				mMesh.indices.push_back(y * width + x + 1);
			}
			mMesh.indices.push_back(y*width + x);
			mMesh.indices.push_back((y + 1) * width + x + 1);
			mMesh.indices.push_back((y + 1) * width + x);

			mMesh.indices.push_back(y * width + x);
			mMesh.indices.push_back(y * width + x + 1);
			mMesh.indices.push_back((y + 1)* width + x + 1);
		}
	}

	return mMesh;
}

MeshPX MeshBuilder::CreateSpherePX(float radius, int rings, int slices, bool isSpace)
{
	MeshPX mMesh;
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

			mMesh.vertices.emplace_back(
				VertexPX{
					vec,
					theta / Constants::TwoPi,
					phi / Constants::Pi
				});
		}

	}

	uint32_t a, b, c, d;
	for (uint32_t y = 0; y < rings; ++y)
	{
		for (uint32_t x = 0; x <= slices; ++x)
		{
			a = static_cast<uint32_t>(x % (slices + 1));
			b = static_cast<uint32_t>((x + 1) % (slices + 1));
			c = static_cast<uint32_t>(y * (slices + 1));
			d = static_cast<uint32_t>((y + 1)*(slices + 1));

			if (!isSpace)
			{
				mMesh.indices.push_back(a + c);
				mMesh.indices.push_back(b + c);
				mMesh.indices.push_back(a + d);

				mMesh.indices.push_back(b + c);
				mMesh.indices.push_back(b + d);
				mMesh.indices.push_back(a + d);
			}
			else
			{
				mMesh.indices.push_back(a + d);
				mMesh.indices.push_back(b + c);
				mMesh.indices.push_back(a + c);

				mMesh.indices.push_back(a + d);
				mMesh.indices.push_back(b + d);
				mMesh.indices.push_back(b + c);
			}
		}
	}
	return mMesh;
}

MeshPN MeshBuilder::CreateCylinderPN(float height, float width)
{
	MeshPN mMesh;
	const float radius = width / 2.0f;
	for (float y = 0.0f; y < height; ++y)
	{
		for (float theta = 0.0f; theta < Constants::TwoPi; theta += (Constants::TwoPi) / (width))
		{
			auto vec = Vector3{
				-sinf(theta)* radius,
				static_cast<float>(-y),
				cosf(theta)* radius };

			mMesh.vertices.emplace_back(VertexPN{ vec, Normalize(vec)});
		}
	}

	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			if ((y == 0) && (x != 0) && (x != width - 1))
			{
				mMesh.indices.push_back(0);
				mMesh.indices.push_back(0 + x + 1);
				mMesh.indices.push_back(0 + x);
			}
			if ((y == height - 1) && (x != 0) && (x != width - 1))
			{
				mMesh.indices.push_back(y*width);
				mMesh.indices.push_back(y * width + x);
				mMesh.indices.push_back(y * width + x + 1);
			}
			mMesh.indices.push_back(y*width + x);
			mMesh.indices.push_back((y + 1) * width + x + 1);
			mMesh.indices.push_back((y + 1) * width + x);

			mMesh.indices.push_back(y * width + x);
			mMesh.indices.push_back(y * width + x + 1);
			mMesh.indices.push_back((y + 1)* width + x + 1);
		}
	}

	return mMesh;
}

MeshPN MeshBuilder::CreateSpherePN(float radius, int rings, int slices, bool isSpace)
{
	MeshPN mMesh;
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

			mMesh.vertices.emplace_back(
				VertexPN{
					vec,
					Normalize(vec)
				});
		}

	}

	uint32_t a, b, c, d;
	for (uint32_t y = 0; y < rings; ++y)
	{
		for (uint32_t x = 0; x <= slices; ++x)
		{
			a = static_cast<uint32_t>(x % (slices + 1));
			b = static_cast<uint32_t>((x + 1) % (slices + 1));
			c = static_cast<uint32_t>(y * (slices + 1));
			d = static_cast<uint32_t>((y + 1)*(slices + 1));

			if (!isSpace)
			{
				mMesh.indices.push_back(a + c);
				mMesh.indices.push_back(b + c);
				mMesh.indices.push_back(a + d);

				mMesh.indices.push_back(b + c);
				mMesh.indices.push_back(b + d);
				mMesh.indices.push_back(a + d);
			}
			else
			{
				mMesh.indices.push_back(a + d);
				mMesh.indices.push_back(b + c);
				mMesh.indices.push_back(a + c);

				mMesh.indices.push_back(a + d);
				mMesh.indices.push_back(b + d);
				mMesh.indices.push_back(b + c);
			}
		}
	}
	return mMesh;
}

Mesh MeshBuilder::CreateSphere(float radius, int rings, int slices, bool isSpace)
{
	Mesh mMesh;
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
			auto vec2 = Vector2{ theta / Constants::TwoPi, phi / Constants::Pi }; // Texture coordinate
			auto vecN = Normalize(vec); // Normal
			auto vecT = Vector3{ -vecN.z,0.0f,vecN.x }; // Tangent
			mMesh.vertices.emplace_back(
				Vertex{
					vec,
					vecN,
					vecT,
					vec2
				});
		}

	}

	uint32_t a, b, c, d;
	for (uint32_t y = 0; y < rings; ++y)
	{
		for (uint32_t x = 0; x <= slices; ++x)
		{
			a = static_cast<uint32_t>(x % (slices + 1));
			b = static_cast<uint32_t>((x + 1) % (slices + 1));
			c = static_cast<uint32_t>(y * (slices + 1));
			d = static_cast<uint32_t>((y + 1)*(slices + 1));

			if (!isSpace)
			{
				mMesh.indices.push_back(a + c);
				mMesh.indices.push_back(b + c);
				mMesh.indices.push_back(a + d);

				mMesh.indices.push_back(b + c);
				mMesh.indices.push_back(b + d);
				mMesh.indices.push_back(a + d);
			}
			else
			{
				mMesh.indices.push_back(a + d);
				mMesh.indices.push_back(b + c);
				mMesh.indices.push_back(a + c);

				mMesh.indices.push_back(a + d);
				mMesh.indices.push_back(b + d);
				mMesh.indices.push_back(b + c);
			}
		}
	}
	return mMesh;
}

MeshPX MeshBuilder::CreateNDCQuad()
{
	MeshPX mMesh;

	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,0.0f}, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,0.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,-0.0f}, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,0.0f}, 1.0f,1.0f });

	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(2);

	mMesh.indices.push_back(1);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(2);

	return mMesh;
}
