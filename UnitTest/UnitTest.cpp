#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(PlayerTest)
	{
	public:
		TEST_METHOD(TestMethod)
		{
			Assert::AreEqual(1.f, 1.f, 0.0001f);
		}

	};
}
