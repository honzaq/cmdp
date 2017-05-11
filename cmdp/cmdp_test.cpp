#include "stdafx.h"
#include "CppUnitTest.h"
#include "cmdp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace cmdp
{		
	TEST_CLASS(CmdpUnitTest)
	{
	public:
		
		TEST_METHOD(FlagsOneMinusOneCharWhiteCharOnStart)
		{
			const wchar_t* argv[] = { L"  \t-a" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 1);
			Assert::IsTrue(cmdp[L"a"] == true);
			Assert::IsTrue(cmdp(L"a").str().empty());
		}

		TEST_METHOD(FlagsOneChars)
		{
			const wchar_t* argv[] = { L"-a", L"--b", L"/c" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 3);
			Assert::IsTrue(cmdp[L"a"] == true);
			Assert::IsTrue(cmdp(L"a").str().empty());
			Assert::IsTrue(cmdp[L"b"] == true);
			Assert::IsTrue(cmdp(L"b").str().empty());
			Assert::IsTrue(cmdp[L"c"] == true);
			Assert::IsTrue(cmdp(L"c").str().empty());
		}

		TEST_METHOD(FlagsOneMoreChar)
		{
			const wchar_t* argv[] = { L"-abc", L"--bcd", L"/cde" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 3);
			Assert::IsTrue(cmdp[L"abc"] == true);
			Assert::IsTrue(cmdp(L"abc").str().empty());
			Assert::IsTrue(cmdp[L"bcd"] == true);
			Assert::IsTrue(cmdp(L"bcd").str().empty());
			Assert::IsTrue(cmdp[L"cde"] == true);
			Assert::IsTrue(cmdp(L"cde").str().empty());
		}

		TEST_METHOD(SimpleParam)
		{
			const wchar_t* argv[] = { L"/name1=value1", L"/name2:value2", L"-name3:value3", L"--name4:value4" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 4);
			Assert::IsTrue(cmdp[L"name1"] == true);
			Assert::IsTrue(cmdp(L"name1").str().compare(L"value1") == 0);
			Assert::IsTrue(cmdp[L"name2"] == true);
			Assert::IsTrue(cmdp(L"name2").str().compare(L"value2") == 0);
			Assert::IsTrue(cmdp[L"name3"] == true);
			Assert::IsTrue(cmdp(L"name3").str().compare(L"value3") == 0);
			Assert::IsTrue(cmdp[L"name4"] == true);
			Assert::IsTrue(cmdp(L"name4").str().compare(L"value4") == 0);
		}

		TEST_METHOD(EmptyParam)
		{
			const wchar_t* argv[] = { L"/name1=\"\"" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 1);
			Assert::IsTrue(cmdp[L"name1"] == true);
			Assert::IsTrue(cmdp(L"name1").str().compare(L"") == 0);
		}

		TEST_METHOD(ParamAndValueSplittedBySpace)
		{
			const wchar_t* argv[] = { L"/name1", L"value1" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 1);
			Assert::IsTrue(cmdp[L"name1"] == true);
			Assert::IsTrue(cmdp(L"name1").str().compare(L"value1") == 0);
		}

		TEST_METHOD(Mix1)
		{
			const wchar_t* argv[] = { L"/name1", L"value1", L"--flag", L"-flag2", L"value2" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 3);
			Assert::IsTrue(cmdp[L"name1"] == true);
			Assert::IsTrue(cmdp(L"name1").str().compare(L"value1") == 0);
			Assert::IsTrue(cmdp[L"flag"] == true);
			Assert::IsTrue(cmdp[L"flag2"] == true);
			Assert::IsTrue(cmdp(L"flag2").str().compare(L"value2") == 0);
		}

		TEST_METHOD(Quoted)
		{
			const wchar_t* argv[] = { L"/name1", L"\"val\\\\ue1\"", L"--flag", L"-name2=\"value2\"" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 3);
			Assert::IsTrue(cmdp[L"name1"] == true);
			Assert::IsTrue(cmdp(L"name1").str().compare(L"val\\ue1") == 0);
			Assert::IsTrue(cmdp[L"flag"] == true);
			Assert::IsTrue(cmdp[L"name2"] == true);
			Assert::IsTrue(cmdp(L"name2").str().compare(L"value2") == 0);
		}

		TEST_METHOD(CompareNoCase)
		{
			const wchar_t* argv[] = { L"/NAME1=value1", L"/NAME2" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 2);
			Assert::IsTrue(cmdp[L"name1"] == true);
			Assert::IsTrue(cmdp(L"name1").str().compare(L"value1") == 0);
			Assert::IsTrue(cmdp[L"name2"] == true);
		}

		TEST_METHOD(SlashSeparatorName)
		{
			const wchar_t* argv[] = { L"/pipe-name" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser cmdp(argc, argv);
			Assert::IsTrue(cmdp.params().size() == 1);
			Assert::IsTrue(cmdp[L"pipe-name"] == true);
		}
	};
}