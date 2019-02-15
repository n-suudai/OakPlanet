
#include "MemoryTest.hpp"
#include "Oak/Core/Memory/AllocateConfig.hpp"
#include "Oak/Core/Memory/MemoryTracker.hpp"
#include  <stdio.h>


#define DebugPrintf(format, ...) { char buff[256]; sprintf_s(buff, format, __VA_ARGS__); OutputDebugStringA(buff);  }


class TestClass : public Oak::GeneralAllocatedObject
{
public:
    TestClass() {}
    ~TestClass() {}

    void Func()
    {
        OutputDebugStringA("TestClass::Func()\n");
    }

private:
    char m_size[16];
};

#define ALIGNMENT_SIZE 32

class TestClassAligned : public  Oak::GeneralAlignAllocatedObject<ALIGNMENT_SIZE>
{
public:
    TestClassAligned() {}
    ~TestClassAligned() {}

    void Func()
    {
        Oak::PtrDiff address = reinterpret_cast<Oak::PtrDiff>(this);
        if (!(address & Oak::PtrDiff(ALIGNMENT_SIZE - 1)))
        {
            OutputDebugStringA("TestClassAligned::Func() = true\n");
        }
        else
        {
            OutputDebugStringA("TestClassAligned::Func() = false\n");
        }
    }

private:
    char m_size[32];
};


int  MemoryTestMain()
{
    Oak::MemoryTracker&  tracker = Oak::MemoryTracker::Get();

    {
        TestClass* pTestClass = OAK_NEW TestClass();

        pTestClass->Func();

        OAK_DELETE pTestClass;
    }

    {
        TestClassAligned* pTestClassAligned = OAK_NEW TestClassAligned();

        pTestClassAligned->Func();

        OAK_DELETE pTestClassAligned;
    }

    DebugPrintf(
        "Instance : %zu\n"
        "Total    : %zu\n"
        "Peak     : %zu\n",
        tracker.GetAliveInstanceCount(),
        tracker.GetTotalMemoryAllocatedBytes(),
        tracker.GetPeakMemoryAllocatedBytes()
    );

    return 0;
}

