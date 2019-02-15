
#include "MemoryTest.hpp"
#include "Oak/Core/Memory/AllocateConfig.hpp"


class TestClass : public Oak::GeneralAllocatedObject
{
public:
    TestClass() {}
    ~TestClass() {}

    void Func()
    {
        OutputDebugStringA("TestClass::Func()\n");
    }
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
};


int  MemoryTestMain()
{
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

    return 0;
}

