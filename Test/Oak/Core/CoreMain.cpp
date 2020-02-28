
#include "Oak/Platform/OS/Win32.hpp"

#include "Oak/Core/StackTrace.hpp"
#include "AssertTest.hpp"
#include "MemoryTest.hpp"
#include "RandomTest.hpp"

#define ExecuteTest(testName) testName##TestMain()

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    Oak::StackTrace::Initialize();

    // ExecuteTest(Assert);
    ExecuteTest(Memory);
    ExecuteTest(Random);

    Oak::StackTrace::Terminate();

    return 0;
}
