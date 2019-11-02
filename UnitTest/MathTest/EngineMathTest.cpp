#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace JimmyGod::Math;
namespace MathTest
{
	TEST_CLASS(EngineMathTest)
	{
	public:

		TEST_METHOD(TestMagnitude)
		{
			Vector3 a = { 5.0f, 1.0f, 1.0f };
			float result = MagnitudeSqr(a);
			Assert::AreEqual(result, 27.0f);
		}

		TEST_METHOD(TestTransformNormal)
		{
			Vector3 a = { 1.0f,2.0f,3.0f };
			Matrix4 b = Matrix4::Identity;
			Vector3 c = TransformNormal(a, b);
			Assert::AreEqual(c.x, 1.0f);
			Assert::AreEqual(c.y, 2.0f);
			Assert::AreEqual(c.z, 3.0f);
		}

		TEST_METHOD(TestTransformCoord)
		{
			Vector3 a = { 1.0f,2.0f,3.0f };
			Matrix4 b = Matrix4::Identity;
			b._41 = 10.0f; b._42 = 10.0f; b._43 = 10.0f;
			Vector3 c = TransformCoord(a, b);
			Assert::AreEqual(c.x, 11.0f);
			Assert::AreEqual(c.y, 12.0f);
			Assert::AreEqual(c.z, 13.0f);
		}

		TEST_METHOD(TestCross)
		{
			Vector3 a{ 5.0f,8.0f,9.0f };
			Vector3 b{ 2.0f, 3.0f, 4.0f };
			Vector3 c = Cross(a, b);
			Assert::AreEqual(c.x, 5.0f);
			Assert::AreEqual(c.y, -2.0f);
			Assert::AreEqual(c.z, -1.0f);
		}

		TEST_METHOD(TestDot)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 1.0f, 5.0f, 7.0f };
			float c = Dot(a, b);
			Assert::AreEqual(c, 32.0f);
		}
	};
}