
#include "Oak/Core/Memory/MemoryTracker.hpp"

#if OAK_USE_HEAP_TRACKING

#include "Oak/Core/Memory/Heap.hpp"
#include "Oak/Platform/Thread/LockGuard.hpp"
#include "Oak/Core/Assert.hpp"
#include "Oak/Core/StackTrace.hpp"

namespace Oak
{

MemoryTracker& MemoryTracker::Get()
{
    static MemoryTracker instance;
    return instance;
}

MemoryTracker::MemoryTracker()
  : m_protection(), m_allocations(), m_nextAllocationBookmark(1)
{
}

MemoryTracker::~MemoryTracker()
{
    // ※この時点で残っているAllocationはリークしている
}

Allocation* MemoryTracker::RecordAllocation(Void* pBlock, SizeT bytes,
                                            const Char* file, Int32 line,
                                            const Char* function, Heap* pHeap)
{
    LockGuard<CriticalSection> lock(m_protection);

    Allocation* pAllocation = reinterpret_cast<Allocation*>(
      AllocatePolicy::AllocateBytesForTracking(sizeof(Allocation)));

    pAllocation->pBlock = pBlock;
    pAllocation->bytes = bytes;
    pAllocation->file = file;
    pAllocation->line = line;
    pAllocation->function = function;
    pAllocation->time = 0;
    pAllocation->backTraceHash = StackTrace::CaptureStackTraceHash();
    pAllocation->pSignature = reinterpret_cast<AllocationSignature*>(
      reinterpret_cast<PtrDiff>(pBlock) + bytes);
    pAllocation->bookmark = m_nextAllocationBookmark;
    pAllocation->pHeap = pHeap;

    // シグネチャを書き込み
    (*pAllocation->pSignature) = Allocation::SIGNATURE;

    m_allocations[pBlock] = pAllocation;

    // リンクリストを構築
    pHeap->AddAllocation(pAllocation);

    m_nextAllocationBookmark++;

    return pAllocation;
}

Void MemoryTracker::RecordDeallocation(Void* pBlock, Heap* pHeap)
{
    LockGuard<CriticalSection> lock(m_protection);

    auto it = m_allocations.find(pBlock);

    if (it != m_allocations.end())
    {
        // シグネチャをチェック
        OAK_ASSERT((*it->second->pSignature) == Allocation::SIGNATURE);

        // リンクリストから切り離す
        pHeap->EraseAllocation(it->second);

        AllocatePolicy::DeallocateBytesForTracking(it->second);

        m_allocations.erase(it);
    }
}

SizeT MemoryTracker::GetAllocationBookmark() const
{
    return m_nextAllocationBookmark;
}

} // namespace Oak

#endif // OAK_USE_HEAP_TRACKING
