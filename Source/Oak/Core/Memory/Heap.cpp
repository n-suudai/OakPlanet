

#include "Oak/Core/Memory/Heap.hpp"
#include "Oak/Core/Assert.hpp"
#include "Oak/Core/Memory/HeapWalk.hpp"

namespace Oak
{

Heap::Heap()
  : m_protection()
  , m_totalAllocatedBytes(0)
  , m_peakAllocatedBytes(0)
  , m_allocatedInstanceCount(0)
  , m_pAllocation(nullptr)
  , m_pParent(nullptr)
  , m_pFirstChild(nullptr)
  , m_pNextSibling(nullptr)
  , m_pPrevSibling(nullptr)
  , m_isActive(false)
{
}

Heap::~Heap()
{
}

Void Heap::Initialize()
{
    LockGuard<CriticalSection> lock(m_protection);

    m_isActive = false;
    m_totalAllocatedBytes = 0;
    m_peakAllocatedBytes = 0;
    m_allocatedInstanceCount = 0;
    m_pAllocation = nullptr;
    m_pParent = nullptr;
    m_pFirstChild = nullptr;
    m_pNextSibling = nullptr;
    m_pPrevSibling = nullptr;
}

Void Heap::Deactivate()
{
    LockGuard<CriticalSection> lock(m_protection);

    strcpy_s(m_name, "");
    m_isActive = false;
    m_totalAllocatedBytes = 0;
    m_peakAllocatedBytes = 0;
    m_allocatedInstanceCount = 0;
    m_pBaseAllocator = nullptr;
}

const Char* Heap::GetName() const
{
    return m_name;
}

Bool Heap::IsActive() const
{
    return m_pBaseAllocator != nullptr && m_isActive;
}

Void* Heap::AllocateBytes(SizeT bytes, const Char* file, Int32 line,
                          const Char* function)
{
    LockGuard<CriticalSection> lock(m_protection);

    OAK_ASSERT(m_pBaseAllocator != nullptr);

    // シグネチャサイズをプラス
    constexpr SizeT signatureSize = sizeof(AllocationSignature);

    // メモリを確保
    Void* pBlock = m_pBaseAllocator->AllocateBytes(bytes + signatureSize);

    // トラッカーへ情報を登録
    MemoryTracker::Get().RecordAllocation(pBlock, bytes, file, line, function,
                                          this);

    return pBlock;
}

Void* Heap::AllocateAlignedBytes(SizeT bytes, SizeT alignment, const Char* file,
                                 Int32 line, const Char* function)
{
    LockGuard<CriticalSection> lock(m_protection);

    OAK_ASSERT(m_pBaseAllocator != nullptr);

    // シグネチャサイズをプラス
    constexpr SizeT signatureSize = sizeof(AllocationSignature);

    // メモリを確保
    Void* pBlock =
      m_pBaseAllocator->AllocateBytesAligned(bytes + signatureSize, alignment);

    // トラッカーへ情報を登録
    MemoryTracker::Get().RecordAllocation(pBlock, bytes, file, line, function,
                                          this);

    return pBlock;
}

Void Heap::DeallocateBytes(Void* pBlock)
{
    LockGuard<CriticalSection> lock(m_protection);

    OAK_ASSERT(m_pBaseAllocator != nullptr);

    // トラッカーから情報を削除
    MemoryTracker::Get().RecordDeallocation(pBlock, this);

    // メモリを破棄
    m_pBaseAllocator->DeallocateBytes(pBlock);
}

Void Heap::DeallocateAlignedBytes(Void* pBlock, SizeT alignment)
{
    LockGuard<CriticalSection> lock(m_protection);

    OAK_ASSERT(m_pBaseAllocator != nullptr);

    // トラッカーから情報を削除
    MemoryTracker::Get().RecordDeallocation(pBlock, this);

    // メモリを破棄
    m_pBaseAllocator->DeallocateBytesAligned(pBlock, alignment);
}

// リンクリストを構築
Void Heap::AddAllocation(Allocation* pAllocation)
{
    LockGuard<CriticalSection> lock(m_protection);

    pAllocation->pPrev = nullptr;
    pAllocation->pNext = m_pAllocation;

    if (m_pAllocation != nullptr)
    {
        m_pAllocation->pPrev = pAllocation;
    }

    m_pAllocation = pAllocation;

    // 確保サイズ情報更新
    m_totalAllocatedBytes += pAllocation->bytes;

    if (m_totalAllocatedBytes > m_peakAllocatedBytes)
    {
        m_peakAllocatedBytes = m_totalAllocatedBytes;
    }

    m_allocatedInstanceCount++;
}

// リンクリストから切り離す
Void Heap::EraseAllocation(Allocation* pAllocation)
{
    LockGuard<CriticalSection> lock(m_protection);

    if (pAllocation->pPrev == nullptr)
    {
        OAK_ASSERT(pAllocation == m_pAllocation);
        m_pAllocation = pAllocation->pNext;
    }
    else
    {
        pAllocation->pPrev->pNext = pAllocation->pNext;
    }

    if (pAllocation->pNext != nullptr)
    {
        pAllocation->pNext->pPrev = pAllocation->pPrev;
    }

    // 確保サイズ情報更新
    m_totalAllocatedBytes -= pAllocation->bytes;
    m_allocatedInstanceCount--;
}

// 親子関係の構築をする関数
Void Heap::AttachTo(Heap* pParent)
{
    LockGuard<CriticalSection> lock(m_protection);

    OAK_ASSERT(pParent != nullptr);

    if (pParent == m_pParent)
    {
        return;
    }

    // 1. 現在の親子関係を切り離す
    if (m_pPrevSibling != nullptr)
    {
        m_pPrevSibling->m_pNextSibling = m_pNextSibling;
    }

    if (m_pNextSibling != nullptr)
    {
        m_pNextSibling->m_pPrevSibling = m_pPrevSibling;
    }

    if (m_pParent != nullptr)
    {
        if (m_pParent->m_pFirstChild == this)
        {
            m_pParent->m_pFirstChild = m_pNextSibling;
        }
    }

    // 2. 新しく親子関係を構築
    m_pNextSibling = pParent->m_pFirstChild;
    m_pPrevSibling = nullptr;
    m_pParent = pParent;
    pParent->m_pFirstChild = this;
}

// リークのチェック関数
Void Heap::MemoryLeakCheck(IMemoryLeakReporter* pReporter, UInt64 bookmarkStart,
                           UInt64 bookmarkEnd) const
{
    OAK_ASSERT(pReporter != nullptr);

    Allocation* pAllocation = m_pAllocation;
    while (pAllocation != nullptr)
    {
        if (pAllocation->bookmark >= bookmarkStart &&
            pAllocation->bookmark <= bookmarkEnd)
        {
            pReporter->Report(this, pAllocation);
        }
        pAllocation = pAllocation->pNext;
    }
}

// 情報収集のための関数
Void Heap::ReportTreeStats(IHeapTreeStatsReporter* pAccumulator,
                           Int32 depth) const
{
    OAK_ASSERT(pAccumulator != nullptr);

    IHeapTreeStatsReporter::HeapTreeStats local, total;

    local.totalBytes = m_totalAllocatedBytes;
    local.peakBytes = m_peakAllocatedBytes;
    local.instanceCount = m_allocatedInstanceCount;

    GetTreeStats(total.totalBytes, total.peakBytes, total.instanceCount);

    pAccumulator->Report(depth, this, local, total);

    Heap* pChild = m_pFirstChild;
    while (pChild != nullptr)
    {
        pChild->ReportTreeStats(pAccumulator, depth + 1);
        pChild = pChild->m_pNextSibling;
    }
}

// メモリ破壊のチェック関数
Void Heap::MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                                UInt64 bookmarkStart, UInt64 bookmarkEnd) const
{
    OAK_ASSERT(pReporter != nullptr);

    Allocation* pAllocation = m_pAllocation;
    while (pAllocation != nullptr)
    {
        if (pAllocation->bookmark >= bookmarkStart &&
            pAllocation->bookmark <= bookmarkEnd)
        {
            if ((*pAllocation->pSignature) != Allocation::SIGNATURE)
            {
                pReporter->Report(this, pAllocation);
            }
        }
        pAllocation = pAllocation->pNext;
    }
}

Void Heap::GetTreeStats(SizeT& totalBytes, SizeT& totalPeakBytes,
                        SizeT& totalInstanceCount) const
{
    totalBytes += m_totalAllocatedBytes;
    totalPeakBytes += m_peakAllocatedBytes;
    totalInstanceCount += m_allocatedInstanceCount;

    Heap* pChild = m_pFirstChild;
    while (pChild != NULL)
    {
        pChild->GetTreeStats(totalBytes, totalPeakBytes, totalInstanceCount);
        pChild = pChild->m_pNextSibling;
    }
}

} // namespace Oak
