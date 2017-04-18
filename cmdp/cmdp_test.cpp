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
			cmdp::parser(argc, argv);
		}

		TEST_METHOD(FlagsOneChars)
		{
			const wchar_t* argv[] = { L"-a", L"--b", L"/c" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser(argc, argv);
		}

		TEST_METHOD(FlagsOneMoreChar)
		{
			const wchar_t* argv[] = { L"-abc" L"--bcd", L"/cde" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser(argc, argv);
		}

		TEST_METHOD(SimpleParam)
		{
			const wchar_t* argv[] = { L"/name1=value1", L"/name2:value2", L"-name3:value3", L"--name4:value4" };
			int argc = sizeof(argv) / sizeof(argv[0]);
			cmdp::parser(argc, argv);
		}

	};
}