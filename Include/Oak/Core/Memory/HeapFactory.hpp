
#pragma once


#include "Oak/Core/Memory/Heap.hpp"


namespace Oak {


class HeapFactory
{
public:
    static HeapFactory& Get();

    HeapFactory();

    ~HeapFactory();

    Void Initialize();

    Heap* GetRootHeap();

    Heap* GetDefaultHeap();

    Heap* CreateHeap(const Char* name, const Char* parentName);

    Heap* CreateHeap(const Char* name);

    Heap* FindHeap(const Char* name);

    Heap* CreateNewHeap(const Char* name);


    // リークのチェック関数
    Void MemoryLeakCheck(IMemoryLeakReporter* pReporter, UInt64 bookmarkStart, UInt64 bookmarkEnd) const;

    Void MemoryLeakCheck(IMemoryLeakReporter* pReporter, UInt64 bookmarkStart = 0);

    // メモリ情報
    Void ReportHeapTreeStats(IHeapTreeStatsReporter* pReporter);

    // メモリ破壊のチェック関数
    Void MemoryAssertionCheck(IMemoryAssertionReporter* pReporter, UInt64 bookmarkStart, UInt64 bookmarkEnd) const;

private:
    static constexpr Char* s_pRootHeapName = "Root";
    static constexpr Char* s_pDefaultHeapName = "Default";

    CriticalSection m_protection;

    enum { MAXHEAPS = 256 };
    Heap    m_heaps[MAXHEAPS];
    Heap*   m_pRootHeap;
    Heap*   m_pDefaultHeap;
};


} // namespace Oak

