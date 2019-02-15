﻿
#include "Oak/Platform/OS/Win32.hpp"

#include "Oak/Core/StackTrace.hpp"
#include "AssertTest.hpp"
#include "MemoryTest.hpp"


#define ExecuteTest(testName) testName ## TestMain()



int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    Oak::StackTrace::Initialize();

    //ExecuteTest(Assert);
    ExecuteTest(Memory);

    Oak::StackTrace::Terminate();

    return 0;
}

