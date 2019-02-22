
#pragma once

#include "Oak/Core/Memory/Heap.hpp"

namespace Oak
{

class HeapFactory
{
public:
    static HeapFactory& Get();

    HeapFactory();

    ~HeapFactory();

    template <typename Policy = AllocatePolicy>
    inline Void Initialize();

    Heap* GetRootHeap();

    Heap* GetDefaultHeap();

    template <typename Policy>
    inline Heap* CreateHeap(const Char* name, const Char* parentName);

    template <typename Policy>
    inline Heap* CreateHeap(const Char* name);

    Heap* FindHeap(const Char* name);

    template <typename Policy>
    inline Heap* CreateNewHeap(const Char* name);

    // リークのチェック関数
    Void MemoryLeakCheck(IMemoryLeakReporter* pReporter, UInt64 bookmarkStart,
                         UInt64 bookmarkEnd) const;

    Void MemoryLeakCheck(IMemoryLeakReporter* pReporter,
                         UInt64 bookmarkStart = 0);

    // メモリ情報
    Void ReportHeapTreeStats(IHeapTreeStatsReporter* pReporter);

    // メモリ破壊のチェック関数
    Void MemoryAssertionCheck(IMemoryAssertionReporter* pReporter,
                              UInt64 bookmarkStart, UInt64 bookmarkEnd) const;

private:
    static constexpr Char* s_pRootHeapName = "Root";
    static constexpr Char* s_pDefaultHeapName = "Default";

    enum
    {
        MAXHEAPS = 256
    };

    CriticalSection m_protection;
    Heap m_heaps[MAXHEAPS];
    Heap* m_pRootHeap;
    Heap* m_pDefaultHeap;
};

template <typename Policy>
inline Void HeapFactory::Initialize()
{
    LockGuard<CriticalSection> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        heap.Initialize();
    }

    m_pRootHeap = CreateNewHeap<Policy>(s_pRootHeapName);
    m_pDefaultHeap = CreateHeap<Policy>(s_pDefaultHeapName);
}

template <typename Policy>
inline Heap* HeapFactory::CreateHeap(const Char* name, const Char* parentName)
{
    LockGuard<CriticalSection> lock(m_protection);

    if (m_pRootHeap == nullptr)
    {
        Initialize<>();
    }

    // 親ヒープを探す
    Heap* pParent = FindHeap(parentName);
    if (pParent == nullptr)
    { // 見つからなければ Root 以下に作成
        pParent = CreateNewHeap<Policy>(parentName);
        pParent->AttachTo(m_pRootHeap);
    }

    // ヒープを探す
    Heap* pHeap = FindHeap(name);
    if (pHeap == nullptr)
    { // 見つからなければ作成
        pHeap = CreateNewHeap<Policy>(name);
    }

    // pParent以下にアタッチ
    pHeap->AttachTo(pParent);

    return pHeap;
}

template <typename Policy>
inline Heap* HeapFactory::CreateHeap(const Char* name)
{
    return CreateHeap<Policy>(name, s_pRootHeapName);
}

template <typename Policy>
inline Heap* HeapFactory::CreateNewHeap(const Char* name)
{
    LockGuard<CriticalSection> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        if (!heap.IsActive<Policy>())
        {
            heap.Activate<Policy>(name);
            return &heap;
        }
    }

    OAK_ASSERT_MESSAGE(
      false,
      "ヒープの作成に失敗しました。作成可能なヒープの数を超えています。");
    return nullptr;
}

} // namespace Oak
