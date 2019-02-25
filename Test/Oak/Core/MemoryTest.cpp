
#include "MemoryTest.hpp"
#include "Oak/Core/Memory/AllocateConfig.hpp"
#include "Oak/Core/Memory/MemoryManager.hpp"
#include "Oak/Core/Memory/HeapWalk.hpp"
#include "Oak/Core/Assert.hpp"
#include "Oak/Core/Log.hpp"
#include <stdio.h>

#define ALIGNMENT_SIZE 32

class TestClassCustom
{
    OAK_DECLARE_HEAP;

public:
    TestClassCustom()
    {
    }
    ~TestClassCustom()
    {
    }

    void Func()
    {
        Oak::Log::Message("TestClassCustom::Func()\n");
    }

private:
    char m_size[64];
};

OAK_DEFINE_HEAP(TestClassCustom, "custom", Oak::AllocatePolicy);

class TestClassCustomChild
{
    OAK_DECLARE_HEAP;

public:
    TestClassCustomChild()
    {
    }
    ~TestClassCustomChild()
    {
    }

    void Func()
    {
        Oak::Log::Message("TestClassCustomChild::Func()\n");
    }

private:
    char m_size[256];
};

OAK_DEFINE_HIERARCHAL_HEAP(TestClassCustomChild, "custom_child", "custom",
                           Oak::AllocatePolicy);

class TestClassCustomChild2
{
    OAK_DECLARE_HEAP;

public:
    TestClassCustomChild2()
    {
    }
    ~TestClassCustomChild2()
    {
    }

    void Func()
    {
        Oak::Log::Message("TestClassCustomChild2::Func()\n");
    }

private:
    char m_size[256];
};

OAK_DEFINE_HIERARCHAL_HEAP(TestClassCustomChild2, "custom_child2",
                           "custom_child", Oak::AllocatePolicy);

class TestClassCustomChildAligned
{
    OAK_DECLARE_HEAP;

public:
    TestClassCustomChildAligned()
    {
    }
    ~TestClassCustomChildAligned()
    {
    }

    void Func()
    {
        Oak::PtrDiff address = reinterpret_cast<Oak::PtrDiff>(this);
        if (!(address & Oak::PtrDiff(ALIGNMENT_SIZE - 1)))
        {
            Oak::Log::Message("TestClassCustomChildAligned::Func() = true\n");
        }
        else
        {
            Oak::Log::Message("TestClassCustomChildAligned::Func() = false\n");
        }
    }

private:
    char m_size[256];
};

OAK_DEFINE_HIERARCHAL_HEAP_ALIGN(TestClassCustomChildAligned,
                                 "custom_child_aligned", "custom_child",
                                 Oak::AllocatePolicy, ALIGNMENT_SIZE);

#if OAK_USE_HEAP_TRACKING

class TreeStatsReporter : public Oak::IHeapTreeStatsReporter
{
public:
    TreeStatsReporter()
    {
    }
    ~TreeStatsReporter()
    {
    }

    Oak::Void BeginReport()
    {
        Oak::Log::Format("MEMORY INFORMATION\n");
        Oak::Log::Format(
          "                            Local                 Total\n");
        Oak::Log::Format(
          "Name                  Memory  Peak  Inst   Memory  Peak  Inst\n");
    }

    Oak::Void Report(Oak::Int32 depth, const Oak::Heap* pHeap,
                     const HeapTreeStats& local, const HeapTreeStats& total)
    {
        for (Oak::Int32 i = 0; i < depth; ++i)
        {
            Oak::Log::Format("..");
        }

        Oak::Int32 spacing = 20 - depth * 2;
        Oak::Log::Format("%-*s %6zu %6zu %5zu  %6zu %6zu %5zu\n", spacing,
                         pHeap->GetName(), local.totalBytes, local.peakBytes,
                         local.instanceCount, total.totalBytes, total.peakBytes,
                         total.instanceCount);
    }

    Oak::Void EndReport()
    {
    }
};

class LeakReporter : public Oak::IMemoryLeakReporter
{
public:
    LeakReporter()
    {
    }
    ~LeakReporter()
    {
    }

    Oak::Void BeginReport()
    {
        m_leakCount = 0;
    }

    Oak::Void Report(const Oak::Heap* pHeap, const Oak::Allocation* pAllocation)
    {
        OAK_ASSERT(pHeap != nullptr);
        OAK_ASSERT(pAllocation != nullptr);

        Oak::Log::Format(
          "%s(%d)\n"
          "{ heap=\"%s\" address=0x%p size=%zubyte time=%s "
          "backTraceHash=0x%016llX bookmark=%llX }\n"
          "[ %08X ]\n",
          pAllocation->file, pAllocation->line, pHeap->GetName(),
          pAllocation->pBlock, pAllocation->bytes,
          /*pAllocation->time*/ "", pAllocation->backTraceHash,
          pAllocation->bookmark, (*pAllocation->pSignature));

        m_leakCount++;
    }

    Oak::Void EndReport()
    {
        if (m_leakCount > 0)
        {
            Oak::Log::Format("%zu memory leaks found!\n", m_leakCount);
        }
        else
        {
            Oak::Log::Format("No memory leaks detected.\n");
        }
    }

public:
    Oak::SizeT m_leakCount;
};

#endif // OAK_USE_HEAP_TRACKING

int MemoryTestMain()
{
    using namespace Oak;

    TestClassCustom* pCustom = OAK_NEW TestClassCustom();
    TestClassCustomChild* pCustomChild = OAK_NEW TestClassCustomChild();
    TestClassCustomChild2* pCustomChild2 = OAK_NEW TestClassCustomChild2();
    TestClassCustomChildAligned* pCustomChildAligned =
      OAK_NEW TestClassCustomChildAligned();

    pCustom->Func();
    pCustomChild->Func();
    pCustomChild2->Func();
    pCustomChildAligned->Func();

#if OAK_USE_HEAP_TRACKING

    TreeStatsReporter statsReporter;
    HeapFactory::Get().ReportHeapTreeStats(&statsReporter);

    LeakReporter leakReporter;
    HeapFactory::Get().MemoryLeakCheck(&leakReporter);

#endif // OAK_USE_HEAP_TRACKING

    OAK_DELETE pCustom;
    OAK_DELETE pCustomChild;
    OAK_DELETE pCustomChild2;
    OAK_DELETE pCustomChildAligned;

#if OAK_USE_HEAP_TRACKING
    HeapFactory::Get().MemoryLeakCheck(&leakReporter);
#endif

    return 0;
}
