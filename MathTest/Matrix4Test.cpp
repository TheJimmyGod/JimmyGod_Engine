#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace JimmyGod::Math;
namespace MathTest
{
	TEST_CLASS(Matrix4Test)
	{
	public:

		TEST_METHOD(TestIdentity)
		{
			Matrix4 a = Matrix4::Identity;
			Assert::AreEqual(a._11, 1.0f);
			Assert::AreEqual(a._22, 1.0f);
			Assert::AreEqual(a._33, 1.0f);
			Assert::AreEqual(a._44, 1.0f);
		}

		TEST_METHOD(TestTranslation)
		{
			Matrix4 a = Matrix4::Identity;
			Vector3 b = { 2.0f, 3.0f, 4.0f };
			a = Matrix4::Translation(b);
			Assert::AreEqual(a._41, 2.0f);
			Assert::AreEqual(a._42, 3.0f);
			Assert::AreEqual(a._43, 4.0f);
		}

		TEST_METHOD(TestScale)
		{
			Matrix4 a = Matrix4::Identity;
			float scale = 10.0f;
			a = Matrix4::Scaling(10.0f);
			Assert::AreEqual(a._11, 10.0f);
			Assert::AreEqual(a._22, 10.0f);
			Assert::AreEqual(a._33, 10.0f);
		}
	};
}