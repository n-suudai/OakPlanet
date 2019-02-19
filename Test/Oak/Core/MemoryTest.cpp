
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


class TestClassCustom
{
    OAK_DECLARE_HEAP;
public:
    TestClassCustom() {}
    ~TestClassCustom() {}

    void Func()
    {
        OutputDebugStringA("TestClassCustom::Func()\n");
    }

private:
    char m_size[64];
};

OAK_DEFINE_HEAP(TestClassCustom, "custom", Oak::GeneralAllocatePolicy);

class TestClassCustomChild
{
    OAK_DECLARE_HEAP;
public:
    TestClassCustomChild() {}
    ~TestClassCustomChild() {}

    void Func()
    {
        OutputDebugStringA("TestClassCustomChild::Func()\n");
    }

private:
    char m_size[256];
};

OAK_DEFINE_HIERARCHAL_HEAP(TestClassCustomChild, "custom_child", "custom", Oak::GeneralAllocatePolicy);


class TestClassCustomChild2
{
    OAK_DECLARE_HEAP;
public:
    TestClassCustomChild2() {}
    ~TestClassCustomChild2() {}

    void Func()
    {
        OutputDebugStringA("TestClassCustomChild2::Func()\n");
    }

private:
    char m_size[256];
};

OAK_DEFINE_HIERARCHAL_HEAP(TestClassCustomChild2, "custom_child2", "custom_child", Oak::GeneralAllocatePolicy);


class TestClassCustomChildAligned
{
    OAK_DECLARE_HEAP;
public:
    TestClassCustomChildAligned() {}
    ~TestClassCustomChildAligned() {}

    void Func()
    {
        Oak::PtrDiff address = reinterpret_cast<Oak::PtrDiff>(this);
        if (!(address & Oak::PtrDiff(ALIGNMENT_SIZE - 1)))
        {
            OutputDebugStringA("TestClassCustomChildAligned::Func() = true\n");
        }
        else
        {
            OutputDebugStringA("TestClassCustomChildAligned::Func() = false\n");
        }
    }

private:
    char m_size[256];
};

OAK_DEFINE_HIERARCHAL_HEAP(TestClassCustomChildAligned, "custom_child_aligned", "custom_child", Oak::GeneralAlignedAllocatePolicy<ALIGNMENT_SIZE>);


class TreeStatsReporter : public Oak::IHeapTreeStatsReporter
{
public:
    TreeStatsReporter() {}
    ~TreeStatsReporter() {}

    Oak::Void BeginReport()
    {
        DebugPrintf("MEMORY INFORMATION\n");
        DebugPrintf("                            Local                 Total\n");
        DebugPrintf("Name                  Memory  Peak  Inst   Memory  Peak  Inst\n");
    }

    Oak::Void Report(Oak::Int32 depth, const Oak::Heap* pHeap, const HeapTreeStats& local, const HeapTreeStats& total)
    {
        for (Oak::Int32 i = 0; i < depth; ++i)
        {
            DebugPrintf("..");
        }

        Oak::Int32 spacing = 20 - depth * 2;
        DebugPrintf(
            "%-*s %6zu %6zu %5zu  %6zu %6zu %5zu\n",
            spacing, pHeap->GetName(),
            local.totalBytes, local.peakBytes, local.instanceCount,
            total.totalBytes, total.peakBytes, total.instanceCount
        );
    }

    Oak::Void EndReport()
    {

    }
};

class LeakReporter : public Oak::IMemoryLeakReporter
{
public:
    LeakReporter() {}
    ~LeakReporter() {}

    Oak::Void BeginReport()
    {
        m_leakCount = 0;
    }

    Oak::Void Report(const Oak::Heap* pHeap, const Oak::Allocation* pAllocation)
    {
        OAK_ASSERT(pHeap != nullptr);
        OAK_ASSERT(pAllocation != nullptr);

        DebugPrintf(
            "%s(%d)\n"
            "{ heap=\"%s\" address=0x%p size=%zubyte time=%s backTraceHash=0x%016llX bookmark=%llX }\n"
            "[ %08X ]\n",
            pAllocation->file, pAllocation->line,
            pHeap->GetName(), pAllocation->pBlock, pAllocation->bytes,
            /*pAllocation->time*/"", pAllocation->backTraceHash, pAllocation->bookmark,
            (*pAllocation->pSignature)
        );

        m_leakCount++;
    }

    Oak::Void EndReport()
    {
        if (m_leakCount > 0)
        {
            DebugPrintf("%zu memory leaks found!\n", m_leakCount);
        }
        else
        {
            DebugPrintf("No memory leaks detected.\n");
        }
    }

public:
    Oak::SizeT m_leakCount;
};

int  MemoryTestMain()
{
#if 0
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
#else

    using namespace Oak;

    TestClassCustom* pCustom = OAK_NEW TestClassCustom();
    TestClassCustomChild* pCustomChild = OAK_NEW TestClassCustomChild();
    TestClassCustomChild2* pCustomChild2 = OAK_NEW TestClassCustomChild2();
    TestClassCustomChildAligned* pCustomChildAligned = OAK_NEW TestClassCustomChildAligned();

    pCustom->Func();
    pCustomChild->Func();
    pCustomChild2->Func();
    pCustomChildAligned->Func();

    TreeStatsReporter statsReporter;
    HeapFactory::Get().ReportHeapTreeStats(&statsReporter);

    LeakReporter leakReporter;
    HeapFactory::Get().MemoryLeakCheck(&leakReporter);

    OAK_DELETE pCustom;
    OAK_DELETE pCustomChild;
    OAK_DELETE pCustomChild2;
    OAK_DELETE pCustomChildAligned;

    HeapFactory::Get().MemoryLeakCheck(&leakReporter);

#endif

    return 0;
}

