
#include "Oak/Core/Memory/HeapFactory.hpp"
#include "Oak/Core/Memory/HeapWalk.hpp"
#include "Oak/Core/Assert.hpp"

namespace Oak
{

HeapFactory& HeapFactory::Get()
{
    static HeapFactory instance;
    return instance;
}

HeapFactory::HeapFactory()
{
    Initialize();
}

HeapFactory::~HeapFactory()
{
}

Void HeapFactory::Initialize()
{
    LockGuard<CriticalSection> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        heap.Initialize();
    }

    m_pRootHeap = CreateNewHeap(s_pRootHeapName);
    m_pDefaultHeap = CreateHeap(s_pDefaultHeapName);
}

Heap* HeapFactory::GetRootHeap()
{
    LockGuard<CriticalSection> lock(m_protection);

    if (m_pRootHeap == nullptr)
    {
        Initialize();
    }

    return m_pRootHeap;
}

Heap* HeapFactory::GetDefaultHeap()
{
    LockGuard<CriticalSection> lock(m_protection);

    if (m_pDefaultHeap == nullptr)
    {
        Initialize();
    }

    return m_pDefaultHeap;
}

Heap* HeapFactory::CreateHeap(const Char* name, const Char* parentName)
{
    LockGuard<CriticalSection> lock(m_protection);

    if (m_pRootHeap == nullptr)
    {
        Initialize();
    }

    // 親ヒープを探す
    Heap* pParent = FindHeap(parentName);
    if (pParent == nullptr)
    { // 見つからなければ Root 以下に作成
        pParent = CreateNewHeap(parentName);
        pParent->AttachTo(m_pRootHeap);
    }

    // ヒープを探す
    Heap* pHeap = FindHeap(name);
    if (pHeap == nullptr)
    { // 見つからなければ作成
        pHeap = CreateNewHeap(name);
    }

    // pParent以下にアタッチ
    pHeap->AttachTo(pParent);

    return pHeap;
}

Heap* HeapFactory::CreateHeap(const Char* name)
{
    return CreateHeap(name, s_pRootHeapName);
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

Heap* HeapFactory::CreateNewHeap(const Char* name)
{
    LockGuard<CriticalSection> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        if (!heap.IsActive())
        {
            heap.Activate(name);
            return &heap;
        }
    }

    OAK_ASSERT_MESSAGE(
      false,
      "ヒープの作成に失敗しました。作成可能なヒープの数を超えています。");
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
Void HeapFactory::MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                                       UInt64 bookmarkStart,
                                       UInt64 bookmarkEnd) const
{
    OAK_ASSERT(pReporter != nullptr);

    pReporter->BeginReport();

    for (auto& heap : m_heaps)
    {
        if (heap.IsActive())
        {
            heap.MemoryAssertionCheck(pReporter, bookmarkStart, bookmarkEnd);
        }
    }

    pReporter->EndReport();
}

} // namespace Oak
