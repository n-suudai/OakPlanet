
#include "Oak/Core/Memory/HeapFactory.hpp"

#if OAK_USE_HEAP_TRACKING

#include "Oak/Core/Memory/HeapWalk.hpp"
#include "Oak/Core/Assert.hpp"
#include "Oak/Core/Memory/AllocateConfig.hpp"
#include "Oak/Core/Memory/MemoryTracker.hpp"

namespace Oak
{

HeapFactory& HeapFactory::Get()
{
    static HeapFactory instance;
    return instance;
}

HeapFactory::HeapFactory()
{
}

HeapFactory::~HeapFactory()
{
}

Heap* HeapFactory::GetRootHeap()
{
    LockGuard<CriticalSection> lock(m_protection);

    if (m_pRootHeap == nullptr)
    {
        Initialize<AllocatePolicy>();
    }

    return m_pRootHeap;
}

Heap* HeapFactory::GetDefaultHeap()
{
    LockGuard<CriticalSection> lock(m_protection);

    if (m_pDefaultHeap == nullptr)
    {
        Initialize<AllocatePolicy>();
    }

    return m_pDefaultHeap;
}

Heap* HeapFactory::FindHeap(const Char* name)
{
    LockGuard<CriticalSection> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        if (heap.IsActive() && !_stricmp(name, heap.GetName()))
        {
            return &heap;
        }
    }

    return nullptr;
}

// リークのチェック関数
Void HeapFactory::MemoryLeakCheck(IMemoryLeakReporter* pReporter,
                                  UInt64 bookmarkStart,
                                  UInt64 bookmarkEnd) const
{
    OAK_ASSERT(pReporter != nullptr);

    pReporter->BeginReport();

    for (auto& heap : m_heaps)
    {
        if (heap.IsActive())
        {
            heap.MemoryLeakCheck(pReporter, bookmarkStart, bookmarkEnd);
        }
    }

    pReporter->EndReport();
}

Void HeapFactory::MemoryLeakCheck(IMemoryLeakReporter* pReporter,
                                  UInt64 bookmarkStart)
{
    MemoryLeakCheck(pReporter, bookmarkStart,
                    MemoryTracker::Get().GetAllocationBookmark());
}

// メモリ情報
Void HeapFactory::ReportHeapTreeStats(IHeapTreeStatsReporter* pReporter)
{
    OAK_ASSERT(pReporter != nullptr);

    pReporter->BeginReport();

    m_pRootHeap->ReportTreeStats(pReporter, 0);

    pReporter->EndReport();
}

// メモリ破壊のチェック関数
Void HeapFactory::MemoryCorruptionCheck(IMemoryCorruptionReporter* pReporter,
                                        UInt64 bookmarkStart,
                                        UInt64 bookmarkEnd) const
{
    OAK_ASSERT(pReporter != nullptr);

    pReporter->BeginReport();

    for (auto& heap : m_heaps)
    {
        if (heap.IsActive())
        {
            heap.MemoryCorruptionCheck(pReporter, bookmarkStart, bookmarkEnd);
        }
    }

    pReporter->EndReport();
}

} // namespace Oak

#endif // OAK_USE_HEAP_TRACKING
